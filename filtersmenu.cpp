#include "filtersmenu.h"
#include "ui_filtersmenu.h"

#include "gaussianblur.h"
#include "boxblur.h"

#include <QGraphicsPixmapItem>
#include <QMouseEvent>

#include <QDebug>

FiltersMenu::FiltersMenu(QImage &image, int filter, QWidget *parent) : QDialog(parent), ui(new Ui::FiltersMenu) {
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);

    this->filter = filter;
    this->image = image;

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

void FiltersMenu::updateScene(QPointF pos) {
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

void FiltersMenu::createConnects() {
    connect(ui->graphicsView, SIGNAL(updatePos(QPointF)), this, SLOT(updateScene(QPointF)));
    connect(ui->radiusSpinBox, SIGNAL(valueChanged(int)), this, SLOT(radiusChanged(int)));
    connect(ui->graphicsView, QOverload<QPointF>::of(&FilterGraphicsView::mouseRelease), this, [this](QPointF pos) {
        x = qBound(0, int(x + pos.x()), int(this->image.width() - width));
        y = qBound(0, int(y + pos.y()), int(this->image.height() - height));
    });
    connect(this, QOverload<void>::of(&FiltersMenu::accepted), this, [this](void) {
        emit blurRadius(ui->radiusSpinBox->value());
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
