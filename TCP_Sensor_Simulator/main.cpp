#include <QCoreApplication>
#include <QTcpSocket>
#include <QHostAddress>
#include <QDebug>
#include <random>
#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <io.h>
#include <windows.h>
#include <QCoreApplication>
#include <QLoggingCategory>
#include <QThread>

using namespace std;

int temperature;

// Randomly generate temperature data in degree Celsius, which is
// represented by a 16-bit number; the higher 8 bits are for the
// integer part, the lower 8 bits for the decimal part.
void tempGenerator(){
    short temp = 0;
    char high = 0;
    char low = 0;

    srand(GetTickCount());
    // Let the temperature have a chance of 5% to be over 37 degree celcius
    if(rand()%20 == 1){
        high = 37 + rand()%5;
    } else {
        high = 36;
    }
    low = rand()%100;

    temp = high << 8;
    temp += low;

    temperature = temp;

    //cout << hex << temp << endl;
}

int main(int argc, char *argv[]){

    // Generate random temperature data every 100ms
    QThread *thread = QThread::create([](){
        while(1){
            Sleep(100);
            tempGenerator();
        }
     });
    thread->start();

    // Send temperature data to server
    Sleep(1000);
    QTcpSocket *client = new QTcpSocket;
    QHostAddress address("127.0.0.1");
    client->connectToHost(address, 8888);
    if(client->waitForConnected()){
        QCoreApplication a(argc, argv);
        QLoggingCategory::setFilterRules("*.debug=true");

        qDebug() << "IP Info: ";
        qDebug() << client->peerAddress();
        qDebug() << client->peerPort();
        qDebug() << client->localAddress();
        qDebug() << client->localPort();

        while(1){
            Sleep((1+rand()%9)*1000);
            client->write((char *)&temperature, 2);
            qDebug() << temperature;
            client->flush();
        }

        a.exit();
    } else {
        qDebug() << "Failed to connect";
    }


    return 0;
}
