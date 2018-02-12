#include "histogram.h"
#include "ui_histogram.h"

#include <QLegendMarker>

Histogram::Histogram(QImage &image, QWidget *parent) : QDialog(parent), ui(new Ui::Histogram) {
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);

    chart = new QChart();

    for(int i = 0; i < 256; i++)
        value[i] = red[i] = green[i] = blue[i] = 0;

    for(int y = 0; y < image.height(); y++) {
        QRgb *line = (QRgb*)image.scanLine(y);

        for(int x = 0; x < image.width(); x++) {
            QColor color = QColor(line[x]);
            value[color.value()]++;
            red[color.red()]++;
            green[color.green()]++;
            blue[color.blue()]++;
        }
    }
    valueData = new QBarSet("", chart);
    redData = new QBarSet("", chart);
    greenData = new QBarSet("", chart);
    blueData = new QBarSet("", chart);

    valueData->setPen(QColor(Qt::black));
    valueData->setBrush(Qt::black);
    redData->setPen(QColor(Qt::red));
    redData->setBrush(Qt::red);
    greenData->setPen(QColor(Qt::green));
    greenData->setBrush(Qt::green);
    blueData->setPen(QColor(Qt::blue));
    blueData->setBrush(Qt::blue);

    for(int i = 0; i < 256; i++) {
        *valueData << value[i];
        *redData << red[i];
        *greenData << green[i];
        *blueData << blue[i];
    }

    seriesValue = new QBarSeries(chart);
    seriesRed = new QBarSeries(chart);
    seriesGreen = new QBarSeries(chart);
    seriesBlue = new QBarSeries(chart);
    seriesValue->append(valueData);
    seriesRed->append(redData);
    seriesGreen->append(greenData);
    seriesBlue->append(blueData);

    chart->addSeries(seriesValue);
    chart->addSeries(seriesRed);
    chart->addSeries(seriesGreen);
    chart->addSeries(seriesBlue);

    chart->legend()->markers(seriesValue)[0]->setVisible(false);
    chart->legend()->markers(seriesRed)[0]->setVisible(false);
    chart->legend()->markers(seriesGreen)[0]->setVisible(false);
    chart->legend()->markers(seriesBlue)[0]->setVisible(false);

    seriesValue->setVisible(true);
    seriesRed->setVisible(false);
    seriesGreen->setVisible(false);
    seriesBlue->setVisible(false);

    chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    ui->gridLayout->addWidget(chartView, 1, 0, 1, 1);

    raise();
    setFocus();
    show();
    activateWindow();

    connect(ui->comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(changeHistogram(int)));
}

Histogram::~Histogram() {
    delete chart;
    delete ui;
}

void Histogram::changeHistogram(int index) {
    switch(index) {
    case 0:
        seriesValue->setVisible(true);
        seriesRed->setVisible(false);
        seriesGreen->setVisible(false);
        seriesBlue->setVisible(false);

        chart->legend()->markers(seriesValue)[0]->setVisible(false);
        break;
    case 1:
        seriesValue->setVisible(false);
        seriesRed->setVisible(true);
        seriesGreen->setVisible(false);
        seriesBlue->setVisible(false);

        chart->legend()->markers(seriesRed)[0]->setVisible(false);
        break;
    case 2:
        seriesValue->setVisible(false);
        seriesRed->setVisible(false);
        seriesGreen->setVisible(true);
        seriesBlue->setVisible(false);

        chart->legend()->markers(seriesGreen)[0]->setVisible(false);
        break;
    case 3:
        seriesValue->setVisible(false);
        seriesRed->setVisible(false);
        seriesGreen->setVisible(false);
        seriesBlue->setVisible(true);

        chart->legend()->markers(seriesBlue)[0]->setVisible(false);
        break;
    case 4:
        seriesRed->setVisible(true);
        seriesGreen->setVisible(true);
        seriesBlue->setVisible(true);

        chart->legend()->markers(seriesRed)[0]->setVisible(false);
        chart->legend()->markers(seriesGreen)[0]->setVisible(false);
        chart->legend()->markers(seriesBlue)[0]->setVisible(false);
        break;
    }
}
