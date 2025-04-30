#ifndef APIHANDLER_H
#define APIHANDLER_H

#include <QObject>
#include <QTimer>
#include <QNetworkAccessManager>
#include <QNetworkReply>


class APIhandler : public QObject{

    Q_OBJECT

public:
    explicit APIhandler(QObject *parent = nullptr);
    void startConnection(int wait = 5000);
signals:
    void update(double latidute, double longitude, double altitude, double velocity);

public slots:
    void sendRequest();
    void getResponse();

private:
    QNetworkAccessManager networkManager;
    QTimer timer;
};

#endif // APIHANDLER_H
