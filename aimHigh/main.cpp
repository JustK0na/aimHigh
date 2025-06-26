#include "mainwindow.h"

#include <QApplication>
#include <QLocale>
#include <QFile>
#include <QDir>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QFont appFont("Courier New", 11); // Or "Monospace", "DejaVu Sans Mono", etc.
    appFont.setStyleHint(QFont::Monospace); // Ensures monospaced appearance
    a.setFont(appFont);

    MainWindow w;
    w.show();
    return a.exec();
}
