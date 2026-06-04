//
// Created by arktizz on 4.06.2026.
//

#ifndef PROJEKT_FRYZJER_ADDAPPOINTMENTDIALOG_H
#define PROJEKT_FRYZJER_ADDAPPOINTMENTDIALOG_H

#include <QDialog>


QT_BEGIN_NAMESPACE

namespace Ui {
    class addappointmentdialog;
}

QT_END_NAMESPACE

class addappointmentdialog : public QDialog {
    Q_OBJECT

public:
    explicit addappointmentdialog(QWidget *parent = nullptr);
    ~addappointmentdialog() override;

private slots:
    void onSaveClicked();

private:
    Ui::addappointmentdialog *ui;
    void setupForm();
};


#endif //PROJEKT_FRYZJER_ADDAPPOINTMENTDIALOG_H
