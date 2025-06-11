#include "mainwindow.h"

#include <QApplication>
#include <QLocale>
#include <QFile>
#include <QDir>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTranslator translator;
    MainWindow w;
    w.show();
    return a.exec();
}
