#include "filtersmenu.h"
#include "ui_filtersmenu.h"

#include "filters/gaussianblur.h"
#include "filters/boxblur.h"

#include <QGraphicsPixmapItem>
#include <QMouseEvent>
#include <QTimer>

FiltersMenu::FiltersMenu(QImage &image, int filter, QWidget *parent) : QDialog(parent), ui(new Ui::FiltersMenu) {
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);

    this->filter = filter;
    this->image = image;

    timer = new QTimer(this);

    createConnects();
    initScene();

    activateWindow();
    raise();
    setFocus();
    show();
}

FiltersMenu::~FiltersMenu() {
    delete ui;
}

void FiltersMenu::updateScene(QPoint pos) {
    int tmpX = qBound(0, int(x + pos.x()), int(image.width() - width));
    int tmpY = qBound(0, int(y + pos.y()), int(image.height() - height));

    scene->setSceneRect(tmpX, tmpY, width, height);

    blurredItem->setPos(tmpX, tmpY);

    switch(filter) {
    case 1:
        blurredItem->setPixmap(QPixmap::fromImage(GaussianBlur(image.copy(tmpX, tmpY, width, height)).blur(ui->radiusSpinBox->value())));
        break;
    case 2:
        blurredItem->setPixmap(QPixmap::fromImage(BoxBlur(image.copy(tmpX, tmpY, width, height)).blur(ui->radiusSpinBox->value())));
        break;
    }
}

void FiltersMenu::resizeEvent(QResizeEvent *) {
    width = ui->graphicsView->width();
    height = ui->graphicsView->height();

    switch(filter) {
    case 1:
        blurredItem->setPixmap(QPixmap::fromImage(GaussianBlur(image.copy(x, y, width, height)).blur(ui->radiusSpinBox->value())));
        break;
    case 2:
        blurredItem->setPixmap(QPixmap::fromImage(BoxBlur(image.copy(x, y, width, height)).blur(ui->radiusSpinBox->value())));
        break;
    }
}

void FiltersMenu::createConnects() {
    connect(ui->graphicsView,   SIGNAL(updatePos(QPoint)),  this, SLOT(updateScene(QPoint)));

    connect(ui->graphicsView, &FilterGraphicsView::mouseRelease, this, [this](QPoint pos) {
        x = qBound(0, int(x + pos.x()), int(this->image.width() - width));
        y = qBound(0, int(y + pos.y()), int(this->image.height() - height));
    });
    connect(this, &FiltersMenu::accepted, this, [this](void) {
        emit blurRadius(ui->radiusSpinBox->value());
    });
    //zmiana promienia rozmycia
    connect(ui->radiusSpinBox, QOverload<int>::of(&QSpinBox::valueChanged),  this, [this](void) {
        timer->start(400);
    });
    //po 0.4 s zmiana podgladu
    connect(timer, &QTimer::timeout, this, [this](void) {
        radiusChanged(ui->radiusSpinBox->value());
    });
}

void FiltersMenu::initScene() {
    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);

    width = ui->graphicsView->width();
    height = ui->graphicsView->height();

    backgroundItem = scene->addPixmap(QPixmap::fromImage(image));
    blurredItem = new QGraphicsPixmapItem(QPixmap::fromImage(image.copy(x, y, width, height)));
    scene->setSceneRect(x, y, width, height);
    scene->addItem(blurredItem);
    blurredItem->setPos(x, y);

    switch(filter) {
    case 1:
        blurredItem->setPixmap(QPixmap::fromImage(GaussianBlur(image.copy(x, y, width, height)).blur(ui->radiusSpinBox->value())));
        break;
    case 2:
        blurredItem->setPixmap(QPixmap::fromImage(BoxBlur(image.copy(x, y, width, height)).blur(ui->radiusSpinBox->value())));
        break;
    }
}

void FiltersMenu::radiusChanged(int radius) {
    switch(filter) {
    case 1:
        blurredItem->setPixmap(QPixmap::fromImage(GaussianBlur(image.copy(x, y, width, height)).blur(radius)));
        break;
    case 2:
        blurredItem->setPixmap(QPixmap::fromImage(BoxBlur(image.copy(x, y, width, height)).blur(radius)));
        break;
    }
}
