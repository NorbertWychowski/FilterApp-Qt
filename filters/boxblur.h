#ifndef BOXBLUR_H
#define BOXBLUR_H

#include <QImage>

class BoxBlur {
public:
    BoxBlur(const QImage &image);
    ~BoxBlur();

    QImage blur(int radius, qint8 **selectedTab = nullptr);

private:
    void boxBlur(int *source, int *dest, int w, int h, int r);

    int *_red;
    int *_green;
    int *_blue;
    int *newRed;
    int *newGreen;
    int *newBlue;
    int _width;
    int _height;
    QImage image;
};

#endif // BOXBLUR_H
