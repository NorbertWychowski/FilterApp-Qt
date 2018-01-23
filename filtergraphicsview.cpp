#include "filtergraphicsview.h"

#include <QApplication>
#include <QMouseEvent>
#include <QGraphicsItem>

FilterGraphicsView::FilterGraphicsView(QWidget *parent) : QGraphicsView(parent) { }


void FilterGraphicsView::mousePressEvent(QMouseEvent *event) {
    QApplication::setOverrideCursor(Qt::ClosedHandCursor);

    offset = event->pos();
}

void FilterGraphicsView::mouseMoveEvent(QMouseEvent *event) {
    if(event->buttons() & Qt::LeftButton) {
        emit updatePos(offset - event->pos());
    }
}

void FilterGraphicsView::mouseReleaseEvent(QMouseEvent *event) {
    emit mouseRelease(offset - event->pos());
    QApplication::setOverrideCursor(Qt::ArrowCursor);
}
