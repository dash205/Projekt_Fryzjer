#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    Clients *clientsPage = new Clients(this);
    int clientsIndex = ui->stackedWidget->addWidget(clientsPage);

    Services *servicesPage = new Services(this);
    int servicesIndex = ui->stackedWidget->addWidget(servicesPage);

    Appointments *appointmentsPage = new Appointments(this);
    int appointmentsIndex = ui->stackedWidget->addWidget(appointmentsPage);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionMain_triggered() {
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::on_actionClients_triggered() {
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::on_actionServices_triggered() {
    ui->stackedWidget->setCurrentIndex(2);
}

void MainWindow::on_actionAppointments_triggered() {
    ui->stackedWidget->setCurrentIndex(3);
}