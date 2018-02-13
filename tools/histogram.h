#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#include <QDialog>
#include <QChartView>
#include <QBarSeries>
#include <QBarSet>
#include <QLegend>
#include <QBarCategoryAxis>
#include <QValueAxis>

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
    void changeHistogram();
    void resizeEvent(QResizeEvent *e);

private:
    void createChart(QImage &image);
    void createConnects();

    QVector<QBarSet *> data;
    QVector<QBarSeries *> series;

    QChart *chart;
    QChartView *chartView;
    QValueAxis *axisY;

    int value[256];
    int red[256];
    int green[256];
    int blue[256];

    Ui::Histogram *ui;
};

#endif // HISTOGRAM_H
