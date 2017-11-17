#ifndef BOXBLUR_H
#define BOXBLUR_H

#include <QImage>

class BoxBlur {
public:
    BoxBlur(QImage &image);
    ~BoxBlur();

    QImage blur(int radius, __int8 **selectedTab = nullptr);

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
