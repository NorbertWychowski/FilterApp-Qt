#include "convolution.h"

#include <QtMath>

Convolution::~Convolution() {
    laplaceKernel.deleteMatrix();
    lowPassKernel.deleteMatrix();
    highPassKernel.deleteMatrix();
}

Matrix Convolution::getGaussianKernel(const int radius) {
    int N = 2 * radius + 1;
    double sigma = qSqrt(-(N*N) / (2 * log(1.0 / 255.0)));

    Matrix matrix(N, N);

    double r, s = 2*sigma*sigma;
    double sum = 0;
    for(int i=-radius; i<= radius; i++) {
        for(int j=-radius; j<=radius; j++) {
            r = qSqrt(i*i + j*j);
            matrix[i+radius][j+radius] = (qExp(-(r*r) / s)) / (M_PI*s);
            sum += matrix[i+radius][j+radius];
        }
    }

    for(int i=0; i<N; i++)
        for(int j=0; j<N; j++)
            matrix[i][j] /= sum;

    return matrix;
}

Matrix Convolution::getLowPassKernel() {
    return lowPassKernel;
}

Matrix Convolution::getLaplaceKernel() {
    return laplaceKernel;
}

Matrix Convolution::getHighPassKernel() {
    return highPassKernel;
}
