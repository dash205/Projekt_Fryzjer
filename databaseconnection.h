#pragma once
#include <QCoreApplication>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QStandardPaths>
#include <QDir>
#include <QSqlTableModel>
#include <QTableView>
#include "appointmentClass.h"
#include "serviceClass.h"
#include "clientClass.h"
#include "serviceClass.h"

class DatabaseConnection {
public:
    static DatabaseConnection& instance();
    int currentUserId{};

    bool openConnection();
    bool beginTransaction();
    bool commitTransaction();
    bool rollbackTransaction();
    //Klienci
    QList<Client> getAllClients();
    //Usługi
    QList<Service> getAllServices();
    bool CanServicesBeDeleted(const Service& service);
    //Wizyty
    Appointment getAppointmentById(int id);
    bool addAppointment(const Appointment& appointment);
    QList<Appointment> getAllAppointments();
    bool updateAppointment(const Appointment& appointment);
    bool deleteAppointment(int id);
    bool addArchivalAppointment(const Appointment& appointment);
    bool sign_correctness(const QString& username, const QString& password);
    bool LoginExist(const QString& username);
    bool verifyPassword(const QString& password, const QString& hashedPassword);
    QString getCurrentAdminPassword(const int& id);
    bool autorisationCheck(const int& id);
    bool passwordChange(const QString& username, const QString& password);
    QString getUsername(const int& id);
    bool archiveAllClientApointments(int clientId);
private:
    DatabaseConnection() = default;
    QSqlDatabase m_db;
};
