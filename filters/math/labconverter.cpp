#include "labconverter.h"

#include <QtMath>

qreal LabConverter::compareColors(const QColor &A, const QColor &B) {
    std::array<qreal, 3> lab1 = toLAB(A);
    std::array<qreal, 3> lab2 = toLAB(B);
    return qSqrt(qPow(lab2[0] - lab1[0], 2) + qPow(lab2[1] - lab1[1], 2) + qPow(lab2[2] - lab1[2], 2));;
}

std::array<qreal, 3> LabConverter::toLAB(const QColor &color) {
    //RGB to XYZ
    qreal r, g, b, X, Y, Z, fx, fy, fz, xr, yr, zr;
    r = color.red() / 255.0;
    g = color.green() / 255.0;
    b = color.blue() / 255.0;

    if (r <= 0.04045)	r = r / 12.92;
    else				r = qPow((r + 0.055) / 1.055, 2.4);

    if (g <= 0.04045)	g = g / 12.92;
    else				g = qPow((g + 0.055) / 1.055, 2.4);

    if (b <= 0.04045)	b = b / 12.92;
    else				b = qPow((b + 0.055) / 1.055, 2.4);

    r *= 100;
    g *= 100;
    b *= 100;

    X = 0.4124  * r + 0.3576 * g + 0.1805 * b;
    Y = 0.2126 * r + 0.7152 * g + 0.0722 * b;
    Z = 0.0193  * r + 0.1192 * g + 0.9505 * b;

    // XYZ to Lab

    xr = X / 100.0;
    yr = Y / 100.0;
    zr = Z / 100.0;

    if (xr > 0.008856)  fx = qPow(xr, 0.33333333333333333333333333333333);
    else                fx = (7.787 * xr) + (0.13793103448275862068965517241379); // 16.0 / 116.0

    if (yr > 0.008856)  fy = qPow(yr, 0.33333333333333333333333333333333);
    else                fy = (7.787 * yr) + (0.13793103448275862068965517241379); // 16.0 / 116.0

    if (zr > 0.008856)  fz = qPow(zr, 0.33333333333333333333333333333333);
    else				fz = (7.787 * zr) + (0.13793103448275862068965517241379); // 16.0 / 116.0

    return std::array<qreal, 3> {(116.0 * fy) - 16.0, 500.0 * (fx - fy), 200.0 * (fy - fz)};
}

