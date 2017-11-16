#include "filtergraphicsview.h"

#include <QMouseEvent>

FilterGraphicsView::FilterGraphicsView(QWidget *parent) : QGraphicsView(parent) { }

void FilterGraphicsView::mousePressEvent(QMouseEvent *event) {
    offset = event->pos();
}

void FilterGraphicsView::mouseMoveEvent(QMouseEvent *event) {
    if(event->buttons() & Qt::LeftButton) {
        emit updatePos(offset - event->pos());
    }
}

void FilterGraphicsView::mouseReleaseEvent(QMouseEvent *event) {
    emit mouseRelease(offset - event->pos());
}
