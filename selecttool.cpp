#include "selecttool.h"

#include "labconverter.h"

#include <QGuiApplication>
#include <QQueue>
#include <array>

SelectTool::SelectTool(QImage *image) {
    this->image = image;

    selectedTab.fill(0, image->width()*image->height());
}

void SelectTool::resizeSelectedTab() {
    selectedTab.fill(0, image->width()*image->height());
}

QVector<qint8> SelectTool::getSelectedTab() {
    return selectedTab;
}

QImage SelectTool::selectByColor(int mouse_x, int mouse_y, int threshold) {
    if(mouse_x < 0 || mouse_y < 0 || mouse_x > image->width() || mouse_y > image->height())
        return QImage();
    QImage selectedArea(image->size(), QImage::Format_ARGB32);
    selectedArea.fill(qRgba(0, 0, 0, 0));

    if (!(QGuiApplication::keyboardModifiers() & Qt::ControlModifier)) {
        selectedTab.fill(0);
    }

    QColor baseColor = image->pixelColor(mouse_x, mouse_y);
    QColor color;
    volatile int row = 0;
    volatile int x1(mouse_x), y1(mouse_y);
    volatile int width = image->width();
    volatile int height = image->height();

    QQueue<std::array<int, 2>> queue;
    queue.push_back({x1, y1});

    while(!queue.isEmpty()) {
        x1 = queue.front()[0];
        y1 = queue.front()[1];
        queue.pop_front();

        row = width * y1;

        if(x1 + 1 < width && selectedTab[row + x1 + 1] != 1) {
            color = image->pixelColor(x1 + 1, y1);
            if(LabConverter::compareColors(baseColor, color) < threshold) {
                queue.push_back({x1 + 1, y1});
                selectedTab[row + x1 + 1] = 1;
            } else {
                selectedTab[row + x1 + 1] = 2;
            }
        }
        if ((x1 - 1 > 0) && selectedTab[row + x1 - 1] != 1) {
            color = image->pixelColor( x1 - 1, y1);
            if (LabConverter::compareColors(baseColor, color) < threshold) {
                queue.push_back({ x1 - 1, y1 });
                selectedTab[row + x1 - 1] = 1;
            } else {
                selectedTab[row + x1 - 1] = 2;
            }
        }
        if ((y1 + 1 <  height) && selectedTab[row + x1 + width] != 1) {
            color = image->pixelColor( x1, y1 + 1);
            if (LabConverter::compareColors(baseColor, color) < threshold) {
                queue.push_back({ x1, y1 + 1 });
                selectedTab[row + x1 + width] = 1;
            } else {
                selectedTab[row + x1 + width] = 2;
            }
        }
        if ((y1 - 1 > 0) && selectedTab[row + x1 - width] != 1) {
            color = image->pixelColor( x1, y1 - 1);
            if (LabConverter::compareColors(baseColor, color) < threshold) {
                queue.push_back({ x1, y1 - 1 });
                selectedTab[row + x1 - width] = 1;
            } else {
                selectedTab[row + x1 - width] = 2;
            }
        }
    }
    row = 0;
    for(int y=0; y<height; y++) {
        for(int x=0; x<width; x++) {
            if(selectedTab[row + x] == 2) {
                selectedArea.setPixelColor(x, y, Qt::green);
            }
        }
        row+=width;
    }
    return selectedArea;
}
