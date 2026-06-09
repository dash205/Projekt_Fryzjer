#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "databaseconnection.h"
#include "addappointmentdialog.h"
#include  <QPixmap>
#include "appointments.h"

void MainWindow::createModel() //Tworzenie modelu i wyświetlanie tabeli w oknie menu głównego
{
    int userId = DatabaseConnection::instance().currentUserId;

    QSqlQueryModel *model = new QSqlQueryModel(this);

    model->setQuery(QString(
        "SELECT "
                "c.first_name || ' ' || c.last_name AS client_name, "
                "s.name, "
                "s.price, "
                "strftime('%d.%m.%Y %H:%M', a.appointment_date) AS appointment_date, "
                "a.notes, "
                "c.phone "
                "FROM appointments a "
                "INNER JOIN clients c ON a.client_id = c.id "
                "INNER JOIN services s ON a.service_id = s.id "
                "WHERE a.user_id = %1 "
                "ORDER BY a.appointment_date ASC"
    ).arg(userId));

    if (model->lastError().isValid()) {
        qDebug() << "BŁĄD SQL:" << model->lastError().text();
    }

    model->setHeaderData(0, Qt::Horizontal, "Imię i Nazwisko");
    model->setHeaderData(1, Qt::Horizontal, "Usługa");
    model->setHeaderData(2, Qt::Horizontal, "Cena");
    model->setHeaderData(3, Qt::Horizontal, "Data");
    model->setHeaderData(4, Qt::Horizontal, "Uwagi");
    model->setHeaderData(5, Qt::Horizontal, "Telefon");


    ui->tableView->setModel(model);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->setSelectionMode(QAbstractItemView::NoSelection);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView->horizontalHeader()->setMinimumWidth(100);
    ui->tableView->setContentsMargins(20, 15, 20, 20);
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //Logika dla nagłówka powitalnego
    ui->welcomeHeader->setText("Witaj "+DatabaseConnection::instance().getUsername(DatabaseConnection::instance().currentUserId)+"!");
    createModel();

    Clients *clientsPage = new Clients(this);
    int clientsIndex = ui->stackedWidget->addWidget(clientsPage);

    Services *servicesPage = new Services(this);
    int servicesIndex = ui->stackedWidget->addWidget(servicesPage);

    appointmentsPage = new Appointments(this);
    int apponmentsIndex = ui->stackedWidget->addWidget(appointmentsPage);


    ui->actionUsers->setVisible(false);

    //sprawdzanie dostępu do okna zarządzania użytkownikami (gdy id admninistratora - wyświetl możliwość przejścia do okna)
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
    createModel();
}

void MainWindow::on_actionClients_triggered() {
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::on_actionServices_triggered() {
    ui->stackedWidget->setCurrentIndex(2);
}

void MainWindow::on_actionAppointments_triggered() {
    ui->stackedWidget->setCurrentIndex(3);
    appointmentsPage->refreshTable();
}
void MainWindow::on_actionUsers_triggered()
{
    if (DatabaseConnection::instance().autorisationCheck(DatabaseConnection::instance().currentUserId)) ui->stackedWidget->setCurrentIndex(4);
}