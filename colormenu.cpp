#include "colormenu.h"
#include "ui_colormenu.h"

#include "filters/colortool.h"
#include <QTimer>

ColorMenu::ColorMenu(int filterColor, QWidget *parent) : QDialog(parent), ui(new Ui::ColorMenu) {
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowFlags(windowFlags() & (~Qt::WindowContextHelpButtonHint));

    this->filterColor = filterColor;
    initUI();

    createConnects();
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
    QColor red(255, 0, 0);
    QColor green(0, 255, 0);
    QColor blue(0, 0, 255);
    int H, S, V, B, C;
    int tmp;
    double F;

    switch (filterColor) {
    case COLORIZE:
        H = ui->cHueSlider->value();
        S = static_cast<int>(ui->cSaturationSlider->value() * 2.55);
        V = static_cast<int>(ui->cValueSlider->value() * 2.55);

        emit colors(H, S, V);

        red = QColor::fromHsv(H, S, qBound(0, red.value() + V, 255));
        green = QColor::fromHsv(H, S, qBound(0, green.value() + V, 255));
        blue = QColor::fromHsv(H, S, qBound(0, blue.value() + V, 255));
        break;

    case HUESATURATION:
        H = ui->sHueSlider->value();
        S = static_cast<int>(ui->sSaturationSlider->value() * 2.55);
        V = static_cast<int>(ui->sValueSlider->value() * 2.55);

        emit colors(H, S, V);

        tmp = red.hue() + H;

        if (tmp < 0) tmp += 360;
        else if (tmp > 359) tmp -= 360;

        red = QColor::fromHsv(tmp, qBound(0, red.saturation() + S, 255), qBound(0, red.value() + V, 255));

        tmp = green.hue() + H;

        if (tmp < 0) tmp += 360;
        else if (tmp > 359) tmp -= 360;

        green = QColor::fromHsv(tmp, qBound(0, green.saturation() + S, 255), qBound(0, green.value() + V, 255));

        tmp = blue.hue() + H;

        if (tmp < 0) tmp += 360;
        else if (tmp > 359) tmp -= 360;

        blue = QColor::fromHsv(tmp, qBound(0, blue.saturation() + S, 255), qBound(0, blue.value() + V, 255));
        break;

    case BRIGHTNESSCONTRAST:
        B = ui->brightnessSlider->value();
        C = ui->contrastSlider->value();

        emit brightnessContrast(B, C);

        F = (259.0 * (C + 255)) / (255.0 * (259 - C));
        red = QColor(qBound(0, static_cast<int>(F * (red.red() - 128) + 128) + B, 255),
                     qBound(0, static_cast<int>(F * (red.green() - 128) + 128) + B, 255),
                     qBound(0, static_cast<int>(F * (red.blue() - 128) + 128) + B, 255));

        green = QColor(qBound(0, static_cast<int>(F * (green.red() - 128) + 128) + B, 255),
                       qBound(0, static_cast<int>(F * (green.green() - 128) + 128) + B, 255),
                       qBound(0, static_cast<int>(F * (green.blue() - 128) + 128) + B, 255));

        blue = QColor(qBound(0, static_cast<int>(F * (blue.red() - 128) + 128) + B, 255),
                      qBound(0, static_cast<int>(F * (blue.green() - 128) + 128) + B, 255),
                      qBound(0, static_cast<int>(F * (blue.blue() - 128) + 128) + B, 255));
        break;
    }

    ui->previewRed->setStyleSheet("background-color: " + red.name());
    ui->previewGreen->setStyleSheet("background-color: " + green.name());
    ui->previewBlue->setStyleSheet("background-color: " + blue.name());

    timer->stop();
}

void ColorMenu::defaultSettings() {
    switch (filterColor) {
    case COLORIZE:
        ui->cHueSlider->setValue(180);
        ui->cHueSpinBox->setValue(180);
        ui->cSaturationSlider->setValue(50);
        ui->cSaturationSpinBox->setValue(50);
        ui->cValueSlider->setValue(0);
        ui->cValueSpinBox->setValue(0);
        break;

    case HUESATURATION:
        ui->sHueSlider->setValue(0);
        ui->sHueSpinBox->setValue(0);
        ui->sSaturationSlider->setValue(0);
        ui->sSaturationSpinBox->setValue(0);
        ui->sValueSlider->setValue(0);
        ui->sValueSpinBox->setValue(0);
        break;

    case BRIGHTNESSCONTRAST:
        ui->brightnessSlider->setValue(0);
        ui->brightnessSpinBox->setValue(0);
        ui->contrastSlider->setValue(0);
        ui->contrastSpinBox->setValue(0);
        break;
    }

}

void ColorMenu::waitWithDraw() {
    timer->start(400);
}

void ColorMenu::initUI() {
    timer = new QTimer(this);
    ui->tabWidget->tabBar()->hide();

    switch (filterColor) {
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

    ui->previewRed->setStyleSheet("background-color: #f00");
    ui->previewGreen->setStyleSheet("background-color: #0f0");
    ui->previewBlue->setStyleSheet("background-color: #00f");
}

void ColorMenu::createConnects() {
    switch (filterColor) {
    case COLORIZE:
        connect(ui->cSaturationSlider, &QSlider::valueChanged, [this](int value) {
            ui->cSaturationSpinBox->setValue(value);
            waitWithDraw();
        });
        connect(ui->cHueSlider, &QSlider::valueChanged, [this](int value) {
            ui->cHueSpinBox->setValue(value);
            waitWithDraw();
        });
        connect(ui->cValueSlider, &QSlider::valueChanged, [this](int value) {
            ui->cValueSpinBox->setValue(value);
            waitWithDraw();
        });
        connect(ui->cSaturationSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), [this](int value) {
            ui->cSaturationSlider->setValue(value);
            waitWithDraw();
        });
        connect(ui->cHueSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), [this](int value) {
            ui->cHueSlider->setValue(value);
            waitWithDraw();
        });
        connect(ui->cValueSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), [this](int value) {
            ui->cValueSlider->setValue(value);
            waitWithDraw();
        });
        break;

    case HUESATURATION:
        connect(ui->sSaturationSlider, &QSlider::valueChanged, [this](int value) {
            ui->sSaturationSpinBox->setValue(value);
            waitWithDraw();
        });
        connect(ui->sHueSlider, &QSlider::valueChanged, [this](int value) {
            ui->sHueSpinBox->setValue(value);
            waitWithDraw();
        });
        connect(ui->sValueSlider, &QSlider::valueChanged, [this](int value) {
            ui->sValueSpinBox->setValue(value);
            waitWithDraw();
        });
        connect(ui->sSaturationSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), [this](int value) {
            ui->sSaturationSlider->setValue(value);
            waitWithDraw();
        });
        connect(ui->sHueSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), [this](int value) {
            ui->sHueSlider->setValue(value);
            waitWithDraw();
        });
        connect(ui->sValueSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), [this](int value) {
            ui->sValueSlider->setValue(value);
            waitWithDraw();
        });
        break;

    case BRIGHTNESSCONTRAST:
        connect(ui->brightnessSlider, &QSlider::valueChanged, [this](int value) {
            ui->brightnessSpinBox->setValue(value);
            waitWithDraw();
        });
        connect(ui->contrastSlider, &QSlider::valueChanged, [this](int value) {
            ui->contrastSpinBox->setValue(value);
            waitWithDraw();
        });
        connect(ui->brightnessSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), [this](int value) {
            ui->brightnessSlider->setValue(value);
            waitWithDraw();
        });
        connect(ui->contrastSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), [this](int value) {
            ui->contrastSlider->setValue(value);
            waitWithDraw();
        });
        break;
    }

    connect(ui->defaultPushButton, SIGNAL(pressed()), this, SLOT(defaultSettings()));
    connect(timer, SIGNAL(timeout()), SLOT(draw()));
}
