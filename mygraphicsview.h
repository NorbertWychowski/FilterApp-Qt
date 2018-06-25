#ifndef MYGRAPHICSVIEW_H
#define MYGRAPHICSVIEW_H

#include <QGraphicsView>
#include <QMap>
#include <QStack>
#include <utility>

class myGraphicsView : public QGraphicsView {
    Q_OBJECT
signals:
    void zoomChanged(double);
    void selectedArea(QRect);
    void updateUndoRedoStack();

public:
    myGraphicsView(QWidget* parent = 0);

    void enableRectSelect();
    void disableRectSelect();
    void enableBrush();
    void disableBrush();
    void setBrush(QColor color, int size);

    void setZoom(double value);

    QStack<QImage> getPreviousVersions() const;
    void deletePreviousVersions();

    static QMap<double, QString> getZoomValues();
    QImage getImage();

protected:
    void wheelEvent(QWheelEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;

private:
    bool isRectSelect;
    bool isBrushEnabled;
    QPoint offset;
    QGraphicsRectItem* rectItem;
    QGraphicsRectItem* rectItemBorder;
    QGraphicsRectItem* rectItemBorder2;
    QMap<double, QString>::const_iterator zoom;
    static const QMap<double, QString> zoomValues;
    QStack<QImage> previousVersions;
    QColor brushColor = Qt::black;
    int brushSize = 5;
};

#endif // MYGRAPHICSVIEW_H
