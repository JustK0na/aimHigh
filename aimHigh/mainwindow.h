#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QWidget>
#include <QVBoxLayout>

#include "apihandler.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void updateISSData(double longitude, double latitude, double altitude, double velocity);

private:
    Ui::MainWindow *ui;

    APIhandler *apiHandler;

    QWidget *stats;
    QLabel *issInfo;

    QWidget *earthModel;
};
#endif // MAINWINDOW_H
