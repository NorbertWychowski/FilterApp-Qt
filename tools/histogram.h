#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#include <QDialog>
#include <QtCharts/QChartView>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QLegend>
#include <QtCharts/QBarCategoryAxis>

QT_CHARTS_USE_NAMESPACE

namespace Ui {
class Histogram;
}

class Histogram : public QDialog {
    Q_OBJECT

public:
    explicit Histogram(QImage &image, QWidget *parent = 0);
    ~Histogram();

private slots:
    void changeHistogram(int index);

private:
    QBarSet *valueData;
    QBarSet *redData;
    QBarSet *greenData;
    QBarSet *blueData;
    QBarSeries *seriesValue;
    QBarSeries *seriesRed;
    QBarSeries *seriesGreen;
    QBarSeries *seriesBlue;

    QChart *chart;
    QChartView *chartView;

    int value[256];
    int red[256];
    int green[256];
    int blue[256];

    Ui::Histogram *ui;
};

#endif // HISTOGRAM_H
