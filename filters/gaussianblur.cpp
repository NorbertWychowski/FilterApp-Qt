#include "gaussianblur.h"

#include <QtConcurrent/QtConcurrentRun>
#include <QFutureSynchronizer>
#include <QtMath>

GaussianBlur::GaussianBlur(const QImage &image, QProgressBar *progressBar) {
    int maxThreads = QThread::idealThreadCount();
    QFutureSynchronizer<void> futures;

    this->progressBar = progressBar;

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

    this->image = image.copy();
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

    double sigma = qSqrt(-(radius * radius) / (2.0 * log(1.0 / 255.0)));

    int *s[] = {_red, _green, _blue};
    int *d[] = {newRed, newGreen, newBlue};

    gaussBlur_4(s, d, sigma);

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

void GaussianBlur::gaussBlur_4(int **source, int **dest, double sigma) {
    int *bxs = boxesForGauss(sigma, 3);
    qDebug() << "2: " << ((bxs[0] + 1) / 2) << " " << ((bxs[1] + 1) / 2) << " " << ((bxs[2] + 1) / 2);

    boxBlur_4(source, dest, _width, _height, (bxs[0] - 1) / 2);
    boxBlur_4(dest, source, _width, _height, (bxs[1] - 1) / 2);
    boxBlur_4(source, dest, _width, _height, (bxs[2] - 1) / 2);
}

int *GaussianBlur::boxesForGauss(double sigma, int n) {
    double wIdeal = qSqrt((12.0 * sigma * sigma / n) + 1.0);
    int wl = qFloor(wIdeal);
    if (wl % 2 == 0) wl--;
    int wu = wl + 2;

    double mIdeal = (12.0 * sigma * sigma - n * wl * wl - 4.0 * n * wl - 3.0 * n) / (-4.0 * wl - 4.0);
    int m = qRound(mIdeal);

    int *sizes = new int[n];
    for (int i = 0; i < n; i++) {
        sizes[i] = (i < m ? wl : wu);
    }
    return sizes;
}

void GaussianBlur::boxBlur_4(int **source, int **dest, int w, int h, int r) {
    double iar = 1.0 / (r + r + 1.0);
    QFutureSynchronizer<void> futures;
    int maxThreads = QThread::idealThreadCount();

    auto fill = [=](int start, int end) {
        for (int i=start; i < end; ++i)
            for (int j=0; j<3; ++j)
                dest[j][i] = source[j][i];
    };

    for (int i = 0; i<maxThreads; ++i) {
        futures.addFuture(QtConcurrent::run(fill, h*w*i / maxThreads, h*w*(i + 1) / maxThreads));
    }

    futures.waitForFinished();
    futures.clearFutures();

    auto boxBlurH_4 = [&](int start, int end) {
        int ti, li, ri, fv[3], lv[3], val[3];
        for (int i = start; i < end; ++i) {
            ti = i * w;
            li = ti;
            ri = ti + r;
            for (int k=0; k<3; ++k) {
                fv[k] = dest[k][ti];
                lv[k] = dest[k][ti + w - 1];
                val[k] = (r + 1) * fv[k];
            }
            for (int j = 0; j < r; ++j)
                for (int k=0; k<3; ++k)
                    val[k] += dest[k][ti + j];
            for (int j = 0; j <= r; ++j) {
                for (int k=0; k<3; ++k) {
                    val[k] += dest[k][ri] - fv[k];
                    source[k][ti] = qRound(val[k] * iar);
                }
                ti++;
                ri++;
            }
            for (int j = r + 1; j < w - r; ++j) {
                for (int k=0; k<3; ++k) {
                    val[k] += dest[k][ri] - dest[k][li];
                    source[k][ti] = qRound(val[k] * iar);
                }
                ri++;
                ti++;
                li++;
            }
            for (int j = w - r; j < w; ++j) {
                for (int k=0; k<3; ++k) {
                    val[k] += lv[k] - dest[k][li];
                    source[k][ti] = qRound(val[k] * iar);
                }
                li++;
                ti++;
            }
        }
    };

    auto boxBlurT_4 = [&](int start, int end) {
        int ti, li, ri, fv[3], lv[3], val[3];
        for (int i = start; i < end; ++i) {
            ti = i;
            li = ti;
            ri = ti + r*w;
            for (int k=0; k<3; ++k) {
                fv[k] = source[k][ti];
                lv[k] = source[k][ti + w * (h - 1)];
                val[k] = (r + 1) * fv[k];
            }
            for (int j = 0; j<r; ++j)
                for (int k=0; k<3; ++k)
                    val[k] += source[k][ti + j * w];
            for (int j = 0; j <= r; ++j) {
                for (int k=0; k<3; ++k) {
                    val[k] += source[k][ri] - fv[k];
                    dest[k][ti] = qRound(val[k] * iar);
                }
                ri += w;
                ti += w;
            }
            for (int j = r + 1; j < h - r; ++j) {
                for (int k=0; k<3; ++k) {
                    val[k] += source[k][ri] - source[k][li];
                    dest[k][ti] = qRound(val[k] * iar);
                }
                li += w;
                ri += w;
                ti += w;
            }
            for (int j = h - r; j < h; ++j) {
                for (int k=0; k<3; ++k) {
                    val[k] += lv[k] - source[k][li];
                    dest[k][ti] = qRound(val[k] * iar);
                }
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
