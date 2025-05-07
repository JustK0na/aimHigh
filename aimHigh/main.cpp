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
   // bool loaded = translator.load(":/i18n/i18n/aimHigh_pl.qm");
   // if(loaded)
    //    a.installTranslator(&translator);

    MainWindow w;
    w.show();
    return a.exec();
}
