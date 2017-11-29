#ifndef COLORTOOL_H
#define COLORTOOL_H

#include <QImage>

enum COLOR_FILTER {
    NEGATIVE,
    SEPIA,
    DESATURATE,
    HUESATURATION,
    COLORIZE,
    BRIGHTNESSCONTRAST,
};

class ColorTool {
public:
    static QImage negative(QImage img);
    static QImage desaturate(QImage img);
    static QImage sepia(QImage img);
    static QImage colorize(int H, int S, int V, QImage img);
    static QImage hueSaturation(int H, int S, int V, QImage img);
    static QImage brightnessContrast(int Br, int C, QImage img);
};

#endif // COLORTOOL_H
