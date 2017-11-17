#ifndef LABCONVERTER_H
#define LABCONVERTER_H

#include <QColor>
#include <array>

class LabConverter {
public:
    static qreal compareColors(QColor &A, QColor &B);

    static std::array<qreal, 3> toLAB(QColor &color);
};

#endif // LABCONVERTER_H
