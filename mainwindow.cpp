#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "filters/gaussianblur.h"
#include "filters/boxblur.h"

#include <QFileDialog>
#include <QGraphicsPixmapItem>
#include <QImage>
#include <QWheelEvent>
#include <QKeyEvent>
#include <QTransform>
#include <QTimer>
#include <QSignalMapper>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    installEventFilter(this);

    selectTool = new SelectionTool(&image);
    filterTool = new FilterTool;

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
    delete filterTool;
    delete imageItem;
    delete selectedAreaItem;
    delete scene;
    delete ui;
}

void MainWindow::zoomChanged(double value) {
    if (value >= 1.0)
        ui->horizontalSlider->setValue(value + 3);
    else {
        if(value == 0.125)
            ui->horizontalSlider->setValue(1);
        else if(value == 0.25)
            ui->horizontalSlider->setValue(2);
        else if(value == 0.5) {
            ui->horizontalSlider->setValue(3);
        }
    }
}

void MainWindow::sliderChanged(int value) {
    if (value >= 4) {
        ui->label->setText(QString::number((value - 3) * 100) + "%");
        ui->graphicsView->setZoom((value - 3));
    } else {
        if(value == 1) {
            ui->label->setText("12.5%");
            ui->graphicsView->setZoom(0.125);
        } else if (value == 2) {
            ui->label->setText("25%");
            ui->graphicsView->setZoom(0.25);
        } else if (value == 3) {
            ui->label->setText("50%");
            ui->graphicsView->setZoom(0.5);
        }
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
}

void MainWindow::setCustomFilter(Matrix matrix) {
    QApplication::setOverrideCursor(Qt::BusyCursor);

    redoStack.clear();
    if (isSelectMask) {
        undoStack.push({image, true});
        image = filterTool->splot(image, USER_FILTER, selectTool->getSelectedTab(), matrix);
        selectedAreaItem->moveBy(-2, -2);
    } else {
        undoStack.push({image, false});
        image = filterTool->splot(image, USER_FILTER, matrix);
    }
    imageItem->setPixmap(QPixmap::fromImage(image));
    selectTool->resizeSelectedTab();

    QApplication::setOverrideCursor(Qt::ArrowCursor);
}

void MainWindow::laplaceFilter() {
    QApplication::setOverrideCursor(Qt::BusyCursor);

    redoStack.clear();
    if (isSelectMask) {
        undoStack.push({image, true});
        image = filterTool->splot(image, LAPLACE_FILTER, selectTool->getSelectedTab());
        selectedAreaItem->moveBy(-2, -2);
    } else {
        undoStack.push({image, false});
        image = filterTool->splot(image, LAPLACE_FILTER);
    }
    imageItem->setPixmap(QPixmap::fromImage(image));
    selectTool->resizeSelectedTab();

    QApplication::setOverrideCursor(Qt::ArrowCursor);
}

void MainWindow::lowPassFilter() {
    filtersMenu = new FiltersMenu(image, LOWPASS_FILTER, this);
    filtersMenu->setWindowTitle("Filtr uśredniający");

    connect(filtersMenu, &FiltersMenu::blurRadius, this, [&](int radius) {
        QApplication::setOverrideCursor(Qt::BusyCursor);

        undoStack.push({image, false});
        redoStack.clear();
        if(isSelectMask)
            image = filterTool->lowPassFilter(image, radius, selectTool->getSelectedTab());
        else
            image = filterTool->lowPassFilter(image, radius);
        imageItem->setPixmap(QPixmap::fromImage(image));

        QApplication::setOverrideCursor(Qt::ArrowCursor);
    });
}

void MainWindow::gaussFilter() {
    filtersMenu = new FiltersMenu(image, GAUSSIAN_FILTER, this);
    filtersMenu->setWindowTitle("Rozmycie Gaussa");

    connect(filtersMenu, &FiltersMenu::blurRadius, this, [&](int radius) {
        QApplication::setOverrideCursor(Qt::BusyCursor);

        undoStack.push({image, false});
        redoStack.clear();
        if(isSelectMask)
            image = filterTool->gaussianFilter(image, radius, selectTool->getSelectedTab());
        else
            image = filterTool->gaussianFilter(image, radius);
        imageItem->setPixmap(QPixmap::fromImage(image));

        QApplication::setOverrideCursor(Qt::ArrowCursor);
    });
}

void MainWindow::highPassFilter() {
    QApplication::setOverrideCursor(Qt::BusyCursor);

    redoStack.clear();
    if (isSelectMask) {
        undoStack.push({image, true});
        image = filterTool->splot(image, HIGHPASS_FILTER, selectTool->getSelectedTab());
        selectedAreaItem->moveBy(-2, -2);
    } else {
        undoStack.push({image, false});
        image = filterTool->splot(image, HIGHPASS_FILTER);
    }
    imageItem->setPixmap(QPixmap::fromImage(image));
    selectTool->resizeSelectedTab();

    QApplication::setOverrideCursor(Qt::ArrowCursor);
}

void MainWindow::colorFilter(int colorFilter) {
    QApplication::setOverrideCursor(Qt::BusyCursor);

    redoStack.clear();
    undoStack.push({image, true});

    switch(colorFilter) {
    case NEGATIVE:
        image = ColorTool::negative(image);
        break;
    case SEPIA:
        image = ColorTool::sepia(image);
        break;
    case DESATURATE:
        image = ColorTool::desaturate(image);
        break;
    case COLORIZE:
        colorMenu = new ColorMenu(COLORIZE, this);
        colorMenu->setWindowTitle("Koloryzacja");

        connect(colorMenu, &QDialog::rejected, this, [&, this]() {
            imageItem->setPixmap(QPixmap::fromImage(image));
        });
        connect(colorMenu, &ColorMenu::colors, this, [&, this](int H, int S, int V) {
            imageItem->setPixmap(QPixmap::fromImage(ColorTool::colorize(H, S, V, image)));
        });
        connect(colorMenu, &ColorMenu::accepted, this, [&, this]() {
            image = imageItem->pixmap().toImage();
            imageItem->setPixmap(QPixmap::fromImage(image));
        });
        break;
    case HUESATURATION:
        colorMenu = new ColorMenu(HUESATURATION, this);
        colorMenu->setWindowTitle("Barwa i nasycenie");

        connect(colorMenu, &QDialog::rejected, this, [&, this]() {
            imageItem->setPixmap(QPixmap::fromImage(image));
        });
        connect(colorMenu, &ColorMenu::colors, this, [&, this](int H, int S, int V) {
            imageItem->setPixmap(QPixmap::fromImage(ColorTool::hueSaturation(H, S, V, image)));
        });
        connect(colorMenu, &ColorMenu::accepted, this, [&, this]() {
            image = imageItem->pixmap().toImage();
            imageItem->setPixmap(QPixmap::fromImage(image));
        });
        break;
    case BRIGHTNESSCONTRAST:
        colorMenu = new ColorMenu(BRIGHTNESSCONTRAST, this);
        colorMenu->setWindowTitle("Jasność i kontrast");

        connect(colorMenu, &QDialog::rejected, this, [&, this]() {
            imageItem->setPixmap(QPixmap::fromImage(image));
        });
        connect(colorMenu, &ColorMenu::brightnessContrast, this, [&, this](int B, int C) {
            imageItem->setPixmap(QPixmap::fromImage(ColorTool::brightnessContrast(B, C, image)));
        });
        connect(colorMenu, &ColorMenu::accepted, this, [&, this]() {
            image = imageItem->pixmap().toImage();
            imageItem->setPixmap(QPixmap::fromImage(image));
        });
        break;
    }

    imageItem->setPixmap(QPixmap::fromImage(image));

    QApplication::setOverrideCursor(Qt::ArrowCursor);
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
    if((event->key() == Qt::Key_Z) && (QGuiApplication::keyboardModifiers() & Qt::ControlModifier)) {
        if(!undoStack.isEmpty()) {
            stackNode tmp = undoStack.pop();
            redoStack.push({ image, tmp.movedSelection });
            image = tmp.image;
            imageItem->setPixmap(QPixmap::fromImage(image));
            selectTool->resizeSelectedTab();

            if(tmp.movedSelection)
                selectedAreaItem->moveBy(2, 2);
        }
    }
    if((event->key() == Qt::Key_Y) && (QGuiApplication::keyboardModifiers() & Qt::ControlModifier)) {
        if(!redoStack.isEmpty()) {
            stackNode tmp = redoStack.pop();
            undoStack.push({ image, tmp.movedSelection });
            image = tmp.image;
            imageItem->setPixmap(QPixmap::fromImage(image));
            selectTool->resizeSelectedTab();

            if(tmp.movedSelection)
                selectedAreaItem->moveBy(-2, -2);
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

            scene->removeItem(selectedAreaItem);
            selectedAreaItem = new QGraphicsPixmapItem();
            scene->addItem(selectedAreaItem);

            if (ui->featherCheckBox->isChecked() && value != 0) {
                selectedAreaItem->setPixmap(QPixmap::fromImage(selectTool->selectByColor(l.x(), l.y(), ui->thresholdSlider->value(), value)));
            } else {
                selectedAreaItem->setPixmap(QPixmap::fromImage(selectTool->selectByColor(l.x(), l.y(), ui->thresholdSlider->value())));
            }

            isSelectMask = true;

            timer.start(500);

            QApplication::setOverrideCursor(Qt::ArrowCursor);
        }
    }
}

void MainWindow::thresholdSliderValueChanged(int value) {
    ui->thresholdLabel->setNum(value);
}

void MainWindow::featherSliderValueChanged(int value) {
    ui->featherLabel->setNum(value);
}

void MainWindow::featherCheckBoxChanged(bool visible) {
    ui->featherSlider->setEnabled(visible);
    ui->featherSlider->setVisible(visible);
    ui->featherLabel->setVisible(visible);
}

void MainWindow::selectByColorButtonClicked(bool b) {
    ui->toolsOptions->setVisible(b);
    ui->toolsOptions->setEnabled(b);

    selectedAreaItem->setPixmap(QPixmap());

    if (!b) {
        isSelectMask = false;

        timer.stop();
    }
    if(ui->rectangleSelectButton->isChecked())
        ui->rectangleSelectButton->setChecked(false);
}

void MainWindow::rectangleSelectButtonClicked(bool clicked) {
    ui->toolsOptions->setVisible(false);
    ui->toolsOptions->setEnabled(false);

    selectedAreaItem->setPixmap(QPixmap());

    if (clicked) {
        ui->graphicsView->enableRectSelect();
    } else {
        ui->graphicsView->disableRectSelect();
        isSelectMask = false;

        timer.stop();
    }
    if(ui->selectByColorButton->isChecked())
        ui->selectByColorButton->setChecked(false);
}

void MainWindow::rectangleSelection(QRect rectangleSelectionArea) {
    QRect tmp = rectangleSelectionArea;

    if(tmp.left() < 0) tmp.setLeft(0);
    if(tmp.right() >= image.width()) tmp.setRight(image.width() - 1);
    if(tmp.top() < 0) tmp.setTop(0);
    if(tmp.bottom() >= image.height()) tmp.setBottom(image.height() - 1);

    scene->removeItem(selectedAreaItem);
    selectedAreaItem = new QGraphicsPixmapItem();
    scene->addItem(selectedAreaItem);

    selectedAreaItem->setPixmap(QPixmap::fromImage(selectTool->rectangleSelect(tmp)));
    selectedAreaItem->resetTransform();
    isSelectMask = true;

    timer.start(500);
}

void MainWindow::invertSelectedAreaColors() {
    QImage tmp = selectedAreaItem->pixmap().toImage();
    tmp.invertPixels();
    selectedAreaItem->setPixmap(QPixmap::fromImage(tmp));
}

void MainWindow::createConnects() {
    //wczytaj / zapisz
    connect(ui->actionOpen,         SIGNAL(triggered(bool)),    this, SLOT(openFileAction()));
    connect(ui->actionSaveAs,       SIGNAL(triggered(bool)),    this, SLOT(saveFileAction()));
    //filtry
    connect(ui->actionCustomMask,   SIGNAL(triggered(bool)),    this, SLOT(customFilter()));
    connect(ui->actionLaplace,      SIGNAL(triggered(bool)),    this, SLOT(laplaceFilter()));
    connect(ui->actionLowPass,      SIGNAL(triggered(bool)),    this, SLOT(lowPassFilter()));
    connect(ui->actionGauss,        SIGNAL(triggered(bool)),    this, SLOT(gaussFilter()));
    connect(ui->actionHighPass,     SIGNAL(triggered(bool)),    this, SLOT(highPassFilter()));
    //filtry koloru
    QSignalMapper *signalMapper = new QSignalMapper(this);

    connect(ui->actionNegative,     SIGNAL(triggered()),        signalMapper, SLOT(map()));
    connect(ui->actionSepia,        SIGNAL(triggered()),        signalMapper, SLOT(map()));
    connect(ui->actionDesaturate,   SIGNAL(triggered()),        signalMapper, SLOT(map()));
    connect(ui->actionColorize,     SIGNAL(triggered()),        signalMapper, SLOT(map()));
    connect(ui->actionBrightnessContrast, SIGNAL(triggered()),  signalMapper, SLOT(map()));
    connect(ui->actionHueSaturation,SIGNAL(triggered()),        signalMapper, SLOT(map()));
    signalMapper->setMapping(ui->actionNegative,    NEGATIVE);
    signalMapper->setMapping(ui->actionSepia,       SEPIA);
    signalMapper->setMapping(ui->actionDesaturate,  DESATURATE);
    signalMapper->setMapping(ui->actionColorize,    COLORIZE);
    signalMapper->setMapping(ui->actionBrightnessContrast, BRIGHTNESSCONTRAST);
    signalMapper->setMapping(ui->actionHueSaturation, HUESATURATION);

    connect(signalMapper,           SIGNAL(mapped(int)),        this, SLOT(colorFilter(int)));
    //kontrolki i zoom
    connect(ui->graphicsView,       SIGNAL(zoomChanged(double)),this, SLOT(zoomChanged(double)));
    connect(ui->horizontalSlider,   SIGNAL(valueChanged(int)),  this, SLOT(sliderChanged(int)));
    connect(ui->thresholdSlider,    SIGNAL(valueChanged(int)),  this, SLOT(thresholdSliderValueChanged(int)));
    connect(ui->featherSlider,      SIGNAL(valueChanged(int)),  this, SLOT(featherSliderValueChanged(int)));
    connect(ui->featherCheckBox,    SIGNAL(clicked(bool)),      this, SLOT(featherCheckBoxChanged(bool)));
    //wybor zaznaczenia i utworzenie jego maski
    connect(ui->selectByColorButton,  SIGNAL(clicked(bool)),    this, SLOT(selectByColorButtonClicked(bool)));
    connect(ui->rectangleSelectButton,SIGNAL(clicked(bool)),    this, SLOT(rectangleSelectButtonClicked(bool)));
    connect(ui->graphicsView,       SIGNAL(selectedArea(QRect)),this, SLOT(rectangleSelection(QRect)));
    //zmiana koloru zaznaczenia co 0.5s
    connect(&timer,                 SIGNAL(timeout()),          this, SLOT(invertSelectedAreaColors()));
}

void MainWindow::initScene() {
    scene = new QGraphicsScene();
    ui->graphicsView->setScene(scene);

    imageItem = new QGraphicsPixmapItem();
    selectedAreaItem = new QGraphicsPixmapItem();
    scene->addItem(imageItem);
    scene->addItem(selectedAreaItem);
}
