#include "mainwindow.h"
#include <QApplication>
#include <QTranslator>
#include <QLibraryInfo>
#include <QWindow>
#include <QScreen>

int main(int argc, char *argv[]) {
    QTranslator qtTranslator;
    qtTranslator.load("qt_" + QLocale::system().name(), QLibraryInfo::location(QLibraryInfo::TranslationsPath));

    QApplication a(argc, argv);
    a.installTranslator(&qtTranslator);
    MainWindow w;
    w.show();

    w.move(w.window()->windowHandle()->screen()->geometry().center() - w.rect().center());

    return a.exec();
}
