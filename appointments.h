//
// Created by Artur on 19.05.2026.
//

#ifndef PROJEKT_FRYZJER_APPOINTMENTS_H
#define PROJEKT_FRYZJER_APPOINTMENTS_H

#include <QWidget>
#include <QSet>


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
    signals:
            void appointmentChanged();
private slots:
    void onDeleteAppointmentClicked();
    void onCellChanged(int row, int column);
    void onAddAppointmentDialogClicked();
    void onSaveAllClicked();
    void onCancelChangesClicked();

private:
    Ui::Appointments *ui;

    QSet<int> modifiedAppointmentIds;
};


#endif //PROJEKT_FRYZJER_APPOINTMENTS_H
