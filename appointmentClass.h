#ifndef PROJEKT_FRYZJER_APPOINTMENTCLASS_H
#define PROJEKT_FRYZJER_APPOINTMENTCLASS_H

#include <QString>
#include <QDateTime>

class Appointment {
public:
    int id = 0;
    int client_id = 0;
    int service_id = 0;
    QString client_name;
    QString service_name;
    QDateTime appointment_date;
    QString notes;
    double price = 0.0;
    int user_id = 0;
};

#endif //PROJEKT_FRYZJER_APPOINTMENTCLASS_H