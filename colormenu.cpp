#include "colormenu.h"
#include "ui_colormenu.h"

#include "filters/colortool.h"

#include <QTimer>

ColorMenu::ColorMenu(QImage image, int filterColor, QWidget *parent) : QDialog(parent), ui(new Ui::ColorMenu) {
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);

    this->filterColor = filterColor;
    this->image = image;
    copy = image.copy();

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
}

ColorMenu::~ColorMenu() {
    delete ui;
}

void ColorMenu::draw() {
    switch(filterColor) {
    case COLORIZE:
        image = ColorTool::colorize(QColor::fromHsv(ui->hueSlider->value(),
                                    ui->saturationSlider->value(),
                                    ui->valueSlider->value()), image);
        break;
    case HUESATURATION:
        break;
    case BRIGHTNESSCONTRAST:
        break;
    }

    timer->stop();
}

void ColorMenu::waitWithDraw() {
    timer->start(400);
}

void ColorMenu::createConnects() {
    connect(ui->saturationSlider, &QSlider::valueChanged, this, [this](int value) {
        ui->saturationSpinBox->setValue(value);
        waitWithDraw();
    });
    connect(ui->hueSlider, &QSlider::valueChanged, this, [this](int value) {
        ui->hueSpinBox->setValue(value);
        waitWithDraw();
    });
    connect(ui->valueSlider, &QSlider::valueChanged, this, [this](int value) {
        ui->valueSpinBox->setValue(value);
        waitWithDraw();
    });
    connect(ui->saturationSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, [this](int value) {
        ui->saturationSlider->setValue(value);
        waitWithDraw();
    });
    connect(ui->hueSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, [this](int value) {
        ui->hueSlider->setValue(value);
        waitWithDraw();
    });
    connect(ui->saturationSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, [this](int value) {
        ui->saturationSlider->setValue(value);
        waitWithDraw();
    });

    connect(this, &QDialog::rejected, this, [this]() {
        image = copy;
    });

    connect(timer, SIGNAL(timeout()), this, SLOT(draw()));
}
