//
// Created by Artur on 19.05.2026.
//

// You may need to build the project (run Qt uic code generator) to get "ui_services.h" resolved

#include "services.h"
#include <QMessageBox>
#include "addservicedialog.h"
#include "databaseconnection.h"
#include "ui_addservicedialog.h"
#include "ui_services.h"

void Services::createModel()
{
    model = new QSqlTableModel(this);
    model->setObjectName("Services");
    model->setTable("services");
    model->setHeaderData(1, Qt::Horizontal, "Nazwa");
    model->setHeaderData(2, Qt::Horizontal, "Cena (zł)");
    model->setHeaderData(3, Qt::Horizontal, "Czas usługi (min)");
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model->select();

    qDebug() << model->lastError().text();
    qDebug() << model->rowCount();
}

void Services::createTable()
{
    ui->ServicesTable->setModel(model);
    ui->ServicesTable->setColumnHidden(0, true);
    ui->ServicesTable->setFocusPolicy(Qt::NoFocus);
    ui->ServicesTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    ui->ServicesTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Fixed);

    int tableWidth = ui->ServicesTable->width();
    ui->ServicesTable->setColumnWidth(1, tableWidth / 2);

    ui->ServicesTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->ServicesTable->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->ServicesTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->ServicesTable->setAlternatingRowColors(true);
    ui->ServicesTable->verticalHeader()->setDefaultSectionSize(40);
    ui->ServicesTable->setShowGrid(false);



}

void Services::onAddClicked()
{
    AddServiceDialog dialog(this);

    if (dialog.exec() == QDialog::Accepted)
    {
        int row = model->rowCount();
        model->insertRow(row);
        model->setData(model->index(row, 1), dialog.getName());
        model->setData(model->index(row, 2), dialog.getPrice());
        model->setData(model->index(row, 3), dialog.getDuration());
        model->submitAll();
        model->select();
    }
}

void Services::onEditClicked()
{
    QModelIndex index = ui->ServicesTable->currentIndex();
    if (!index.isValid())
    {
        QMessageBox::warning(this, tr("Błąd"), tr("Usługa nie została wybrana."));
        return;
    }

    AddServiceDialog dialog(this);

    dialog.setName(model->data(model->index(index.row(), 1), Qt::DisplayRole).toString());
    dialog.setPrice(model->data(model->index(index.row(), 2), Qt::EditRole).toDouble());
    dialog.setDuration(model->data(model->index(index.row(), 3), Qt::EditRole).toInt());

    if (dialog.exec() == QDialog::Accepted)
    {
        model->setData(model->index(index.row(), 1), dialog.getName());
        model->setData(model->index(index.row(), 2), dialog.getPrice());
        model->setData(model->index(index.row(), 3), dialog.getDuration());
        model->submitAll();
        model->select();
    }
}

void Services::onDeleteClicked()
{
    QModelIndex index = ui->ServicesTable->currentIndex();

    if (!index.isValid())
    {
        QMessageBox::warning(this, tr("Błąd"), tr("Usługa nie została wybrana."));
        return;
    }
    QMessageBox::StandardButton reply = QMessageBox::question(this, "Usun",
        "Czy na pewno chcesz usunąć tą usługę?", QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes)
    {
        model->removeRow(index.row());
        model->submitAll();
        model->select();
    }
}

Services::Services(QWidget *parent) : QWidget(parent), ui(new Ui::Services) {
    ui->setupUi(this);
    connect(ui->AddService, &QPushButton::clicked, this, &Services::onAddClicked);
    connect(ui->DeleteService, &QPushButton::clicked, this, &Services::onDeleteClicked);
    connect(ui->EditService, &QPushButton::clicked, this, &Services::onEditClicked);
    createModel();
    createTable();
}

Services::~Services() {
    delete ui;
}
