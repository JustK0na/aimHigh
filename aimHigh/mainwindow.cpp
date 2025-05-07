#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTimer>
#include <time.h>
#include <stdio.h>

MainWindow::MainWindow(QWidget *parent):QMainWindow(parent), ui(new Ui::MainWindow){
    ui->setupUi(this);

    stats = new QWidget(this);
    issInfo = new QLabel(tr("Waiting for ISS data..."), stats);
    QFont font("Arial", 12);
    issInfo->setFont(font);
    QVBoxLayout *statsLayout = new QVBoxLayout(stats);
    statsLayout->addWidget(issInfo);


    earthModel = new QWidget(this);
    earthModel->setStyleSheet("background-color: black; border: 2px solid gray;");
    QVBoxLayout *earthLayout = new QVBoxLayout(earthModel);
    earthLayout->setContentsMargins(5, 5, 5, 5);

    earthWidget *earthView = new earthWidget(earthModel);
    earthLayout->addWidget(earthView);



    QWidget *central = new QWidget(this);
    QHBoxLayout *mainLayout = new QHBoxLayout(central);
    mainLayout->addWidget(earthModel, 3);
    mainLayout->addWidget(stats, 1);


    setCentralWidget(central);

    apiHandler = new APIhandler(this);
    connect(apiHandler, &APIhandler::update, this, &MainWindow::updateISSData);
    apiHandler->startConnection(5000);
}

void MainWindow::updateISSData(double latitude, double longitude, double altitude, double velocity, double timestamp){
    int timestampINT = (int)timestamp;
    //from 1 january 1970
    //+2 h bo UTC.

    struct tm ts;
    char buf[80];
    time_t now = (time_t)timestampINT;

    ts = *localtime(&now);
    strftime(buf, sizeof(buf), "%a %Y-%m-%d %H:%M:%S %Z", &ts);

    QString info = tr("ISS is now here: \nLatitude: %1\nLongitude: %2\nAltitude: %3 km\nVelocity: %4 km\\h\nDate: %5")
                       .arg(latitude, 0, 'f', 2)
                       .arg(longitude, 0, 'f', 2)
                       .arg(altitude, 0, 'f', 2)
                       .arg(velocity, 0, 'f', 2)
                       .arg(buf);
    issInfo->setText(info);
    QFont font("Arial", 12);
    issInfo->setFont(font);
}


MainWindow::~MainWindow(){
    delete ui;
}
