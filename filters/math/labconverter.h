#ifndef LABCONVERTER_H
#define LABCONVERTER_H

#include <QColor>
#include <array>

class LabConverter {
public:
    static qreal compareColors(const QColor &A, const QColor &B);

    static std::array<qreal, 3> toLAB(const QColor &color);
};

#endif // LABCONVERTER_H
