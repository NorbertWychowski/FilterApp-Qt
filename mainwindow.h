#ifndef MAINWINDOW_H
#define MAINWINDOW_H

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

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void zoomChanged(double);
    void sliderChanged(int);

    void openFileAction();
    void saveFileAction();

    void customFilter();
    void setCustomFilter(Matrix matrix);
    void laplaceFilter();
    void lowPassFilter();
    void gaussFilter();
    void highPassFilter();

    void colorFilter(int colorFilter);

    void thresholdSliderValueChanged(int value);
    void featherSliderValueChanged(int value);
    void featherCheckBoxChanged(bool visible);
    void selectByColorButtonClicked(bool clicked);
    void rectangleSelectButtonClicked(bool clicked);
    void rectangleSelection(QRect rectangleSelectionArea);

    void invertSelectedAreaColors();

private:
    Ui::MainWindow *ui;

    bool isSelectMask;

    struct stackNode {
        QImage image;
        bool movedSelection;
    };

    QGraphicsScene *scene = nullptr;
    QGraphicsPixmapItem *imageItem = nullptr;
    QGraphicsPixmapItem *selectedAreaItem = nullptr;
    CustomMaskDialog *dialog = nullptr;
    FiltersMenu *filtersMenu = nullptr;
    ColorMenu *colorMenu = nullptr;
    SelectionTool *selectTool = nullptr;
    QStack<stackNode> undoStack;
    QStack<stackNode> redoStack;
    QImage image;
    QTimer timer;

    void createConnects();
    void initScene();

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
};

#endif // MAINWINDOW_H
