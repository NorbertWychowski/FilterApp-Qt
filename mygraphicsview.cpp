#include "mygraphicsview.h"

#include <QGuiApplication>
#include <QWheelEvent>
#include <QGraphicsRectItem>

#include <QDebug>

myGraphicsView::myGraphicsView(QWidget *parent) : QGraphicsView(parent) {
    isRectSelect = false;
}

void myGraphicsView::setZoom(double value) {
    resetMatrix();
    scale(value, value);
}

void myGraphicsView::enableRectSelect() {
    this->isRectSelect = true;
    rectItem = scene()->addRect(rectSelect, QPen(Qt::darkGreen));
}

void myGraphicsView::disableRectSelect() {
    this->isRectSelect = false;
    scene()->removeItem((QGraphicsItem*)rectItem);
}

void myGraphicsView::wheelEvent(QWheelEvent *event) {
    if (QGuiApplication::keyboardModifiers() & Qt::ControlModifier) {
        int numDegrees = event->delta() / 8;
        int numSteps = numDegrees / 15;

        if (numSteps > 0 && zoom < 5.0) {
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
    offset = event->pos();
}

void myGraphicsView::mouseMoveEvent(QMouseEvent *event) {
    if(isRectSelect) {
        if(event->buttons() & Qt::LeftButton) {
            int x1 = offset.x() < event->pos().x() ? offset.x() : event->pos().x();
            int y1 = offset.y() < event->pos().y() ? offset.y() : event->pos().y();
            int x2 = offset.x() > event->pos().x() ? offset.x() : event->pos().x();
            int y2 = offset.y() > event->pos().y() ? offset.y() : event->pos().y();
            rectItem->setRect(QRect(QPoint(x1, y1), QPoint(x2, y2)));
        }
    }
}

void myGraphicsView::mouseReleaseEvent(QMouseEvent *event) {

}


