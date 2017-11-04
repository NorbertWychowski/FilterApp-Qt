#include "selecttool.h"

#include "labconverter.h"

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
    int N(feather*2+1), M(feather*2+1);
    int hN(N / 2), hM(M / 2);
    double **mask = createGaussianKernel(feather, N, M);

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
            }
        }
        if ((x1 - 1 >= 0) && selectedTab[y1][x1 - 1] != 1) {
            color = image->pixelColor(x1 - 1, y1);
            if (LabConverter::compareColors(baseColor, color) < threshold) {
                queue.push_back({ x1 - 1, y1 });
                selectedTab[y1][x1 - 1] = 1;
            }
        }
        if ((y1 + 1 <  height) && selectedTab[y1 + 1][x1] != 1) {
            color = image->pixelColor(x1, y1 + 1);
            if (LabConverter::compareColors(baseColor, color) < threshold) {
                queue.push_back({ x1, y1 + 1 });
                selectedTab[y1 + 1][x1] = 1;
            }
        }
        if ((y1 - 1 >= 0) && selectedTab[y1 - 1][x1] != 1) {
            color = image->pixelColor(x1, y1 - 1);
            if (LabConverter::compareColors(baseColor, color) < threshold) {
                queue.push_back({ x1, y1 - 1 });
                selectedTab[y1 - 1][x1] = 1;
            }
        }
    }

    int **res = new int *[height];
    for (int i = 0; i<height; i++)
        res[i] = new int[width];

    volatile float value = 0;
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if ((y > hN) && (y < height - hN) && (x > hM) && (x < width - hM)) {
                for (int i = 0; i < N; i++) {
                    for (int j = 0; j < M; j++) {
                        value += selectedTab[y + i - hN][x + j - hM] * mask[i][j];
                    }
                }

                if (value > 0.4) value = 1;
                else value = 0;

                res[y][x] = value;
                value = 0;
            } else {
                res[y][x] = selectedTab[y][x];
            }
        }
    }

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (res[y][x] == 0) {
                for (int yT = -1; yT <= 1; yT++) {
                    int tmpY = y + yT;
                    if (tmpY < 0) continue;
                    if (tmpY >= height) break;
                    for (int xT = -1; xT <= 1; xT++) {
                        int tmpX = x + xT;
                        if (tmpX < 0) continue;
                        if (tmpX >= width) break;

                        if (res[tmpY][tmpX] == 1) {
                            res[y][x] = 2;
                            selectedArea.setPixelColor(x, y, Qt::green);
                        }
                    }
                }
            }
        }
    }

    for (int i = 0; i<height; i++)
        delete[] selectedTab[i];
    delete[] selectedTab;

    selectedTab = res;

    for (int i = 0; i<M; i++)
        delete[] mask[i];
    delete[] mask;

    return selectedArea;
}

double ** SelectTool::createGaussianKernel(double sigma, int N, int M) {
    double **matrix = new double *[N];

    for (int i = 0; i<N; i++)
        matrix[i] = new double[M];

    double r, s = 2.0*sigma*sigma;
    double sum = 0;
    for (int i = -0.5*N; i <= 0.5*N; i++) {
        for (int j = -0.5*M; j <= 0.5*M; j++) {
            r = qSqrt(i*i + j*j);
            matrix[int(i + 0.5*N)][int(j + 0.5*M)] = (qExp(-(r*r) / s)) / (M_PI*s);
            sum += matrix[int(i + 0.5*N)][int(j + 0.5*M)];
        }
    }
    QDebug deb = qDebug();
    for (int i = 0; i<N; i++) {
        for (int j = 0; j<M; j++) {
            matrix[i][j] /= sum;
            deb << matrix[i][j] << " ";
        }
        deb << "\n";
    }

    return matrix;
}
