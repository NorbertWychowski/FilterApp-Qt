#include "filtersmenu.h"
#include "ui_filtersmenu.h"

#include "filters/gaussianblur.h"
#include "filters/boxblur.h"
#include "filters/filtertool.h"

#include <QGraphicsPixmapItem>
#include <QMouseEvent>
#include <QTimer>
#include <QWidget>

FiltersMenu::FiltersMenu(QImage &image, int filter, QWidget *parent) : QDialog(parent), ui(new Ui::FiltersMenu) {
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowFlags(windowFlags() & (~Qt::WindowContextHelpButtonHint));

    this->filter = filter;
    this->image = image;

    timer = new QTimer(this);

    createConnects();
    initScene();

    raise();
    setFocus();
    show();
    activateWindow();
}

FiltersMenu::~FiltersMenu() {
    delete ui;
}

void FiltersMenu::updateScene(QPoint pos) {
    int tmpX = qBound(0, int(x + pos.x()), int(image.width() - width));
    int tmpY = qBound(0, int(y + pos.y()), int(image.height() - height));

    scene->setSceneRect(tmpX, tmpY, width, height);

    blurredItem->setPos(tmpX, tmpY);

    switch (filter) {
    case GAUSSIAN_FILTER:
        blurredItem->setPixmap(QPixmap::fromImage(GaussianBlur(image.copy(tmpX, tmpY, width, height)).blur(ui->radiusSpinBox->value())));
        break;

    case LOWPASS_FILTER:
        blurredItem->setPixmap(QPixmap::fromImage(BoxBlur(image.copy(tmpX, tmpY, width, height)).blur(ui->radiusSpinBox->value())));
        break;
    }
}

void FiltersMenu::mouseReleased(QPoint pos) {
    x = qBound(0, int(x + pos.x()), int(this->image.width() - width));
    y = qBound(0, int(y + pos.y()), int(this->image.height() - height));
}

void FiltersMenu::blurRadiusSelected() {
    emit blurRadius(ui->radiusSpinBox->value());
}

void FiltersMenu::waitWithPreviewBlur() {
    timer->start(400);
}

void FiltersMenu::blurPreview() {
    radiusChanged(ui->radiusSpinBox->value());
    timer->stop();
}

void FiltersMenu::resizeEvent(QResizeEvent *) {
    width = ui->graphicsView->width();
    height = ui->graphicsView->height();

    switch (filter) {
    case GAUSSIAN_FILTER:
        blurredItem->setPixmap(QPixmap::fromImage(GaussianBlur(image.copy(x, y, width, height)).blur(ui->radiusSpinBox->value())));
        break;

    case LOWPASS_FILTER:
        blurredItem->setPixmap(QPixmap::fromImage(BoxBlur(image.copy(x, y, width, height)).blur(ui->radiusSpinBox->value())));
        break;
    }
}

void FiltersMenu::createConnects() {
    //przesuwanie podgladu myszka
    connect(ui->graphicsView,   SIGNAL(updatePos(QPoint)),   this, SLOT(updateScene(QPoint)));
    connect(ui->graphicsView,   SIGNAL(mouseRelease(QPoint)), this, SLOT(mouseReleased(QPoint)));
    //po kliknieciu "ok" - wyslanie wybranego promienia rozmycia
    connect(this,               SIGNAL(accepted()),          this, SLOT(blurRadiusSelected()));
    //rozmycie podgladu po 0.4s od ostatniej zmiany spinboxa - optymalizacja
    connect(ui->radiusSpinBox,  SIGNAL(valueChanged(int)),   this, SLOT(waitWithPreviewBlur()));
    connect(timer,              SIGNAL(timeout()),           this, SLOT(blurPreview()));
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

    switch (filter) {
    case 1:
        blurredItem->setPixmap(QPixmap::fromImage(GaussianBlur(image.copy(x, y, width, height)).blur(ui->radiusSpinBox->value())));
        break;

    case 2:
        blurredItem->setPixmap(QPixmap::fromImage(BoxBlur(image.copy(x, y, width, height)).blur(ui->radiusSpinBox->value())));
        break;
    }
}

void FiltersMenu::radiusChanged(int radius) {
    switch (filter) {
    case GAUSSIAN_FILTER:
        blurredItem->setPixmap(QPixmap::fromImage(GaussianBlur(image.copy(x, y, width, height)).blur(radius)));
        break;

    case LOWPASS_FILTER:
        blurredItem->setPixmap(QPixmap::fromImage(BoxBlur(image.copy(x, y, width, height)).blur(radius)));
        break;
    }
}
