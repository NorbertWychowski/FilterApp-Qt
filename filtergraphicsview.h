#ifndef FILTERGRAPHICSVIEW_H
#define FILTERGRAPHICSVIEW_H

#include <QGraphicsView>

class FilterGraphicsView : public QGraphicsView {
    Q_OBJECT

signals:
    void updatePos(QPointF pos);
    void mouseRelease(QPointF pos);

public:
    FilterGraphicsView(QWidget *parent=0);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    QPointF offset;
};

#endif // FILTERGRAPHICSVIEW_H
