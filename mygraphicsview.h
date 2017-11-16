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
    void enableRectSelect();
    void disableRectSelect();

private:
    double zoom = 1;
    bool isRectSelect;
    QPoint offset;
    QRect rectSelect;
    QGraphicsRectItem *rectItem;

protected:
    void wheelEvent(QWheelEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
};

#endif // MYGRAPHICSVIEW_H
