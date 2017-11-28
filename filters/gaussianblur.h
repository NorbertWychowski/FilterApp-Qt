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

    QImage blur(int radius, qint8 **selectedTab = nullptr);

private:
    void gaussBlur_4(int* source, int* dest, int r);
    int* boxesForGauss(double sigma, int n);
    void boxBlur_4(int* source, int* dest, int w, int h, int r);

    int* _red;
    int* _green;
    int* _blue;
    int* newRed;
    int* newGreen;
    int* newBlue;

    int _width;
    int _height;
    QImage image;
};

#endif // GAUSSIANBLUR_H
