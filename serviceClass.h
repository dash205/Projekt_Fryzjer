#ifndef PROJEKT_FRYZJER_SERVICECLASS_H
#define PROJEKT_FRYZJER_SERVICECLASS_H

#include <QString>

class Service { //klasa pomocnicza dla danych z tabeli usług
public:
    int id=0;
    QString name;
    double price;
    int duration_minutes;
};

#endif //PROJEKT_FRYZJER_SERVICECLASS_H