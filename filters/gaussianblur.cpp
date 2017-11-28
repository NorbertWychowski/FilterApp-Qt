#include "gaussianblur.h"

#include <QtConcurrent/QtConcurrentRun>
#include <QFutureSynchronizer>
#include <QtMath>

GaussianBlur::GaussianBlur(QImage &image) {
    int maxThreads = QThread::idealThreadCount();
    QFutureSynchronizer<void> futures;

    _width = image.width();
    _height = image.height();
    _red = new int[_width * _height];
    _green = new int[_width * _height];
    _blue = new int[_width * _height];

    for(int j=0; j<maxThreads; ++j)
        futures.addFuture(QtConcurrent::run([=] {
        for (int i = j*_height/maxThreads; i<(j+1)*_height/maxThreads; ++i) {
            QRgb *t = (QRgb*)image.scanLine(i);
            for (int j = 0; j<_width; ++j) {
                _red[i*_width + j] = qRed(t[j]);
                _green[i*_width + j] = qGreen(t[j]);
                _blue[i*_width + j] = qBlue(t[j]);
            }
        }
    }));

    futures.waitForFinished();
    futures.clearFutures();

    this->image = image.copy(0, 0, _width, _height);
}

GaussianBlur::~GaussianBlur() {
    delete[] newRed;
    delete[] newGreen;
    delete[] newBlue;
    delete[] _red;
    delete[] _green;
    delete[] _blue;
}

QImage GaussianBlur::blur(int radius, qint8 **selectedTab) {
    int maxThreads = QThread::idealThreadCount();
    QFutureSynchronizer<void> futures;

    newRed = new int[_width * _height];
    newGreen = new int[_width * _height];
    newBlue = new int[_width * _height];

    int r = qSqrt(-(radius * radius) / (2.0 * log(1.0 / 255.0)));

    gaussBlur_4(_red, newRed, r);
    gaussBlur_4(_green, newGreen, r);
    gaussBlur_4(_blue, newBlue, r);

    auto createImage = [&](int start, int end) {
        int row = start*_width;
        if (selectedTab != nullptr) {
            for (int i = start; i< end; ++i) {
                for (int j = 0; j<_width; ++j) {
                    if (selectedTab[i][j] == 1) {
                        image.setPixel(j, i, qRgb(newRed[row + j], newGreen[row + j], newBlue[row + j]));
                    }
                }
                row += _width;
            }
        } else {
            for (int i = start; i< end; ++i) {
                for (int j = 0; j<_width; ++j) {
                    image.setPixel(j, i, qRgb(newRed[row + j], newGreen[row + j], newBlue[row + j]));
                }
                row += _width;
            }
        }
    };

    for (int i = 0; i<maxThreads; ++i)
        futures.addFuture(QtConcurrent::run(createImage, i*_height / maxThreads, (i + 1)*_height / maxThreads));

    futures.waitForFinished();
    futures.clearFutures();

    return image;
}

void GaussianBlur::gaussBlur_4(int *source, int *dest, int r) {
    int *bxs = boxesForGauss(r, 3);
    boxBlur_4(source, dest, _width, _height, (bxs[0] - 1) / 2);
    boxBlur_4(dest, source, _width, _height, (bxs[1] - 1) / 2);
    boxBlur_4(source, dest, _width, _height, (bxs[2] - 1) / 2);
}

int *GaussianBlur::boxesForGauss(double sigma, int n) {
    double wIdeal = sqrt((12 * sigma * sigma / n) + 1);
    int wl = (int)floor(wIdeal);
    if (wl % 2 == 0) wl--;
    int wu = wl + 2;

    double mIdeal = (double)(12 * sigma * sigma - n * wl * wl - 4 * n * wl - 3 * n) / (-4 * wl - 4);
    int m = (int)round(mIdeal);

    int *sizes = new int[n];
    for (int i = 0; i < n; i++) {
        sizes[i] = (i < m ? wl : wu);
    }
    return sizes;
}

void GaussianBlur::boxBlur_4(int *source, int *dest, int w, int h, int r) {
    double iar = 1.0 / (r + r + 1.0);
    QFutureSynchronizer<void> futures;
    int maxThreads = QThread::idealThreadCount();

    auto fill = [=](int start, int end) {
        for(int i=start; i < end; ++i)
            dest[i] = source[i];
    };

    for (int i = 0; i<maxThreads; ++i) {
        futures.addFuture(QtConcurrent::run(fill, h*w*i / maxThreads, h*w*(i + 1) / maxThreads));
    }

    futures.waitForFinished();
    futures.clearFutures();

    auto boxBlurH_4 = [&](int start, int end) {
        int ti, li, ri, fv, lv, val;
        for (int i = start; i < end; ++i) {
            ti = i * w;
            li = ti;
            ri = ti + r;
            fv = dest[ti];
            lv = dest[ti + w - 1];
            val = (r + 1) * fv;
            for (int j = 0; j < r; ++j) val += dest[ti + j];
            for (int j = 0; j <= r; ++j) {
                val += dest[ri++] - fv;
                source[ti++] = (int)round(val * iar);
            }
            for (int j = r + 1; j < w - r; ++j) {
                val += dest[ri++] - dest[li++];
                source[ti++] = (int)round(val * iar);
            }
            for (int j = w - r; j < w; ++j) {
                val += lv - dest[li++];
                source[ti++] = (int)round(val * iar);
            }
        }
    };

    auto boxBlurT_4 = [&](int start, int end) {
        int ti, li, ri, fv, lv, val;
        for (int i = start; i < end; ++i) {
            ti = i;
            li = ti;
            ri = ti + r*w;
            fv = source[ti];
            lv = source[ti + w * (h - 1)];
            val = (r + 1) * fv;
            for (int j = 0; j<r; ++j) val += source[ti + j * w];
            for (int j = 0; j <= r; ++j) {
                val += source[ri] - fv;
                dest[ti] = (int)round(val * iar);
                ri += w;
                ti += w;
            }
            for (int j = r + 1; j < h - r; ++j) {
                val += source[ri] - source[li];
                dest[ti] = (int)round(val * iar);
                li += w;
                ri += w;
                ti += w;
            }
            for (int j = h - r; j < h; ++j) {
                val += lv - source[li];
                dest[ti] = (int)round(val * iar);
                li += w;
                ti += w;
            }
        }
    };

    for (int i = 0; i<maxThreads; ++i) {
        futures.addFuture(QtConcurrent::run(boxBlurH_4, h*i / maxThreads, h*(i + 1) / maxThreads));
    }


    futures.waitForFinished();
    futures.clearFutures();

    for (int i = 0; i<maxThreads; ++i) {
        futures.addFuture(QtConcurrent::run(boxBlurT_4, w*i / maxThreads, w*(i + 1) / maxThreads));
    }

    futures.waitForFinished();
    futures.clearFutures();
}
