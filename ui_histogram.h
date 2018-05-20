/********************************************************************************
** Form generated from reading UI file 'histogram.ui'
**
** Created by: Qt User Interface Compiler version 5.10.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_HISTOGRAM_H
#define UI_HISTOGRAM_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>

QT_BEGIN_NAMESPACE

class Ui_Histogram
{
public:
    QGridLayout *gridLayout;

    void setupUi(QDialog *Histogram)
    {
        if (Histogram->objectName().isEmpty())
            Histogram->setObjectName(QStringLiteral("Histogram"));
        Histogram->resize(900, 500);
        QSizePolicy sizePolicy(QSizePolicy::Ignored, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(Histogram->sizePolicy().hasHeightForWidth());
        Histogram->setSizePolicy(sizePolicy);
        Histogram->setModal(true);
        gridLayout = new QGridLayout(Histogram);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));

        retranslateUi(Histogram);

        QMetaObject::connectSlotsByName(Histogram);
    } // setupUi

    void retranslateUi(QDialog *Histogram)
    {
        Histogram->setWindowTitle(QApplication::translate("Histogram", "Histogram", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Histogram: public Ui_Histogram {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_HISTOGRAM_H
