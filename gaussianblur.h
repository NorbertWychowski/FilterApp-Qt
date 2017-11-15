#ifndef GAUSSIANBLUR_H
#define GAUSSIANBLUR_H

/*
 * (Super) Fast Gaussian Blur C# Implementation
 * https://github.com/mdymel/superfastblur/blob/master/SuperfastBlur/GaussianBlur.cs
 */

#include <QImage>

class GaussianBlur : public QObject {

    Q_OBJECT
public:
    GaussianBlur(QImage &image);
    ~GaussianBlur();

    QImage blur(int radius, int **selectedTab = nullptr);

private:
    void gaussBlur_4(int* source, int* dest, int r);
    int* boxesForGauss(int sigma, int n);
    void boxBlur_4(int* source, int* dest, int w, int h, int r);

    QRgb* source;
    int* _red;
    int* _green;
    int* _blue;
    int* newRed;
    int* newGreen;
    int* newBlue;
    int* dest ;

    int _width;
    int _height;
    QImage image;
};

#endif // GAUSSIANBLUR_H
