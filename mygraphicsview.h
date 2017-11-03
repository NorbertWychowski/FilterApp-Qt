#ifndef MYGRAPHICSVIEW_H
#define MYGRAPHICSVIEW_H

#include <QGraphicsView>

class myGraphicsView : public QGraphicsView {
    Q_OBJECT
signals:
    void zoomChanged(int);

public:
    myGraphicsView(QWidget * parent = 0);
    myGraphicsView(QGraphicsScene * scene, QWidget * parent = 0);

    void setZoom(double value);

protected slots:
    void wheelEvent(QWheelEvent *event);

private:
    double zoom = 1;
};

#endif // MYGRAPHICSVIEW_H
