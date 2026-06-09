#include "mainwindow.h"
#include "logindialog.h"
#include <QApplication>
#include <QIcon>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setStyleSheet(R"(
        QMainWindow, QWidget#centralwidget {
            background-color: #121212;
        }

        QFrame.card {
            background-color: #1E1E1E;
            border-radius: 15px;
            border: 1px solid #333333;
        }

        QLabel#welcomeHeader {
            font-size: 55px;
            font-weight: bold;
            color: #FFFFFF;
        }

        QLabel#subHeaderText {
            font-size: 14px;
            color: #AAAAAA;
        }

        QCalendarWidget {
            background-color: #1E1E1E;
            color: #FFFFFF;
            border: 1px solid #333333;
            border-radius: 8px;
        }
        QCalendarWidget QToolButton {
            color: white;
            background: transparent;
            font-weight: bold;
        }
        QCalendarWidget QWidget#qt_calendar_navigationbar {
            background-color: #252525;
            border-radius: 8px 8px 0 0;
        }
        QCalendarWidget QAbstractItemView:enabled {
            selection-background-color: #3A86FF;
            selection-color: white;
        }
        QTableView {
            background-color: #1E1E1E;
            color: #FFFFFF;
            border: 1px solid #333333;
            gridline-color: #2A2A2A;
            font-size: 14px;
            padding: 2px;
            }

        QHeaderView::section {
            background-color: #252525;
            color: #AAAAAA;
            padding: 8px 12px;
            border: none;
            border-bottom: 1px solid #333333;
            border-right: 1px solid #333333;
            font-weight: bold;
        }


        QTableView::item:selected{
            background-color: #2A3B5C;
            color: #FFFFFF;
        }


        QHeaderView::section:selected{
            background-color: #1E2D45;
            color: #8AB4F8;
        }


        QScrollBar:vertical {
            background: #1E1E1E;
            width: 12px;
            margin: 0;
        }
        QScrollBar::handle:vertical {
            background: #444444;
            min-height: 40px;
            border-radius: 6px;
        }
        QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {
            height: 0;
        }
    )");
    a.setWindowIcon(QIcon(":/resources/nozyczki_icon.png"));
    if (!DatabaseConnection::instance().openConnection()) {
        return -1;
    }

    LoginDialog login;
    if (login.exec() != QDialog::Accepted){ return 0;}
    MainWindow w;
    w.show();

    return QCoreApplication::exec();
}
