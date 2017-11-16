#include "convolution.h"

#include <QtMath>

Convolution::~Convolution() {
    laplaceKernel.deleteMatrix();
    highPassKernel.deleteMatrix();
}

Matrix Convolution::getLaplaceKernel() {
    return laplaceKernel;
}

Matrix Convolution::getHighPassKernel() {
    return highPassKernel;
}
