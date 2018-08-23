#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "tools/histogram.h"
#include "filters/filtertool.h"
#include "filters/colortool.h"
#include "custommaskdialog.h"
#include "selectiontool.h"
#include "filtersmenu.h"
#include "colormenu.h"

#include <QMainWindow>
#include <QGraphicsScene>
#include <QStack>
#include <QTimer>
#include <QColorDialog>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    void zoomChanged(double key);
    void zoomSliderChanged(int value);

    void openFileAction();
    void saveFileAction();

    void customFilter();
    void setCustomFilter(Matrix matrix);
    void laplaceFilter();
    void lowPassFilter();
    void gaussFilter();
    void highPassFilter();

    void showHistogram();

    void colorFilter(int colorFilter);

    void thresholdSliderValueChanged(int value);
    void brushSizeSliderValueChanged(int value);
    void featherSliderValueChanged(int value);
    void featherCheckBoxChanged(bool visible);
    void rectangleSelectButtonClicked(bool);
    void selectByColorButtonClicked(bool);
    void brushButtonClicked(bool);
    void brushColorButtonClicked(bool);
    void rectangleSelection(QRect rectangleSelectionArea);

    void invertSelectedAreaColors();
    void updateUndoRedoStack();

private:
    Ui::MainWindow *ui;

    bool isSelectMask;

    QGraphicsScene *scene = nullptr;
    QGraphicsPixmapItem *imageItem = nullptr;
    QGraphicsPixmapItem *selectedAreaItem = nullptr;
    CustomMaskDialog *dialog = nullptr;
    FiltersMenu *filtersMenu = nullptr;
    FilterTool *filterTool = nullptr;
    Histogram *histogram = nullptr;
    ColorMenu *colorMenu = nullptr;
    SelectionTool *selectTool = nullptr;
    QStack<QImage> undoStack;
    QStack<QImage> redoStack;
    QImage image;
    QTimer timer;
    QColor brushColor;

    void createConnects();
    void initScene();

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
};

#endif // MAINWINDOW_H
