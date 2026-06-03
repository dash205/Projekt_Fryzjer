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
    ui->tableWidget->setEditTriggers(QAbstractItemView::DoubleClicked);

    connect(ui->btnAdd, &QPushButton::clicked, this, &Appointments::onAddAppointmentClicked);
    connect(ui->btnDelete, &QPushButton::clicked, this, &Appointments::onDeleteAppointmentClicked);
    connect(ui->tableWidget, &QTableWidget::cellChanged, this, &Appointments::onCellChanged);
    connect(ui->btnDialogAdd, &QPushButton::clicked, this, &Appointments::onAddAppointmentDialogClicked);

    refreshTable();
    setupForm();
}

Appointments::~Appointments() {
    delete ui;
}

void Appointments::refreshTable() {
    ui->tableWidget->blockSignals(true);

    QList<Appointment> list = DatabaseConnection::instance().getAllAppointments();
    ui->tableWidget->setRowCount(0);

    QList<Service> allServices = DatabaseConnection::instance().getAllServices();

    for (const auto &app : list) {
        int row = ui->tableWidget->rowCount();
        ui->tableWidget->insertRow(row);

        QTableWidgetItem *itemClient = new QTableWidgetItem(app.client_name);
        // QTableWidgetItem *itemService = new QTableWidgetItem(app.service_name);
        QTableWidgetItem *itemDate = new QTableWidgetItem(app.appointment_date.toString("yyyy-MM-dd HH:mm"));
        QTableWidgetItem *itemPrice = new QTableWidgetItem(QString::number(app.price, 'f', 2) + " zł");
        QTableWidgetItem *itemNotes = new QTableWidgetItem(app.notes);

        itemClient->setData(Qt::UserRole, app.id);
        itemClient->setData(Qt::UserRole+1, app.client_id);
        // itemClient->setData(Qt::UserRole+2, app.service_id);

        itemClient->setFlags(itemClient->flags() & ~Qt::ItemIsEditable);
        // itemService->setFlags(itemService->flags() & ~Qt::ItemIsEditable);
        itemPrice->setFlags(itemPrice->flags() & ~Qt::ItemIsEditable);

        QComboBox *rowComboService = new QComboBox(ui->tableWidget);

        for (const auto &s: allServices) {
            rowComboService->addItem(s.name, s.id);
        }

        int currentIndex = rowComboService->findData(app.service_id);
        rowComboService->setCurrentIndex(currentIndex);

        ui->tableWidget->setItem(row, 0, itemClient);
        ui->tableWidget->setCellWidget(row, 1, rowComboService);
        ui->tableWidget->setItem(row, 2, itemDate);
        ui->tableWidget->setItem(row, 3, itemPrice);
        ui->tableWidget->setItem(row, 4, itemNotes);

        int appointmentId = app.id;
        int clientId = app.client_id;

        connect(rowComboService, &QComboBox::activated, this, [this, appointmentId, clientId, row]() {
            //Wskaznik na ComboBox z edytowanego wiersza
            QComboBox *combo = qobject_cast<QComboBox*>(ui->tableWidget->cellWidget(row, 1));
            if (!combo) return;

            int newServiceId = combo->currentData().toInt();
            QString dateStr = ui->tableWidget->item(row, 2)->text();
            QString notesStr = ui->tableWidget->item(row, 4)->text();

            Appointment updatedApp;
            updatedApp.id = appointmentId;
            updatedApp.client_id = clientId;
            updatedApp.service_id = newServiceId;
            updatedApp.appointment_date = QDateTime::fromString(dateStr, "yyyy-MM-dd HH:mm");
            updatedApp.notes = notesStr;

            if (DatabaseConnection::instance().updateAppointment(updatedApp)) {
                refreshTable();
            } else {
                QMessageBox::critical(this, "Błąd", "Nie udało się zaktualizować usługi.");
                refreshTable();
            }
        });

    }
    ui->tableWidget->blockSignals(false);
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

void Appointments::onDeleteAppointmentClicked() {
    int currentRow = ui->tableWidget->currentRow();

    if (currentRow < 0) {
        QMessageBox::warning(this, "Brak wyboru", "Zaznacz w tabeli wizytę, którą chcesz usunąć.");
        return;
    }

    QTableWidgetItem *item = ui->tableWidget->item(currentRow, 0);
    if (!item) return;

    int appointmentId = item->data(Qt::UserRole).toInt();
    auto reply = QMessageBox::question(this, "Potwierdzenie usunięcia",
                                       "Czy na pewno chcesz bezpowrotnie usunąć tę wizytę?",
                                       QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::No) {
        return;
    }

    if (DatabaseConnection::instance().deleteAppointment(appointmentId)) {
        refreshTable();
    } else {
        QMessageBox::critical(this, "Błąd", "Nie udało się usunąć wizyty z bazy danych.");
    }
}

void Appointments::onCellChanged(int row, int column) {
    QTableWidgetItem *idItem = ui->tableWidget->item(row, 0);
    if (!idItem) return;

    QComboBox *combo = qobject_cast<QComboBox*>(ui->tableWidget->cellWidget(row, 1));
    if (!combo) return;
    int currentServiceId = combo->currentData().toInt();

    QTableWidgetItem *dateItem = ui->tableWidget->item(row, 2);
    QTableWidgetItem *notesItem = ui->tableWidget->item(row, 4);
    if (!dateItem || !notesItem) return;

    QString dateStr = dateItem->text();
    QString notesStr = notesItem->text();

    if (column == 2) {
        QDateTime checkDate = QDateTime::fromString(dateStr, "yyyy-MM-dd HH:mm");
        if (!checkDate.isValid()) {
            QMessageBox::warning(this, "Zły format daty", "Wprowadź datę w formacie: YYYY-MM-DD HH:MM");
            refreshTable();
            return;
        }
    }

    Appointment updatedApp;
    updatedApp.id = idItem->data(Qt::UserRole).toInt();
    updatedApp.client_id = idItem->data(Qt::UserRole+1).toInt();
    updatedApp.service_id = currentServiceId;
    updatedApp.appointment_date = QDateTime::fromString(dateStr, "yyyy-MM-dd HH:mm");
    updatedApp.notes = notesStr;

    if (DatabaseConnection::instance().updateAppointment(updatedApp)) {
        refreshTable();
    } else {
        QMessageBox::critical(this, "Błąd zapisu", "Nie udało się zaktualizować danych.");
        refreshTable();
    }
}

void Appointments::onAddAppointmentDialogClicked() {

}
