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
    void colors(int H, int S, int V);
    void brightnessContrast(int B, int C);

public:
    explicit ColorMenu(int filterColor, QWidget* parent = 0);
    ~ColorMenu();

private slots:
    void draw();
    void defaultSettings();

private:
    Ui::ColorMenu* ui;

    int filterColor;

    QTimer* timer;

    void waitWithDraw();
    void initUI();
    void createConnects();
};

#endif // COLORMENU_H
