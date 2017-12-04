#include "boxblur.h"

#include <QtConcurrent/QtConcurrentRun>
#include <QFutureSynchronizer>

BoxBlur::BoxBlur(const QImage &image, QProgressBar *progressBar) {
    _width = image.width();
    _height = image.height();

    this->progressBar = progressBar;

    _red = new int[_width*_height];
    _green = new int[_width*_height];
    _blue = new int[_width*_height];

    for (int i = 0; i<_height; ++i) {
        QRgb *t = (QRgb*)image.scanLine(i);
        for (int j = 0; j<image.width(); ++j) {
            _red[i*_width + j] = qRed(t[j]);
            _green[i*_width + j] = qGreen(t[j]);
            _blue[i*_width + j] = qBlue(t[j]);
        }
    }

    this->image = image.copy(0, 0, _width, _height);
}

BoxBlur::~BoxBlur() {
    delete[] _red;
    delete[] _green;
    delete[] _blue;
    delete[] newRed;
    delete[] newGreen;
    delete[] newBlue;
}

QImage BoxBlur::blur(int radius, qint8 **selectedTab) {
    int maxThreads = QThread::idealThreadCount();
    QFutureSynchronizer<void> futures;

    newRed = new int[_width * _height];
    newGreen = new int[_width * _height];
    newBlue = new int[_width * _height];

    auto fill = [=](int start, int end) {
        for (int i = start; i<end; ++i) {
            newRed[i] = _red[i];
            newGreen[i] = _green[i];
            newBlue[i] = _blue[i];
        }
    };

    for (int i = 0; i<maxThreads; ++i)
        futures.addFuture(QtConcurrent::run(fill, i*_width * _height / maxThreads, (i + 1)*_width * _height / maxThreads));

    futures.waitForFinished();
    futures.clearFutures();

    boxBlur(_red, newRed, _width, _height, radius);
    boxBlur(_green, newGreen, _width, _height, radius);
    boxBlur(_blue, newBlue, _width, _height, radius);

    auto createImage = [=](int start, int end) {
        int row = start*_width;
        if (selectedTab != nullptr) {
            for (int i = start; i< end; ++i) {
                for (int j = 0; j<_width; ++j) {
                    if (selectedTab[i][j] == 1) {
                        newRed[i] = _red[i];
                        newGreen[i] = _green[i];
                        newBlue[i] = _blue[i];

                        if(newRed[i] < 0)           newRed[i] = 0;
                        else if(newRed[i] > 255)    newRed[i] = 255;
                        if(newGreen[i] < 0 )        newGreen[i] = 0;
                        else if(newGreen[i] > 255)  newGreen[i] = 255;
                        if(newBlue[i] < 0)          newBlue[i] = 0;
                        else if(newBlue[i] > 255)   newBlue[i] = 255;

                        image.setPixel(j, i, qRgb(newRed[row + j], newGreen[row + j], newBlue[row + j]));
                    }
                }
                row += _width;
            }
        } else {
            for (int i = start; i< end; ++i) {
                for (int j = 0; j<_width; ++j) {
                    newRed[i] = _red[i];
                    newGreen[i] = _green[i];
                    newBlue[i] = _blue[i];

                    if(newRed[i] < 0)           newRed[i] = 0;
                    else if(newRed[i] > 255)    newRed[i] = 255;
                    if(newGreen[i] < 0 )        newGreen[i] = 0;
                    else if(newGreen[i] > 255)  newGreen[i] = 255;
                    if(newBlue[i] < 0)          newBlue[i] = 0;
                    else if(newBlue[i] > 255)   newBlue[i] = 255;

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

void BoxBlur::boxBlur(int *source, int *dest, int w, int h, int r) {
    double iar = 1.0 / (r + r + 1.0);

    auto boxBlurH_4 = [=](int start, int end) {
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

    auto boxBlurT_4 = [=](int start, int end) {
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

    QFutureSynchronizer<void> futures;

    int maxThreads = QThread::idealThreadCount();
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
