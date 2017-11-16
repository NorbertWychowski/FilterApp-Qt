#include "mygraphicsview.h"

#include <QGuiApplication>
#include <QWheelEvent>

myGraphicsView::myGraphicsView(QWidget *parent) : QGraphicsView(parent) {
}

void myGraphicsView::setZoom(double value) {
    resetMatrix();
    scale(value, value);
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

