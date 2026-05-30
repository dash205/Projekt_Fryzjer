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

class DatabaseConnection {
public:
    static DatabaseConnection& instance();

    bool openConnection();
    // Klient
    bool addClient(const Client& client);
    QList<Client> getAllClients();
    bool updateClient(const Client& client);
    bool deleteClient(int id);
    // Uslugi
    bool addService(const Service& service);
    QList<Service> getAllServices();
    bool updateService(const Service& service);
    bool deleteService(int id);
    //Wizyty
    bool addAppointment(const Appointment& appointment);
    QList<Appointment> getAllAppointments();
    bool updateAppointment(const Appointment& appointment);
    bool deleteAppointment(int id);

private:
    DatabaseConnection() = default;
    QSqlDatabase m_db;
};
