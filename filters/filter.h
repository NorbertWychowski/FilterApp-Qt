#ifndef FILTER_H
#define FILTER_H

#include "math/matrix.h"
#include <vector>
#include <QImage>

enum FILTER {
    GAUSSIAN_FILTER = 1,
    LOWPASS_FILTER  = 2,
    LAPLACE_FILTER  = 3,
    HIGHPASS_FILTER = 4,
    USER_FILTER     = 5,
};

class Filter {
public:
    Filter();

    QImage splot(QImage &img, FILTER choose, Matrix userKernel = Matrix());
    QImage splot(QImage &img, FILTER choose, __int8 **selectedTab, Matrix userKernel = Matrix());

};

#endif // FILTER_H
