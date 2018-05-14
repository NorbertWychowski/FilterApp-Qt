#include "mygraphicsview.h"

#include <QGuiApplication>
#include <QWheelEvent>
#include <QGraphicsRectItem>
#include <QGraphicsView>

const QMap<double, QString> myGraphicsView::zoomValues = QMap<double, QString>( {
    std::make_pair(0.125, "12.5%"),
    std::make_pair(0.25, "25%"),
    std::make_pair(0.5, "50%"),
    std::make_pair(0.66, "66%"),
    std::make_pair(1.0, "100%"),
    std::make_pair(2.0, "200%"),
    std::make_pair(3.0, "300%"),
    std::make_pair(4.0, "400%"),
    std::make_pair(5.0, "500%"),
    std::make_pair(6.0, "600%"),
    std::make_pair(7.0, "700%"),
    std::make_pair(8.0, "800%"),
});

myGraphicsView::myGraphicsView(QWidget* parent) : QGraphicsView(parent) {
    isRectSelect = false;
    zoom = zoomValues.find(1.0);
}

void myGraphicsView::enableRectSelect() {
    this->isRectSelect = true;
}

void myGraphicsView::disableRectSelect() {
    this->isRectSelect = false;
}

void myGraphicsView::setZoom(double value) {
    resetMatrix();
    scale(value, value);
}

QMap<double, QString> myGraphicsView::getZoomValues() {
    return zoomValues;
}

void myGraphicsView::wheelEvent(QWheelEvent* event) {
    if (QGuiApplication::keyboardModifiers() & Qt::ControlModifier) {

        if (event->delta() > 0) {
            if(zoom + 1 != zoomValues.end())
                zoom++;
        } else if (event->delta() < 0) {
            if(zoom != zoomValues.begin())
                zoom--;
        }

        double _scale = zoom.key();
        resetMatrix();
        scale(_scale, _scale);
        emit zoomChanged(zoom.key());
    } else {
        QGraphicsView::wheelEvent(event);
    }
}

void myGraphicsView::mousePressEvent(QMouseEvent* event) {
    QPoint local = mapFromGlobal(event->globalPos());
    QPointF l = mapToScene(local);

    offset = l.toPoint();

    if(event->buttons() & Qt::LeftButton && isRectSelect) {
        rectItemBorder = scene()->addRect(QRect(l.x(), l.y(), 0, 0), QPen(Qt::white, 1));
        rectItem = scene()->addRect(QRect(l.x(), l.y(), 0, 0), QPen(Qt::black, 1));
        //rectItemBorder2 = scene()->addRect(QRect(l.x(), l.y(), 0, 0), QPen(Qt::white, 1));
    }

    event->ignore();
}

void myGraphicsView::mouseMoveEvent(QMouseEvent* event) {
    QPoint local = mapFromGlobal(event->globalPos());
    QPointF l = mapToScene(local);

    if(isRectSelect) {
        if(event->buttons() & Qt::LeftButton) {
            rectItem->setRect(QRect(QPoint(offset.x(), offset.y()),
                                    QPoint(l.x(), l.y())).normalized());
            rectItemBorder->setRect(QRect(QPoint(offset.x() - 1, offset.y() - 1),
                                          QPoint(l.x() + 1, l.y() + 1)).normalized());
//            rectItemBorder2->setRect(QRect(QPoint(offset.x() + 1, offset.y() + 1),
//                                           QPoint(l.x() - 1, l.y() - 1)).normalized());
        }
    }
}
void myGraphicsView::mouseReleaseEvent(QMouseEvent* event) {
    if(event->button() & Qt::LeftButton) {
        if(isRectSelect) {
            if(!rectItem->rect().isNull())
                emit selectedArea(rectItem->rect().toRect());

            scene()->removeItem((QGraphicsItem*)rectItem);
            scene()->removeItem((QGraphicsItem*)rectItemBorder);
//            scene()->removeItem((QGraphicsItem*)rectItemBorder2);
        }
    }
}
