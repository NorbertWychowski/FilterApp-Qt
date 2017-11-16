#ifndef CONVOLUTION_H
#define CONVOLUTION_H

#include <matrix.h>


class Convolution {
public:

    ~Convolution();

    Matrix getLaplaceKernel();
    Matrix getHighPassKernel();

private:
    Matrix laplaceKernel = {
        { -4, -1, 0, -1, -4 },
        { -1,  2, 3,  2, -1 },
        {  0,  3, 4,  3,  0 },
        { -1,  2, 3,  2, -1 },
        { -4, -1, 0, -1, -4 }
    };
    Matrix highPassKernel = {
        {  0, -1, -1, -1,  0 },
        { -1,  2, -4,  2, -1 },
        { -1, -4, 13, -4, -1 },
        { -1,  2, -4,  2, -1 },
        {  0, -1, -1, -1,  0 }
    };
};

#endif // CONVOLUTION_H
