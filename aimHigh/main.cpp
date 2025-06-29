#include "mainwindow.h"

#include <QApplication>
#include <QLocale>
#include <QFile>
#include <QDir>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QFont appFont("Courier New", 11);
    appFont.setStyleHint(QFont::Monospace);
    a.setFont(appFont);

    MainWindow w;
    w.show();
    return a.exec();
}
