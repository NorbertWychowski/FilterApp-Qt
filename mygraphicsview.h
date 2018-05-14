#ifndef MYGRAPHICSVIEW_H
#define MYGRAPHICSVIEW_H

#include <QGraphicsView>
#include <QMap>
#include <utility>

class myGraphicsView : public QGraphicsView {
    Q_OBJECT
signals:
    void zoomChanged(double);
    void selectedArea(QRect);

public:
    myGraphicsView(QWidget* parent = 0);

    void enableRectSelect();
    void disableRectSelect();

    void setZoom(double value);

    static QMap<double, QString> getZoomValues();

protected:
    void wheelEvent(QWheelEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;

private:
    bool isRectSelect;
    QPoint offset;
    QGraphicsRectItem* rectItem;
    QGraphicsRectItem* rectItemBorder;
    QGraphicsRectItem* rectItemBorder2;
    QMap<double, QString>::const_iterator zoom;
    static const QMap<double, QString> zoomValues;
};

#endif // MYGRAPHICSVIEW_H
