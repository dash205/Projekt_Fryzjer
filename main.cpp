#include "mainwindow.h"
#include "databaseconnection.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    if (!DatabaseConnection::instance().openConnection()) {
        return -1;
    }

    // DatabaseConnection::instance().addClient("krzysiek3");

    MainWindow w;
    w.show();

    return QCoreApplication::exec();
}
