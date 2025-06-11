#include "apihandler.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QUrl>

APIhandler::APIhandler(QObject *parent) : QObject(parent) {
    connect(&timer, &QTimer::timeout, this, &APIhandler::sendRequest);
}

void APIhandler::startConnection(int wait){
    timer.start(wait);
    sendRequest();
}

void APIhandler::sendRequest(){
    QUrl url("https://api.wheretheiss.at/v1/satellites/25544");
    QNetworkRequest request(url);
    QNetworkReply *reply = networkManager.get(request);
    connect(reply, &QNetworkReply::finished, this, &APIhandler::getResponse);
}

void APIhandler::getResponse(){
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());

    if(!reply)
        return;

    QByteArray responseData = reply->readAll();
    QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData);
    QJsonObject jsonObj = jsonDoc.object();

    double latitude = jsonObj["latitude"].toDouble();
    double longitude = jsonObj["longitude"].toDouble();
    double altitude = jsonObj["altitude"].toDouble();
    double velocity = jsonObj["velocity"].toDouble();
    double timestamp = jsonObj["timestamp"].toDouble();

    emit update(latitude, longitude, altitude, velocity, timestamp);

    reply->deleteLater();
}
void APIhandler::pauseLiveTracking() {
    if (timer.isActive()) {
        timer.stop();
    }
}

void APIhandler::resumeLiveTracking() {
    if (!timer.isActive()) {
        timer.start();
    }
}
void APIhandler::fetchISSAtTimestamp(qint64 timestamp) {
    QString url = QString("https://api.wheretheiss.at/v1/satellites/25544/positions?timestamps=%1&units=kilometers").arg(timestamp);
    QNetworkRequest request(url);

    QNetworkReply *reply = networkManager.get(request);


    connect(reply, &QNetworkReply::finished, this, [=]() {
        QNetworkReply *replyObj = qobject_cast<QNetworkReply*>(sender());
        if (!replyObj)
            return;

        QByteArray data = replyObj->readAll();
        QJsonDocument doc = QJsonDocument::fromJson(data);

        if (doc.isArray()) {
            QJsonArray arr = doc.array();
            if (!arr.isEmpty()) {
                QJsonObject obj = arr.first().toObject();
                double latitude = obj["latitude"].toDouble();
                double longitude = obj["longitude"].toDouble();
                double altitude = obj["altitude"].toDouble();
                double velocity = obj["velocity"].toDouble();
                double timestamp = obj["timestamp"].toDouble();

                emit update(latitude, longitude, altitude, velocity, timestamp);
            }
        }

        replyObj->deleteLater();
    });
}
