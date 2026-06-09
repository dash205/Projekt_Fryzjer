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

//Konstruktor okna wizyt
Appointments::Appointments(QWidget *parent) : QWidget(parent), ui(new Ui::Appointments) {
    ui->setupUi(this);

    //Ustawienia widgetu tabeli
    ui->tableWidget->setColumnCount(5);
    ui->tableWidget->setHorizontalHeaderLabels({"Klient", "Usługa", "Data i Godzina", "Cena", "Notatki"});

    ui->tableWidget->verticalHeader()->setDefaultSectionSize(40);

    ui->tableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(4, QHeaderView::Stretch);

    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setEditTriggers(QAbstractItemView::DoubleClicked);

    //Łączenie widgetów z metodami
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

//Odświeżanie tabeli
void Appointments::refreshTable() {
    //Blokowanie sygnałów tabeli, żeby podczas wypisywania nie były wywoływane takich metody jak np. onCellChanged
    ui->tableWidget->blockSignals(true);
    modifiedAppointmentIds.clear();

    //Pobieranie wszystkich wizyt z bazy danych
    QList<Appointment> list = DatabaseConnection::instance().getAllAppointments();
    ui->tableWidget->setRowCount(0);

    //Filtrowanie wizyt (wyszukiwanie)
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

    //Sortowanie wizyt
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

    //Pobieranie wszystkich dostępnych usług, aby móc ich użyć w comboBox
    QList<Service> allServices = DatabaseConnection::instance().getAllServices();

    //Wypisywanie wszystkich przefiltrowanych i posortowanych wizyt
    for (const auto &app : list) {
        int row = ui->tableWidget->rowCount();
        ui->tableWidget->insertRow(row);

        //Pola klienta, daty oraz ceny
        QTableWidgetItem *itemClient = new QTableWidgetItem(app.client_name);
        QTableWidgetItem *itemDate = new QTableWidgetItem(app.appointment_date.toString("yyyy-MM-dd HH:mm"));
        QTableWidgetItem *itemPrice = new QTableWidgetItem(QString::number(app.price, 'f', 2) + " zł");

        //Przypisywanie id oraz id klienta do schowka Qt::UserRole
        itemClient->setData(Qt::UserRole, app.id);
        itemClient->setData(Qt::UserRole+1, app.client_id);

        //Uniemożliwienie możliwości edycji pola klientów oraz ceny
        itemClient->setFlags(itemClient->flags() & ~Qt::ItemIsEditable);
        itemPrice->setFlags(itemPrice->flags() & ~Qt::ItemIsEditable);

        //Tworzenie comboBox'a posiadającego wszystkie aktualne wizyty
        QComboBox *rowComboService = new QComboBox(ui->tableWidget);
        for (const auto &s : allServices) {
            rowComboService->addItem(s.name, s.id);
        }
        int currentIndex = rowComboService->findData(app.service_id);
        rowComboService->setCurrentIndex(currentIndex);

        //Tworzenie scrollowalnego pola notatek
        QTextEdit *notesEdit = new QTextEdit(ui->tableWidget);
        notesEdit->setPlainText(app.notes);
        notesEdit->setStyleSheet("border: none; background: transparent;");

        //Wstawianie wszystkich pól do wiersza
        ui->tableWidget->setItem(row, 0, itemClient);
        ui->tableWidget->setCellWidget(row, 1, rowComboService);
        ui->tableWidget->setItem(row, 2, itemDate);
        ui->tableWidget->setItem(row, 3, itemPrice);
        ui->tableWidget->setCellWidget(row, 4, notesEdit);
        int appointmentId = app.id;

        //Przy zmianie usługi oraz notatek, id tej wizyty zostaje przesłane modifiedAppointments w celu zastosowania zmian
        connect(rowComboService, &QComboBox::activated, this, [this,appointmentId]() {
           modifiedAppointmentIds.insert(appointmentId);
        });

        connect(notesEdit, &QTextEdit::textChanged, this, [this,appointmentId]() {
            modifiedAppointmentIds.insert(appointmentId);
        });

    }
    //Po wypisaniu tabeli sygnał jest odblokowany
    ui->tableWidget->blockSignals(false);
}

//Obsługa usuwania wizyt
void Appointments::onDeleteAppointmentClicked() {
    //Zaznaczenie wiersza (wizyty)
    int currentRow = ui->tableWidget->currentRow();

    if (currentRow < 0) {
        QMessageBox::warning(this, "Brak wyboru", "Zaznacz w tabeli wizytę, którą chcesz usunąć.");
        return;
    }

    //Sprawdzenie poprawności wiersza
    QTableWidgetItem *item = ui->tableWidget->item(currentRow, 0);
    if (!item) return;


    //Pobieranie id wizyty ze schowka Qt::UserRole
    int appointmentId = item->data(Qt::UserRole).toInt();
    //Tworzenie obiektu wizyty
    Appointment selectedApp = DatabaseConnection::instance().getAppointmentById(appointmentId);
    auto reply = QMessageBox::question(this, "Potwierdzenie usunięcia",
                                       "Czy na pewno chcesz bezpowrotnie usunąć tę wizytę?",
                                       QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::No) {
        return;
    }

    //Rozpoczęcie transakcji w celu zarchiwizowania oraz usunięcia wizyty
    DatabaseConnection::instance().beginTransaction();

    //Próba archiwizacji
    bool archiveOk = DatabaseConnection::instance().addArchivalAppointment(selectedApp);
    //Próba usunięcia wizyty
    bool deleteOk = DatabaseConnection::instance().deleteAppointment(appointmentId);

    if (archiveOk && deleteOk) {
        //Jeśli dwie operacje się powiodły zmiany zostają zastosowane
        DatabaseConnection::instance().commitTransaction();
        QMessageBox::information(this, "Sukces", "Wizyta usunięta i zarchiwizowana.");
        refreshTable();
        appointmentChanged();
    } else {
        //W przypadku błędu cofamy wszystkie zmiany
        DatabaseConnection::instance().rollbackTransaction();
        QMessageBox::critical(this, "Błąd", "Nie udało się usunąć wizyty z bazy danych.");
    }
}

//Jeśli jakiś wiersz się zmienił to zostaje dodany do modyfikowanych ID
void Appointments::onCellChanged(int row, int column) {
    if (column != 2 && column != 4) return;

    QTableWidgetItem *idItem = ui->tableWidget->item(row, 0);
    if (!idItem) return;

    int appointmentId = idItem->data(Qt::UserRole).toInt();

    modifiedAppointmentIds.insert(appointmentId);

}

//Otwarcie okna dialogowego do dodawania wizyt
void Appointments::onAddAppointmentDialogClicked() {
    addappointmentdialog dialog(this);

    if (dialog.exec() == QDialog::Accepted) {
        refreshTable();
        appointmentChanged();
    }
}

//Zapisywanie zmian w przypadku edycji pól tabeli
void Appointments::onSaveAllClicked() {
    //Sprawdzanie, czy jakaś wizyta została zmieniona
    if (modifiedAppointmentIds.empty()) {
        QMessageBox::information(this, "Zapisz zmiany", "Nie wykryto żadnych zmian do zapisania.");
    }

    //Walidacja danych
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

    //Tworzenie obiektu, aktualizacja tabeli i bazy danych
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

//Anulowanie niechcianych zmian
void Appointments::onCancelChangesClicked() {
    if (modifiedAppointmentIds.empty()) return;

    auto reply = QMessageBox::question(this, "Anulowanie zmian",
        "Czy na pewno chcesz odrzucić wszystkie zmiany? ", QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        refreshTable();
        appointmentChanged();
    }

}
