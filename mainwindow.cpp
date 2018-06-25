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

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    installEventFilter(this);

    selectTool = new SelectionTool(&image);
    filterTool = new FilterTool;

    ui->featherSlider->setVisible(false);
    ui->featherSlider->setEnabled(false);
    ui->featherLabel->setVisible(false);
    ui->toolsOptions->setVisible(false);
    ui->toolsOptions->setEnabled(false);
    ui->toolsOptions->tabBar()->hide();
    ui->actionSaveAs->setEnabled(false);
    ui->groupBox->setEnabled(false);
    ui->menuFiltry->setEnabled(false);
    ui->menuKolory->setEnabled(false);
    ui->menuNarz_dzia->setEnabled(false);

    createConnects();
    initScene();
    isSelectMask = false;
    brushColor = Qt::black;

    ui->zoomSlider->setMaximum(myGraphicsView::getZoomValues().size());

    int pos = 1;
    for(auto it :  myGraphicsView::getZoomValues().keys()) {
        if(it == 1.0) break;
        pos++;
    }
    ui->zoomSlider->setValue(pos);
}

MainWindow::~MainWindow() {
    delete selectTool;
    delete filterTool;
    delete imageItem;
    delete selectedAreaItem;
    delete scene;
    delete ui;
}

void MainWindow::zoomChanged(double key) {
    ui->zoomLabel->setText(myGraphicsView::getZoomValues().value(key));

    int pos = 1;
    for(auto it :  myGraphicsView::getZoomValues().keys()) {
        if(it == key) break;
        pos++;
    }

    ui->zoomSlider->setValue(pos);
}

void MainWindow::zoomSliderChanged(int value) {
    int pos = 1;
    for(auto it :  myGraphicsView::getZoomValues().keys()) {
        if(pos++ == value) {
            ui->graphicsView->setZoom(it);
            ui->zoomLabel->setText(myGraphicsView::getZoomValues().value(it));
            break;
        }
    }
}

void MainWindow::openFileAction() {
    QString file = QFileDialog::getOpenFileName(this, tr("Otwórz"), "C://", tr("Wszystkie pliki (*.*);;"
                   "JPEG (*.jpg *jpeg);; PNG (*.png)"), &QString(tr("Wszystkie pliki (*.*)")));
    if(!file.isEmpty()) {
        image.load(file);
        if (image.format() != QImage::Format_RGB32)
            image = image.convertToFormat(QImage::Format_RGB32);

        ui->actionSaveAs->setEnabled(true);
        ui->groupBox->setEnabled(true);
        ui->menuFiltry->setEnabled(true);
        ui->menuKolory->setEnabled(true);
        ui->menuNarz_dzia->setEnabled(true);

        undoStack.clear();
        redoStack.clear();

        selectTool->resizeSelectedTab();
        selectedAreaItem->setPixmap(QPixmap());

        imageItem->setPixmap(QPixmap::fromImage(image));
        scene->setSceneRect(image.rect());
    }
}

void MainWindow::saveFileAction() {
    QString file = QFileDialog::getSaveFileName(this, tr("Zapisz jako"), "C://", "Images (*.png *.xpm *.jpg)");
    image.save(file);
}

void MainWindow::customFilter() {
    dialog = new CustomMaskDialog(this);
    connect(dialog, SIGNAL(customMask(Matrix)), SLOT(setCustomFilter(Matrix)));
}

void MainWindow::setCustomFilter(Matrix matrix) {
    QApplication::setOverrideCursor(Qt::BusyCursor);

    redoStack.clear();
    if (isSelectMask) {
        undoStack.push(image);
        image = filterTool->splot(image, USER_FILTER, selectTool->getSelectedTab(), matrix);
        selectedAreaItem->moveBy(-2, -2);
    } else {
        undoStack.push(image);
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
        undoStack.push(image);
        image = filterTool->splot(image, LAPLACE_FILTER, selectTool->getSelectedTab());
        selectedAreaItem->moveBy(-2, -2);
    } else {
        undoStack.push(image);
        image = filterTool->splot(image, LAPLACE_FILTER);
    }
    imageItem->setPixmap(QPixmap::fromImage(image));
    selectTool->resizeSelectedTab();

    QApplication::setOverrideCursor(Qt::ArrowCursor);
}

void MainWindow::lowPassFilter() {
    filtersMenu = new FiltersMenu(image, LOWPASS_FILTER, this);
    filtersMenu->setWindowTitle("Filtr uśredniający");

    connect(filtersMenu, &FiltersMenu::blurRadius, [&](int radius) {
        QApplication::setOverrideCursor(Qt::BusyCursor);

        undoStack.push(image);
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

    connect(filtersMenu, &FiltersMenu::blurRadius, [&](int radius) {
        QApplication::setOverrideCursor(Qt::BusyCursor);

        undoStack.push(image);
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
        undoStack.push(image);
        image = filterTool->splot(image, HIGHPASS_FILTER, selectTool->getSelectedTab());
        selectedAreaItem->moveBy(-2, -2);
    } else {
        undoStack.push(image);
        image = filterTool->splot(image, HIGHPASS_FILTER);
    }
    imageItem->setPixmap(QPixmap::fromImage(image));
    selectTool->resizeSelectedTab();

    QApplication::setOverrideCursor(Qt::ArrowCursor);
}

void MainWindow::showHistogram() {
    if(!image.isNull())
        histogram = new Histogram(image, this);
}

void MainWindow::colorFilter(int colorFilter) {
    QApplication::setOverrideCursor(Qt::BusyCursor);

    redoStack.clear();
    undoStack.push(image);

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

        connect(colorMenu, &QDialog::rejected, [&, this]() {
            imageItem->setPixmap(QPixmap::fromImage(image));
        });
        connect(colorMenu, &ColorMenu::colors, [&, this](int H, int S, int V) {
            imageItem->setPixmap(QPixmap::fromImage(ColorTool::colorize(H, S, V, image)));
        });
        connect(colorMenu, &ColorMenu::accepted, [&, this]() {
            image = imageItem->pixmap().toImage();
            imageItem->setPixmap(QPixmap::fromImage(image));
        });
        break;
    case HUESATURATION:
        colorMenu = new ColorMenu(HUESATURATION, this);
        colorMenu->setWindowTitle("Barwa i nasycenie");

        connect(colorMenu, &QDialog::rejected, [&, this]() {
            imageItem->setPixmap(QPixmap::fromImage(image));
        });
        connect(colorMenu, &ColorMenu::colors, [&, this](int H, int S, int V) {
            imageItem->setPixmap(QPixmap::fromImage(ColorTool::hueSaturation(H, S, V, image)));
        });
        connect(colorMenu, &ColorMenu::accepted, [&, this]() {
            image = imageItem->pixmap().toImage();
            imageItem->setPixmap(QPixmap::fromImage(image));
        });
        break;
    case BRIGHTNESSCONTRAST:
        colorMenu = new ColorMenu(BRIGHTNESSCONTRAST, this);
        colorMenu->setWindowTitle("Jasność i kontrast");

        connect(colorMenu, &QDialog::rejected, [&, this]() {
            imageItem->setPixmap(QPixmap::fromImage(image));
        });
        connect(colorMenu, &ColorMenu::brightnessContrast, [&, this](int B, int C) {
            imageItem->setPixmap(QPixmap::fromImage(ColorTool::brightnessContrast(B, C, image)));
        });
        connect(colorMenu, &ColorMenu::accepted, [&, this]() {
            image = imageItem->pixmap().toImage();
            imageItem->setPixmap(QPixmap::fromImage(image));
        });
        break;
    }

    imageItem->setPixmap(QPixmap::fromImage(image));

    QApplication::setOverrideCursor(Qt::ArrowCursor);
}

void MainWindow::keyPressEvent(QKeyEvent* event) {
    if((event->key() == Qt::Key_Z) && (QGuiApplication::keyboardModifiers() & Qt::ControlModifier)) {
        if(!undoStack.isEmpty()) {
            QImage tmp = undoStack.pop();
            redoStack.push(image);
            image = tmp.copy();
            imageItem->setPixmap(QPixmap::fromImage(image));

            selectTool->resizeSelectedTab();
            scene->removeItem(selectedAreaItem);
            selectedAreaItem = new QGraphicsPixmapItem();
            scene->addItem(selectedAreaItem);

            isSelectMask = false;
        }
    }
    if((event->key() == Qt::Key_Y) && (QGuiApplication::keyboardModifiers() & Qt::ControlModifier)) {
        if(!redoStack.isEmpty()) {
            QImage tmp = redoStack.pop();
            undoStack.push(image);
            image = tmp.copy();
            imageItem->setPixmap(QPixmap::fromImage(image));

            selectTool->resizeSelectedTab();
            scene->removeItem(selectedAreaItem);
            selectedAreaItem = new QGraphicsPixmapItem();
            scene->addItem(selectedAreaItem);
        }
    }
}

void MainWindow::mousePressEvent(QMouseEvent* event) {
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

void MainWindow::brushSizeSliderValueChanged(int value) {
    ui->brushSizeLabel->setNum(value);
    if(brushColor.spec() != QColor::Invalid) {
        ui->graphicsView->setBrush(brushColor, value);
        ui->brushColorButton->setStyleSheet("background-color: " + brushColor.name());
    }
}

void MainWindow::featherSliderValueChanged(int value) {
    ui->featherLabel->setNum(value);
}

void MainWindow::featherCheckBoxChanged(bool visible) {
    ui->featherSlider->setEnabled(visible);
    ui->featherSlider->setVisible(visible);
    ui->featherLabel->setVisible(visible);
}

void MainWindow::rectangleSelectButtonClicked(bool) {
    ui->toolsOptions->setVisible(false);
    ui->toolsOptions->setEnabled(false);

    selectedAreaItem->setPixmap(QPixmap());
    isSelectMask = false;

    ui->graphicsView->enableRectSelect();
}

void MainWindow::selectByColorButtonClicked(bool) {
    ui->toolsOptions->setVisible(true);
    ui->toolsOptions->setEnabled(true);

    selectedAreaItem->setPixmap(QPixmap());
    ui->graphicsView->disableRectSelect();
    isSelectMask = false;

    ui->toolsOptions->setCurrentIndex(0);
}

void MainWindow::brushButtonClicked(bool) {
    ui->toolsOptions->setVisible(true);
    ui->toolsOptions->setEnabled(true);

    selectedAreaItem->setPixmap(QPixmap());
    ui->graphicsView->disableRectSelect();
    ui->graphicsView->enableBrush();
    ui->graphicsView->setBrush(brushColor, ui->brushSizeSlider->value());

    isSelectMask = false;
    timer.stop();

    ui->toolsOptions->setCurrentIndex(1);
}

void MainWindow::brushColorButtonClicked(bool) {
    QColor tmp  = QColorDialog::getColor(Qt::green, this, "Wybierz kolor");
    if(tmp.spec() != QColor::Invalid) {
        brushColor = tmp;
        ui->graphicsView->setBrush(brushColor, ui->brushSizeSlider->value());
        ui->brushColorButton->setStyleSheet("background-color: " + brushColor.name());
    }
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

void MainWindow::updateUndoRedoStack() {
    QStack<QImage> tmp = ui->graphicsView->getPreviousVersions();
    ui->graphicsView->deletePreviousVersions();

    image = ui->graphicsView->getImage();

    for(QImage i : tmp)
        undoStack.push(i);

    redoStack.clear();
}

void MainWindow::createConnects() {
    //wczytaj / zapisz
    connect(ui->actionOpen,         SIGNAL(triggered(bool)), SLOT(openFileAction()));
    connect(ui->actionSaveAs,       SIGNAL(triggered(bool)), SLOT(saveFileAction()));
    //filtry
    connect(ui->actionCustomMask,   SIGNAL(triggered(bool)), SLOT(customFilter()));
    connect(ui->actionLaplace,      SIGNAL(triggered(bool)), SLOT(laplaceFilter()));
    connect(ui->actionLowPass,      SIGNAL(triggered(bool)), SLOT(lowPassFilter()));
    connect(ui->actionGauss,        SIGNAL(triggered(bool)), SLOT(gaussFilter()));
    connect(ui->actionHighPass,     SIGNAL(triggered(bool)), SLOT(highPassFilter()));
    connect(ui->actionHistogram,    SIGNAL(triggered(bool)), SLOT(showHistogram()));

    //filtry koloru
    QSignalMapper* signalMapper = new QSignalMapper(this);
    connect(ui->actionNegative,           SIGNAL(triggered()), signalMapper, SLOT(map()));
    connect(ui->actionSepia,              SIGNAL(triggered()), signalMapper, SLOT(map()));
    connect(ui->actionDesaturate,         SIGNAL(triggered()), signalMapper, SLOT(map()));
    connect(ui->actionColorize,           SIGNAL(triggered()), signalMapper, SLOT(map()));
    connect(ui->actionBrightnessContrast, SIGNAL(triggered()), signalMapper, SLOT(map()));
    connect(ui->actionHueSaturation,      SIGNAL(triggered()), signalMapper, SLOT(map()));
    signalMapper->setMapping(ui->actionNegative,           NEGATIVE);
    signalMapper->setMapping(ui->actionSepia,              SEPIA);
    signalMapper->setMapping(ui->actionDesaturate,         DESATURATE);
    signalMapper->setMapping(ui->actionColorize,           COLORIZE);
    signalMapper->setMapping(ui->actionBrightnessContrast, BRIGHTNESSCONTRAST);
    signalMapper->setMapping(ui->actionHueSaturation,      HUESATURATION);
    connect(signalMapper, SIGNAL(mapped(int)), SLOT(colorFilter(int)));

    //kontrolki i zoom
    connect(ui->graphicsView,          SIGNAL(zoomChanged(double)), SLOT(zoomChanged(double)));
    connect(ui->zoomSlider,            SIGNAL(valueChanged(int)),   SLOT(zoomSliderChanged(int)));
    connect(ui->thresholdSlider,       SIGNAL(valueChanged(int)),   SLOT(thresholdSliderValueChanged(int)));
    connect(ui->brushSizeSlider,       SIGNAL(valueChanged(int)),   SLOT(brushSizeSliderValueChanged(int)));
    connect(ui->featherSlider,         SIGNAL(valueChanged(int)),   SLOT(featherSliderValueChanged(int)));
    connect(ui->featherCheckBox,       SIGNAL(clicked(bool)),       SLOT(featherCheckBoxChanged(bool)));
    connect(ui->selectByColorButton,   SIGNAL(clicked(bool)),       SLOT(selectByColorButtonClicked(bool)));
    connect(ui->brushButton,           SIGNAL(clicked(bool)),       SLOT(brushButtonClicked(bool)));
    connect(ui->brushColorButton,      SIGNAL(clicked(bool)),       SLOT(brushColorButtonClicked(bool)));
    connect(ui->rectangleSelectButton, SIGNAL(clicked(bool)),       SLOT(rectangleSelectButtonClicked(bool)));
    connect(ui->graphicsView,          SIGNAL(selectedArea(QRect)), SLOT(rectangleSelection(QRect)));
    connect(&timer,                    SIGNAL(timeout()),           SLOT(invertSelectedAreaColors()));
    connect(ui->graphicsView,          SIGNAL(updateUndoRedoStack()), SLOT(updateUndoRedoStack()));
}

void MainWindow::initScene() {
    scene = new QGraphicsScene();
    ui->graphicsView->setScene(scene);

    imageItem = new QGraphicsPixmapItem();
    selectedAreaItem = new QGraphicsPixmapItem();
    scene->addItem(imageItem);
    scene->addItem(selectedAreaItem);
}
