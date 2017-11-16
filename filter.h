#ifndef FILTR_H
#define FILTR_H

#include "convolution.h"
#include <vector>
#include <QImage>

enum FILTER {
    GAUSSIAN_FILTER = 1,
    LOWPASS_FILTER = 2,
    LAPLACE_FILTER = 3,
    HIGHPASS_FILTER = 4,
    USER_FILTER = 5
};

class Filter {
public:
    Filter();

    QImage splot(QImage &img, FILTER choose, Matrix userKernel = Matrix());
    QImage splot(QImage &img, FILTER choose, int **selectedTab, Matrix userKernel = Matrix());

private:
    Convolution convolution;

};

#endif // FILTR_H
