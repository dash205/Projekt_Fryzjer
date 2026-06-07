//
// Created by arktizz on 4.06.2026.
//

// You may need to build the project (run Qt uic code generator) to get "ui_addappointmentdialog.h" resolved

#include "addappointmentdialog.h"
#include "ui_addappointmentdialog.h"
#include "databaseconnection.h"
#include "appointmentClass.h"

#include <QMessageBox>
#include <QCompleter>


addappointmentdialog::addappointmentdialog(QWidget *parent) : QDialog(parent), ui(new Ui::addappointmentdialog) {
    ui->setupUi(this);
    setupForm();

    connect(ui->btnSave, &QPushButton::clicked, this, &addappointmentdialog::onSaveClicked);
    connect(ui->btnCancel, &QPushButton::clicked, this, &QDialog::reject);
}

addappointmentdialog::~addappointmentdialog() {
    delete ui;
}

void addappointmentdialog::setupForm() {
    ui->comboClient->clear();
    ui->comboService->clear();

    QList<Client> clients = DatabaseConnection::instance().getAllClients();
    for (const auto& client : clients) {
        QString fullName = client.first_name + " " + client.last_name;
        ui->comboClient->addItem(fullName, client.id);
    }

    ui->comboClient->setEditable(true);
    ui->comboClient->setInsertPolicy(QComboBox::NoInsert);
    ui->comboClient->completer()->setFilterMode(Qt::MatchContains);

    QList<Service> services = DatabaseConnection::instance().getAllServices();
    for (const auto& service : services) {
        ui->comboService->addItem(service.name, service.id);
    }

    ui->dateTimeEdit->setDateTime(QDateTime::currentDateTime());

}

void addappointmentdialog::onSaveClicked() {
    if (ui->comboClient->currentIndex() == -1 || ui->comboService->currentIndex() == -1) {
        QMessageBox::warning(this, "Brak danych", "Upewnij się, że w bazie istnieją klienci oraz usługi.");
        return;
    }
    Appointment newApp;

    newApp.client_id = ui->comboClient->currentData().toInt();
    newApp.service_id = ui->comboService->currentData().toInt();
    newApp.appointment_date = ui->dateTimeEdit->dateTime();
    newApp.notes = ui->inputNotes->text();

    if (DatabaseConnection::instance().addAppointment(newApp)) {
        this->accept();
    } else {
        QMessageBox::critical(this, "Błąd", "Nie udało się zapisać danych do bazy.");
    }
}
