#ifndef FILTERSMENU_H
#define FILTERSMENU_H

#include <QDialog>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QGraphicsView>

#include "filtergraphicsview.h"

namespace Ui {
class FiltersMenu;
}

class FiltersMenu : public QDialog {
    Q_OBJECT

signals:
    void blurRadius(int radius);

public:
    explicit FiltersMenu(QImage &image, int filter, QWidget *parent = 0);
    ~FiltersMenu();

    void updateScene(QPoint pos);
    void resizeEvent(QResizeEvent *);

private:
    Ui::FiltersMenu *ui;

    int filter;
    int x = 0, y = 0;
    int lastX, lastY;
    int width, height;

    QImage image;
    QGraphicsScene *scene;
    QGraphicsPixmapItem *backgroundItem;
    QGraphicsPixmapItem *blurredItem;
    QTimer *timer;

    void createConnects();
    void initScene();
    void radiusChanged(int radius);
};

#endif // FILTERSMENU_H
