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

protected slots:
    void updateScene(QPoint pos);
    void resizeEvent(QResizeEvent *);
    void radiusChanged(int radius);

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

    void createConnects();
    void initScene();
};

#endif // FILTERSMENU_H
