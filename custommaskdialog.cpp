#include "custommaskdialog.h"
#include "ui_custommaskdialog.h"

CustomMaskDialog::CustomMaskDialog(QWidget *parent) : QDialog(parent), ui(new Ui::CustomMaskDialog) {
    ui->setupUi(this);

    setAttribute(Qt::WA_DeleteOnClose);

    activateWindow();
    raise();
    setFocus();

    createSpinBoxes();
    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(buttonBoxAccepted()));
}

CustomMaskDialog::~CustomMaskDialog() {
    while (!spinBoxes.empty())
        delete spinBoxes.front(), spinBoxes.pop_front();
    delete ui;
}

void CustomMaskDialog::buttonBoxAccepted() {
    Matrix matrix(5);
    int i = 0;
    int j = 0;

    for (QSpinBox *spinBox : spinBoxes) {
        matrix[i++][j] = spinBox->value();
        if(i == 5) j++;
    }
    emit customMask(matrix);
}

void CustomMaskDialog::createSpinBoxes() {
    int j = 0;
    for (int i = 0; i<25; i++) {
        if (i % 5 == 0)
            j++;

        QSpinBox *spinBox = new QSpinBox();
        spinBox->setMinimum(-100);
        spinBox->setMaximum(100);
        spinBox->setValue(0);
        spinBox->setButtonSymbols(QSpinBox::NoButtons);
        qobject_cast<QGridLayout*>(ui->widget->layout())->addWidget(spinBox, j, i % 5);
        spinBoxes.push_back(spinBox);
    }
}
