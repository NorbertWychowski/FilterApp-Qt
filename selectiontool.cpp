#include "selectiontool.h"

#include "filters/gaussianblur.h"
#include "filters/filtertool.h"
#include "filters/math/labconverter.h"

#include <QGuiApplication>
#include <QQueue>
#include <array>
#include <QtMath>
#include <QPainter>

SelectionTool::SelectionTool(QImage *image) {
    this->image = image;

    selectedTab = new qint8 *[static_cast<uint>(image->height())];

    for (int i = 0; i < image->height(); i++)
        selectedTab[i] = new qint8[static_cast<uint>(image->width())]();
}

SelectionTool::~SelectionTool() {
    for (int i = 0; i < image->height(); i++)
        delete[] selectedTab[i];

    delete[] selectedTab;
}

void SelectionTool::resizeSelectedTab() {
    selectedTab = new qint8 *[static_cast<uint>(image->height())];

    for (int i = 0; i < image->height(); i++)
        selectedTab[i] = new qint8[static_cast<uint>(image->width())]();

    selectedArea = QImage(image->size(), QImage::Format_ARGB32);
    selectedArea.fill(qRgba(0, 0, 0, 0));
}

qint8 **SelectionTool::getSelectedTab() {
    return selectedTab;
}

QImage SelectionTool::selectByColor(int mouse_x, int mouse_y, int threshold) {
    if (mouse_x < 0 || mouse_y < 0 || mouse_x > image->width() || mouse_y > image->height())
        return QImage();

    if (!(QGuiApplication::keyboardModifiers() & Qt::ControlModifier)) {
        for (int y = 0; y < image->height(); y++)
            for (int x = 0; x < image->width(); x++)
                selectedTab[y][x] = 0;

        selectedArea.fill(qRgba(0, 0, 0, 0));
    }

    QColor baseColor = image->pixelColor(mouse_x, mouse_y);
    int width = image->width();
    int height = image->height();

    QQueue<std::array<int, 2>> queue;
    queue.push_back({ mouse_x, mouse_y });

    int count = 0;

    while (!queue.isEmpty()) {
        int x1 = qBound(0, queue.front()[0], width - 1);
        int y1 = qBound(0, queue.front()[1], height - 1);
        queue.pop_front();

        if (selectedTab[y1][x1] != 1) {
            if (LabConverter::compareColors(baseColor, image->pixelColor(x1, y1)) < threshold) {
                if (selectedTab[y1][x1] == 2)
                    selectedArea.setPixelColor(x1, y1, Qt::transparent);

                selectedTab[y1][x1] = 1;

                queue.push_back({x1 + 1, y1});
                queue.push_back({x1 - 1, y1});
                queue.push_back({x1, y1 + 1});
                queue.push_back({x1, y1 - 1});
            } else {
                count++;

                if (count == 8)
                    count = 0;

                selectedTab[y1][x1] = 2;

                if (count < 4) {
                    selectedArea.setPixelColor(x1, y1, Qt::white);
                } else if (count < 8) {
                    selectedArea.setPixelColor(x1, y1, Qt::black);
                }
            }
        }
    }

    return selectedArea;
}

QImage SelectionTool::selectByColor(int mouse_x, int mouse_y, int threshold, int feather) {
    if (mouse_x < 0 || mouse_y < 0 || mouse_x > image->width() || mouse_y > image->height())
        return QImage();

    QImage tmp = *image;
    QPainter painter(&tmp);

    if (!(QGuiApplication::keyboardModifiers() & Qt::ControlModifier)) {
        for (int y = 0; y < image->height(); y++)
            for (int x = 0; x < image->width(); x++)
                selectedTab[y][x] = 0;

        selectedArea.fill(qRgba(0, 0, 0, 0));
    }

    QColor baseColor = tmp.pixelColor(mouse_x, mouse_y);
    volatile int x1(mouse_x), y1(mouse_y);
    int width = image->width();
    int height = image->height();

    bool blurTab[6][6]  = { {false} };
    int blurTabWidth  = width / 6;
    int blurTabHeight = height / 6;

    QQueue<std::array<int, 2>> queue;
    queue.push_back({ x1, y1 });
    int count = 0;

    while (!queue.isEmpty()) {
        x1 = qBound(0, queue.front()[0], width - 1);
        y1 = qBound(0, queue.front()[1], height - 1);
        queue.pop_front();

        if (selectedTab[y1][x1] != 1) {
            int i = (x1 + 1) / blurTabWidth;
            int j = y1 / blurTabHeight;

            if (!blurTab[i][j]) {
                QImage tmp = image->copy(i * blurTabWidth, j * blurTabHeight, blurTabWidth, blurTabHeight);
                painter.drawImage(i * blurTabWidth, j * blurTabHeight, GaussianBlur(tmp).blur(feather));
                blurTab[i][j] = true;
            }

            if (LabConverter::compareColors(baseColor, tmp.pixelColor(x1, y1)) < threshold) {
                if (selectedTab[y1][x1] == 2)
                    selectedArea.setPixelColor(x1, y1, Qt::transparent);

                selectedTab[y1][x1] = 1;

                queue.push_back({ x1 + 1, y1 });
                queue.push_back({ x1 - 1, y1 });
                queue.push_back({ x1, y1 + 1 });
                queue.push_back({ x1, y1 - 1 });
            } else {
                count++;

                if (count == 8)
                    count = 0;

                selectedTab[y1][x1] = 2;

                if (count < 4) {
                    selectedArea.setPixelColor(x1, y1, Qt::white);
                } else if (count < 8) {
                    selectedArea.setPixelColor(x1, y1, Qt::black);
                }
            }
        }
    }

    return selectedArea;
}

QImage SelectionTool::rectangleSelect(QRect rectSelect) {
    QPoint p1 = rectSelect.topLeft();
    QPoint p2 = rectSelect.bottomRight();

    if (!(QGuiApplication::keyboardModifiers() & Qt::ControlModifier)) {
        for (int y = 0; y < image->height(); y++)
            for (int x = 0; x < image->width(); x++)
                selectedTab[y][x] = 0;

        selectedArea.fill(qRgba(0, 0, 0, 0));
    }

    for (int i = p1.x(); i <= p2.x(); ++i)
        for (int j = p1.y(); j <= p2.y(); ++j) {
            selectedTab[j][i] = 1;
            selectedArea.setPixelColor(i, j, Qt::transparent);
        }

    int count = 0;

    for (int i = p1.x() - 1; i < p2.x() + 1; ++i) {
        if (i < 0) continue;

        if (i >= image->width()) break;

        count++;

        if (count == 16)
            count = 0;

        if (p1.y() > 0)
            if (selectedTab[p1.y() - 1][i] == 0) {
                selectedTab[p1.y() - 1][i] = 2;

                if (count < 8) {
                    selectedArea.setPixelColor(i, p1.y() - 1, Qt::white);
                    selectedArea.setPixelColor(i, p1.y(), Qt::white);
                } else if (count < 16) {
                    selectedArea.setPixelColor(i, p1.y(), Qt::black);
                    selectedArea.setPixelColor(i, p1.y() - 1, Qt::black);
                }
            }

        if (p2.y() < image->height() - 1)
            if (selectedTab[p2.y() + 1][i] == 0) {
                selectedTab[p2.y() + 1][i] = 2;

                if (count < 8) {
                    selectedArea.setPixelColor(i, p2.y() + 1, Qt::white);
                    selectedArea.setPixelColor(i, p2.y(), Qt::white);
                } else if (count < 16) {
                    selectedArea.setPixelColor(i, p2.y(), Qt::black);
                    selectedArea.setPixelColor(i, p2.y() + 1, Qt::black);
                }
            }

    }

    for (int i = p1.y() - 1; i < p2.y() + 1; ++i) {
        if (i < 0) continue;

        if (i >= image->height()) break;

        count++;

        if (count == 16)
            count = 0;

        if (p1.x() > 0)
            if (selectedTab[i][p1.x() - 1] == 0) {
                selectedTab[i][p1.x() - 1] = 2;

                if (count < 8) {
                    selectedArea.setPixelColor(p1.x() - 1, i, Qt::white);
                    selectedArea.setPixelColor(p1.x(), i, Qt::white);
                } else if (count < 16) {
                    selectedArea.setPixelColor(p1.x(), i, Qt::black);
                    selectedArea.setPixelColor(p1.x() - 1, i, Qt::black);
                }
            }

        if (p2.x() < image->width() - 1)
            if (selectedTab[i][p2.x() + 1] == 0) {
                selectedTab[i][p2.x() + 1] = 2;

                if (count < 8) {
                    selectedArea.setPixelColor(p2.x() + 1, i, Qt::white);
                    selectedArea.setPixelColor(p2.x(), i, Qt::white);
                } else if (count < 16) {
                    selectedArea.setPixelColor(p2.x(), i, Qt::black);
                    selectedArea.setPixelColor(p2.x() + 1, i, Qt::black);
                }
            }
    }

    return selectedArea;
}

