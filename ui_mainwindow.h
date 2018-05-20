/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QWidget>
#include "mygraphicsview.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionOpen;
    QAction *actionCustomMask;
    QAction *actionLaplace;
    QAction *actionLowPass;
    QAction *actionGauss;
    QAction *actionHighPass;
    QAction *actionSaveAs;
    QAction *actionHueSaturation;
    QAction *actionColorize;
    QAction *actionBrightnessContrast;
    QAction *actionNegative;
    QAction *actionSepia;
    QAction *actionDesaturate;
    QAction *actionHistogram;
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    QLabel *zoomLabel;
    QSlider *zoomSlider;
    myGraphicsView *graphicsView;
    QFrame *frame;
    QGridLayout *gridLayout_2;
    QSpacerItem *verticalSpacer;
    QTabWidget *toolsOptions;
    QWidget *selectByColorTab;
    QGridLayout *gridLayout_3;
    QSlider *thresholdSlider;
    QLabel *thresholdLabel;
    QLabel *featherLabel;
    QSlider *featherSlider;
    QSpacerItem *verticalSpacer_2;
    QLabel *label_2;
    QCheckBox *featherCheckBox;
    QWidget *brushTab;
    QGridLayout *gridLayout_4;
    QLabel *label_4;
    QLabel *brushSizeLabel;
    QSlider *brushSizeSlider;
    QSpacerItem *verticalSpacer_3;
    QLabel *label_3;
    QPushButton *brushColorButton;
    QGroupBox *groupBox;
    QGridLayout *gridLayout_5;
    QPushButton *rectangleSelectButton;
    QPushButton *selectByColorButton;
    QPushButton *brushButton;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuFiltry;
    QMenu *menuWykrycie_kraw_dzi;
    QMenu *menuRozmycie;
    QMenu *menuUwydatnienie;
    QMenu *menuKolory;
    QMenu *menuNarz_dzia;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(1600, 900);
        MainWindow->setBaseSize(QSize(10, 10));
        actionOpen = new QAction(MainWindow);
        actionOpen->setObjectName(QStringLiteral("actionOpen"));
        actionCustomMask = new QAction(MainWindow);
        actionCustomMask->setObjectName(QStringLiteral("actionCustomMask"));
        actionLaplace = new QAction(MainWindow);
        actionLaplace->setObjectName(QStringLiteral("actionLaplace"));
        actionLowPass = new QAction(MainWindow);
        actionLowPass->setObjectName(QStringLiteral("actionLowPass"));
        actionGauss = new QAction(MainWindow);
        actionGauss->setObjectName(QStringLiteral("actionGauss"));
        actionHighPass = new QAction(MainWindow);
        actionHighPass->setObjectName(QStringLiteral("actionHighPass"));
        actionSaveAs = new QAction(MainWindow);
        actionSaveAs->setObjectName(QStringLiteral("actionSaveAs"));
        actionHueSaturation = new QAction(MainWindow);
        actionHueSaturation->setObjectName(QStringLiteral("actionHueSaturation"));
        actionColorize = new QAction(MainWindow);
        actionColorize->setObjectName(QStringLiteral("actionColorize"));
        actionBrightnessContrast = new QAction(MainWindow);
        actionBrightnessContrast->setObjectName(QStringLiteral("actionBrightnessContrast"));
        actionNegative = new QAction(MainWindow);
        actionNegative->setObjectName(QStringLiteral("actionNegative"));
        actionSepia = new QAction(MainWindow);
        actionSepia->setObjectName(QStringLiteral("actionSepia"));
        actionDesaturate = new QAction(MainWindow);
        actionDesaturate->setObjectName(QStringLiteral("actionDesaturate"));
        actionHistogram = new QAction(MainWindow);
        actionHistogram->setObjectName(QStringLiteral("actionHistogram"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        gridLayout = new QGridLayout(centralWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        zoomLabel = new QLabel(centralWidget);
        zoomLabel->setObjectName(QStringLiteral("zoomLabel"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(zoomLabel->sizePolicy().hasHeightForWidth());
        zoomLabel->setSizePolicy(sizePolicy);

        gridLayout->addWidget(zoomLabel, 1, 2, 1, 1);

        zoomSlider = new QSlider(centralWidget);
        zoomSlider->setObjectName(QStringLiteral("zoomSlider"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(zoomSlider->sizePolicy().hasHeightForWidth());
        zoomSlider->setSizePolicy(sizePolicy1);
        zoomSlider->setMinimumSize(QSize(150, 0));
        zoomSlider->setMinimum(1);
        zoomSlider->setMaximum(1);
        zoomSlider->setPageStep(1);
        zoomSlider->setValue(1);
        zoomSlider->setOrientation(Qt::Horizontal);

        gridLayout->addWidget(zoomSlider, 1, 3, 1, 1);

        graphicsView = new myGraphicsView(centralWidget);
        graphicsView->setObjectName(QStringLiteral("graphicsView"));
        graphicsView->setAlignment(Qt::AlignCenter);
        graphicsView->setRenderHints(QPainter::Antialiasing|QPainter::HighQualityAntialiasing|QPainter::TextAntialiasing);

        gridLayout->addWidget(graphicsView, 0, 1, 1, 3);

        frame = new QFrame(centralWidget);
        frame->setObjectName(QStringLiteral("frame"));
        frame->setMinimumSize(QSize(250, 0));
        frame->setMaximumSize(QSize(250, 16777215));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Sunken);
        gridLayout_2 = new QGridLayout(frame);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_2->addItem(verticalSpacer, 3, 0, 1, 2);

        toolsOptions = new QTabWidget(frame);
        toolsOptions->setObjectName(QStringLiteral("toolsOptions"));
        toolsOptions->setEnabled(true);
        selectByColorTab = new QWidget();
        selectByColorTab->setObjectName(QStringLiteral("selectByColorTab"));
        gridLayout_3 = new QGridLayout(selectByColorTab);
        gridLayout_3->setSpacing(6);
        gridLayout_3->setContentsMargins(11, 11, 11, 11);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        thresholdSlider = new QSlider(selectByColorTab);
        thresholdSlider->setObjectName(QStringLiteral("thresholdSlider"));
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(thresholdSlider->sizePolicy().hasHeightForWidth());
        thresholdSlider->setSizePolicy(sizePolicy2);
        thresholdSlider->setMinimumSize(QSize(172, 0));
        thresholdSlider->setStyleSheet(QLatin1String("QSlider::groove:horizontal {\n"
"    border: 1px solid black;\n"
"    height: 20px;\n"
"    margin: 2px 0;\n"
"}\n"
"\n"
"QSlider::handle:horizontal {\n"
"	background-color: black;\n"
"    border: 1px solid black;\n"
"    width: 2px;\n"
"    border-radius: 3px;\n"
"}\n"
"\n"
"QSlider::sub-page {\n"
"	background: #3add36;\n"
"	border: 1px solid black;\n"
"}\n"
"\n"
"QSlider::sub-page:horizontal {\n"
"    border: 1px solid black;\n"
"    height: 20px;\n"
"    margin: 2px 0;\n"
"}"));
        thresholdSlider->setMaximum(100);
        thresholdSlider->setSingleStep(1);
        thresholdSlider->setPageStep(1);
        thresholdSlider->setValue(15);
        thresholdSlider->setOrientation(Qt::Horizontal);

        gridLayout_3->addWidget(thresholdSlider, 1, 0, 1, 1);

        thresholdLabel = new QLabel(selectByColorTab);
        thresholdLabel->setObjectName(QStringLiteral("thresholdLabel"));
        QSizePolicy sizePolicy3(QSizePolicy::Minimum, QSizePolicy::Preferred);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(thresholdLabel->sizePolicy().hasHeightForWidth());
        thresholdLabel->setSizePolicy(sizePolicy3);
        thresholdLabel->setMinimumSize(QSize(25, 0));
        thresholdLabel->setMaximumSize(QSize(20, 16777215));

        gridLayout_3->addWidget(thresholdLabel, 1, 1, 1, 1);

        featherLabel = new QLabel(selectByColorTab);
        featherLabel->setObjectName(QStringLiteral("featherLabel"));
        sizePolicy3.setHeightForWidth(featherLabel->sizePolicy().hasHeightForWidth());
        featherLabel->setSizePolicy(sizePolicy3);
        featherLabel->setMinimumSize(QSize(25, 0));
        featherLabel->setMaximumSize(QSize(20, 16777215));

        gridLayout_3->addWidget(featherLabel, 3, 1, 1, 1);

        featherSlider = new QSlider(selectByColorTab);
        featherSlider->setObjectName(QStringLiteral("featherSlider"));
        sizePolicy2.setHeightForWidth(featherSlider->sizePolicy().hasHeightForWidth());
        featherSlider->setSizePolicy(sizePolicy2);
        featherSlider->setMinimumSize(QSize(172, 0));
        featherSlider->setStyleSheet(QLatin1String("QSlider::groove:horizontal {\n"
"    border: 1px solid black;\n"
"    height: 20px;\n"
"    margin: 2px 0;\n"
"}\n"
"\n"
"QSlider::handle:horizontal {\n"
"	background-color: black;\n"
"    border: 1px solid black;\n"
"    width: 2px;\n"
"    border-radius: 3px;\n"
"}\n"
"\n"
"QSlider::sub-page {\n"
"	background: #3add36;\n"
"	border: 1px solid black;\n"
"}\n"
"\n"
"QSlider::sub-page:horizontal {\n"
"    border: 1px solid black;\n"
"    height: 20px;\n"
"    margin: 2px 0;\n"
"}"));
        featherSlider->setMaximum(100);
        featherSlider->setSingleStep(1);
        featherSlider->setPageStep(1);
        featherSlider->setValue(15);
        featherSlider->setOrientation(Qt::Horizontal);

        gridLayout_3->addWidget(featherSlider, 3, 0, 1, 1);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_3->addItem(verticalSpacer_2, 4, 0, 1, 2);

        label_2 = new QLabel(selectByColorTab);
        label_2->setObjectName(QStringLiteral("label_2"));
        QSizePolicy sizePolicy4(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(label_2->sizePolicy().hasHeightForWidth());
        label_2->setSizePolicy(sizePolicy4);

        gridLayout_3->addWidget(label_2, 0, 0, 1, 2);

        featherCheckBox = new QCheckBox(selectByColorTab);
        featherCheckBox->setObjectName(QStringLiteral("featherCheckBox"));
        sizePolicy4.setHeightForWidth(featherCheckBox->sizePolicy().hasHeightForWidth());
        featherCheckBox->setSizePolicy(sizePolicy4);

        gridLayout_3->addWidget(featherCheckBox, 2, 0, 1, 2);

        toolsOptions->addTab(selectByColorTab, QString());
        brushTab = new QWidget();
        brushTab->setObjectName(QStringLiteral("brushTab"));
        gridLayout_4 = new QGridLayout(brushTab);
        gridLayout_4->setSpacing(6);
        gridLayout_4->setContentsMargins(11, 11, 11, 11);
        gridLayout_4->setObjectName(QStringLiteral("gridLayout_4"));
        label_4 = new QLabel(brushTab);
        label_4->setObjectName(QStringLiteral("label_4"));

        gridLayout_4->addWidget(label_4, 2, 0, 1, 1);

        brushSizeLabel = new QLabel(brushTab);
        brushSizeLabel->setObjectName(QStringLiteral("brushSizeLabel"));
        brushSizeLabel->setMinimumSize(QSize(25, 0));

        gridLayout_4->addWidget(brushSizeLabel, 1, 1, 1, 1);

        brushSizeSlider = new QSlider(brushTab);
        brushSizeSlider->setObjectName(QStringLiteral("brushSizeSlider"));
        brushSizeSlider->setStyleSheet(QLatin1String("QSlider::groove:horizontal {\n"
"    border: 1px solid black;\n"
"    height: 20px;\n"
"    margin: 2px 0;\n"
"}\n"
"\n"
"QSlider::handle:horizontal {\n"
"	background-color: black;\n"
"    border: 1px solid black;\n"
"    width: 2px;\n"
"    border-radius: 3px;\n"
"}\n"
"\n"
"QSlider::sub-page {\n"
"	background: #3add36;\n"
"	border: 1px solid black;\n"
"}\n"
"\n"
"QSlider::sub-page:horizontal {\n"
"    border: 1px solid black;\n"
"    height: 20px;\n"
"    margin: 2px 0;\n"
"}"));
        brushSizeSlider->setMinimum(1);
        brushSizeSlider->setMaximum(200);
        brushSizeSlider->setValue(5);
        brushSizeSlider->setOrientation(Qt::Horizontal);

        gridLayout_4->addWidget(brushSizeSlider, 1, 0, 1, 1);

        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_4->addItem(verticalSpacer_3, 4, 0, 1, 2);

        label_3 = new QLabel(brushTab);
        label_3->setObjectName(QStringLiteral("label_3"));

        gridLayout_4->addWidget(label_3, 0, 0, 1, 1);

        brushColorButton = new QPushButton(brushTab);
        brushColorButton->setObjectName(QStringLiteral("brushColorButton"));
        brushColorButton->setStyleSheet(QLatin1String("QPushButton {\n"
"	background-color: black;\n"
"}"));

        gridLayout_4->addWidget(brushColorButton, 3, 0, 1, 1);

        toolsOptions->addTab(brushTab, QString());

        gridLayout_2->addWidget(toolsOptions, 4, 0, 1, 2);

        groupBox = new QGroupBox(frame);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setStyleSheet(QStringLiteral(""));
        groupBox->setFlat(false);
        gridLayout_5 = new QGridLayout(groupBox);
        gridLayout_5->setSpacing(6);
        gridLayout_5->setContentsMargins(11, 11, 11, 11);
        gridLayout_5->setObjectName(QStringLiteral("gridLayout_5"));
        rectangleSelectButton = new QPushButton(groupBox);
        rectangleSelectButton->setObjectName(QStringLiteral("rectangleSelectButton"));
        QIcon icon;
        icon.addFile(QStringLiteral("icons/rectangle_select_icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        rectangleSelectButton->setIcon(icon);
        rectangleSelectButton->setCheckable(true);
        rectangleSelectButton->setAutoExclusive(true);

        gridLayout_5->addWidget(rectangleSelectButton, 0, 0, 1, 1);

        selectByColorButton = new QPushButton(groupBox);
        selectByColorButton->setObjectName(QStringLiteral("selectByColorButton"));
        QIcon icon1;
        icon1.addFile(QStringLiteral("icons/magic_wand_icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        selectByColorButton->setIcon(icon1);
        selectByColorButton->setCheckable(true);
        selectByColorButton->setAutoRepeat(false);
        selectByColorButton->setAutoExclusive(true);

        gridLayout_5->addWidget(selectByColorButton, 0, 1, 1, 1);

        brushButton = new QPushButton(groupBox);
        brushButton->setObjectName(QStringLiteral("brushButton"));
        QIcon icon2;
        icon2.addFile(QStringLiteral("icons/brush_icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        brushButton->setIcon(icon2);
        brushButton->setCheckable(true);
        brushButton->setAutoExclusive(true);

        gridLayout_5->addWidget(brushButton, 0, 2, 1, 1);


        gridLayout_2->addWidget(groupBox, 0, 0, 1, 2);


        gridLayout->addWidget(frame, 0, 0, 1, 1);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1600, 25));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        menuFile->setStyleSheet(QStringLiteral(""));
        menuFiltry = new QMenu(menuBar);
        menuFiltry->setObjectName(QStringLiteral("menuFiltry"));
        menuWykrycie_kraw_dzi = new QMenu(menuFiltry);
        menuWykrycie_kraw_dzi->setObjectName(QStringLiteral("menuWykrycie_kraw_dzi"));
        menuRozmycie = new QMenu(menuFiltry);
        menuRozmycie->setObjectName(QStringLiteral("menuRozmycie"));
        menuUwydatnienie = new QMenu(menuFiltry);
        menuUwydatnienie->setObjectName(QStringLiteral("menuUwydatnienie"));
        menuKolory = new QMenu(menuBar);
        menuKolory->setObjectName(QStringLiteral("menuKolory"));
        menuNarz_dzia = new QMenu(menuBar);
        menuNarz_dzia->setObjectName(QStringLiteral("menuNarz_dzia"));
        MainWindow->setMenuBar(menuBar);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuFiltry->menuAction());
        menuBar->addAction(menuKolory->menuAction());
        menuBar->addAction(menuNarz_dzia->menuAction());
        menuFile->addAction(actionOpen);
        menuFile->addSeparator();
        menuFile->addAction(actionSaveAs);
        menuFiltry->addAction(menuRozmycie->menuAction());
        menuFiltry->addAction(menuUwydatnienie->menuAction());
        menuFiltry->addAction(menuWykrycie_kraw_dzi->menuAction());
        menuFiltry->addAction(actionCustomMask);
        menuWykrycie_kraw_dzi->addAction(actionLaplace);
        menuRozmycie->addAction(actionLowPass);
        menuRozmycie->addAction(actionGauss);
        menuUwydatnienie->addAction(actionHighPass);
        menuKolory->addAction(actionHueSaturation);
        menuKolory->addAction(actionColorize);
        menuKolory->addAction(actionBrightnessContrast);
        menuKolory->addSeparator();
        menuKolory->addAction(actionNegative);
        menuKolory->addAction(actionSepia);
        menuKolory->addAction(actionDesaturate);
        menuNarz_dzia->addAction(actionHistogram);

        retranslateUi(MainWindow);

        toolsOptions->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "Filter App", nullptr));
        actionOpen->setText(QApplication::translate("MainWindow", "Otw\303\263rz", nullptr));
        actionCustomMask->setText(QApplication::translate("MainWindow", "W\305\202asna maska", nullptr));
        actionLaplace->setText(QApplication::translate("MainWindow", "Laplace", nullptr));
        actionLowPass->setText(QApplication::translate("MainWindow", "U\305\233redniaj\304\205cy", nullptr));
        actionGauss->setText(QApplication::translate("MainWindow", "Gauss", nullptr));
        actionHighPass->setText(QApplication::translate("MainWindow", "Wyostrzenie", nullptr));
        actionSaveAs->setText(QApplication::translate("MainWindow", "Zapisz jako...", nullptr));
        actionHueSaturation->setText(QApplication::translate("MainWindow", "Barwa i nasycenie", nullptr));
        actionColorize->setText(QApplication::translate("MainWindow", "Koloryzacja", nullptr));
        actionBrightnessContrast->setText(QApplication::translate("MainWindow", "Jasno\305\233\304\207 i kontrast", nullptr));
        actionNegative->setText(QApplication::translate("MainWindow", "Negatyw", nullptr));
        actionSepia->setText(QApplication::translate("MainWindow", "Sepia", nullptr));
        actionDesaturate->setText(QApplication::translate("MainWindow", "Desaturacja", nullptr));
        actionHistogram->setText(QApplication::translate("MainWindow", "Histogram", nullptr));
        zoomLabel->setText(QApplication::translate("MainWindow", "100%", nullptr));
        thresholdLabel->setText(QApplication::translate("MainWindow", "15", nullptr));
        featherLabel->setText(QApplication::translate("MainWindow", "15", nullptr));
        label_2->setText(QApplication::translate("MainWindow", "Progowanie", nullptr));
        featherCheckBox->setText(QApplication::translate("MainWindow", "Zmi\304\231kczanie kraw\304\231dzi", nullptr));
        toolsOptions->setTabText(toolsOptions->indexOf(selectByColorTab), QApplication::translate("MainWindow", "Narz\304\231dzia", nullptr));
        label_4->setText(QApplication::translate("MainWindow", "Kolor", nullptr));
        brushSizeLabel->setText(QApplication::translate("MainWindow", "5", nullptr));
        label_3->setText(QApplication::translate("MainWindow", "Rozmiar", nullptr));
        brushColorButton->setText(QString());
        toolsOptions->setTabText(toolsOptions->indexOf(brushTab), QApplication::translate("MainWindow", "Narz\304\231dzia", nullptr));
        groupBox->setTitle(QString());
        rectangleSelectButton->setText(QString());
        selectByColorButton->setText(QString());
        brushButton->setText(QString());
        menuFile->setTitle(QApplication::translate("MainWindow", "&Plik", nullptr));
        menuFiltry->setTitle(QApplication::translate("MainWindow", "Filtry", nullptr));
        menuWykrycie_kraw_dzi->setTitle(QApplication::translate("MainWindow", "Wykrycie kraw\304\231dzi", nullptr));
        menuRozmycie->setTitle(QApplication::translate("MainWindow", "Rozmycie", nullptr));
        menuUwydatnienie->setTitle(QApplication::translate("MainWindow", "Uwydatnienie", nullptr));
        menuKolory->setTitle(QApplication::translate("MainWindow", "Kolory", nullptr));
        menuNarz_dzia->setTitle(QApplication::translate("MainWindow", "Narz\304\231dzia", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
