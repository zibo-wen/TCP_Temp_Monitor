#include "widget.h"
#include <QtCharts/QValueAxis>


Widget::Widget(QWidget *parent)
    : QWidget(parent){    
    int windowWidth = 650;
    int windowHeight = 555;

    this->setFixedSize(windowWidth, windowHeight);

    startTimer(1000);

    title = new QLabel(this);
    title->setGeometry(0, 10, windowWidth, 50);
    QFont titleF;
    titleF.setBold(true);
    titleF.setPointSize(20);
    title->setFont(titleF);
    title->setAlignment(Qt::AlignHCenter);
    title->setText("Temperature Monitoring System");

    timeLabel = new QLabel(this);
    timeLabel->setGeometry(windowWidth/2,55,windowWidth/2,30);
    QFont f;
    f.setPointSize(10);
    timeLabel->setFont(f);
    timeLabel->setAlignment(Qt::AlignHCenter);

    currTemp  = new QLabel(this);
    currTemp->setGeometry(0,55,windowWidth/2,30);
    f.setPointSize(10);
    currTemp->setFont(f);
    currTemp->setStyleSheet("color:red");
    currTemp->setAlignment(Qt::AlignHCenter);
    currTemp->setText("Waiting for connection...");

    currTempLCD = new QLCDNumber(this);
    currTempLCD->setGeometry(windowWidth/2-90,80,180,50);

    chartView = new QChartView(this);
    setCharts();
    chartView->setGeometry(windowWidth/2-300,135,600,400);
    chartView->setChart(mainChart);

    player = new QMediaPlayer;
    player->setMedia(QUrl("qrc:/sounds/alert.mp3"));
    player->setVolume(35);

    if(setServer()){
        qDebug()<< "Waiting for connection";
        connect(server, SIGNAL(newConnection()), this, SLOT(newConnectionDetect()));
    } else {
        qDebug()<< "Failed.";
    }
}

Widget::~Widget(){
}


bool Widget::setServer(){
    server = new QTcpServer();
    QHostAddress serverAddress("127.0.0.1");
    return server->listen(serverAddress, 8888);
}

void Widget::newConnectionDetect(){
    qDebug() << "New connection detected";
    timer.start();
    currTemp->setText("New connection detected.");
    tempDataSocket = server->nextPendingConnection();

    // Triggers readyRead signal and handle the data everytime the server
    // receives new socket
    connect(tempDataSocket, &QTcpSocket::readyRead, [this](){

        // Read from socket and conver the 16 bits data into float format
        short temp = 0;
        tempDataSocket->read((char *)&temp, 2);
        short intPart = temp >> 8;
        float decimalPart = (temp & 0x00ff)/100.00;
        float floatTemp = intPart + decimalPart;
        qDebug() << floatTemp;

        // Feed temperature data into plots and delete data from 60s ago
        if (dotSeries->count() == 0){
            timer.restart();
        } else {
            float offset = timer.elapsed()/1000.0;
            for(int i = dotSeries->count()-1;i>=0; i--){
                dotSeries->replace(i, dotSeries->at(i).x()-offset, dotSeries->at(i).y());
            }
            while(dotSeries->at(0).x()<-60){
                qDebug() << "Now removing dot: "+QString::number(dotSeries->at(0).y());
                dotSeries->remove(0);
            }
        }
        dotSeries->append(0, floatTemp);
        timer.restart();

        // Display the current temperature
        currTemp->setStyleSheet("color:black");
        currTemp->setText("Current temperature: "+QString::number(floatTemp));
        currTempLCD->display(floatTemp);

        // Alert if high temperature detected
        if (floatTemp >= 37.2) {
            currTemp->setStyleSheet("color:red");
            if (player->state() == QMediaPlayer::PlayingState){
                player->stop();
            }
            player->play();
        }
    });
}


void Widget::setCharts(){
    qDebug() << "Creating Charts";

    mainChart = new QChart();
    dotSeries = new QScatterSeries();
    alertLine = new QLineSeries();

    //Add axes with formats
    QValueAxis *axisX = new QValueAxis;
    QValueAxis *axisY = new QValueAxis;
    axisX->setRange(-60, 10);
    axisX->setLabelFormat("%.2f");
    axisX->setGridLineVisible(true);
    axisX->setTickCount(8);
    axisX->setMinorTickCount(1);
    axisX->setTitleText("Time (s)");
    axisY->setRange(34, 43);
    axisY->setLabelFormat("%.2f");
    axisY->setGridLineVisible(true);
    axisY->setTickCount(10);
    axisY->setTitleText("Body Temperature (°C)");
    mainChart->addAxis(axisX, Qt::AlignBottom);
    mainChart->addAxis(axisY, Qt::AlignLeft);
    mainChart->legend()->hide();

    //Add alert line
    mainChart->addSeries(alertLine);
    alertLine->setPen(Qt::DashLine);
    alertLine->setColor(Qt::red);
    alertLine->attachAxis(axisX);
    alertLine->attachAxis(axisY);
    alertLine->append(-60, 37.2);
    alertLine->append(10, 37.2);

    //Add data points
    mainChart->addSeries(dotSeries);
    dotSeries->attachAxis(axisX);
    dotSeries->attachAxis(axisY);
}


void Widget::timerEvent(QTimerEvent * event){
    QLocale locale(QLocale("en_US"));
    timeLabel->setText(locale.toString(QDateTime::currentDateTime(), "ddd, MMMM d, yyyy, hh:mm:ss"));
}

