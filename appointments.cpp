#include "appointments.h"
#include "ui_appointments.h"
#include "databaseconnection.h"
#include "appointmentClass.h"

#include <QHeaderView>
#include <QTableWidgetItem>
#include <QMessageBox>
#include <QComboBox>
#include <algorithm>
#include <QTextEdit>

#include "addappointmentdialog.h"

Appointments::Appointments(QWidget *parent) : QWidget(parent), ui(new Ui::Appointments) {
    ui->setupUi(this);

    ui->tableWidget->setColumnCount(5);
    ui->tableWidget->setHorizontalHeaderLabels({"Klient", "Usługa", "Data i Godzina", "Cena", "Notatki"});

    ui->tableWidget->verticalHeader()->setDefaultSectionSize(40);

    ui->tableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Interactive);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Interactive);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(4, QHeaderView::Stretch);

    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setEditTriggers(QAbstractItemView::DoubleClicked);


    connect(ui->btnDialogAdd, &QPushButton::clicked, this, &Appointments::onAddAppointmentDialogClicked);
    connect(ui->btnDelete, &QPushButton::clicked, this, &Appointments::onDeleteAppointmentClicked);
    connect(ui->tableWidget, &QTableWidget::cellChanged, this, &Appointments::onCellChanged);

    connect(ui->inputSearch, &QLineEdit::textChanged, this, &Appointments::refreshTable);
    connect(ui->comboSort, &QComboBox::currentIndexChanged, this, &Appointments::refreshTable);

    connect(ui->btnSaveAll, &QPushButton::clicked, this, &Appointments::onSaveAllClicked);
    connect(ui->btnCancelChanges, &QPushButton::clicked, this, &Appointments::onCancelChangesClicked);

    refreshTable();
}

Appointments::~Appointments() {
    delete ui;
}

void Appointments::refreshTable() {
    ui->tableWidget->blockSignals(true);

    modifiedAppointmentIds.clear();

    QList<Appointment> list = DatabaseConnection::instance().getAllAppointments();
    ui->tableWidget->setRowCount(0);

    QString filterText= ui->inputSearch->text().trimmed();
    if (!filterText.isEmpty()) {
        QList<Appointment> filteredList;
        for (const auto &app : list) {
            if (app.client_name.contains(filterText, Qt::CaseInsensitive) ||
                app.notes.contains(filterText, Qt::CaseInsensitive)) {
                filteredList.append(app);
            }
        }
        list = filteredList;
    }

    int sortIndex = ui->comboSort->currentIndex();

    std::sort(list.begin(), list.end(), [sortIndex](const Appointment &a, const Appointment &b) {
        if (sortIndex == 0) {
            return a.appointment_date > b.appointment_date;
        } else if (sortIndex == 1) {
            return a.appointment_date < b.appointment_date;
        } else if (sortIndex == 2) {
            return a.client_name.localeAwareCompare(b.client_name) < 0;
        }
        return false;
    });

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
        for (const auto &s : allServices) {
            rowComboService->addItem(s.name, s.id);
        }
        int currentIndex = rowComboService->findData(app.service_id);
        rowComboService->setCurrentIndex(currentIndex);

        QTextEdit *notesEdit = new QTextEdit(ui->tableWidget);
        notesEdit->setPlainText(app.notes);
        notesEdit->setStyleSheet("border: none; background: transparent;");

        ui->tableWidget->setItem(row, 0, itemClient);
        ui->tableWidget->setCellWidget(row, 1, rowComboService);
        ui->tableWidget->setItem(row, 2, itemDate);
        ui->tableWidget->setItem(row, 3, itemPrice);
        // ui->tableWidget->setItem(row, 4, itemNotes);
        ui->tableWidget->setCellWidget(row, 4, notesEdit);
        int appointmentId = app.id;

        connect(rowComboService, &QComboBox::activated, this, [this,appointmentId]() {
           modifiedAppointmentIds.insert(appointmentId);
        });

        connect(notesEdit, &QTextEdit::textChanged, this, [this,appointmentId]() {
            modifiedAppointmentIds.insert(appointmentId);
        });

    }
    ui->tableWidget->blockSignals(false);
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
    Appointment selectedApp = DatabaseConnection::instance().getAppointmentById(appointmentId);
    auto reply = QMessageBox::question(this, "Potwierdzenie usunięcia",
                                       "Czy na pewno chcesz bezpowrotnie usunąć tę wizytę?",
                                       QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::No) {
        return;
    }

    DatabaseConnection::instance().beginTransaction();

    bool archiveOk = DatabaseConnection::instance().addArchivalAppointment(selectedApp);
    bool deleteOk = DatabaseConnection::instance().deleteAppointment(appointmentId);

    if (archiveOk && deleteOk) {
        DatabaseConnection::instance().commitTransaction();
        QMessageBox::information(this, "Sukces", "Wizyta usunięta i zarchiwizowana.");
        refreshTable();
        appointmentChanged();
    } else {
        DatabaseConnection::instance().rollbackTransaction();
        QMessageBox::critical(this, "Błąd", "Nie udało się usunąć wizyty z bazy danych.");
    }
}

void Appointments::onCellChanged(int row, int column) {
    if (column != 2 && column != 4) return;

    QTableWidgetItem *idItem = ui->tableWidget->item(row, 0);
    if (!idItem) return;

    int appointmentId = idItem->data(Qt::UserRole).toInt();

    modifiedAppointmentIds.insert(appointmentId);

}

void Appointments::onAddAppointmentDialogClicked() {
    addappointmentdialog dialog(this);

    if (dialog.exec() == QDialog::Accepted) {
        refreshTable();
        appointmentChanged();
    }
}

void Appointments::onSaveAllClicked() {
    if (modifiedAppointmentIds.empty()) {
        QMessageBox::information(this, "Zapisz zmiany", "Nie wykryto żadnych zmian do zapisania.");
    }

    for (int row = 0; row < ui->tableWidget->rowCount(); ++row) {
        QTableWidgetItem *idItem = ui->tableWidget->item(row, 0);
        if (!idItem) continue;
        int appointmentId = idItem->data(Qt::UserRole).toInt();

        if (modifiedAppointmentIds.contains(appointmentId)) {
            QString dateStr = ui->tableWidget->item(row, 2)->text();
            QDateTime checkDate = QDateTime::fromString(dateStr, "yyyy-MM-dd HH:mm");
            if (!checkDate.isValid()) {
                QMessageBox::warning(this, "Błąd walidacji danych!",QString(
                    "Wiersz %1 posiada niepoprawny format daty.").arg(row+1));
                return;
            }
        }
        appointmentChanged();
    }

    bool isOk = true;
    for (int row = 0; row < ui->tableWidget->rowCount(); ++row) {
        QTableWidgetItem *idItem = ui->tableWidget->item(row, 0);
        if (!idItem) continue;

        int appointmentId = idItem->data(Qt::UserRole).toInt();

        if (modifiedAppointmentIds.contains(appointmentId)) {
            QComboBox *combo = qobject_cast<QComboBox*>(ui->tableWidget->cellWidget(row, 1));
            int currentServiceId = combo ? combo->currentData().toInt() : 0;

            QString dateStr = ui->tableWidget->item(row, 2)->text();
            // QString notesStr = ui->tableWidget->item(row, 4)->text();
            QTextEdit *notesEdit = qobject_cast<QTextEdit*>(ui->tableWidget->cellWidget(row, 4));
            QString notesStr = notesEdit->toPlainText();

            Appointment updatedApp;
            updatedApp.id = appointmentId;
            updatedApp.client_id = idItem->data(Qt::UserRole + 1).toInt();
            updatedApp.service_id = currentServiceId;
            updatedApp.appointment_date = QDateTime::fromString(dateStr, "yyyy-MM-dd HH:mm");
            updatedApp.notes = notesStr;

            if (!DatabaseConnection::instance().updateAppointment(updatedApp)) {
                isOk = false;
            }
        }
    }

    if (isOk) {
        if (!modifiedAppointmentIds.empty()) {
            QMessageBox::information(this, "Sukces", "Wszystkie zmiany zostały zapisane");
        }
    } else {
        QMessageBox::critical(this, "Błąd", "Wystąpił błąd podczas aktualizacji danych.");
    }

    refreshTable();
    appointmentChanged();

}

void Appointments::onCancelChangesClicked() {
    if (modifiedAppointmentIds.empty()) return;

    auto reply = QMessageBox::question(this, "Anulowanie zmian",
        "Czy na pewno chcesz odrzucić wszystkie zmiany? ", QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        refreshTable();
        appointmentChanged();
    }

}
