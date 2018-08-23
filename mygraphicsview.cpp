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

myGraphicsView::myGraphicsView(QWidget *parent) : QGraphicsView(parent) {
    disableRectSelect();
    disableBrush();
    zoom = zoomValues.find(1.0);
}

void myGraphicsView::enableRectSelect() {
    disableBrush();
    this->isRectSelect = true;
}

void myGraphicsView::disableRectSelect() {
    this->isRectSelect = false;
}

void myGraphicsView::enableBrush() {
    disableRectSelect();
    this->isBrushEnabled = true;
}

void myGraphicsView::disableBrush() {
    this->isBrushEnabled = false;
}

void myGraphicsView::setBrush(QColor color, int size) {
    this->brushColor = color;
    this->brushSize = size;
}

void myGraphicsView::setZoom(double value) {
    resetMatrix();
    scale(value, value);
}

QStack<QImage> myGraphicsView::getPreviousVersions() const {
    return previousVersions;
}

void myGraphicsView::deletePreviousVersions() {
    previousVersions.clear();
}

QMap<double, QString> myGraphicsView::getZoomValues() {
    return zoomValues;
}

QImage myGraphicsView::getImage() {
    return dynamic_cast<QGraphicsPixmapItem *>(scene()->items()[1])->pixmap().toImage();
}

void myGraphicsView::wheelEvent(QWheelEvent *event) {
    if (QGuiApplication::keyboardModifiers() & Qt::ControlModifier) {

        if (event->delta() > 0) {
            if (zoom + 1 != zoomValues.end())
                zoom++;
        } else if (event->delta() < 0) {
            if (zoom != zoomValues.begin())
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

void myGraphicsView::mousePressEvent(QMouseEvent *event) {
    QPoint local = mapFromGlobal(event->globalPos());
    QPointF l = mapToScene(local);

    offset = l.toPoint();

    if (event->buttons() & Qt::LeftButton) {
        if (isRectSelect) {
            rectItemBorder = scene()->addRect(QRect(static_cast<int>(l.x()), static_cast<int>(l.y()), 0, 0), QPen(Qt::white, 1));
            rectItem = scene()->addRect(QRect(static_cast<int>(l.x()), static_cast<int>(l.y()), 0, 0), QPen(Qt::black, 1));
        } else if (isBrushEnabled) {
            QGraphicsPixmapItem *imageItem = dynamic_cast<QGraphicsPixmapItem *>(scene()->items()[1]);
            QPixmap pixmap = imageItem->pixmap();
            previousVersions.push(pixmap.toImage());
            QPainter painter(&pixmap);
            painter.setRenderHint(QPainter::Antialiasing, true);
            painter.setPen(QPen(brushColor, brushSize, Qt::SolidLine, Qt::RoundCap));

            painter.drawLine(offset, l);
            offset = l.toPoint();
            imageItem->setPixmap(pixmap);
        }
    }

    event->ignore();
}
#include <QDebug>
void myGraphicsView::mouseMoveEvent(QMouseEvent *event) {
    QPoint local = mapFromGlobal(event->globalPos());
    QPointF l = mapToScene(local);

    qDebug() << isBrushEnabled;

    if (event->buttons() & Qt::LeftButton) {
        if (isRectSelect) {
            rectItem->setRect(QRect(QPoint(offset.x(), offset.y()), QPoint(static_cast<int>(l.x()), static_cast<int>(l.y()))).normalized());
            rectItemBorder->setRect(QRect(QPoint(offset.x() - 1, offset.y() - 1), QPoint(static_cast<int>(l.x() + 1), static_cast<int>(l.y() + 1))).normalized());
        } else if (isBrushEnabled) {
            QGraphicsPixmapItem *imageItem = dynamic_cast<QGraphicsPixmapItem *>(scene()->items()[1]);
            QPixmap pixmap = imageItem->pixmap();
            QPainter painter(&pixmap);
            painter.setRenderHint(QPainter::Antialiasing, true);
            painter.setPen(QPen(brushColor, brushSize, Qt::SolidLine, Qt::RoundCap));

            painter.drawLine(offset, l);
            offset = l.toPoint();
            imageItem->setPixmap(pixmap);
        }
    }
}
void myGraphicsView::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() & Qt::LeftButton) {
        if (isRectSelect) {
            if (!rectItem->rect().isNull())
                emit selectedArea(rectItem->rect().toRect());

            scene()->removeItem(dynamic_cast<QGraphicsItem *>(rectItem));
            scene()->removeItem(dynamic_cast<QGraphicsItem *>(rectItemBorder));
        } else if (isBrushEnabled) {
            emit updateUndoRedoStack();
        }
    }
}
