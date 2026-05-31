//
// Created by Artur on 19.05.2026.
//

#ifndef PROJEKT_FRYZJER_APPOINTMENTS_H
#define PROJEKT_FRYZJER_APPOINTMENTS_H

#include <QWidget>


QT_BEGIN_NAMESPACE

namespace Ui {
    class Appointments;
}

QT_END_NAMESPACE

class Appointments : public QWidget {
    Q_OBJECT

public:
    explicit Appointments(QWidget *parent = nullptr);
    ~Appointments() override;

    void refreshTable();
    void setupForm();

private slots:
    void onAddAppointmentClicked();
    void onDeleteAppointmentClicked();

private:
    Ui::Appointments *ui;
};


#endif //PROJEKT_FRYZJER_APPOINTMENTS_H
