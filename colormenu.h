#ifndef COLORMENU_H
#define COLORMENU_H

#include <QDialog>
#include <QGraphicsScene>

namespace Ui {
class ColorMenu;
}

class ColorMenu : public QDialog {
    Q_OBJECT

signals:
    void colorize(QColor color);

public:
    explicit ColorMenu(int filterColor, QWidget *parent = 0);
    ~ColorMenu();

private slots:
    void draw();

private:
    Ui::ColorMenu *ui;

    int filterColor;

    QTimer *timer;

    void waitWithDraw();
    void createConnects();
};

#endif // COLORMENU_H
