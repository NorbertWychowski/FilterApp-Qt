#include "gaussianblur.h"

#include <QtConcurrent/QtConcurrentRun>
#include <QDebug>

GaussianBlur::GaussianBlur(QImage &image) {
    _width = image.width();
    _height = image.height();

    source = new QRgb[_width * _height];
    for(int i=0; i<image.height(); ++i) {
        QRgb *t = (QRgb*)image.scanLine(i);
        for(int j=0; j<_width; ++j)
            source[i*_width + j] = t[j];
    }

    _red = new int[_width * _height];
    _green = new int[_width * _height];
    _blue = new int[_width * _height];

    for(int i=0; i<(_width * _height); ++i) {
        QRgb col = source[i];
        _red[i] = qRed(col);
        _green[i] = qGreen(col);
        _blue[i] = qBlue(col);
    }

    this->image = image.copy(0, 0, _width, _height);
}

GaussianBlur::~GaussianBlur() {
    delete[] newRed;
    delete[] newGreen;
    delete[] newBlue;
    delete[] dest;
    delete[] source;
    delete[] _red;
    delete[] _green;
    delete[] _blue;
}

QImage GaussianBlur::blur(int radius, int **selectedTab) {
    newRed = new int[_width * _height];
    newGreen = new int[_width * _height];
    newBlue = new int[_width * _height];
    dest = new int[_width * _height];

    gaussBlur_4(_red, newRed, radius);
    gaussBlur_4(_green, newGreen, radius);
    gaussBlur_4(_blue, newBlue, radius);

    QList<QFuture<void>> futures;
    futures.append(QtConcurrent::run([=] (void) {
        for(int i=0; i<_width * _height; ++i) {
            if (newRed[i] > 255)        newRed[i] = 255;
            else if (newRed[i] < 0)     newRed[i] = 0;
        }
    }));
    futures.append(QtConcurrent::run([=] (void) {
        for(int i=0; i<_width * _height; ++i) {
            if (newGreen[i] > 255)      newGreen[i] = 255;
            else if (newGreen[i] < 0)   newGreen[i] = 0;
        }
    }));
    futures.append(QtConcurrent::run([=] (void) {
        for(int i=0; i<_width * _height; ++i) {
            if (newBlue[i] > 255)       newBlue[i] = 255;
            else if (newBlue[i] < 0)    newBlue[i] = 0;
        }
    }));
    futures.append(QtConcurrent::run([=] (void) {
        int row = 0;
        if(selectedTab != nullptr) {
            for(int i=0; i< _height; ++i) {
                for(int j=0; j<_width; ++j) {
                    if(selectedTab[i][j] == 1) {
                        image.setPixel(j, i, qRgb(newRed[row + j], newGreen[row + j], newBlue[row+ j]));
                    } else {
                        image.setPixel(j, i, qRgb(_red[row + j], _green[row + j], _blue[row+ j]));
                    }
                }
                row += _width;
            }
        } else {
            for(int i=0; i< _height; ++i) {
                for(int j=0; j<_width; ++j)
                    image.setPixel(j, i, qRgb(newRed[row + j], newGreen[row + j], newBlue[row+ j]));
                row += _width;
            }
        }
    }));

    for(auto future : futures)
        future.waitForFinished();
    futures.clear();

    return image;
}

void GaussianBlur::gaussBlur_4(int *source, int *dest, int r) {
    int *bxs = boxesForGauss(r, 3);
    boxBlur_4(source, dest, _width, _height, (bxs[0] - 1) / 2);
    boxBlur_4(dest, source, _width, _height, (bxs[1] - 1) / 2);
    boxBlur_4(source, dest, _width, _height, (bxs[2] - 1) / 2);
}

int *GaussianBlur::boxesForGauss(int sigma, int n) {
    double wIdeal = sqrt((12 * sigma * sigma / n) + 1);
    int wl = (int)floor(wIdeal);
    if (wl % 2 == 0) wl--;
    int wu = wl + 2;

    double mIdeal = (double)(12 * sigma * sigma - n * wl * wl - 4 * n * wl - 3 * n) / (-4 * wl - 4);
    int m = (int)round(mIdeal);

    int *sizes = new int[n];
    for (int i = 0; i < n; i++) sizes[i] = (i < m ? wl : wu);
    return sizes;
}

void GaussianBlur::boxBlur_4(int *source, int *dest, int w, int h, int r) {
    for (int i = 0; i < (_width*_height); ++i) dest[i] = source[i];

    double iar = 1.0 / (r + r + 1.0);

    auto boxBlurH_4 = [&](int start, int end) {
        for(int i = start; i < end; ++i) {
            int ti = i * w;
            int li = ti;
            int ri = ti + r;
            int fv = dest[ti];
            int lv = dest[ti + w - 1];
            int val = (r + 1) * fv;
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
        for(int i = start; i < end; ++i) {
            int ti = i;
            int li = ti;
            int ri = ti + r*w;
            int fv = source[ti];
            int lv = source[ti + w * (h - 1)];
            int val = (r + 1) * fv;
            for(int j=0; j<r; ++j) val += source[ti + j * w];
            for(int j=0; j<=r; ++j) {
                val += source[ri] - fv;
                dest[ti] = (int)round(val * iar);
                ri+= w;
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

    QList<QFuture<void>> futures;

    int maxThreads = QThread::idealThreadCount();
    for(int i=0; i<maxThreads; ++i) {
        futures.append(QtConcurrent::run(boxBlurH_4, h*i/maxThreads, h*(i+1)/maxThreads));
    }

    for(auto future : futures)
        future.waitForFinished();
    futures.clear();

    for(int i=0; i<maxThreads; ++i) {
        futures.append(QtConcurrent::run(boxBlurT_4, w*i/maxThreads,  w*(i+1)/maxThreads));
    }

    for(auto future : futures)
        future.waitForFinished();
    futures.clear();
}
