#ifndef FILTERGRAPHICSVIEW_H
#define FILTERGRAPHICSVIEW_H

#include <QGraphicsView>

class FilterGraphicsView : public QGraphicsView {
    Q_OBJECT

signals:
    void updatePos(QPoint pos);
    void mouseRelease(QPoint pos);

public:
    FilterGraphicsView(QWidget *parent = nullptr);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    QPoint offset;

};

#endif // FILTERGRAPHICSVIEW_H
