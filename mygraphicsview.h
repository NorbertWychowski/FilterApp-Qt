#ifndef MYGRAPHICSVIEW_H
#define MYGRAPHICSVIEW_H

#include <QGraphicsView>

class myGraphicsView : public QGraphicsView {
    Q_OBJECT
signals:
    void zoomChanged(int);
    void selectedArea(QRect);

public:
    myGraphicsView(QWidget * parent = 0);

    void enableRectSelect();
    void disableRectSelect();

    void setZoom(double value);

protected:
    void wheelEvent(QWheelEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    double zoom = 1;
    bool isRectSelect;
    QPoint offset;
    QGraphicsRectItem *rectItem;
    QGraphicsRectItem *rectItemBorder;
    QGraphicsRectItem *rectItemBorder2;
};

#endif // MYGRAPHICSVIEW_H
