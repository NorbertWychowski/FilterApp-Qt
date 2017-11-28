#include "filtertool.h"
#include "gaussianblur.h"
#include "boxblur.h"

#include <QtMath>
#include <QtConcurrent/QtConcurrent>
#include <QFutureSynchronizer>

QImage FilterTool::splot(QImage &img, FILTER choose, Matrix userKernel) {
    Matrix kernel;
    QImage res;
    GaussianBlur *gaussianBlur = nullptr;
    BoxBlur *boxBlur = nullptr;

    switch (choose) {
    case LOWPASS_FILTER:
        boxBlur = new BoxBlur(img);
        res = boxBlur->blur(5);
        delete boxBlur;
        return res;
        break;
    case LAPLACE_FILTER:
        kernel = Matrix::getLaplaceKernel();
        break;
    case HIGHPASS_FILTER:
        kernel = Matrix::getHighPassKernel();
        break;
    case GAUSSIAN_FILTER:
        gaussianBlur = new GaussianBlur(img);
        res =  gaussianBlur->blur(5);
        delete gaussianBlur;
        return res;
        break;
    case USER_FILTER:
        kernel = userKernel;
        break;
    default:
        break;
    }

    int N = kernel.getNDimension();
    int M = kernel.getMDimension();
    double norm = Matrix::getNorm(kernel);

    if (img.format() != QImage::Format_RGB32)
        img = img.convertToFormat(QImage::Format_RGB32);

    res = QImage(img.width() - N + 1, img.height() - N + 1, img.format());
    int maxThreads = QThread::idealThreadCount();
    QFutureSynchronizer<void> futures;

    auto f = [&](int start, int end) {
        int r = 0;
        int g = 0;
        int b = 0;
        QRgb color;

        if(start == 0) start += N/2;
        if(end == img.height()) end -= N/2;
        for (int y = start; y < end; ++y) {
            for (int x = N/2; x < img.width() - N/2; ++x) {
                for (int i = 0; i<M; ++i) {
                    QRgb *imgLine = (QRgb*)(img.scanLine(y - N/2 + i));
                    for (int j = 0; j<N; ++j) {
                        color = imgLine[x - N + j];
                        r += kernel[i][j] * qRed(color);
                        g += kernel[i][j] * qGreen(color);
                        b += kernel[i][j] * qBlue(color);
                    }
                }
                if (norm != 0) {
                    r = qBound(0, int(r / norm), 255);
                    g = qBound(0, int(g / norm), 255);
                    b = qBound(0, int(b / norm), 255);
                } else {
                    r = qBound(0, r, 255);
                    g = qBound(0, g, 255);
                    b = qBound(0, b, 255);
                }
                res.setPixel(x - N/2, y - N/2, qRgb(r, g, b));
                r = g = b = 0;
            }
        }
    };

    for(int i=0; i<maxThreads; ++i)
        futures.addFuture(QtConcurrent::run(f, i*img.height()/maxThreads, (i+1)*img.height()/maxThreads));

    futures.waitForFinished();

    return res;
}

QImage FilterTool::splot(QImage &img, FILTER choose, qint8 ** selectedTab, Matrix userKernel) {
    Matrix kernel;
    QImage res;
    GaussianBlur *gaussianBlur = nullptr;
    BoxBlur *boxBlur = nullptr;

    switch (choose) {
    case LOWPASS_FILTER:
        boxBlur = new BoxBlur(img);
        res = boxBlur->blur(5, selectedTab);
        delete boxBlur;
        return res;
        break;
    case LAPLACE_FILTER:
        kernel = Matrix::getLaplaceKernel();
        break;
    case HIGHPASS_FILTER:
        kernel = Matrix::getHighPassKernel();
        break;
    case GAUSSIAN_FILTER:
        gaussianBlur = new GaussianBlur(img);
        res =  gaussianBlur->blur(5, selectedTab);
        delete gaussianBlur;
        return res;
        break;
    case USER_FILTER:
        kernel = userKernel;
        break;
    default:
        break;
    }

    int N = kernel.getNDimension();
    int M = kernel.getMDimension();
    double norm = Matrix::getNorm(kernel);

    if (img.format() != QImage::Format_RGB32)
        img = img.convertToFormat(QImage::Format_RGB32);

    res = QImage(img.width() - N + 1, img.height() - N + 1, img.format());
    int maxThreads = QThread::idealThreadCount();

    QFutureSynchronizer<void> futures;
    auto f = [&](int start, int end) {
        int r = 0;
        int g = 0;
        int b = 0;
        QRgb color;

        if(start == 0) start += N/2;
        if(end == img.height()) end -= N/2;
        for (int y = start; y < end; ++y) {
            for (int x = N/2; x < img.width() - N/2; ++x) {
                if(selectedTab[y][x] == 1) {
                    for (int i = 0; i<M; ++i) {
                        QRgb *imgLine = (QRgb*)img.scanLine(y - N/2 + i);
                        for (int j = 0; j<N; ++j) {
                            color = imgLine[x - N/2 + j];
                            r += kernel[i][j] * qRed(color);
                            g += kernel[i][j] * qGreen(color);
                            b += kernel[i][j] * qBlue(color);
                        }
                    }
                    if (norm != 0) {
                        r = qBound(0, int(r / norm), 255);
                        g = qBound(0, int(g / norm), 255);
                        b = qBound(0, int(b / norm), 255);
                    } else {
                        r = qBound(0, r, 255);
                        g = qBound(0, g, 255);
                        b = qBound(0, b, 255);
                    }
                    res.setPixel(x - M/2, y - N/2, qRgb(r, g, b));
                    r = g = b = 0;
                } else {
                    res.setPixel(x - M/2, y - N/2, img.pixel(x, y));
                }
            }
        }
    };

    for(int i=0; i<maxThreads; ++i)
        futures.addFuture(QtConcurrent::run(f, i*img.height()/maxThreads, (i+1)*img.height()/maxThreads));

    futures.waitForFinished();

    return res;
}
