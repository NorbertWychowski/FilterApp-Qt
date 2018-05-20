/********************************************************************************
** Form generated from reading UI file 'filtersmenu.ui'
**
** Created by: Qt User Interface Compiler version 5.10.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FILTERSMENU_H
#define UI_FILTERSMENU_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include "filtergraphicsview.h"

QT_BEGIN_NAMESPACE

class Ui_FiltersMenu
{
public:
    QGridLayout *gridLayout;
    QDialogButtonBox *buttonBox;
    QSpacerItem *horizontalSpacer;
    QLabel *label_2;
    QLabel *label;
    QSpinBox *radiusSpinBox;
    FilterGraphicsView *graphicsView;

    void setupUi(QDialog *FiltersMenu)
    {
        if (FiltersMenu->objectName().isEmpty())
            FiltersMenu->setObjectName(QStringLiteral("FiltersMenu"));
        FiltersMenu->resize(500, 400);
        FiltersMenu->setModal(true);
        gridLayout = new QGridLayout(FiltersMenu);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        buttonBox = new QDialogButtonBox(FiltersMenu);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        gridLayout->addWidget(buttonBox, 3, 0, 1, 3);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 2, 2, 1, 1);

        label_2 = new QLabel(FiltersMenu);
        label_2->setObjectName(QStringLiteral("label_2"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label_2->sizePolicy().hasHeightForWidth());
        label_2->setSizePolicy(sizePolicy);

        gridLayout->addWidget(label_2, 2, 0, 1, 1);

        label = new QLabel(FiltersMenu);
        label->setObjectName(QStringLiteral("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        radiusSpinBox = new QSpinBox(FiltersMenu);
        radiusSpinBox->setObjectName(QStringLiteral("radiusSpinBox"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(radiusSpinBox->sizePolicy().hasHeightForWidth());
        radiusSpinBox->setSizePolicy(sizePolicy1);
        radiusSpinBox->setValue(5);

        gridLayout->addWidget(radiusSpinBox, 2, 1, 1, 1);

        graphicsView = new FilterGraphicsView(FiltersMenu);
        graphicsView->setObjectName(QStringLiteral("graphicsView"));
        graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        graphicsView->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);

        gridLayout->addWidget(graphicsView, 1, 0, 1, 3);


        retranslateUi(FiltersMenu);
        QObject::connect(buttonBox, SIGNAL(accepted()), FiltersMenu, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), FiltersMenu, SLOT(reject()));

        QMetaObject::connectSlotsByName(FiltersMenu);
    } // setupUi

    void retranslateUi(QDialog *FiltersMenu)
    {
        FiltersMenu->setWindowTitle(QApplication::translate("FiltersMenu", "Dialog", nullptr));
        label_2->setText(QApplication::translate("FiltersMenu", "<html><head/><body><p><span style=\" font-weight:600;\">Promie\305\204 rozmycia</span></p></body></html>", nullptr));
        label->setText(QApplication::translate("FiltersMenu", "<html><head/><body><p><span style=\" font-weight:600;\">Podgl\304\205d</span></p></body></html>", nullptr));
    } // retranslateUi

};

namespace Ui {
    class FiltersMenu: public Ui_FiltersMenu {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FILTERSMENU_H
