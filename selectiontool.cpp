#include "selectiontool.h"
#include "labconverter.h"
#include "gaussianblur.h"
#include "filter.h"

#include <QGuiApplication>
#include <QQueue>
#include <array>
#include <QtMath>

#include <QtConcurrent/QtConcurrent>
#include <QFutureSynchronizer>

SelectionTool::SelectionTool(QImage *image) {
    this->image = image;

    selectedTab = new __int8 *[image->height()];
    for (int i = 0; i<image->height(); i++)
        selectedTab[i] = new __int8[image->width()]();
}

SelectionTool::~SelectionTool() {
    for (int i = 0; i<image->height(); i++)
        delete[] selectedTab[i];
    delete[] selectedTab;
}

void SelectionTool::resizeSelectedTab() {
    selectedTab = new __int8 *[image->height()];
    for (int i = 0; i<image->height(); i++)
        selectedTab[i] = new __int8[image->width()]();

    selectedArea = QImage(image->size(), QImage::Format_ARGB32);
    selectedArea.fill(qRgba(0, 0, 0, 0));
}

__int8 ** SelectionTool::getSelectedTab() {
    return selectedTab;
}

QImage SelectionTool::selectByColor(int mouse_x, int mouse_y, int threshold) {
    if (mouse_x < 0 || mouse_y < 0 || mouse_x > image->width() || mouse_y > image->height())
        return QImage();

    if (!(QGuiApplication::keyboardModifiers() & Qt::ControlModifier)) {
        for (int y = 0; y<image->height(); y++)
            for (int x = 0; x<image->width(); x++)
                selectedTab[y][x] = 0;

        selectedArea.fill(qRgba(0, 0, 0, 0));
    }

    QColor baseColor = image->pixelColor(mouse_x, mouse_y);
    QColor color;
    volatile int x1(mouse_x), y1(mouse_y);
    int width = image->width();
    int height = image->height();

    QQueue<std::array<int, 2>> queue;
    queue.push_back({ x1, y1 });
    selectedTab[y1][x1] = true;

    while (!queue.isEmpty()) {
        x1 = queue.front()[0];
        y1 = queue.front()[1];
        queue.pop_front();

        if (x1 + 1 < width && selectedTab[y1][x1 + 1] != 1) {
            color = image->pixelColor(x1 + 1, y1);
            if (LabConverter::compareColors(baseColor, color) < threshold) {
                queue.push_back({ x1 + 1, y1 });
                selectedTab[y1][x1 + 1] = 1;
                selectedArea.setPixelColor(x1 + 1, y1, Qt::transparent);
            } else {
                selectedTab[y1][x1 + 1] = 2;
                selectedArea.setPixelColor(x1 + 1, y1, Qt::green);
            }
        }
        if ((x1 > 0) && selectedTab[y1][x1 - 1] != 1) {
            color = image->pixelColor(x1 - 1, y1);
            if (LabConverter::compareColors(baseColor, color) < threshold) {
                queue.push_back({ x1 - 1, y1 });
                selectedTab[y1][x1 - 1] = 1;
                selectedArea.setPixelColor(x1 - 1, y1, Qt::transparent);
            } else {
                selectedTab[y1][x1 - 1] = 2;
                selectedArea.setPixelColor(x1 - 1, y1, Qt::green);
            }
        }
        if ((y1 + 1 <  height) && selectedTab[y1 + 1][x1] != 1) {
            color = image->pixelColor(x1, y1 + 1);
            if (LabConverter::compareColors(baseColor, color) < threshold) {
                queue.push_back({ x1, y1 + 1 });
                selectedTab[y1 + 1][x1] = 1;
                selectedArea.setPixelColor(x1, y1 + 1, Qt::transparent);
            } else {
                selectedTab[y1 + 1][x1] = 2;
                selectedArea.setPixelColor(x1, y1 + 1, Qt::green);
            }
        }
        if ((y1 > 0) && selectedTab[y1 - 1][x1] != 1) {
            color = image->pixelColor(x1, y1 - 1);
            if (LabConverter::compareColors(baseColor, color) < threshold) {
                queue.push_back({ x1, y1 - 1 });
                selectedTab[y1 - 1][x1] = 1;
                selectedArea.setPixelColor(x1, y1 - 1, Qt::transparent);
            } else {
                selectedTab[y1 - 1][x1] = 2;
                selectedArea.setPixelColor(x1, y1 - 1, Qt::green);
            }
        }
    }

    return selectedArea;
}

QImage SelectionTool::selectByColor(int mouse_x, int mouse_y, int threshold, int feather) {
    if (mouse_x < 0 || mouse_y < 0 || mouse_x > image->width() || mouse_y > image->height())
        return QImage();

    QImage tmp = GaussianBlur(*image).blur(feather);

    if (!(QGuiApplication::keyboardModifiers() & Qt::ControlModifier)) {
        for (int y = 0; y<image->height(); y++)
            for (int x = 0; x<image->width(); x++)
                selectedTab[y][x] = 0;

        selectedArea.fill(qRgba(0, 0, 0, 0));
    }

    QColor baseColor = tmp.pixelColor(mouse_x, mouse_y);
    QColor color;
    volatile int x1(mouse_x), y1(mouse_y);
    int width = image->width();
    int height = image->height();

    QQueue<std::array<int, 2>> queue;
    queue.push_back({ x1, y1 });
    selectedTab[y1][x1] = true;

    while (!queue.isEmpty()) {
        x1 = queue.front()[0];
        y1 = queue.front()[1];
        queue.pop_front();

        if (x1 + 1 < width && selectedTab[y1][x1 + 1] != 1) {
            color = tmp.pixelColor(x1 + 1, y1);
            if (LabConverter::compareColors(baseColor, color) < threshold) {
                queue.push_back({ x1 + 1, y1 });
                selectedTab[y1][x1 + 1] = 1;
                selectedArea.setPixelColor(x1 + 1, y1, Qt::transparent);
            } else {
                selectedTab[y1][x1 + 1] = 2;
                selectedArea.setPixelColor(x1 + 1, y1, Qt::green);
            }
        }
        if ((x1 > 0) && selectedTab[y1][x1 - 1] != 1) {
            color = tmp.pixelColor(x1 - 1, y1);
            if (LabConverter::compareColors(baseColor, color) < threshold) {
                queue.push_back({ x1 - 1, y1 });
                selectedTab[y1][x1 - 1] = 1;
                selectedArea.setPixelColor(x1 - 1, y1, Qt::transparent);
            } else {
                selectedTab[y1][x1 - 1] = 2;
                selectedArea.setPixelColor(x1 - 1, y1, Qt::green);
            }
        }
        if ((y1 + 1 <  height) && selectedTab[y1 + 1][x1] != 1) {
            color = tmp.pixelColor(x1, y1 + 1);
            if (LabConverter::compareColors(baseColor, color) < threshold) {
                queue.push_back({ x1, y1 + 1 });
                selectedTab[y1 + 1][x1] = 1;
                selectedArea.setPixelColor(x1, y1 + 1, Qt::transparent);
            } else {
                selectedTab[y1 + 1][x1] = 2;
                selectedArea.setPixelColor(x1, y1 + 1, Qt::green);
            }
        }
        if ((y1 > 0) && selectedTab[y1 - 1][x1] != 1) {
            color = tmp.pixelColor(x1, y1 - 1);
            if (LabConverter::compareColors(baseColor, color) < threshold) {
                queue.push_back({ x1, y1 - 1 });
                selectedTab[y1 - 1][x1] = 1;
                selectedArea.setPixelColor(x1, y1 - 1, Qt::transparent);
            } else {
                selectedTab[y1 - 1][x1] = 2;
                selectedArea.setPixelColor(x1, y1 - 1, Qt::green);
            }
        }
    }

    return selectedArea;
}

QImage SelectionTool::rectangleSelect(QRect rectSelect) {
    QPoint p1 = rectSelect.topLeft();
    QPoint p2 = rectSelect.bottomRight();

    if (!(QGuiApplication::keyboardModifiers() & Qt::ControlModifier)) {
        for (int y = 0; y<image->height(); y++)
            for (int x = 0; x<image->width(); x++)
                selectedTab[y][x] = 0;

        selectedArea.fill(qRgba(0, 0, 0, 0));
    }
    for(int i = p1.x(); i <= p2.x(); ++i)
        for(int j = p1.y(); j <= p2.y(); ++j) {
            selectedTab[j][i] = 1;
            selectedArea.setPixelColor(i, j, Qt::transparent);
        }
    for(int i = p1.x() - 1; i < p2.x() + 1; ++i) {
        if(selectedTab[p1.y() - 1][i] == 0) {
            selectedTab[p1.y() - 1][i] = 2;
            selectedArea.setPixelColor(i, p1.y() - 1, Qt::green);
        }
        if(selectedTab[p2.y() + 1][i] == 0) {
            selectedTab[p2.y() + 1][i] = 2;
            selectedArea.setPixelColor(i, p2.y() + 1, Qt::green);
        }
    }

    for(int i = p1.y() - 1; i < p2.y() + 1; ++i) {
        if(selectedTab[i][p1.x() - 1] == 0) {
            selectedTab[i][p1.x() - 1] = 2;
            selectedArea.setPixelColor(p1.x() - 1, i, Qt::green);
        }
        if(selectedTab[i][p2.x() + 1] == 0) {
            selectedTab[i][p2.x() + 1] = 2;
            selectedArea.setPixelColor(p2.x() + 1, i, Qt::green);
        }
    }

    return selectedArea;
}

