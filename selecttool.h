#ifndef SELECTTOOL_H
#define SELECTTOOL_H

#include <QVector>
#include <QImage>

class SelectTool {
public:
    SelectTool(QImage *image);
    ~SelectTool();

    void resizeSelectedTab();
    int ** getSelectedTab();

    QImage selectByColor(int mouse_x, int mouse_y, int threshold);
    QImage selectByColor(int mouse_x, int mouse_y, int threshold, int feather);
    double ** createGaussianKernel(double sigma, int N, int M);

    void p();

private:
    int **selectedTab;
    QImage *image;
};

#endif // SELECTTOOL_H
