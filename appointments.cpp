#include "appointments.h"
#include "ui_appointments.h"
#include "databaseconnection.h"
#include "appointmentClass.h"

#include <QHeaderView>
#include <QTableWidgetItem>


Appointments::Appointments(QWidget *parent) : QWidget(parent), ui(new Ui::Appointments) {
    ui->setupUi(this);

    ui->tableWidget->setColumnCount(4);
    ui->tableWidget->setHorizontalHeaderLabels({"Klient", "Usługa", "Data i Godzina", "Cena"});

    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    refreshTable();
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

        itemClient->setData(Qt::UserRole, app.id);

        ui->tableWidget->setItem(row, 0, itemClient);
        ui->tableWidget->setItem(row, 1, itemService);
        ui->tableWidget->setItem(row, 2, itemDate);
        ui->tableWidget->setItem(row, 3, itemPrice);
    }
}
