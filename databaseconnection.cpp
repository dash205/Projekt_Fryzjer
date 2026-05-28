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

QList<Service> DatabaseConnection::getAllServices() {
}

bool DatabaseConnection::updateService(const Service &service) {
}

bool DatabaseConnection::deleteService(int id) {
}

bool DatabaseConnection::addAppointment(const Appointment &appointment) {
}

QList<Appointment> DatabaseConnection::getAllAppointments() {
    QList<Appointment> list;
    QSqlQuery query;

    query.prepare(
        "SELECT "
        "a.id, "
        "a.client_id, "
        "a.service_id, "
        "(c.first_name || ' ' || c.last_name) AS client_full_name, "
        "s.name AS service_name, "
        "a.appointment_date, "
        "a.notes, "
        "s.price "
        "FROM appointments a "
        "INNER JOIN clients c ON a.client_id = c.id "
        "INNER JOIN services s ON a.service_id = s.id "
        "ORDER BY a.appointment_date DESC"
    );

    if (!query.exec()) {
        qDebug()<<query.lastError().text();
        return list;
    }

    while (query.next()) {
        Appointment app;

        app.id = query.value("id").toInt();
        app.client_id = query.value("client_id").toInt();
        app.service_id = query.value("service_id").toInt();
        app.notes = query.value("notes").toString();
        app.appointment_date = query.value("appointment_date").toDateTime();
        app.client_name = query.value("client_full_name").toString();
        app.service_name = query.value("service_name").toString();
        app.price = query.value("price").toDouble();

        list.append(app);
    }
    return list;
}

bool DatabaseConnection::updateAppointment(const Appointment &appointment) {
}

bool DatabaseConnection::deleteAppointment(int id) {
}
