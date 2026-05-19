#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "clients.h"
#include "services.h"
#include "appointments.h"

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

private slots:
    void on_actionClients_triggered();
    void on_actionMain_triggered();
    void on_actionServices_triggered();
    void on_actionAppointments_triggered();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
