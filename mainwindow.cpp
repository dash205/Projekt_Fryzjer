#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "databaseconnection.h"
#include  <QPixmap>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->welcomeHeader->setText("Witaj "+DatabaseConnection::instance().getUsername(DatabaseConnection::instance().currentUserId)+"!");

    Clients *clientsPage = new Clients(this);
    int clientsIndex = ui->stackedWidget->addWidget(clientsPage);

    Services *servicesPage = new Services(this);
    int servicesIndex = ui->stackedWidget->addWidget(servicesPage);

    Appointments *appointmentsPage = new Appointments(this);
    int apponmentsIndex = ui->stackedWidget->addWidget(appointmentsPage);
    ui->actionUsers->setVisible(false);

    if (DatabaseConnection::instance().autorisationCheck(DatabaseConnection::instance().currentUserId))
    {
        ui->subHeaderText->setText("Jesteś zalogowany jako administrator.");
        ui->actionUsers->setVisible(true);
        users *usersPage = new users(this);
        int usersIndex = ui->stackedWidget->addWidget(usersPage);
        connect(ui->actionUsers, &QAction::triggered, this, &MainWindow::on_actionUsers_triggered);
    }
    QPixmap pixmap(":/resources/nozyczki.png");
    if (pixmap.isNull())
    {
        qDebug()<<"pixmap error";
    }else
    {
        ui->icon->setPixmap(pixmap);
        ui->icon->setScaledContents(true);
    }
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
void MainWindow::on_actionUsers_triggered()
{
    if (DatabaseConnection::instance().autorisationCheck(DatabaseConnection::instance().currentUserId)) ui->stackedWidget->setCurrentIndex(4);
}