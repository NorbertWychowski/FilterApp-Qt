#include "colortool.h"

#include <QtConcurrent/QtConcurrent>
#include <QFutureSynchronizer>

QImage ColorTool::negative(QImage img) {
    int width = img.width();
    int height = img.height();

    QImage tmp = img.copy();

    for (int y = 0; y < height; ++y) {
        QRgb *line = reinterpret_cast<QRgb *>(tmp.scanLine(y));

        for (int x = 0; x < width; ++x) {
            int R = 255 - qRed(line[x]);
            int G = 255 - qGreen(line[x]);
            int B = 255 - qBlue(line[x]);
            line[x] = qRgb(R, G, B);
        }
    }

    return tmp;
}

QImage ColorTool::desaturate(QImage img) {
    int width = img.width();
    int height = img.height();

    QImage tmp = img.copy();

    for (int y = 0; y < height; ++y) {
        QRgb *line = reinterpret_cast<QRgb *>(tmp.scanLine(y));

        for (int x = 0; x < width; ++x) {
            int R = qRed(line[x]);
            int G = qGreen(line[x]);
            int B = qBlue(line[x]);
            int gray = (R + G + B) / 3;
            line[x] = qRgb(gray, gray, gray);
        }
    }

    return tmp;
}

QImage ColorTool::sepia(QImage img) {
    int width = img.width();
    int height = img.height();
    int W = 30;

    QImage tmp = img.copy();

    for (int y = 0; y < height; ++y) {
        QRgb *line = reinterpret_cast<QRgb *>(tmp.scanLine(y));

        for (int x = 0; x < width; ++x) {
            int R = qRed(line[x]);
            int G = qGreen(line[x]);
            int B = qBlue(line[x]);
            int gray = (R + G + B) / 3;
            R = qBound(0, gray + 2 * W, 255);
            G = qBound(0, gray + W, 255);
            B = gray;

            line[x] = qRgb(R, G, B);
        }
    }

    return tmp;
}

QImage ColorTool::colorize(int H, int S, int V, QImage img) {
    int maxThread = QThread::idealThreadCount();
    int width = img.width();
    int height = img.height();

    QImage tmp = img.copy();
    auto colorizeLambda = [&](int start, int end) {
        for (int y = start; y < end; ++y) {
            QRgb *line = reinterpret_cast<QRgb *>(tmp.scanLine(y));

            for (int x = 0; x < width; ++x) {
                QColor c(line[x]);
                line[x] = QColor::fromHsv(H, S, qBound(0, c.value() + V, 255)).rgb();
            }
        }
    };

    QFutureSynchronizer<void> futures;

    for (int i = 0; i < maxThread; ++i) {
        futures.addFuture(QtConcurrent::run(colorizeLambda, i * height / maxThread, (i + 1.0)*height / maxThread));
    }

    futures.waitForFinished();

    return tmp;
}

QImage ColorTool::hueSaturation(int H, int S, int V, QImage img) {
    int maxThread = QThread::idealThreadCount();
    int width = img.width();
    int height = img.height();

    QImage tmp = img.copy();
    auto saturation = [&](int start, int end) {
        for (int y = start; y < end; ++y) {
            QRgb *line = reinterpret_cast<QRgb *>(tmp.scanLine(y));

            for (int x = 0; x < width; ++x) {
                QColor c(line[x]);
                int newH = c.hue() + H;

                if (newH < 0) newH += 360;
                else if (newH > 359) newH -= 360;

                line[x] = QColor::fromHsv(newH, qBound(0, c.saturation() + S, 255), qBound(0, c.value() + V, 255)).rgb();
            }
        }
    };

    QFutureSynchronizer<void> futures;

    for (int i = 0; i < maxThread; ++i) {
        futures.addFuture(QtConcurrent::run(saturation, i * height / maxThread, (i + 1.0)*height / maxThread));
    }

    futures.waitForFinished();

    return tmp;
}

QImage ColorTool::brightnessContrast(int Br, int C, QImage img) {
    const double F = (259.0 * (C + 255)) / (255.0 * (259 - C));
    int width = img.width();
    int height = img.height();

    QImage tmp = img.copy();

    for (int y = 0; y < height; ++y) {
        QRgb *line = reinterpret_cast<QRgb *>(tmp.scanLine(y));

        for (int x = 0; x < width; ++x) {
            int R = qBound(0, static_cast<int>(F * (qRed(line[x]) - 128) + 128) + Br, 255);
            int G = qBound(0, static_cast<int>(F * (qGreen(line[x]) - 128) + 128) +  Br, 255);
            int B = qBound(0, static_cast<int>(F * (qBlue(line[x]) - 128) + 128) + Br, 255);

            line[x] = qRgb(R, G, B);
        }
    }

    return tmp;
}
