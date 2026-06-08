#include "mainwindow.h"
#include "logindialog.h"
#include <QApplication>
#include <QIcon>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setStyleSheet(R"(
        /* Tło główne okna */
        QMainWindow, QWidget#centralwidget {
            background-color: #121212;
        }

        /* Karty / Panele (np. tło pod ikoną nożyc) */
        QFrame.card {
            background-color: #1E1E1E;
            border-radius: 15px;
            border: 1px solid #333333;
        }

        /* Nagłówek "Witaj admin!" */
        QLabel#welcomeHeader {
            font-size: 36px;
            font-weight: bold;
            color: #FFFFFF;
        }

        /* Podtytuł */
        QLabel#subHeaderText {
            font-size: 14px;
            color: #AAAAAA;
        }

        /* Stylizacja Kalendarza */
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
            selection-background-color: #3A86FF; /* Kolor akcentu */
            selection-color: white;
        }
/* Główny styl tabeli */
QTableView {
    background-color: #1E1E1E;
    color: #FFFFFF;
    border: 1px solid #333333;
    gridline-color: #2A2A2A;       /* Bardzo subtelne linie siatki */
    font-size: 14px;
    padding: 2px;
}

/* Nagłówki kolumn */
QHeaderView::section {
    background-color: #252525;
    color: #AAAAAA;
    padding: 8px 12px;
    border: none;
    border-bottom: 1px solid #333333;
    border-right: 1px solid #333333;
    font-weight: bold;
}

/* Zaznaczenie wiersza */
QTableView::item:selected {
    background-color: #3A86FF;     /* Kolor akcentu */
    color: #FFFFFF;
}

/* Alternatywne tło wierszy (wymaga setAlternatingRowColors(true)) */
QTableView::item:alternate {
    background-color: #222222;
}

/* Pasek przewijania */
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
    height: 0; /* Ukryj strzałki na scrollbarze dla nowoczesnego wyglądu */
}
    )");
    a.setWindowIcon(QIcon(":/resources/nozyczki_icon.png"));
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
