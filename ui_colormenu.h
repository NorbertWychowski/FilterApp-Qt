/********************************************************************************
** Form generated from reading UI file 'colormenu.ui'
**
** Created by: Qt User Interface Compiler version 5.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_COLORMENU_H
#define UI_COLORMENU_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ColorMenu
{
public:
    QGridLayout *gridLayout;
    QDialogButtonBox *buttonBox;
    QTabWidget *tabWidget;
    QWidget *tabColorize;
    QGridLayout *gridLayout_2;
    QLabel *label_2;
    QSpinBox *cValueSpinBox;
    QSlider *cValueSlider;
    QSpinBox *cSaturationSpinBox;
    QSpinBox *cHueSpinBox;
    QLabel *label_3;
    QLabel *label_4;
    QSlider *cSaturationSlider;
    QSlider *cHueSlider;
    QSpacerItem *verticalSpacer;
    QLabel *label;
    QWidget *tabSaturation;
    QGridLayout *gridLayout_5;
    QLabel *label_5;
    QSlider *sHueSlider;
    QLabel *label_7;
    QSlider *sValueSlider;
    QSpinBox *sValueSpinBox;
    QSpinBox *sSaturationSpinBox;
    QSpinBox *sHueSpinBox;
    QLabel *label_6;
    QLabel *label_8;
    QSlider *sSaturationSlider;
    QSpacerItem *verticalSpacer_2;
    QWidget *tabBrightnessContrast;
    QGridLayout *gridLayout_4;
    QLabel *label_10;
    QLabel *label_11;
    QSlider *brightnessSlider;
    QSlider *contrastSlider;
    QSpinBox *brightnessSpinBox;
    QSpinBox *contrastSpinBox;
    QSpacerItem *verticalSpacer_3;
    QLabel *label_9;
    QWidget *restoreDefaultWidget;
    QGridLayout *gridLayout_3;
    QPushButton *defaultPushButton;
    QSpacerItem *horizontalSpacer;
    QWidget *previewWidget;
    QGridLayout *gridLayout_6;
    QLabel *label_14;
    QLabel *label_13;
    QLabel *label_12;
    QFrame *frame2;
    QGridLayout *gridLayout_8;
    QWidget *previewGreen;
    QFrame *frame1;
    QGridLayout *gridLayout_7;
    QWidget *previewRed;
    QFrame *frame3;
    QGridLayout *gridLayout_9;
    QWidget *previewBlue;

    void setupUi(QDialog *ColorMenu)
    {
        if (ColorMenu->objectName().isEmpty())
            ColorMenu->setObjectName(QStringLiteral("ColorMenu"));
        ColorMenu->resize(500, 400);
        ColorMenu->setMinimumSize(QSize(500, 400));
        ColorMenu->setMaximumSize(QSize(500, 400));
        ColorMenu->setModal(true);
        gridLayout = new QGridLayout(ColorMenu);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        buttonBox = new QDialogButtonBox(ColorMenu);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        gridLayout->addWidget(buttonBox, 3, 0, 1, 1);

        tabWidget = new QTabWidget(ColorMenu);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        tabWidget->setStyleSheet(QLatin1String("QTabWidget::pane {\n"
"	border: 0;\n"
"}"));
        tabWidget->setTabPosition(QTabWidget::South);
        tabWidget->setTabShape(QTabWidget::Rounded);
        tabWidget->setElideMode(Qt::ElideMiddle);
        tabWidget->setUsesScrollButtons(false);
        tabWidget->setTabBarAutoHide(false);
        tabColorize = new QWidget();
        tabColorize->setObjectName(QStringLiteral("tabColorize"));
        gridLayout_2 = new QGridLayout(tabColorize);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        label_2 = new QLabel(tabColorize);
        label_2->setObjectName(QStringLiteral("label_2"));

        gridLayout_2->addWidget(label_2, 1, 0, 1, 1);

        cValueSpinBox = new QSpinBox(tabColorize);
        cValueSpinBox->setObjectName(QStringLiteral("cValueSpinBox"));
        cValueSpinBox->setMinimum(-100);
        cValueSpinBox->setMaximum(100);
        cValueSpinBox->setValue(0);

        gridLayout_2->addWidget(cValueSpinBox, 3, 2, 1, 1);

        cValueSlider = new QSlider(tabColorize);
        cValueSlider->setObjectName(QStringLiteral("cValueSlider"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(cValueSlider->sizePolicy().hasHeightForWidth());
        cValueSlider->setSizePolicy(sizePolicy);
        cValueSlider->setMinimum(-100);
        cValueSlider->setMaximum(100);
        cValueSlider->setValue(0);
        cValueSlider->setOrientation(Qt::Horizontal);

        gridLayout_2->addWidget(cValueSlider, 3, 1, 1, 1);

        cSaturationSpinBox = new QSpinBox(tabColorize);
        cSaturationSpinBox->setObjectName(QStringLiteral("cSaturationSpinBox"));
        cSaturationSpinBox->setMaximum(100);
        cSaturationSpinBox->setValue(50);

        gridLayout_2->addWidget(cSaturationSpinBox, 2, 2, 1, 1);

        cHueSpinBox = new QSpinBox(tabColorize);
        cHueSpinBox->setObjectName(QStringLiteral("cHueSpinBox"));
        QSizePolicy sizePolicy1(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(cHueSpinBox->sizePolicy().hasHeightForWidth());
        cHueSpinBox->setSizePolicy(sizePolicy1);
        cHueSpinBox->setMaximum(359);
        cHueSpinBox->setValue(180);

        gridLayout_2->addWidget(cHueSpinBox, 1, 2, 1, 1);

        label_3 = new QLabel(tabColorize);
        label_3->setObjectName(QStringLiteral("label_3"));

        gridLayout_2->addWidget(label_3, 2, 0, 1, 1);

        label_4 = new QLabel(tabColorize);
        label_4->setObjectName(QStringLiteral("label_4"));

        gridLayout_2->addWidget(label_4, 3, 0, 1, 1);

        cSaturationSlider = new QSlider(tabColorize);
        cSaturationSlider->setObjectName(QStringLiteral("cSaturationSlider"));
        sizePolicy.setHeightForWidth(cSaturationSlider->sizePolicy().hasHeightForWidth());
        cSaturationSlider->setSizePolicy(sizePolicy);
        cSaturationSlider->setMaximum(100);
        cSaturationSlider->setValue(50);
        cSaturationSlider->setOrientation(Qt::Horizontal);

        gridLayout_2->addWidget(cSaturationSlider, 2, 1, 1, 1);

        cHueSlider = new QSlider(tabColorize);
        cHueSlider->setObjectName(QStringLiteral("cHueSlider"));
        sizePolicy.setHeightForWidth(cHueSlider->sizePolicy().hasHeightForWidth());
        cHueSlider->setSizePolicy(sizePolicy);
        cHueSlider->setMaximum(359);
        cHueSlider->setValue(180);
        cHueSlider->setOrientation(Qt::Horizontal);

        gridLayout_2->addWidget(cHueSlider, 1, 1, 1, 1);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_2->addItem(verticalSpacer, 4, 0, 1, 3);

        label = new QLabel(tabColorize);
        label->setObjectName(QStringLiteral("label"));
        QFont font;
        font.setBold(true);
        font.setWeight(75);
        label->setFont(font);

        gridLayout_2->addWidget(label, 0, 0, 1, 3);

        tabWidget->addTab(tabColorize, QString());
        tabSaturation = new QWidget();
        tabSaturation->setObjectName(QStringLiteral("tabSaturation"));
        gridLayout_5 = new QGridLayout(tabSaturation);
        gridLayout_5->setObjectName(QStringLiteral("gridLayout_5"));
        label_5 = new QLabel(tabSaturation);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setFont(font);

        gridLayout_5->addWidget(label_5, 0, 0, 1, 1);

        sHueSlider = new QSlider(tabSaturation);
        sHueSlider->setObjectName(QStringLiteral("sHueSlider"));
        sHueSlider->setMinimum(-180);
        sHueSlider->setMaximum(180);
        sHueSlider->setOrientation(Qt::Horizontal);

        gridLayout_5->addWidget(sHueSlider, 1, 1, 1, 1);

        label_7 = new QLabel(tabSaturation);
        label_7->setObjectName(QStringLiteral("label_7"));

        gridLayout_5->addWidget(label_7, 2, 0, 1, 1);

        sValueSlider = new QSlider(tabSaturation);
        sValueSlider->setObjectName(QStringLiteral("sValueSlider"));
        sValueSlider->setMinimum(-100);
        sValueSlider->setMaximum(100);
        sValueSlider->setOrientation(Qt::Horizontal);

        gridLayout_5->addWidget(sValueSlider, 3, 1, 1, 1);

        sValueSpinBox = new QSpinBox(tabSaturation);
        sValueSpinBox->setObjectName(QStringLiteral("sValueSpinBox"));
        sValueSpinBox->setMinimum(-100);
        sValueSpinBox->setMaximum(100);

        gridLayout_5->addWidget(sValueSpinBox, 3, 2, 1, 1);

        sSaturationSpinBox = new QSpinBox(tabSaturation);
        sSaturationSpinBox->setObjectName(QStringLiteral("sSaturationSpinBox"));
        sSaturationSpinBox->setMinimum(-100);
        sSaturationSpinBox->setMaximum(100);

        gridLayout_5->addWidget(sSaturationSpinBox, 2, 2, 1, 1);

        sHueSpinBox = new QSpinBox(tabSaturation);
        sHueSpinBox->setObjectName(QStringLiteral("sHueSpinBox"));
        sHueSpinBox->setMinimum(-180);
        sHueSpinBox->setMaximum(180);

        gridLayout_5->addWidget(sHueSpinBox, 1, 2, 1, 1);

        label_6 = new QLabel(tabSaturation);
        label_6->setObjectName(QStringLiteral("label_6"));

        gridLayout_5->addWidget(label_6, 1, 0, 1, 1);

        label_8 = new QLabel(tabSaturation);
        label_8->setObjectName(QStringLiteral("label_8"));

        gridLayout_5->addWidget(label_8, 3, 0, 1, 1);

        sSaturationSlider = new QSlider(tabSaturation);
        sSaturationSlider->setObjectName(QStringLiteral("sSaturationSlider"));
        sSaturationSlider->setMinimum(-100);
        sSaturationSlider->setMaximum(100);
        sSaturationSlider->setValue(0);
        sSaturationSlider->setOrientation(Qt::Horizontal);

        gridLayout_5->addWidget(sSaturationSlider, 2, 1, 1, 1);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_5->addItem(verticalSpacer_2, 4, 0, 1, 3);

        tabWidget->addTab(tabSaturation, QString());
        tabBrightnessContrast = new QWidget();
        tabBrightnessContrast->setObjectName(QStringLiteral("tabBrightnessContrast"));
        gridLayout_4 = new QGridLayout(tabBrightnessContrast);
        gridLayout_4->setObjectName(QStringLiteral("gridLayout_4"));
        label_10 = new QLabel(tabBrightnessContrast);
        label_10->setObjectName(QStringLiteral("label_10"));

        gridLayout_4->addWidget(label_10, 1, 0, 1, 1);

        label_11 = new QLabel(tabBrightnessContrast);
        label_11->setObjectName(QStringLiteral("label_11"));

        gridLayout_4->addWidget(label_11, 2, 0, 1, 1);

        brightnessSlider = new QSlider(tabBrightnessContrast);
        brightnessSlider->setObjectName(QStringLiteral("brightnessSlider"));
        brightnessSlider->setMinimum(-127);
        brightnessSlider->setMaximum(127);
        brightnessSlider->setOrientation(Qt::Horizontal);

        gridLayout_4->addWidget(brightnessSlider, 1, 1, 1, 1);

        contrastSlider = new QSlider(tabBrightnessContrast);
        contrastSlider->setObjectName(QStringLiteral("contrastSlider"));
        contrastSlider->setMinimum(-127);
        contrastSlider->setMaximum(127);
        contrastSlider->setOrientation(Qt::Horizontal);

        gridLayout_4->addWidget(contrastSlider, 2, 1, 1, 1);

        brightnessSpinBox = new QSpinBox(tabBrightnessContrast);
        brightnessSpinBox->setObjectName(QStringLiteral("brightnessSpinBox"));
        brightnessSpinBox->setMinimum(-127);
        brightnessSpinBox->setMaximum(127);

        gridLayout_4->addWidget(brightnessSpinBox, 1, 2, 1, 1);

        contrastSpinBox = new QSpinBox(tabBrightnessContrast);
        contrastSpinBox->setObjectName(QStringLiteral("contrastSpinBox"));
        contrastSpinBox->setMinimum(-127);
        contrastSpinBox->setMaximum(127);

        gridLayout_4->addWidget(contrastSpinBox, 2, 2, 1, 1);

        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_4->addItem(verticalSpacer_3, 3, 0, 1, 3);

        label_9 = new QLabel(tabBrightnessContrast);
        label_9->setObjectName(QStringLiteral("label_9"));
        label_9->setFont(font);

        gridLayout_4->addWidget(label_9, 0, 0, 1, 3);

        tabWidget->addTab(tabBrightnessContrast, QString());

        gridLayout->addWidget(tabWidget, 1, 0, 1, 1);

        restoreDefaultWidget = new QWidget(ColorMenu);
        restoreDefaultWidget->setObjectName(QStringLiteral("restoreDefaultWidget"));
        gridLayout_3 = new QGridLayout(restoreDefaultWidget);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        defaultPushButton = new QPushButton(restoreDefaultWidget);
        defaultPushButton->setObjectName(QStringLiteral("defaultPushButton"));

        gridLayout_3->addWidget(defaultPushButton, 0, 1, 1, 1);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_3->addItem(horizontalSpacer, 0, 0, 1, 1);


        gridLayout->addWidget(restoreDefaultWidget, 2, 0, 1, 1);

        previewWidget = new QWidget(ColorMenu);
        previewWidget->setObjectName(QStringLiteral("previewWidget"));
        gridLayout_6 = new QGridLayout(previewWidget);
        gridLayout_6->setObjectName(QStringLiteral("gridLayout_6"));
        label_14 = new QLabel(previewWidget);
        label_14->setObjectName(QStringLiteral("label_14"));
        label_14->setAlignment(Qt::AlignCenter);

        gridLayout_6->addWidget(label_14, 0, 2, 1, 1);

        label_13 = new QLabel(previewWidget);
        label_13->setObjectName(QStringLiteral("label_13"));
        label_13->setAlignment(Qt::AlignCenter);

        gridLayout_6->addWidget(label_13, 0, 1, 1, 1);

        label_12 = new QLabel(previewWidget);
        label_12->setObjectName(QStringLiteral("label_12"));
        label_12->setAlignment(Qt::AlignCenter);

        gridLayout_6->addWidget(label_12, 0, 0, 1, 1);

        frame2 = new QFrame(previewWidget);
        frame2->setObjectName(QStringLiteral("frame2"));
        frame2->setMinimumSize(QSize(0, 30));
        frame2->setFrameShape(QFrame::WinPanel);
        frame2->setFrameShadow(QFrame::Sunken);
        frame2->setLineWidth(2);
        gridLayout_8 = new QGridLayout(frame2);
        gridLayout_8->setObjectName(QStringLiteral("gridLayout_8"));
        gridLayout_8->setContentsMargins(2, 2, 2, 2);
        previewGreen = new QWidget(frame2);
        previewGreen->setObjectName(QStringLiteral("previewGreen"));
        previewGreen->setStyleSheet(QStringLiteral("background-color: #0f0"));

        gridLayout_8->addWidget(previewGreen, 0, 0, 1, 1);


        gridLayout_6->addWidget(frame2, 1, 1, 1, 1);

        frame1 = new QFrame(previewWidget);
        frame1->setObjectName(QStringLiteral("frame1"));
        frame1->setMinimumSize(QSize(0, 30));
        frame1->setFrameShape(QFrame::WinPanel);
        frame1->setFrameShadow(QFrame::Sunken);
        frame1->setLineWidth(2);
        gridLayout_7 = new QGridLayout(frame1);
        gridLayout_7->setObjectName(QStringLiteral("gridLayout_7"));
        gridLayout_7->setContentsMargins(2, 2, 2, 2);
        previewRed = new QWidget(frame1);
        previewRed->setObjectName(QStringLiteral("previewRed"));
        previewRed->setStyleSheet(QStringLiteral("background-color: #f00"));

        gridLayout_7->addWidget(previewRed, 0, 0, 1, 1);


        gridLayout_6->addWidget(frame1, 1, 0, 1, 1);

        frame3 = new QFrame(previewWidget);
        frame3->setObjectName(QStringLiteral("frame3"));
        frame3->setMinimumSize(QSize(0, 30));
        frame3->setFrameShape(QFrame::WinPanel);
        frame3->setFrameShadow(QFrame::Sunken);
        frame3->setLineWidth(2);
        gridLayout_9 = new QGridLayout(frame3);
        gridLayout_9->setObjectName(QStringLiteral("gridLayout_9"));
        gridLayout_9->setContentsMargins(2, 2, 2, 2);
        previewBlue = new QWidget(frame3);
        previewBlue->setObjectName(QStringLiteral("previewBlue"));
        previewBlue->setStyleSheet(QStringLiteral("background-color: #00f"));

        gridLayout_9->addWidget(previewBlue, 0, 0, 1, 1);


        gridLayout_6->addWidget(frame3, 1, 2, 1, 1);


        gridLayout->addWidget(previewWidget, 0, 0, 1, 1);


        retranslateUi(ColorMenu);
        QObject::connect(buttonBox, SIGNAL(accepted()), ColorMenu, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), ColorMenu, SLOT(reject()));

        tabWidget->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(ColorMenu);
    } // setupUi

    void retranslateUi(QDialog *ColorMenu)
    {
        ColorMenu->setWindowTitle(QApplication::translate("ColorMenu", "Dialog", nullptr));
        label_2->setText(QApplication::translate("ColorMenu", "Barwa", nullptr));
        label_3->setText(QApplication::translate("ColorMenu", "Nasycenie", nullptr));
        label_4->setText(QApplication::translate("ColorMenu", "Jasno\305\233\304\207", nullptr));
        label->setText(QApplication::translate("ColorMenu", "Kolor", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tabColorize), QApplication::translate("ColorMenu", "colorize", nullptr));
        label_5->setText(QApplication::translate("ColorMenu", "Kolor", nullptr));
#ifndef QT_NO_WHATSTHIS
        sHueSlider->setWhatsThis(QString());
#endif // QT_NO_WHATSTHIS
        label_7->setText(QApplication::translate("ColorMenu", "Nasycenie", nullptr));
#ifndef QT_NO_WHATSTHIS
        label_6->setWhatsThis(QString());
#endif // QT_NO_WHATSTHIS
        label_6->setText(QApplication::translate("ColorMenu", "Barwa", nullptr));
        label_8->setText(QApplication::translate("ColorMenu", "Jasno\305\233\304\207", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tabSaturation), QApplication::translate("ColorMenu", "saturation", nullptr));
        label_10->setText(QApplication::translate("ColorMenu", "Jasno\305\233\304\207", nullptr));
        label_11->setText(QApplication::translate("ColorMenu", "Kontrast", nullptr));
        label_9->setText(QApplication::translate("ColorMenu", "Jasno\305\233\304\207 i kontrast", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tabBrightnessContrast), QApplication::translate("ColorMenu", "brightnessContrast", nullptr));
        defaultPushButton->setText(QApplication::translate("ColorMenu", "Przywr\303\263\304\207 domy\305\233lne", nullptr));
        label_14->setText(QApplication::translate("ColorMenu", "B", nullptr));
        label_13->setText(QApplication::translate("ColorMenu", "G", nullptr));
        label_12->setText(QApplication::translate("ColorMenu", "R", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ColorMenu: public Ui_ColorMenu {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_COLORMENU_H
