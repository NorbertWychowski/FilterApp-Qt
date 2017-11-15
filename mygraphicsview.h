#ifndef MYGRAPHICSVIEW_H
#define MYGRAPHICSVIEW_H

#include <QGraphicsView>

class myGraphicsView : public QGraphicsView {
    Q_OBJECT
signals:
    void zoomChanged(int);

public:
    myGraphicsView(QWidget * parent = 0);

    void setZoom(double value);

private:
    double zoom = 1;

protected:
    void wheelEvent(QWheelEvent *event) override;
};

#endif // MYGRAPHICSVIEW_H
