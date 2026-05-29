#include "clients.h"
#include "ui_clients.h" // Ta linijka naprawia błąd "incomplete type"
#include <QMessageBox>
#include <QSqlError>
#include <QSqlTableModel>

Clients::Clients(QWidget *parent) : QWidget(parent), ui(new Ui::Clients) {
    ui->setupUi(this);

    // Konfiguracja modelu bazy danych
    clientModel = new QSqlTableModel(this);
    clientModel->setTable("clients");
    clientModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    clientModel->select();

    clientModel->setHeaderData(1, Qt::Horizontal, "Imię");
    clientModel->setHeaderData(2, Qt::Horizontal, "Nazwisko");
    clientModel->setHeaderData(3, Qt::Horizontal, "Telefon");
    clientModel->setHeaderData(4, Qt::Horizontal, "E-mail");

    // Konfiguracja widoku tabeli (element ui->tableView musi istnieć w Qt Designerze)
    ui->tableView->setModel(clientModel);
    ui->tableView->hideColumn(0);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

Clients::~Clients() {
    delete ui;
}

void Clients::on_btnAdd_clicked() {
    int row = clientModel->rowCount();
    clientModel->insertRow(row);
    QModelIndex index = clientModel->index(row, 1);
    ui->tableView->setCurrentIndex(index);
    ui->tableView->edit(index);
}

void Clients::on_btnDelete_clicked() {
    int selectedRow = ui->tableView->currentIndex().row();
    if (selectedRow >= 0) {
        clientModel->removeRow(selectedRow);
    } else {
        QMessageBox::warning(this, "Błąd", "Wybierz klienta do usunięcia!");
    }
}

void Clients::on_btnSave_clicked() {
    if (clientModel->submitAll()) {
        QMessageBox::information(this, "Sukces", "Zmiany zostały zapisane w bazie.");
    } else {
        QMessageBox::critical(this, "Błąd zapisu", clientModel->lastError().text());
    }
}