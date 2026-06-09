#include "databaseconnection.h"

#include <QCryptographicHash>
#include <QMessageBox>

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

bool DatabaseConnection::beginTransaction() {
    return m_db.transaction();
}

bool DatabaseConnection::commitTransaction() {
    return m_db.commit();
}

bool DatabaseConnection::rollbackTransaction() {
    return m_db.rollback();
}

bool DatabaseConnection::CanServicesBeDeleted(const ServiceData& service)
{
    QSqlQuery query(m_db);
    query.prepare("Select a.* from appointments a "
                  "join services s on s.id = a.service_id where s.id = (:id)");
    query.bindValue(":id", service.id);

    if (!query.exec()) {qDebug()<<query.lastError().text(); return false;}

    return query.next();
}

Appointment DatabaseConnection::getAppointmentById(int id) {
    Appointment app;
    app.id = -1;

    QSqlQuery query;
    query.prepare("SELECT id, client_id, service_id, appointment_date, notes, user_id "
                  "FROM appointments WHERE id = :id");
    query.bindValue(":id", id);

    if (!query.exec()) {
        qDebug()<<"Błąd poczas pobierania wizyty o id "<<id<<":"<<query.lastError().text();
        return app;
    }

    if (query.next()) {
        app.id = query.value(0).toInt();
        app.client_id = query.value(1).toInt();
        app.service_id = query.value(2).toInt();
        app.appointment_date = query.value(3).toDateTime();
        app.notes = query.value(4).toString();
        app.user_id = query.value(5).toInt();

    }

    return app;
}

QList<Client> DatabaseConnection::getAllClients() {
    QList<Client> list;
    QSqlQuery query("SELECT id, first_name, last_name FROM clients");

    if (!query.exec()) {
        qDebug()<<query.lastError().text();
        return list;
    }

    while (query.next()) {
        Client c;
        c.id = query.value("id").toInt();
        c.first_name = query.value("first_name").toString();
        c.last_name = query.value("last_name").toString();
        list.append(c);
    }
    return list;
}

QList<Service> DatabaseConnection::getAllServices() {
    QList<Service> list;
    QSqlQuery query("SELECT id, name FROM services");

    if (!query.exec()) {
        qDebug()<<query.lastError().text();
        return list;
    }

    while (query.next()) {
        Service s;
        s.id = query.value("id").toInt();
        s.name = query.value("name").toString();
        list.append(s);
    }
    return list;
}

bool DatabaseConnection::addAppointment(const Appointment &appointment) {
    QSqlQuery query;
    query.prepare(
        "INSERT INTO appointments (client_id, service_id, appointment_date, notes, user_id) "
        "VALUES (:client_id, :service_id, :date, :notes, :user_id)");

    query.bindValue(":client_id", appointment.client_id);
    query.bindValue(":service_id", appointment.service_id);
    query.bindValue(":date", appointment.appointment_date);
    query.bindValue(":notes", appointment.notes);
    query.bindValue(":user_id", appointment.user_id);

    if (!query.exec()) {
        qDebug()<<"Blad dodawania wizyty"<<query.lastError().text();
        return false;
    }

    return true;
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
    QSqlQuery query;

    query.prepare(
        "UPDATE appointments SET "
        "client_id = :client_id, "
        "service_id = :service_id, "
        "appointment_date = :date, "
        "notes = :notes "
        "WHERE id = :id");

    query.bindValue(":client_id", appointment.client_id);
    query.bindValue(":service_id", appointment.service_id);
    query.bindValue(":date", appointment.appointment_date.toString("yyyy-MM-dd HH:mm:ss"));
    query.bindValue(":notes", appointment.notes);
    query.bindValue(":id", appointment.id);

    if (!query.exec()) {
        qDebug()<<"Blad przy edytowaniu wizyty: "<<query.lastError().text();
        return false;
    }
    return true;

}

bool DatabaseConnection::deleteAppointment(int id) {
    QSqlQuery query;

    query.prepare(
        "DELETE FROM appointments WHERE id = :id");
    query.bindValue(":id", id);

    if (!query.exec()) {
        qDebug()<<"Blad podczas usuwania wizyty: "<<query.lastError().text();
        return false;
    }
    return true;
}

bool DatabaseConnection::addArchivalAppointment(const Appointment &appointment) {
    QSqlQuery clientQuery;
    QString clientName = "klient";
    clientQuery.prepare("SELECT first_name, last_name FROM clients WHERE id = :id");
    clientQuery.bindValue(":id", appointment.client_id);

    if (clientQuery.exec() && clientQuery.next()) {
        clientName = clientQuery.value(0).toString() + " " + clientQuery.value(1).toString();
    }

    QSqlQuery serviceQuery;
    QString serviceName = "usluga";
    serviceQuery.prepare("SELECT name FROM services WHERE id = :id");
    serviceQuery.bindValue(":id", appointment.service_id);

    if (serviceQuery.exec() && serviceQuery.next()) {
        serviceName = serviceQuery.value(0).toString();
    }

    QSqlQuery userQuery;
    QString userName = "user";
    userQuery.prepare("SELECT user_name FROM users WHERE user_id = :user_id");
    userQuery.bindValue(":user_id", appointment.user_id);

    if (userQuery.exec() && userQuery.next()) {
        userName = userQuery.value(0).toString();
    }

    QSqlQuery insertQuery;
    insertQuery.prepare(
        "INSERT INTO archived_appointments (client_name, service_name, appointment_date, notes, user_name) "
        "VALUES (:client_name, :service_name, :appointment_date, :notes, :user_name)");

    insertQuery.bindValue(":client_name", clientName);
    insertQuery.bindValue(":service_name", serviceName);
    insertQuery.bindValue(":appointment_date", appointment.appointment_date);
    insertQuery.bindValue(":notes", appointment.notes);
    insertQuery.bindValue(":user_name", userName);

    if (!insertQuery.exec()) {
        qDebug()<<insertQuery.lastError().text();
        return false;
    }

    return true;

}

bool DatabaseConnection::sign_correctness(const QString& login, const QString& password)
{
    QSqlQuery query;

    query.prepare( "Select user_id, password from users WHERE login = :login");
    query.bindValue(":login", login);
    if (!query.exec())
    {
        qDebug()<<query.lastError().text(); return false;
    }
    if (!query.next()) return false;

    int id = query.value("user_id").toInt();

    QString hashedPassword = query.value("password").toString();

    if (verifyPassword(password, hashedPassword))
    {
        instance().currentUserId = id;
        return true;
    }
    return false;
}

bool DatabaseConnection::verifyPassword(const QString& password, const QString& hashedPassword)
{
    auto parts = hashedPassword.split(":");
    if (parts.size() != 2) { return false; }

    QByteArray salt =QByteArray::fromBase64(parts[0].toUtf8());
    QByteArray expectedHash = QByteArray::fromBase64(parts[1].toUtf8());

    QByteArray data = salt + password.toUtf8();
    QByteArray testHash = QCryptographicHash::hash(data, QCryptographicHash::Sha256);

    if (expectedHash.size() != testHash.size()) { return false; }
    int diff = 0;
    for (int i = 0; i < testHash.size(); i++)
    {
        diff |= static_cast<unsigned char>(testHash[i] ^ expectedHash[i]);
    }
    return diff == 0;
}


bool DatabaseConnection::LoginExist(const QString& username)
{
    QSqlQuery query;
    query.prepare("Select login from users where login = :login");
    query.bindValue(":login", username);
    if (!query.exec())
    {
        qDebug()<<query.lastError().text();
    }
    return query.next();
}

bool DatabaseConnection::autorisationCheck(const int& id)
{
    QSqlQuery query;
    query.prepare("Select user_type from users where user_id = :id");
    query.bindValue(":id", id);
    if (!query.exec())
    {
        qDebug()<<query.lastError().text();
    }
    if (!query.next()) return false;
    qDebug()<<"Typ:"<<query.value("user_type").toString();
    return "admin"==query.value("user_type").toString();
}

QString DatabaseConnection::getUsername(const int& id)
{
    QSqlQuery query;
    query.prepare("Select user_name from users where user_id = :id");
    query.bindValue(":id", id);
    if (!query.exec())
    {
        qDebug()<<query.lastError().text();
    }
    if (!query.next()) return "";
    return query.value("user_name").toString();
}

QString DatabaseConnection::getCurrentAdminPassword(const int& id)
{
    QSqlQuery query;
    query.prepare("Select password from users where user_id = :user_id");
    query.bindValue(":user_id", id);
    if (!query.exec())
    {
        qDebug()<<query.lastError().text();

    }
    if (!query.next()) return "";

    return query.value("password").toString();
}

bool DatabaseConnection::archiveAllClientApointments(int clientId) {
    if (!beginTransaction()) {
        qDebug() << "Nie można rozpocząć transakcji archiwizacji.";
        return false;
    }

    //pobieramy wszystkie wizyty danego klienta
    QSqlQuery query;
    query.prepare("SELECT id, client_id, service_id, appointment_date, notes, user_id FROM appointments WHERE client_id = :client_id");
    query.bindValue(":client_id", clientId);

    if (!query.exec()) {
        qDebug() << "Błąd pobierania wizyt do archiwizacji" << query.lastError().text();
        rollbackTransaction();
        return false;
    }

    while (query.next()) {
        Appointment app;
        app.id = query.value("id").toInt();
        app.client_id = query.value("client_id").toInt();
        app.service_id = query.value("service_id").toInt();
        app.appointment_date = query.value("appointment_date").toDateTime();
        app.notes = query.value("notes").toString();
        app.user_id = query.value("user_id").toInt();

        //archiwizujemy wizyty klienta
        if (!addArchivalAppointment(app)) {
            qDebug() << "Błąd podczas archiwizacji pojedynczej wizyty";
            rollbackTransaction();
            return false;
        }
    }
    // usuwamy wizyty klienta
    QSqlQuery deleteQuery;

    deleteQuery.prepare("DELETE FROM appointments WHERE client_id = :client_id");
    deleteQuery.bindValue(":client_id", clientId);

    if (!deleteQuery.exec()) {
        qDebug() << "Błąd usuwania starych wizyt: " << deleteQuery.lastError().text();
        rollbackTransaction();
        return false;
    }
    return commitTransaction();
}
