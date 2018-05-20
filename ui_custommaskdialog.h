/********************************************************************************
** Form generated from reading UI file 'custommaskdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.10.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CUSTOMMASKDIALOG_H
#define UI_CUSTOMMASKDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CustomMaskDialog
{
public:
    QGridLayout *gridLayout_2;
    QLabel *label;
    QDialogButtonBox *buttonBox;
    QWidget *widget;
    QGridLayout *gridLayout_3;

    void setupUi(QDialog *CustomMaskDialog)
    {
        if (CustomMaskDialog->objectName().isEmpty())
            CustomMaskDialog->setObjectName(QStringLiteral("CustomMaskDialog"));
        CustomMaskDialog->resize(400, 300);
        CustomMaskDialog->setModal(true);
        gridLayout_2 = new QGridLayout(CustomMaskDialog);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        label = new QLabel(CustomMaskDialog);
        label->setObjectName(QStringLiteral("label"));
        QFont font;
        font.setBold(true);
        font.setWeight(75);
        label->setFont(font);

        gridLayout_2->addWidget(label, 0, 0, 1, 1);

        buttonBox = new QDialogButtonBox(CustomMaskDialog);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        gridLayout_2->addWidget(buttonBox, 2, 0, 1, 2);

        widget = new QWidget(CustomMaskDialog);
        widget->setObjectName(QStringLiteral("widget"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(widget->sizePolicy().hasHeightForWidth());
        widget->setSizePolicy(sizePolicy);
        gridLayout_3 = new QGridLayout(widget);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));

        gridLayout_2->addWidget(widget, 1, 0, 1, 2);


        retranslateUi(CustomMaskDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), CustomMaskDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), CustomMaskDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(CustomMaskDialog);
    } // setupUi

    void retranslateUi(QDialog *CustomMaskDialog)
    {
        CustomMaskDialog->setWindowTitle(QApplication::translate("CustomMaskDialog", "W\305\202asna maska", nullptr));
        label->setText(QApplication::translate("CustomMaskDialog", "Macierz:", nullptr));
    } // retranslateUi

};

namespace Ui {
    class CustomMaskDialog: public Ui_CustomMaskDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CUSTOMMASKDIALOG_H
