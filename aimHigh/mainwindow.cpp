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

    networkManager = new QNetworkAccessManager(this);

    issLocationLabel = new QLabel("Loading data...", this);
    setCentralWidget(issLocationLabel);

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::fetchISSData);
    timer->start(5000);

    fetchISSData();

}

void MainWindow::fetchISSData(){
    // URL to the ISS API
    QUrl url("https://api.wheretheiss.at/v1/satellites/25544");

    // Create the network request
    QNetworkRequest request(url);

    // Send the request and connect the reply signal to the handleResponse method
    QNetworkReply *reply = networkManager->get(request);
    connect(reply, &QNetworkReply::finished, this, &MainWindow::handleResponse);
}

void MainWindow::handleResponse(){
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());
    if (!reply)
        return;

    if (reply->error() != QNetworkReply::NoError) {
        issLocationLabel->setText("Failed to fetch ISS data.");
        reply->deleteLater();
        return;
    }

    // Parse the JSON response
    QByteArray responseData = reply->readAll();
    QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData);
    QJsonObject jsonObj = jsonDoc.object();

    // Extract latitude and longitude from the JSON response
    double latitude = jsonObj["latitude"].toDouble();
    double longitude = jsonObj["longitude"].toDouble();

    // Update the QLabel with the new data
    issLocationLabel->setText(QString("ISS Location:\nLat: %1\nLon: %2").arg(latitude).arg(longitude));

    // Clean up the reply
    reply->deleteLater();
}

MainWindow::~MainWindow(){
    delete ui;
}
