#ifndef FILTR_H
#define FILTR_H

#include <QImage>

class Filter {
public:
    Filter();

    QImage splot(QImage &img, int choose, int *tab = nullptr);
    QImage splot(QImage &img, int choose, QVector<qint8> selectedTab, int *tab = nullptr);

private:
    //rozmycie - usredniajacy, dolnoprzepustowy
    int mask1[25] = {
        1, 1, 1, 1, 1,
        1, 1, 1, 1, 1,
        1, 1, 1, 1, 1,
        1, 1, 1, 1, 1,
        1, 1, 1, 1, 1,
    };
    //laplace - wykrywanie krawedzi
    int mask2[25] = {
        0,  0,  0,  0, 0,
        0,  0, -1,  0, 0,
        0, -1,  4, -1, 0,
        0,  0, -1,  0, 0,
        0,  0,  0,  0, 0,
    };
    //gauss
    int mask3[25] = {
        1,  4,  7,  4, 1,
        4, 16, 26, 16, 4,
        7, 26, 41, 26, 7,
        4, 26, 16, 26, 4,
        1,  4,  7,  4, 1,
    };
    //gornoprzepustowy
    int mask4[25] = {
        0, -1, -1, -1,  0,
        -1,  2, -4,  2, -1,
        -1, -4, 13, -4, -1,
        -1,  2, -4,  2, -1,
        0, -1, -1, -1,  0,
    };
};

#endif // FILTR_H
