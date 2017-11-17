#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "gaussianblur.h"
#include "boxblur.h"

#include <QFileDialog>
#include <QGraphicsPixmapItem>
#include <QImage>
#include <QWheelEvent>
#include <QKeyEvent>
#include <QTransform>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    installEventFilter(this);

    selectTool = new SelectionTool(&image);


    ui->featherSlider->setVisible(false);
    ui->featherSlider->setEnabled(false);
    ui->featherLabel->setVisible(false);
    ui->toolsOptions->setVisible(false);
    ui->toolsOptions->setEnabled(false);


    createConnects();
    initScene();
    isSelectMask = false;
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
    scene->setSceneRect(image.rect());
}

void MainWindow::saveFileAction() {
    QString file = QFileDialog::getSaveFileName(this, tr("Zapisz jako"), "C://", "Images (*.png *.xpm *.jpg)");
    image.save(file);
}

void MainWindow::customFilter() {
    dialog = new CustomMaskDialog(this);
    connect(dialog, SIGNAL(customMask(Matrix)), this, SLOT(setCustomFilter(Matrix)));

    dialog->show();
}

void MainWindow::setCustomFilter(Matrix matrix) {
    QApplication::setOverrideCursor(Qt::BusyCursor);

    undoStack.push(image);
    redoStack.clear();
    if (isSelectMask) {
        image = filter.splot(image, USER_FILTER, selectTool->getSelectedTab(), matrix);
        selectedAreaItem->moveBy(-2, -2);
    } else {
        image = filter.splot(image, USER_FILTER, matrix);
    }
    imageItem->setPixmap(QPixmap::fromImage(image));
    selectTool->resizeSelectedTab();

    QApplication::setOverrideCursor(Qt::ArrowCursor);
}

void MainWindow::laplaceFilter() {
    QApplication::setOverrideCursor(Qt::BusyCursor);

    undoStack.push(image);
    redoStack.clear();
    if (isSelectMask) {
        image = filter.splot(image, LAPLACE_FILTER, selectTool->getSelectedTab());
        selectedAreaItem->moveBy(-2, -2);
    } else {
        image = filter.splot(image, LAPLACE_FILTER);
    }
    imageItem->setPixmap(QPixmap::fromImage(image));
    selectTool->resizeSelectedTab();

    QApplication::setOverrideCursor(Qt::ArrowCursor);
}

void MainWindow::lowPassFilter() {
    filtersMenu = new FiltersMenu(image, LOWPASS_FILTER, this);
    filtersMenu->setWindowTitle("Filtr uśredniający");

    connect(filtersMenu, QOverload<int>::of(&FiltersMenu::blurRadius), this, [&](int radius) {
        QApplication::setOverrideCursor(Qt::BusyCursor);

        undoStack.push(image);
        redoStack.clear();
        if (isSelectMask) {
            image = BoxBlur(image).blur(radius, selectTool->getSelectedTab());
        } else {
            image = BoxBlur(image).blur(radius);
        }
        imageItem->setPixmap(QPixmap::fromImage(image));
        selectTool->resizeSelectedTab();

        QApplication::setOverrideCursor(Qt::ArrowCursor);
    });
}

void MainWindow::gaussFilter() {
    filtersMenu = new FiltersMenu(image, GAUSSIAN_FILTER, this);
    filtersMenu->setWindowTitle("Rozmycie Gaussa");

    connect(filtersMenu, QOverload<int>::of(&FiltersMenu::blurRadius), this, [&](int radius) {
        QApplication::setOverrideCursor(Qt::BusyCursor);

        undoStack.push(image);
        redoStack.clear();
        if (isSelectMask) {
            image = GaussianBlur(image).blur(radius, selectTool->getSelectedTab());
        } else {
            image = GaussianBlur(image).blur(radius);
        }
        imageItem->setPixmap(QPixmap::fromImage(image));
        selectTool->resizeSelectedTab();

        QApplication::setOverrideCursor(Qt::ArrowCursor);
    });
}

void MainWindow::highPassFilter() {
    QApplication::setOverrideCursor(Qt::BusyCursor);

    undoStack.push(image);
    redoStack.clear();
    if (isSelectMask) {
        image = filter.splot(image, HIGHPASS_FILTER, selectTool->getSelectedTab());
        selectedAreaItem->moveBy(-2, -2);
    } else {
        image = filter.splot(image, HIGHPASS_FILTER);
    }
    imageItem->setPixmap(QPixmap::fromImage(image));
    selectTool->resizeSelectedTab();

    QApplication::setOverrideCursor(Qt::ArrowCursor);
}

void MainWindow::thresholdSliderValueChanged(int value) {
    ui->thresholdLabel->setNum(value);
}

void MainWindow::featherSliderValueChanged(int value) {
    ui->featherLabel->setNum(value);
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
    if((event->key() == Qt::Key_Z) && (QGuiApplication::keyboardModifiers() & Qt::ControlModifier)) {
        if(!undoStack.isEmpty()) {
            redoStack.push(image);
            image = undoStack.pop();
            imageItem->setPixmap(QPixmap::fromImage(image));
            selectTool->resizeSelectedTab();
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

void MainWindow::mousePressEvent(QMouseEvent *event) {
    if (event->buttons() & Qt::LeftButton) {
        if(ui->selectByColorButton->isChecked()) {
            QPoint local = ui->graphicsView->mapFromGlobal(event->globalPos());
            QPointF l = ui->graphicsView->mapToScene(local);
            double value = ui->featherSlider->value();

            QApplication::setOverrideCursor(Qt::BusyCursor);

            if (ui->featherCheckBox->isChecked() && value != 0) {
                selectedAreaItem->setPixmap(QPixmap::fromImage(selectTool->selectByColor(l.x(), l.y(), ui->thresholdSlider->value(), value)));
            } else {
                selectedAreaItem->setPixmap(QPixmap::fromImage(selectTool->selectByColor(l.x(), l.y(), ui->thresholdSlider->value())));
            }

            isSelectMask = true;

            QApplication::setOverrideCursor(Qt::ArrowCursor);
        }
    }
}

void MainWindow::featherCheckBoxChanged(bool b) {
    ui->featherSlider->setEnabled(b);
    ui->featherSlider->setVisible(b);
    ui->featherLabel->setVisible(b);
}

void MainWindow::createConnects() {
    connect(ui->actionOpen,         SIGNAL(triggered(bool)),    this, SLOT(openFileAction()));
    connect(ui->actionSaveAs,       SIGNAL(triggered(bool)),    this, SLOT(saveFileAction()));

    connect(ui->actionCustomMask,   SIGNAL(triggered(bool)),    this, SLOT(customFilter()));
    connect(ui->actionLaplace,      SIGNAL(triggered(bool)),    this, SLOT(laplaceFilter()));
    connect(ui->actionLowPass,      SIGNAL(triggered(bool)),    this, SLOT(lowPassFilter()));
    connect(ui->actionGauss,        SIGNAL(triggered(bool)),    this, SLOT(gaussFilter()));
    connect(ui->actionHighPass,     SIGNAL(triggered(bool)),    this, SLOT(highPassFilter()));

    connect(ui->graphicsView,       SIGNAL(zoomChanged(int)),   this, SLOT(zoomChanged(int)));
    connect(ui->horizontalSlider,   SIGNAL(valueChanged(int)),  this, SLOT(sliderChanged(int)));
    connect(ui->thresholdSlider,    SIGNAL(valueChanged(int)),  this, SLOT(thresholdSliderValueChanged(int)));
    connect(ui->featherSlider,      SIGNAL(valueChanged(int)),  this, SLOT(featherSliderValueChanged(int)));
    connect(ui->featherCheckBox,    SIGNAL(clicked(bool)),      this, SLOT(featherCheckBoxChanged(bool)));

    connect(ui->selectByColorButton, &QPushButton::clicked,     this, [this](bool b) {
        ui->toolsOptions->setVisible(b);
        ui->toolsOptions->setEnabled(b);

        selectedAreaItem->setPixmap(QPixmap());

        if (!b) {
            isSelectMask = false;
        }
        if(ui->rectangleSelectButton->isChecked())
            ui->rectangleSelectButton->setChecked(false);
    });

    connect(ui->rectangleSelectButton, &QPushButton::clicked,   this, [this](bool b) {
        ui->toolsOptions->setVisible(false);
        ui->toolsOptions->setEnabled(false);

        selectedAreaItem->setPixmap(QPixmap());

        if (b) {
            ui->graphicsView->enableRectSelect();
        } else {
            ui->graphicsView->disableRectSelect();
            isSelectMask = false;
        }
        if(ui->selectByColorButton->isChecked())
            ui->selectByColorButton->setChecked(false);
    });

    connect(ui->graphicsView, &myGraphicsView::rectSelected,    this, [this](QRect selectRect) {
        selectedAreaItem->setPixmap(QPixmap::fromImage(selectTool->rectangleSelect(selectRect)));
        isSelectMask = true;
    });
}

void MainWindow::initScene() {
    scene = new QGraphicsScene();
    ui->graphicsView->setScene(scene);

    imageItem = new QGraphicsPixmapItem();
    selectedAreaItem = new QGraphicsPixmapItem();
    scene->addItem(imageItem);
    scene->addItem(selectedAreaItem);
}
