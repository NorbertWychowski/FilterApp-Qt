#ifndef FILTER_H
#define FILTER_H

#include "math/matrix.h"
#include <vector>
#include <QImage>

enum FILTER {
    GAUSSIAN_FILTER,
    LOWPASS_FILTER,
    LAPLACE_FILTER,
    HIGHPASS_FILTER,
    USER_FILTER,
};

class FilterTool {
public:
    static QImage splot(QImage &img, FILTER choose, Matrix userKernel = Matrix());
    static QImage splot(QImage &img, FILTER choose, qint8 **selectedTab, Matrix userKernel = Matrix());

};

#endif // FILTER_H
