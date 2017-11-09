#ifndef CUSTOMMASKDIALOG_H
#define CUSTOMMASKDIALOG_H

#include <QDialog>
#include <QSpinBox>
#include "matrix.h"

namespace Ui {
class CustomMaskDialog;
}

class CustomMaskDialog : public QDialog {
    Q_OBJECT

signals:
    void customMask(Matrix);

public:
    explicit CustomMaskDialog(QWidget *parent = 0);
    ~CustomMaskDialog();

private slots:
    void buttonBoxAccepted();

private:
    Ui::CustomMaskDialog *ui;

    QList<QSpinBox*> spinBoxes;
};

#endif // CUSTOMMASKDIALOG_H
