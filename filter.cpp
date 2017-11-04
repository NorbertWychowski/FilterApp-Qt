#include "filter.h"

Filter::Filter() {
}

QImage Filter::splot(QImage &img, int choose, int *tab) {
    if (img.format() != QImage::Format_RGB32)
        img = img.convertToFormat(QImage::Format_RGB32);

    QImage res(img.width() - 4, img.height() - 4, img.format());

    int *mask;
    switch (choose) {
    case 1:
        mask = mask1;
        break;
    case 2:
        mask = mask2;
        break;
    case 3:
        mask = mask3;
        break;
    case 4:
        mask = mask4;
        break;
    case 5:
        mask = tab;
        break;
    default:
        break;
    }

    int norm = 0;
    for (int i = 0; i<25; i++) {
        norm += mask[i];
    }

    int r = 0;
    int g = 0;
    int b = 0;
    QRgb color;

    for (int y = 2; y < img.height() - 2; y++) {
        for (int x = 2; x < img.width() - 2; x++) {
            for (int i = 0; i < 5; i++) {
                const QRgb *imgLine = reinterpret_cast<const QRgb*>(img.scanLine(y - 2 + i));
                for (int j = 0; j < 5; j++) {
                    color = imgLine[x - 2 + j];
                    r += mask[i * 5 + j] * qRed(color);
                    g += mask[i * 5 + j] * qGreen(color);
                    b += mask[i * 5 + j] * qBlue(color);
                }
            }
            if (norm != 0) {
                r = qBound(0, r / norm, 255);
                g = qBound(0, g / norm, 255);
                b = qBound(0, b / norm, 255);
            } else {
                r = qBound(0, r, 255);
                g = qBound(0, g, 255);
                b = qBound(0, b, 255);
            }
            res.setPixel(x - 2, y - 2, qRgb(r, g, b));
            r = g = b = 0;
        }
    }

    return res;
}

QImage Filter::splot(QImage &img, int choose, int ** selectedTab, int *tab) {
    if (img.format() != QImage::Format_RGB32)
        img = img.convertToFormat(QImage::Format_RGB32);

    QImage res(img.width() - 4, img.height() - 4, img.format());

    int *mask;
    switch (choose) {
    case 1:
        mask = mask1;
        break;
    case 2:
        mask = mask2;
        break;
    case 3:
        mask = mask3;
        break;
    case 4:
        mask = mask4;
        break;
    case 5:
        mask = tab;
        break;
    default:
        break;
    }

    int norm = 0;
    for (int i = 0; i<25; i++) {
        norm += mask[i];
    }

    int r = 0;
    int g = 0;
    int b = 0;
    QRgb color;

    for (int y = 2; y < img.height() - 2; y++) {
        for (int x = 2; x < img.width() - 2; x++) {
            if(selectedTab[y][x] == 1) {
                for (int i = 0; i < 5; i++) {
                    const QRgb *imgLine = reinterpret_cast<const QRgb*>(img.scanLine(y - 2 + i));
                    for (int j = 0; j < 5; j++) {
                        color = imgLine[x - 2 + j];
                        r += mask[i * 5 + j] * qRed(color);
                        g += mask[i * 5 + j] * qGreen(color);
                        b += mask[i * 5 + j] * qBlue(color);
                    }
                }
                if (norm != 0) {
                    r = qBound(0, r / norm, 255);
                    g = qBound(0, g / norm, 255);
                    b = qBound(0, b / norm, 255);
                } else {
                    r = qBound(0, r, 255);
                    g = qBound(0, g, 255);
                    b = qBound(0, b, 255);
                }
                res.setPixel(x - 2, y - 2, qRgb(r, g, b));
                r = g = b = 0;
            } else {
                res.setPixel(x - 2, y - 2, img.pixel(x - 2, y - 2));
            }
        }
    }

    return res;
}
