#include "colormenu.h"
#include "ui_colormenu.h"

#include "filters/colortool.h"

#include <QTimer>

ColorMenu::ColorMenu(int filterColor, QWidget *parent) : QDialog(parent), ui(new Ui::ColorMenu) {
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);

    this->filterColor = filterColor;

    timer = new QTimer(this);
    ui->tabWidget->tabBar()->hide();

    createConnects();

    switch(filterColor) {
    case COLORIZE:
        ui->tabWidget->setCurrentIndex(0);
        break;
    case HUESATURATION:
        ui->tabWidget->setCurrentIndex(1);
        break;
    case BRIGHTNESSCONTRAST:
        ui->tabWidget->setCurrentIndex(2);
        break;
    }

    raise();
    setFocus();
    show();
    activateWindow();
    timer->start(1);
}

ColorMenu::~ColorMenu() {
    delete ui;
}

void ColorMenu::draw() {
    switch(filterColor) {
    case COLORIZE:
        emit colors(ui->cHueSlider->value(),
                    ui->cSaturationSlider->value()*2.55,
                    ui->cValueSlider->value()*2.55);
        break;
    case HUESATURATION:
        emit colors(ui->sHueSlider->value(),
                    ui->sSaturationSlider->value()*2.55,
                    ui->sValueSlider->value()*2.55);
        break;
    case BRIGHTNESSCONTRAST:
        emit brightnessContrast(ui->brightnessSlider->value(), ui->contrastSlider->value());
        break;
    }

    timer->stop();
}

void ColorMenu::waitWithDraw() {
    timer->start(400);
}

void ColorMenu::createConnects() {
    switch(filterColor) {
    case COLORIZE:
        connect(ui->cSaturationSlider, &QSlider::valueChanged, this, [this](int value) {
            ui->cSaturationSpinBox->setValue(value);
            waitWithDraw();
        });
        connect(ui->cHueSlider, &QSlider::valueChanged, this, [this](int value) {
            ui->cHueSpinBox->setValue(value);
            waitWithDraw();
        });
        connect(ui->cValueSlider, &QSlider::valueChanged, this, [this](int value) {
            ui->cValueSpinBox->setValue(value);
            waitWithDraw();
        });
        connect(ui->cSaturationSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, [this](int value) {
            ui->cSaturationSlider->setValue(value);
            waitWithDraw();
        });
        connect(ui->cHueSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, [this](int value) {
            ui->cHueSlider->setValue(value);
            waitWithDraw();
        });
        connect(ui->cValueSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, [this](int value) {
            ui->cValueSlider->setValue(value);
            waitWithDraw();
        });
        break;
    case HUESATURATION:
        connect(ui->sSaturationSlider, &QSlider::valueChanged, this, [this](int value) {
            ui->sSaturationSpinBox->setValue(value);
            waitWithDraw();
        });
        connect(ui->sHueSlider, &QSlider::valueChanged, this, [this](int value) {
            ui->sHueSpinBox->setValue(value);
            waitWithDraw();
        });
        connect(ui->sValueSlider, &QSlider::valueChanged, this, [this](int value) {
            ui->sValueSpinBox->setValue(value);
            waitWithDraw();
        });
        connect(ui->sSaturationSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, [this](int value) {
            ui->sSaturationSlider->setValue(value);
            waitWithDraw();
        });
        connect(ui->sHueSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, [this](int value) {
            ui->sHueSlider->setValue(value);
            waitWithDraw();
        });
        connect(ui->sValueSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, [this](int value) {
            ui->sValueSlider->setValue(value);
            waitWithDraw();
        });
        break;
    case BRIGHTNESSCONTRAST:
        connect(ui->brightnessSlider, &QSlider::valueChanged, this, [this](int value) {
            ui->brightnessSpinBox->setValue(value);
            waitWithDraw();
        });
        connect(ui->contrastSlider, &QSlider::valueChanged, this, [this](int value) {
            ui->contrastSpinBox->setValue(value);
            waitWithDraw();
        });
        connect(ui->brightnessSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, [this](int value) {
            ui->brightnessSlider->setValue(value);
            waitWithDraw();
        });
        connect(ui->contrastSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, [this](int value) {
            ui->contrastSlider->setValue(value);
            waitWithDraw();
        });
        break;
    }

    connect(timer, SIGNAL(timeout()), this, SLOT(draw()));
}
