#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QGraphicsPixmapItem>
#include <QImage>
#include <QWheelEvent>
#include <QTimeLine>

#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {
    ui->setupUi(this);

    scene = new QGraphicsScene();
    ui->graphicsView->setScene(scene);

    imageItem = new QGraphicsPixmapItem();
    selectedAreaItem = new QGraphicsPixmapItem();
    scene->addItem(imageItem);
    scene->addItem(selectedAreaItem);

    selectTool = new SelectTool(&image);

    ui->featherSlider->setVisible(false);
    ui->featherSlider->setEnabled(false);
    ui->featherLabel->setVisible(false);

    connect(ui->actionOpen, SIGNAL(triggered(bool)), this, SLOT(openFileAction()));
    connect(ui->actionSaveAs, SIGNAL(triggered(bool)), this, SLOT(saveFileAction()));

    connect(ui->actionCustomMask, SIGNAL(triggered(bool)), this, SLOT(customMask()));
    connect(ui->actionLaplace, SIGNAL(triggered(bool)), this, SLOT(laplaceMask()));
    connect(ui->actionSquare, SIGNAL(triggered(bool)), this, SLOT(squareMask()));
    connect(ui->actionGauss, SIGNAL(triggered(bool)), this, SLOT(gaussMask()));
    connect(ui->actionHighPass, SIGNAL(triggered(bool)), this, SLOT(highPassMask()));

    connect(ui->graphicsView, SIGNAL(zoomChanged(int)), this, SLOT(zoomChanged(int)));
    connect(ui->horizontalSlider, SIGNAL(valueChanged(int)), this, SLOT(sliderChanged(int)));
    connect(ui->thresholdSlider, SIGNAL(valueChanged(int)), this, SLOT(thresholdSliderValueChanged(int)));
    connect(ui->featherSlider, SIGNAL(valueChanged(int)), this, SLOT(featherSliderValueChanged(int)));
    connect(ui->selectByColorButton, SIGNAL(clicked(bool)), this, SLOT(selectByColorButtonClicked(bool)));
    connect(ui->featherCheckBox, SIGNAL(clicked(bool)), this, SLOT(featherCheckBoxChanged(bool)));
}

MainWindow::~MainWindow() {
    delete selectTool;
    delete imageItem;
    delete selectedAreaItem;
    delete scene;
    delete ui;
}

void MainWindow::zoomChanged(int value) {
    ui->label->setText(QString::number(value) + "%");
    double v = value / 100.0;
    if (v >= 1)
        ui->horizontalSlider->setValue(v + 2);
    else
        ui->horizontalSlider->setValue(v * 4);
}

void MainWindow::sliderChanged(int value) {
    if (value >= 3) {
        ui->label->setText(QString::number((value - 2) * 100) + "%");
        ui->graphicsView->setZoom((value - 2));
    } else {
        ui->label->setText(QString::number(value * 25) + "%");
        ui->graphicsView->setZoom((value / 4.0));
    }
}

void MainWindow::openFileAction() {
    QString file = QFileDialog::getOpenFileName(this, tr("Otwórz"), "C://", "All files (*.*);;PNG (*.png)", &QString("PNG (*.png)"));

    image.load(file);
    selectTool->resizeSelectedTab();
    selectedAreaItem->setPixmap(QPixmap());

    imageItem->setPixmap(QPixmap::fromImage(image));
}

void MainWindow::saveFileAction() {
    QString file = QFileDialog::getSaveFileName(this, tr("Zapisz jako"), "C://", "Images (*.png *.xpm *.jpg)");
    image.save(file);
}

void MainWindow::customMask() {
    dialog = new CustomMaskDialog(this);
    connect(dialog, SIGNAL(customMask(int*)), this, SLOT(setCustomMask(int*)));

    dialog->show();
}

void MainWindow::setCustomMask(int *tab) {
    if (ui->selectByColorButton->isChecked())
        image = filter.splot(image, 5, selectTool->getSelectedTab(), tab);
    else
        image = filter.splot(image, 5, tab);
    imageItem->setPixmap(QPixmap::fromImage(image));
    selectTool->resizeSelectedTab();

    delete[] tab;
}

void MainWindow::laplaceMask() {
    if (ui->selectByColorButton->isChecked())
        image = filter.splot(image, 2, selectTool->getSelectedTab());
    else
        image = filter.splot(image, 2);
    imageItem->setPixmap(QPixmap::fromImage(image));
    selectTool->resizeSelectedTab();
}

void MainWindow::squareMask() {
    if (ui->selectByColorButton->isChecked())
        image = filter.splot(image, 1, selectTool->getSelectedTab());
    else
        image = filter.splot(image, 1);
    imageItem->setPixmap(QPixmap::fromImage(image));
    selectTool->resizeSelectedTab();
}

void MainWindow::gaussMask() {
    if (ui->selectByColorButton->isChecked())
        image = filter.splot(image, 3, selectTool->getSelectedTab());
    else
        image = filter.splot(image, 3);
    imageItem->setPixmap(QPixmap::fromImage(image));
    selectTool->resizeSelectedTab();
}

void MainWindow::highPassMask() {
    if (ui->selectByColorButton->isChecked())
        image = filter.splot(image, 4, selectTool->getSelectedTab());
    else
        image = filter.splot(image, 4);
    imageItem->setPixmap(QPixmap::fromImage(image));
    selectTool->resizeSelectedTab();
}

void MainWindow::thresholdSliderValueChanged(int value) {
    ui->thresholdLabel->setNum(value);
}

void MainWindow::featherSliderValueChanged(int value) {
    ui->featherLabel->setNum(value);
}

void MainWindow::mousePressEvent(QMouseEvent *event) {
    if ((event->buttons() & Qt::LeftButton) && ui->selectByColorButton->isChecked()) {
        QPoint local = ui->graphicsView->mapFromGlobal(event->globalPos());
        QPointF l = ui->graphicsView->mapToScene(local);
        double value = ui->featherSlider->value()/10;
        if (ui->featherCheckBox->isChecked() && value != 0) {
            selectedAreaItem->setPixmap(QPixmap::fromImage(selectTool->selectByColor(l.x(), l.y(), ui->thresholdSlider->value(), value)));
        } else {
            selectedAreaItem->setPixmap(QPixmap::fromImage(selectTool->selectByColor(l.x(), l.y(), ui->thresholdSlider->value())));
        }

    }
}

void MainWindow::selectByColorButtonClicked(bool b) {
    if (!b) {
        selectedAreaItem->setPixmap(QPixmap());
    }
}

void MainWindow::featherCheckBoxChanged(bool b) {
    ui->featherSlider->setEnabled(b);
    ui->featherSlider->setVisible(b);
    ui->featherLabel->setVisible(b);
}
