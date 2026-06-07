#include "mainwindow.h"
#include "logindialog.h"
#include <QApplication>
#include <QIcon>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(":/resources/norzyczki_icon.png"));
    if (!DatabaseConnection::instance().openConnection()) {
        return -1;
    }

    // DatabaseConnection::instance().addClient("krzysiek3");
    LoginDialog login;
    if (login.exec() != QDialog::Accepted){ return 0;}
    MainWindow w;
    w.show();

    return QCoreApplication::exec();
}
