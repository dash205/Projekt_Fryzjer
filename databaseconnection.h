#include <QCoreApplication>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QStandardPaths>
#include <QDir>
#include <QSqlTableModel>
#include <QTableView>
#include "appointmentClass.h"
#include "serviceData.h"
#include "clientClass.h"
#include "serviceClass.h"

class DatabaseConnection {
public:
    static DatabaseConnection& instance();

    bool openConnection();
    //Klienci
    QList<Client> getAllClients();
    //Usługi
    QList<Service> getAllServices();
    bool CanServicesBeDeleted(const ServiceData& service);
    //Wizyty
    bool addAppointment(const Appointment& appointment);
    QList<Appointment> getAllAppointments();
    bool updateAppointment(const Appointment& appointment);
    bool deleteAppointment(int id);

private:
    DatabaseConnection() = default;
    QSqlDatabase m_db;
};
