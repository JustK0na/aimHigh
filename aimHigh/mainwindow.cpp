#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent):QMainWindow(parent), ui(new Ui::MainWindow){
    ui->setupUi(this);

    stats = new QWidget(this);
    issInfo = new QLabel(tr("Waiting for ISS data..."), stats);
    QVBoxLayout *statsLayout = new QVBoxLayout(stats);
    statsLayout->addWidget(issInfo);


    earthModel = new QWidget(this);
    earthModel->setStyleSheet("background-color: blue;");

    QWidget *central = new QWidget(this);
    QHBoxLayout *mainLayout = new QHBoxLayout(central);
    mainLayout->addWidget(earthModel, 3);
    mainLayout->addWidget(stats, 1);


    setCentralWidget(central);

    apiHandler = new APIhandler(this);
    connect(apiHandler, &APIhandler::update, this, &MainWindow::updateISSData);
    apiHandler->startConnection(5000);
}

void MainWindow::updateISSData(double latitude, double longitude, double altitude, double velocity){
    QString info = tr("ISS is now here: \nLatitude: %1\nLongitude: %2\nAltitude: %3 km\nVelocity: %4 km\\h")
                       .arg(latitude, 0, 'f', 2)
                       .arg(longitude, 0, 'f', 2)
                       .arg(altitude, 0, 'f', 2)
                       .arg(velocity, 0, 'f', 2);
    issInfo->setText(info);
}


MainWindow::~MainWindow(){
    delete ui;
}
