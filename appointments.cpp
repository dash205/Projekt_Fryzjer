#include "appointments.h"
#include "ui_appointments.h"
#include "databaseconnection.h"
#include "appointmentClass.h"

#include <QHeaderView>
#include <QTableWidgetItem>
#include <QMessageBox>

Appointments::Appointments(QWidget *parent) : QWidget(parent), ui(new Ui::Appointments) {
    ui->setupUi(this);

    ui->tableWidget->setColumnCount(5);
    ui->tableWidget->setHorizontalHeaderLabels({"Klient", "Usługa", "Data i Godzina", "Cena", "Notatki"});

    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    connect(ui->btnAdd, &QPushButton::clicked, this, &Appointments::onAddAppointmentClicked);

    refreshTable();
    setupForm();
}

Appointments::~Appointments() {
    delete ui;
}

void Appointments::refreshTable() {
    QList<Appointment> list = DatabaseConnection::instance().getAllAppointments();

    ui->tableWidget->setRowCount(0);

    for (const auto &app : list) {
        int row = ui->tableWidget->rowCount();
        ui->tableWidget->insertRow(row);

        QTableWidgetItem *itemClient = new QTableWidgetItem(app.client_name);
        QTableWidgetItem *itemService = new QTableWidgetItem(app.service_name);
        QTableWidgetItem *itemDate = new QTableWidgetItem(app.appointment_date.toString("yyyy-MM-dd HH:mm"));
        QTableWidgetItem *itemPrice = new QTableWidgetItem(QString::number(app.price, 'f', 2) + " zł");
        QTableWidgetItem *itemNotes = new QTableWidgetItem(app.notes);

        itemClient->setData(Qt::UserRole, app.id);

        ui->tableWidget->setItem(row, 0, itemClient);
        ui->tableWidget->setItem(row, 1, itemService);
        ui->tableWidget->setItem(row, 2, itemDate);
        ui->tableWidget->setItem(row, 3, itemPrice);
        ui->tableWidget->setItem(row, 4, itemNotes);

    }
}

void Appointments::setupForm() {
    ui->comboClient->clear();
    ui->comboService->clear();

    QList<Client> clients = DatabaseConnection::instance().getAllClients();
    for (const auto& client : clients) {
        QString fullName = client.first_name + " " + client.last_name;
        ui->comboClient->addItem(fullName, client.id);
    }

    QList<Service> services = DatabaseConnection::instance().getAllServices();
    for (const auto& service : services) {
        ui->comboService->addItem(service.name, service.id);
    }

    ui->dateTimeEdit->setDateTime(QDateTime::currentDateTime());

}

void Appointments::onAddAppointmentClicked() {
    if (ui->comboClient->currentIndex() == -1 || ui->comboService->currentIndex() == -1) {
        QMessageBox::warning(this, "Brak danych", "Upewnij się, że w bazie istnieją klienci oraz usługi.");
        return;
    }
    Appointment newApp;

    newApp.client_id = ui->comboClient->currentData().toInt();
    newApp.service_id = ui->comboClient->currentData().toInt();
    newApp.appointment_date = ui->dateTimeEdit->dateTime();
    newApp.notes = ui->lineEdit->text();

    if (DatabaseConnection::instance().addAppointment(newApp)) {
        refreshTable();

        ui->lineEdit->clear();
        ui->dateTimeEdit->setDateTime(QDateTime::currentDateTime());
    } else {
        QMessageBox::critical(this, "Błąd", "Nie udało się zapisać danych do bazy.");
    }
}
