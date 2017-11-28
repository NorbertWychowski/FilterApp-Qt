#include "colortool.h"

#include <QtConcurrent/QtConcurrent>
#include <QFutureSynchronizer>

QImage ColorTool::negative(QImage &img) {
    int width = img.width();
    int height = img.height();

    for(int y = 0; y<height; ++y) {
        QRgb *line = (QRgb*)img.scanLine(y);
        for(int x = 0; x < width; ++x) {
            int R = 255 - qRed(line[x]);
            int G = 255 - qGreen(line[x]);
            int B = 255 - qBlue(line[x]);
            line[x] = qRgb(R, G, B);
        }
    }
    return img;
}

QImage ColorTool::desaturate(QImage &img) {
    int width = img.width();
    int height = img.height();

    for(int y = 0; y<height; ++y) {
        QRgb *line = (QRgb*)img.scanLine(y);
        for(int x = 0; x < width; ++x) {
            int R = qRed(line[x]);
            int G = qGreen(line[x]);
            int B = qBlue(line[x]);
            int gray = (R + G + B) / 3;
            line[x] = qRgb(gray, gray, gray);
        }
    }

    return img;
}

QImage ColorTool::sepia(QImage &img) {
    int width = img.width();
    int height = img.height();
    int W = 30;

    for(int y = 0; y<height; ++y) {
        QRgb* line = (QRgb*)img.scanLine(y);
        for(int x = 0; x < width; ++x) {
            int R = qRed(line[x]);
            int G = qGreen(line[x]);
            int B = qBlue(line[x]);
            int gray = (R + G + B) / 3;
            R = qBound(0, gray + 2*W, 255);
            G = qBound(0, gray + W, 255);
            B = gray;

            line[x] = qRgb(R, G, B);
        }
    }

    return img;
}

QImage ColorTool::colorize(QColor color, QImage &img) {
    int maxThread = QThread::idealThreadCount();
    int width = img.width();
    int height = img.height();

    int H, S, V;

    color.getHsv(&H, &S, &V);

    auto colorizeLambda = [=](int start, int end) {
        for(int y = start; y<end; ++y) {
            QRgb *line = (QRgb*)img.scanLine(y);
            for(int x = 0; x < width; ++x) {
                int Vt = qBound(0, QColor(line[x]).toHsv().value() + V, 255);
                line[x] = QColor::fromHsv(H, S, Vt).rgb();
            }
        }
    };

    QFutureSynchronizer<void> futures;
    for(int i=0; i<maxThread; ++i) {
        futures.addFuture(QtConcurrent::run(colorizeLambda, i*height/maxThread, (i+1.0)*height/maxThread));
    }

    futures.waitForFinished();

    return img;
}
