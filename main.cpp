#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setOrganizationName("Stephen Burchill");
    app.setApplicationName("Assignment 1: Image Processing");
    MainWindow mainWindow;
    mainWindow.show();

    return app.exec();
}
