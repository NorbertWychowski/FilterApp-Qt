#include "filter.h"
#include "QtMath"
#include <QDebug>

Filter::Filter() {
}

QImage Filter::splot(QImage &img, FILTER choose, Matrix userKernel) {
    Matrix kernel;
    GaussianBlur *blur = nullptr;
    QImage res;
    switch (choose) {
    case LOWPASS_FILTER:
        kernel = convolution.getLowPassKernel();
        break;
    case LAPLACE_FILTER:
        kernel = convolution.getLaplaceKernel();
        break;
    case HIGHPASS_FILTER:
        kernel = convolution.getHighPassKernel();
        break;
    case GAUSSIAN_FILTER:
        blur = new GaussianBlur(img);
        res =  blur->blur(5);
        break;
    case USER_FILTER:
        kernel = userKernel;
        break;
    default:
        break;
    }

    if(blur != nullptr) {
        delete blur;
        return res;
    }

    int N = kernel.getNDimension();
    int M = kernel.getMDimension();
    double norm = 0;
    for(int i=0; i<N; ++i)
        for(int j=0; j<M; ++j)
            norm += kernel[i][j];

    int r = 0;
    int g = 0;
    int b = 0;
    QRgb color;
    if (img.format() != QImage::Format_RGB32)
        img = img.convertToFormat(QImage::Format_RGB32);

    res = QImage(img.width() - kernel.getMDimension() + 1, img.height() - kernel.getNDimension() + 1, img.format());

    for (int y = N/2; y < img.height() - N/2; ++y) {
        for (int x = M/2; x < img.width() - M/2; ++x) {
            for (int i = 0; i<M; ++i) {
                const QRgb *imgLine = reinterpret_cast<const QRgb*>(img.scanLine(y - N/2 + i));
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
            res.setPixel(x - M/2, y - N/2, qRgb(r, g, b));
            r = g = b = 0;
        }
    }

    return res;
}

QImage Filter::splot(QImage &img, FILTER choose, int ** selectedTab, Matrix userKernel) {
    Matrix kernel;
    switch (choose) {
    case LOWPASS_FILTER:
        kernel = convolution.getLowPassKernel();
        break;
    case LAPLACE_FILTER:
        kernel = convolution.getLaplaceKernel();
        break;
    case HIGHPASS_FILTER:
        kernel = convolution.getHighPassKernel();
        break;
    case GAUSSIAN_FILTER:
        kernel = convolution.getGaussianKernel(5);
        break;
    case USER_FILTER:
        kernel = userKernel;
        break;
    default:
        break;
    }

    int N = kernel.getNDimension();
    int M = kernel.getMDimension();
    double norm = 0;
    for(int i=0; i<M; ++i)
        for(int j=0; j<N; ++j)
            norm += kernel[i][j];

    int r = 0;
    int g = 0;
    int b = 0;
    QRgb color;
    if (img.format() != QImage::Format_RGB32)
        img = img.convertToFormat(QImage::Format_RGB32);

    QImage res(img.width() - M + 1, img.height() - N + 1, img.format());

    for (int y = N/2; y < img.height() - N/2; ++y) {
        for (int x = M/2; x < img.width() - M/2; ++x) {
            if(selectedTab[y][x] == 1) {
                for (int i = 0; i<M; ++i) {
                    const QRgb *imgLine = reinterpret_cast<const QRgb*>(img.scanLine(y - N/2 + i));
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
                res.setPixel(x - M/2, y - N/2, qRgb(r, g, b));
                r = g = b = 0;
            }
        }
    }
    return res;
}
