#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "filters/filter.h"
#include "custommaskdialog.h"
#include "selectiontool.h"
#include "filtersmenu.h"

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
    void featherCheckBoxChanged(bool);

private:
    Ui::MainWindow *ui;

    bool isSelectMask;

    QGraphicsScene *scene = nullptr;
    QGraphicsPixmapItem *imageItem = nullptr;
    QGraphicsPixmapItem *selectedAreaItem = nullptr;
    CustomMaskDialog *dialog = nullptr;
    FiltersMenu *filtersMenu = nullptr;
    SelectionTool *selectTool = nullptr;
    QStack<QImage> undoStack;
    QStack<QImage> redoStack;
    QImage image;
    Filter filter;
    QTimer timer;

    void createConnects();
    void initScene();

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
};

#endif // MAINWINDOW_H
