#include "selecttool.h"
#include "labconverter.h"
#include "filter.h"

#include <QGuiApplication>
#include <QQueue>
#include <array>
#include <QtMath>

#include <QDebug>

SelectTool::SelectTool(QImage *image) {
    this->image = image;

    selectedTab = new int *[image->height()];
    for (int i = 0; i<image->height(); i++)
        selectedTab[i] = new int[image->width()]();
}

SelectTool::~SelectTool() {
    for (int i = 0; i<image->height(); i++)
        delete[] selectedTab[i];
    delete[] selectedTab;
}

void SelectTool::resizeSelectedTab() {
    selectedTab = new int *[image->height()];
    for (int i = 0; i<image->height(); i++)
        selectedTab[i] = new int[image->width()]();
}

int ** SelectTool::getSelectedTab() {
    return selectedTab;
}

QImage SelectTool::selectByColor(int mouse_x, int mouse_y, int threshold) {
    if (mouse_x < 0 || mouse_y < 0 || mouse_x > image->width() || mouse_y > image->height())
        return QImage();
    QImage selectedArea(image->size(), QImage::Format_ARGB32);
    selectedArea.fill(qRgba(0, 0, 0, 0));

    if (!(QGuiApplication::keyboardModifiers() & Qt::ControlModifier)) {
        for (int y = 0; y<image->height(); y++)
            for (int x = 0; x<image->width(); x++)
                selectedTab[y][x] = 0;
    }

    QColor baseColor = image->pixelColor(mouse_x, mouse_y);
    QColor color;
    volatile int x1(mouse_x), y1(mouse_y);
    int width = image->width();
    int height = image->height();

    QQueue<std::array<int, 2>> queue;
    queue.push_back({ x1, y1 });
    selectedTab[y1][x1] = 1;

    while (!queue.isEmpty()) {
        x1 = queue.front()[0];
        y1 = queue.front()[1];
        queue.pop_front();

        if (x1 + 1 < width && selectedTab[y1][x1 + 1] != 1) {
            color = image->pixelColor(x1 + 1, y1);
            if (LabConverter::compareColors(baseColor, color) < threshold) {
                queue.push_back({ x1 + 1, y1 });
                selectedTab[y1][x1 + 1] = 1;
            } else {
                selectedTab[y1][x1 + 1] = 2;
            }
        }
        if ((x1 - 1 >= 0) && selectedTab[y1][x1 - 1] != 1) {
            color = image->pixelColor(x1 - 1, y1);
            if (LabConverter::compareColors(baseColor, color) < threshold) {
                queue.push_back({ x1 - 1, y1 });
                selectedTab[y1][x1 - 1] = 1;
            } else {
                selectedTab[y1][x1 - 1] = 2;
            }
        }
        if ((y1 + 1 <  height) && selectedTab[y1 + 1][x1] != 1) {
            color = image->pixelColor(x1, y1 + 1);
            if (LabConverter::compareColors(baseColor, color) < threshold) {
                queue.push_back({ x1, y1 + 1 });
                selectedTab[y1 + 1][x1] = 1;
            } else {
                selectedTab[y1 + 1][x1] = 2;
            }
        }
        if ((y1 - 1 >= 0) && selectedTab[y1 - 1][x1] != 1) {
            color = image->pixelColor(x1, y1 - 1);
            if (LabConverter::compareColors(baseColor, color) < threshold) {
                queue.push_back({ x1, y1 - 1 });
                selectedTab[y1 - 1][x1] = 1;
            } else {
                selectedTab[y1 - 1][x1] = 2;
            }
        }
    }
    for (int y = 0; y<height; y++) {
        for (int x = 0; x<width; x++) {
            if (selectedTab[y][x] == 2) {
                selectedArea.setPixelColor(x, y, Qt::green);
            }
        }
    }

    return selectedArea;
}

QImage SelectTool::selectByColor(int mouse_x, int mouse_y, int threshold, int feather) {
    if (mouse_x < 0 || mouse_y < 0 || mouse_x > image->width() || mouse_y > image->height())
        return QImage();
    QImage selectedArea(image->size(), QImage::Format_ARGB32);
    selectedArea.fill(qRgba(0, 0, 0, 0));
    QImage tmp;

    if (!(QGuiApplication::keyboardModifiers() & Qt::ControlModifier)) {
        for (int y = 0; y<image->height(); y++)
            for (int x = 0; x<image->width(); x++)
                selectedTab[y][x] = 0;
    }
    Filter f ;
    tmp = f.splot(*image, USER_FILTER, Convolution::getGaussianKernel(((feather>>1)<<1) + 1));

    QColor baseColor = tmp.pixelColor(mouse_x, mouse_y);
    QColor color;
    volatile int x1(mouse_x), y1(mouse_y);
    int width = image->width();
    int height = image->height();

    QQueue<std::array<int, 2>> queue;
    queue.push_back({ x1, y1 });
    selectedTab[y1][x1] = 1;

    while (!queue.isEmpty()) {
        x1 = queue.front()[0];
        y1 = queue.front()[1];
        queue.pop_front();

        if (x1 + 1 < width && selectedTab[y1][x1 + 1] != 1) {
            color = tmp.pixelColor(x1 + 1, y1);
            if (LabConverter::compareColors(baseColor, color) < threshold) {
                queue.push_back({ x1 + 1, y1 });
                selectedTab[y1][x1 + 1] = 1;
            } else {
                selectedTab[y1][x1 + 1] = 2;
            }
        }
        if ((x1 - 1 >= 0) && selectedTab[y1][x1 - 1] != 1) {
            color = tmp.pixelColor(x1 - 1, y1);
            if (LabConverter::compareColors(baseColor, color) < threshold) {
                queue.push_back({ x1 - 1, y1 });
                selectedTab[y1][x1 - 1] = 1;
            } else {
                selectedTab[y1][x1 - 1] = 2;
            }
        }
        if ((y1 + 1 <  height) && selectedTab[y1 + 1][x1] != 1) {
            color = tmp.pixelColor(x1, y1 + 1);
            if (LabConverter::compareColors(baseColor, color) < threshold) {
                queue.push_back({ x1, y1 + 1 });
                selectedTab[y1 + 1][x1] = 1;
            } else {
                selectedTab[y1 + 1][x1] = 2;
            }
        }
        if ((y1 - 1 >= 0) && selectedTab[y1 - 1][x1] != 1) {
            color = tmp.pixelColor(x1, y1 - 1);
            if (LabConverter::compareColors(baseColor, color) < threshold) {
                queue.push_back({ x1, y1 - 1 });
                selectedTab[y1 - 1][x1] = 1;
            } else {
                selectedTab[y1 - 1][x1] = 2;
            }
        }
    }
    for (int y = 0; y<height; y++) {
        for (int x = 0; x<width; x++) {
            if (selectedTab[y][x] == 2) {
                selectedArea.setPixelColor(x, y, Qt::green);
            }
        }
    }
    return selectedArea;
}
