#include "mygraphicsview.h"

#include <QGuiApplication>
#include <QWheelEvent>
#include <QGraphicsRectItem>
#include <QGraphicsView>


myGraphicsView::myGraphicsView(QWidget *parent) : QGraphicsView(parent) {
    isRectSelect = false;
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

void myGraphicsView::wheelEvent(QWheelEvent *event) {
    if (QGuiApplication::keyboardModifiers() & Qt::ControlModifier) {
        int numDegrees = event->delta() / 8;
        int numSteps = numDegrees / 15;

        if (numSteps > 0 && zoom < 6.0) {
            if (zoom <1)
                zoom *= 2;
            else
                zoom += 1;

            resetMatrix();
            scale(zoom, zoom);
            emit zoomChanged(100 * zoom);
        } else if (numSteps < 0 && zoom > 0.25) {
            if (zoom <= 1)
                zoom *= 0.5;
            else
                zoom += -1;

            resetMatrix();
            scale(zoom, zoom);
            emit zoomChanged(100 * zoom);
        }
    } else {
        QGraphicsView::wheelEvent(event);
    }
}

void myGraphicsView::mousePressEvent(QMouseEvent *event) {
    QPoint local = mapFromGlobal(event->globalPos());
    QPointF l = mapToScene(local);

    offset = l.toPoint();

    if(event->buttons() & Qt::LeftButton && isRectSelect) {
        rectItemBorder = scene()->addRect(QRect(l.x(), l.y(), 0, 0), QPen(Qt::white, 1));
        rectItem = scene()->addRect(QRect(l.x(), l.y(), 0, 0), QPen(Qt::black, 1));
        rectItemBorder2 = scene()->addRect(QRect(l.x(), l.y(), 0, 0), QPen(Qt::white, 1));
    }

    event->ignore();
}

void myGraphicsView::mouseMoveEvent(QMouseEvent *event) {
    QPoint local = mapFromGlobal(event->globalPos());
    QPointF l = mapToScene(local);

    if(isRectSelect) {
        if(event->buttons() & Qt::LeftButton) {
            rectItem->setRect(QRect(QPoint(offset.x(), offset.y()),
                                    QPoint(l.x(), l.y())).normalized());
            rectItemBorder->setRect(QRect(QPoint(offset.x() - 1, offset.y() - 1),
                                          QPoint(l.x() + 1, l.y() + 1)).normalized());
            rectItemBorder2->setRect(QRect(QPoint(offset.x() + 1, offset.y() + 1),
                                           QPoint(l.x() - 1, l.y() - 1)).normalized());
        }
    }
}
void myGraphicsView::mouseReleaseEvent(QMouseEvent *event) {
    if(event->button() & Qt::LeftButton) {
        if(isRectSelect) {
            if(!rectItem->rect().isNull())
                emit rectSelected(rectItem->rect().toRect());

            scene()->removeItem((QGraphicsItem*)rectItem);
            scene()->removeItem((QGraphicsItem*)rectItemBorder);
            scene()->removeItem((QGraphicsItem*)rectItemBorder2);
        }
    }
}
