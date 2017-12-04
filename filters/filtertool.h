#ifndef FILTER_H
#define FILTER_H

#include "math/matrix.h"
#include <vector>
#include <QImage>
#include <QProgressBar>

enum FILTER {
    GAUSSIAN_FILTER,
    LOWPASS_FILTER,
    LAPLACE_FILTER,
    HIGHPASS_FILTER,
    USER_FILTER,
};

class FilterTool {

public:
    QImage splot(QImage &img, FILTER choose, Matrix userKernel = Matrix());
    QImage splot(QImage &img, FILTER choose, qint8 **selectedTab, Matrix userKernel = Matrix());
    QImage gaussianFilter(QImage &image, int radius, qint8 **selectedTab = nullptr);
    QImage lowPassFilter(QImage &image, int radius, qint8 **selectedTab = nullptr);
};

#endif // FILTER_H
