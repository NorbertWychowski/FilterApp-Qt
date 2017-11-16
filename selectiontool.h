#ifndef SELECTIONTOOL_H
#define SELECTIONTOOL_H

#include <QVector>
#include <QImage>

class SelectionTool {
public:
    SelectionTool(QImage *image);
    ~SelectionTool();

    void resizeSelectedTab();
    int ** getSelectedTab();

    QImage selectByColor(int mouse_x, int mouse_y, int threshold);
    QImage selectByColor(int mouse_x, int mouse_y, int threshold, int feather);
    QImage rectangleSelect(QPoint p1, QPoint p2);

private:
    int **selectedTab;
    QImage *image;
};

#endif // SELECTIONTOOL_H
