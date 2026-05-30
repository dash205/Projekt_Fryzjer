#include "databaseconnection.h"

DatabaseConnection& DatabaseConnection::instance() {
    static DatabaseConnection inst;
    return inst;
}

bool DatabaseConnection::openConnection() {
    m_db = QSqlDatabase::addDatabase("QSQLITE");

    QString dbPath = QCoreApplication::applicationDirPath() + "/salon_fryzjerski.db";
    m_db.setDatabaseName(dbPath);

    if (!m_db.open()) {
        qDebug()<<"Blad otwarcia bazy"<<m_db.lastError().text();
        return false;
    }

    return true;
}

// bool DatabaseConnection::addClient(const Client &client) {
//     QSqlQuery query(m_db);
//
//     query.prepare("INSERT INTO client (name) VALUES (:name)");
//
//     query.bindValue(":name", name);
//
//     if(!query.exec()) {
//         qDebug()<<"Blad dodwania klienta"<<query.lastError().text();
//         return false;
//     }
//     return true;
// }

bool DatabaseConnection::addClient(const Client &client) {
}

QList<Client> DatabaseConnection::getAllClients() {
}

bool DatabaseConnection::updateClient(const Client &client) {
}

bool DatabaseConnection::deleteClient(int id) {
}

bool DatabaseConnection::addService(const Service &service) {
}


bool DatabaseConnection::updateService(const Service &service) {
}

bool DatabaseConnection::deleteService(int id) {
}

bool DatabaseConnection::addAppointment(const Appointment &appointment) {
}

QList<Appointment> DatabaseConnection::getAllAppointments() {
}

bool DatabaseConnection::updateAppointment(const Appointment &appointment) {
}

bool DatabaseConnection::deleteAppointment(int id) {
}
