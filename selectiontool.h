#ifndef SELECTIONTOOL_H
#define SELECTIONTOOL_H

#include <QVector>
#include <QImage>

class SelectionTool {
public:
    SelectionTool(QImage *image);
    ~SelectionTool();

    void resizeSelectedTab();
    __int8 ** getSelectedTab();

    QImage selectByColor(int mouse_x, int mouse_y, int threshold);
    QImage selectByColor(int mouse_x, int mouse_y, int threshold, int feather);
    QImage rectangleSelect(QRect rectSelect);

private:
    __int8 **selectedTab;
    QImage *image;
    QImage selectedArea;
};

#endif // SELECTIONTOOL_H
