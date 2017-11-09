#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "filter.h"
#include "custommaskdialog.h"
#include "selecttool.h"

#include <QMainWindow>
#include <QGraphicsScene>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void zoomChanged(int);
    void sliderChanged(int);

    void openFileAction();
    void saveFileAction();

    void customFilter();
    void setCustomFilter(Matrix matrix);
    void laplaceFilter();
    void lowPassFilter();
    void gaussFilter();
    void highPassFilter();

    void thresholdSliderValueChanged(int);
    void featherSliderValueChanged(int);
    void mousePressEvent(QMouseEvent *event);
    void selectByColorButtonClicked(bool);
    void featherCheckBoxChanged(bool);

private:
    Ui::MainWindow *ui;

    QGraphicsScene *scene = nullptr;
    QGraphicsPixmapItem *imageItem = nullptr;
    QGraphicsPixmapItem *selectedAreaItem = nullptr;
    CustomMaskDialog *dialog = nullptr;
    SelectTool *selectTool = nullptr;
    QImage image;
    Filter filter;
};

#endif // MAINWINDOW_H
