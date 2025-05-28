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

private:
    Ui::MainWindow *ui;         ///< Qt Main Widget

    APIhandler *apiHandler;     ///< Handles the network calls
    earthWidget *earthView;     ///<Manages earth and ISS rendering

    QWidget *stats;             ///< Container for the ISS stats labels
    QLabel *issInfo;            ///< Label showing formatted ISS data

    QWidget *earthModel;        ///< Widget for Earth model

    QWidget *container;
};
#endif // MAINWINDOW_H
