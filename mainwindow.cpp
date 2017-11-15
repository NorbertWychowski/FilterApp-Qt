#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QGraphicsPixmapItem>
#include <QImage>
#include <QWheelEvent>
#include <QKeyEvent>
#include <QTransform>

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

    createConnects();
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
    QString file = QFileDialog::getOpenFileName(this, tr("Otwórz"), "C://", tr("Wszystkie pliki (*.*);;"
                   "JPEG (*.jpg *jpeg);; PNG (*.png)"), &QString(tr("Wszystkie pliki (*.*)")));

    image.load(file);
    if (image.format() != QImage::Format_RGB32)
        image = image.convertToFormat(QImage::Format_RGB32);

    undoStack.clear();

    selectTool->resizeSelectedTab();
    selectedAreaItem->setPixmap(QPixmap());

    imageItem->setPixmap(QPixmap::fromImage(image));
}

void MainWindow::saveFileAction() {
    QString file = QFileDialog::getSaveFileName(this, tr("Zapisz jako"), "C://", "Images (*.png *.xpm *.jpg)");
    image.save(file);
}

void MainWindow::customFilter() {
    dialog = new CustomMaskDialog(this);
    connect(dialog, SIGNAL(customMask(Matrix)), this, SLOT(setCustomMask(Matrix)));

    dialog->show();
}

void MainWindow::setCustomFilter(Matrix matrix) {
    undoStack.push(image);
    redoStack.clear();
    if (ui->selectByColorButton->isChecked()) {
        image = filter.splot(image, USER_FILTER, selectTool->getSelectedTab(), matrix);
        selectedAreaItem->setTransform(QTransform().translate(-2.0, -2.0));
    } else {
        image = filter.splot(image, USER_FILTER, matrix);
    }
    imageItem->setPixmap(QPixmap::fromImage(image));
    selectTool->resizeSelectedTab();
}

void MainWindow::laplaceFilter() {
    undoStack.push(image);
    redoStack.clear();
    if (ui->selectByColorButton->isChecked()) {
        image = filter.splot(image, LAPLACE_FILTER, selectTool->getSelectedTab());
        selectedAreaItem->setTransform(QTransform().translate(-2.0, -2.0));
    } else {
        image = filter.splot(image, LAPLACE_FILTER);
    }
    imageItem->setPixmap(QPixmap::fromImage(image));
    selectTool->resizeSelectedTab();
}

void MainWindow::lowPassFilter() {
    undoStack.push(image);
    redoStack.clear();
    if (ui->selectByColorButton->isChecked()) {
        image = filter.splot(image, LOWPASS_FILTER, selectTool->getSelectedTab());
        selectedAreaItem->setTransform(QTransform().translate(-2.0, -2.0));
    } else {
        image = filter.splot(image, LOWPASS_FILTER);
    }
    imageItem->setPixmap(QPixmap::fromImage(image));
    selectTool->resizeSelectedTab();
}
#include <ctime>

void MainWindow::gaussFilter() {
    undoStack.push(image);
    redoStack.clear();
    if (ui->selectByColorButton->isChecked()) {
        image = filter.splot(image, GAUSSIAN_FILTER, selectTool->getSelectedTab());
        selectedAreaItem->setTransform(QTransform().translate(-2.0, -2.0));
    } else {
        image = filter.splot(image, GAUSSIAN_FILTER);
    }
    imageItem->setPixmap(QPixmap::fromImage(image));
    selectTool->resizeSelectedTab();
}

void MainWindow::highPassFilter() {
    undoStack.push(image);
    redoStack.clear();
    if (ui->selectByColorButton->isChecked()) {
        image = filter.splot(image, HIGHPASS_FILTER, selectTool->getSelectedTab());
        selectedAreaItem->setTransform(QTransform().translate(-2.0, -2.0));
    } else {
        image = filter.splot(image, HIGHPASS_FILTER);
    }
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
        double value = ui->featherSlider->value();
        if (ui->featherCheckBox->isChecked() && value != 0) {
            selectedAreaItem->setPixmap(QPixmap::fromImage(selectTool->selectByColor(l.x(), l.y(), ui->thresholdSlider->value(), value)));
        } else {
            selectedAreaItem->setPixmap(QPixmap::fromImage(selectTool->selectByColor(l.x(), l.y(), ui->thresholdSlider->value())));
        }
    }
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
    if((event->key() == Qt::Key_Z) && (QGuiApplication::keyboardModifiers() & Qt::ControlModifier)) {
        if(!undoStack.isEmpty()) {
            redoStack.push(image);
            image = undoStack.pop();
            imageItem->setPixmap(QPixmap::fromImage(image));
            selectTool->resizeSelectedTab();
            qDebug() << undoStack.size() << " " << redoStack.size();
        }
    }
    if((event->key() == Qt::Key_Y) && (QGuiApplication::keyboardModifiers() & Qt::ControlModifier)) {
        if(!redoStack.isEmpty()) {
            undoStack.push(image.copy(0, 0, image.width(), image.height()));
            image = redoStack.pop();
            imageItem->setPixmap(QPixmap::fromImage(image));
            selectTool->resizeSelectedTab();
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

void MainWindow::createConnects() {
    connect(ui->actionOpen, SIGNAL(triggered(bool)), this, SLOT(openFileAction()));
    connect(ui->actionSaveAs, SIGNAL(triggered(bool)), this, SLOT(saveFileAction()));

    connect(ui->actionCustomMask, SIGNAL(triggered(bool)), this, SLOT(customFilter()));
    connect(ui->actionLaplace, SIGNAL(triggered(bool)), this, SLOT(laplaceFilter()));
    connect(ui->actionLowPass, SIGNAL(triggered(bool)), this, SLOT(lowPassFilter()));
    connect(ui->actionGauss, SIGNAL(triggered(bool)), this, SLOT(gaussFilter()));
    connect(ui->actionHighPass, SIGNAL(triggered(bool)), this, SLOT(highPassFilter()));

    connect(ui->graphicsView, SIGNAL(zoomChanged(int)), this, SLOT(zoomChanged(int)));
    connect(ui->horizontalSlider, SIGNAL(valueChanged(int)), this, SLOT(sliderChanged(int)));
    connect(ui->thresholdSlider, SIGNAL(valueChanged(int)), this, SLOT(thresholdSliderValueChanged(int)));
    connect(ui->featherSlider, SIGNAL(valueChanged(int)), this, SLOT(featherSliderValueChanged(int)));
    connect(ui->selectByColorButton, SIGNAL(clicked(bool)), this, SLOT(selectByColorButtonClicked(bool)));
    connect(ui->featherCheckBox, SIGNAL(clicked(bool)), this, SLOT(featherCheckBoxChanged(bool)));
}
