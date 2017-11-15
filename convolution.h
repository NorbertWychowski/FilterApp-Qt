#ifndef CONVOLUTION_H
#define CONVOLUTION_H

#include <matrix.h>


class Convolution {
public:

    ~Convolution();

    static Matrix getGaussianKernel(const int radius); //N = 2*radius + 1 e.g. radius = 5 -> N = 11
    Matrix getLowPassKernel();
    Matrix getLaplaceKernel();
    Matrix getHighPassKernel();

private:
    Matrix lowPassKernel = {
        { 1.0, 1.0, 1.0, 1.0, 1.0 },
        { 1.0, 1.0, 1.0, 1.0, 1.0 },
        { 1.0, 1.0, 1.0, 1.0, 1.0 },
        { 1.0, 1.0, 1.0, 1.0, 1.0 },
        { 1.0, 1.0, 1.0, 1.0, 1.0 }
    };
    Matrix laplaceKernel = {
        { 0,  0,  0,  0, 0 },
        { 0,  0, -1,  0, 0 },
        { 0, -1,  4, -1, 0 },
        { 0,  0, -1,  0, 0 },
        { 0,  0,  0,  0, 0 }
    };
    Matrix highPassKernel = {
        { 0, -1, -1, -1,  0 },
        { -1,  2, -4,  2, -1 },
        { -1, -4, 13, -4, -1 },
        { -1,  2, -4,  2, -1 },
        { 0, -1, -1, -1,  0 }
    };
};

#endif // CONVOLUTION_H
