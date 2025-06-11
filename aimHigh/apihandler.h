// apihandler.h

/**
 * @file apihandler.h
 * @brief Fetches ISS data periodically from the web API.
 */

#ifndef APIHANDLER_H
#define APIHANDLER_H

#include <QObject>
#include <QTimer>
#include <QNetworkAccessManager>
#include <QNetworkReply>

/**
 * @class APIhandler
 * @brief Polls the ISS API and publishes new data.
 */
class APIhandler : public QObject{

    Q_OBJECT

public:
    /**
     * @brief APIhandler constructor.
     */
    explicit APIhandler(QObject *parent = nullptr);
    /**
     * @brief Start polling the API every `wait` milliseconds.
     * @param wait interval for next API call in ms (5000ms).
     */
    void startConnection(int wait = 5000);

    /**
     * @brief Stops timer for live tracking.
     */
    void pauseLiveTracking();
    /**
     * @brief Resumes timer for live tracking
     */
    void resumeLiveTracking();
    /**
     * @brief Connects and sends request using API call to specific date
     * @param timestamp Date to poll data from.
     */
    void fetchISSAtTimestamp(qint64 timestamp);
signals:
    /**
     * @brief Emitted whenever new ISS data is available.
     * @param latidute Current latidute of the ISS.
     * @param longitude  Current longitude of the ISS.
     * @param altitude  Altitude in kilometers.
     * @param velocity  Velocity in km/h.
     * @param timestamp UTC timestamp since 1970 in seconds.
     */
    void update(double latidute, double longitude, double altitude, double velocity, double timestamp);

public slots:
    void sendRequest();         //!< Sends a new HTTP GET
    void getResponse();         //!< Parses the JSON reply

private:
    QNetworkAccessManager networkManager;   ///< Manages network calls
    QTimer timer;                           ///< Timer for trigering polls

    QTimer *updateTimer = nullptr;
};

#endif // APIHANDLER_H
