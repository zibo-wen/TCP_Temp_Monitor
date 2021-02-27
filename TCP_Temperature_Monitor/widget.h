#ifndef WIDGET_H
#define WIDGET_H

#include <QTime>
#include <QWidget>
#include <QtNetwork>
#include <QHostAddress>
#include <QChartView>
#include <QtCharts/QScatterSeries>
#include <QtCharts/QLineSeries>
#include <QLabel>
#include <QLCDNumber>
#include <QMediaPlayer>
#include <QDir>
#include <QSound>

QT_CHARTS_USE_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

protected:
    bool setServer();
    void setCharts();
    void timerEvent(QTimerEvent *event);

    QTime timer;

    QTcpServer *server;
    QTcpSocket *tempDataSocket;

    QLabel         *title;
    QLabel         *timeLabel;
    QLabel         *currTemp;
    QChartView     *chartView;
    QChart         *mainChart;
    QScatterSeries *dotSeries;
    QLineSeries    *alertLine;
    QLCDNumber     *currTempLCD;
    QMediaPlayer   *player;

private slots:
    void newConnectionDetect();
};
#endif // WIDGET_H
