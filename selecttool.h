#ifndef SELECTTOOL_H
#define SELECTTOOL_H

#include <QVector>
#include <QImage>

class SelectTool {
public:
    SelectTool(QImage *image);

    void resizeSelectedTab();
    QVector<qint8> getSelectedTab();

    QImage selectByColor(int mouse_x, int mouse_y, int threshold);

    void p();

private:
    QVector<qint8> selectedTab;
    QImage *image;
};

#endif // SELECTTOOL_H
