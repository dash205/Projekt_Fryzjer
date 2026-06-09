#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "clients.h"
#include "services.h"
#include "users.h"
#include "appointments.h"
#include <QTableView>
#include <QSqlQuery>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;
    void createModel();
private slots:
    void on_actionClients_triggered();
    void on_actionMain_triggered();
    void on_actionServices_triggered();
    void on_actionAppointments_triggered();
    void on_actionUsers_triggered();




private:
    Ui::MainWindow *ui;

    QSqlQueryModel* usersAppointmentsModel;
    Appointments *appointmentsPage;
};
#endif // MAINWINDOW_H
