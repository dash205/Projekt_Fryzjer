#ifndef PROJEKT_FRYZJER_APPOINTMENTCLASS_H
#define PROJEKT_FRYZJER_APPOINTMENTCLASS_H

#include <QString>
#include <QDateTime>

class Appointment {
    int id = 0;
    int client_id;
    int service_id;
    QDateTime appointment_date;
    QString notes;
};

#endif //PROJEKT_FRYZJER_APPOINTMENTCLASS_H