// mainwindow.h

/**
 * @file mainwindow.h
 * @brief Defines the MainWindow class.
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QWidget>
#include <QVBoxLayout>
#include <Qt3DCore>
#include <QPushButton>
#include <QDateTimeEdit>
#include "apihandler.h"
#include "earthwidget.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE
/**
 * @class MainWindow
 * @brief Main application window showing an Earth model and live ISS stats.
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * @brief main window constructor.
     */
    MainWindow(QWidget *parent = nullptr);
    /**
     * @brief Loads 3D scenes from blender.
     */
    QWidget* init3DScene();
    /**
     * @brief Moves object from blender that simulates ISS.
     * @param lat  Current latitude of the ISS.
     * @param lon Current longitude of the ISS.
     * @param altKm  Altitude in kilometers.
     */
    QVector3D convertLatLonTo3D(float lat, float lon, float altKm);
    /**
     * @brief Rotates Earth and ISS to simulate passing days.
     * @param timestamp Current time.
     */
    void updateEarthRotation(double timestamp);
    /**
     * @brief Updates all string to switch to another language..
     * @param languageCode string: "pl" or "en".
     */
    void switchLanguage(const QString &languageCode);

    /** @brief Destructor. */
    ~MainWindow();

private slots:
    /**
     * @brief Slot that updates ISS data whenever new arrives.
     * @param latitude  Current latitude of the ISS.
     * @param longitude Current longitude of the ISS.
     * @param altitude  Altitude in kilometers.
     * @param velocity  Velocity in km/h.
     * @param timestamp UTC timestamp since 1970 in seconds.
     */
    void updateISSData(double latidude, double longitude, double altitude, double velocity, double timestamp);
    /**
     * @brief Sends infot to apihandler to get data about specific time.
     */
    void fetchISSAtSelectedTime();
    /**
     * @brief Sends info to apihandler to start timer for live tracking
     */
    void resumeLiveTracking();

private:
    Ui::MainWindow *ui;         ///< Qt Main Widget

    Qt3DCore::QTransform *issTransform = nullptr; ///< Transforms positions of ISS imported from blender

    Qt3DCore::QTransform *earthRotationTransform = nullptr; ///< Transforms positions of Earth imported from blender

    Qt3DCore::QEntity *earthSystemRoot = nullptr; ///< Locks both ISS and Earth to rotate them together with passing day

    APIhandler *apiHandler;     ///< Handles the network calls
    earthWidget *earthView;     ///<Manages earth and ISS rendering

    QWidget *stats;             ///< Container for the ISS stats labels
    QLabel *issInfo;            ///< Label showing formatted ISS data

    QWidget *earthModel;        ///< Widget for Earth model

    QWidget *container;         ///< Container for blender scene

    QDateTimeEdit *dateTimeEdit; ///< Changing date for ISS position calculations

    QPushButton *resumeLiveButton; ///< Button for user to set current date

    QPushButton *fetchDateButton; ///< Button for user to set custom date

    QTranslator translator;

    QString currentLanguage;
};
#endif // MAINWINDOW_H
