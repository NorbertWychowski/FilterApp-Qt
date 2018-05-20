#include "histogram.h"
#include "ui_histogram.h"

#include <QtConcurrent/QtConcurrentRun>
#include <QFutureSynchronizer>
#include <QLegendMarker>

Histogram::Histogram(QImage& image, QWidget* parent) : QDialog(parent), ui(new Ui::Histogram) {
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowFlags(windowFlags() & (~Qt::WindowContextHelpButtonHint));

    if(!image.isNull()) {
        createChart(image);
        createConnects();
    }

    raise();
    setFocus();
    show();
    activateWindow();

    double width = chart->size().width() / 256.0;
    for(QBarSeries* s : series)
        s->setBarWidth(width < 1.0 ? 1.0 : width);
}

Histogram::~Histogram() {
    delete chart;
    delete ui;
}

void Histogram::changeHistogram() {
    QLegendMarker* marker = qobject_cast<QLegendMarker*> (sender());
    Q_ASSERT(marker);

    Qt::GlobalColor color[] = {Qt::black, Qt::red, Qt::green, Qt::blue};

    switch(marker->type()) {
    case QLegendMarker::LegendMarkerTypeBar:
        marker->series()->setVisible(!marker->series()->isVisible());
        marker->setVisible(true);

        if(marker->series()->isVisible()) {
            if(chart->series().indexOf(marker->series()) != -1)
                marker->setBrush(color[chart->series().indexOf(marker->series())]);
        } else marker->setBrush(Qt::white);
        break;
    default:
        break;
    }

    bool active[4] = {false};
    for(int i = 0; i < 4; i++)
        active[i] = series[i]->isVisible();

    int max = 0;
    for(int i = 0; i < 256; i++) {
        if(active[0])
            if(value[i] > max) max = value[i];
        if(active[1])
            if(red[i] > max) max = red[i];
        if(active[2])
            if(green[i] > max) max = green[i];
        if(active[3])
            if(blue[i] > max) max = blue[i];
    }

    axisY->setRange(0, max);
}

void Histogram::resizeEvent(QResizeEvent* e) {
    double width = chart->size().width() / 256.0;
    for(int i = 0; i < 4; i++)
        series[i]->setBarWidth(width < 1.0 ? 1.0 : width);
}

void Histogram::createChart(QImage& image) {
    int maxThreads = QThread::idealThreadCount();
    QFutureSynchronizer<void> futures;

    chart = new QChart();
    chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    for(int i = 0; i < 256; i++)
        value[i] = red[i] = green[i] = blue[i] = 0;

    auto loadData = [&](int start, int end) {
        for(int y = start; y < end; y++) {
            QRgb* line = (QRgb*)image.scanLine(y);

            for(int x = 0; x < image.width(); x++) {
                QColor color = QColor(line[x]);
                value[color.value()]++;
                red[color.red()]++;
                green[color.green()]++;
                blue[color.blue()]++;
            }
        }
    };

    int h = image.height() / maxThreads;
    for (int i = 0; i < maxThreads; ++i)
        futures.addFuture(QtConcurrent::run(loadData, i * h, (i + 1) * h));

    futures.waitForFinished();
    futures.clearFutures();

    for(int i = 0; i < 4; i++)
        data.push_back(new QBarSet("", chart));

    data[0]->setPen(QColor(Qt::black));
    data[0]->setBrush(Qt::black);
    data[1]->setPen(QColor(Qt::red));
    data[1]->setBrush(Qt::red);
    data[2]->setPen(QColor(Qt::green));
    data[2]->setBrush(Qt::green);
    data[3]->setPen(QColor(Qt::blue));
    data[3]->setBrush(Qt::blue);

    int max = value[0];
    for(int i = 0; i < 256; i++) {
        if(value[i] > max) max = value[i];

        *data[0] << value[i];
        *data[1] << red[i];
        *data[2] << green[i];
        *data[3] << blue[i];
    }

    axisY = new QValueAxis();
    axisY->setRange(0, max);
    axisY->setLabelsVisible(false);

    for(int i = 0; i < 4; i++) {
        series.push_back(new QBarSeries(chart));
        series[i]->append(data[i]);
        chart->addSeries(series[i]);
        chart->setAxisY(axisY, series[i]);

        if(i == 0) continue;
        series[i]->setVisible(false);
    }

    for(int i = 1; i < chart->legend()->markers().size(); i++) {
        chart->legend()->markers().at(i)->setVisible(true);
        chart->legend()->markers().at(i)->setBrush(Qt::white);
    }

    ui->gridLayout->addWidget(chartView, 0, 0, 1, 1);
}

void Histogram::createConnects() {
    for(QLegendMarker* marker : chart->legend()->markers()) {
        disconnect(marker, SIGNAL(clicked()), this, SLOT(changeHistogram()));
        connect(marker, SIGNAL(clicked()), this, SLOT(changeHistogram()));
    }
}
