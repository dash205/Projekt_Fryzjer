//
// Created by ms on 4.06.2026.
//

#ifndef PROJEKT_FRYZJER_USERDIALOG_H
#define PROJEKT_FRYZJER_USERDIALOG_H

#include <QDialog>
#include <QCryptographicHash>
#include <QRandomGenerator>
#include <QByteArray>
#include "databaseconnection.h"


QT_BEGIN_NAMESPACE

namespace Ui
{
    class userdialog;
}

QT_END_NAMESPACE

class addUserDialog : public QDialog
{
    Q_OBJECT

public:
    explicit addUserDialog(QWidget* parent = nullptr);
    ~addUserDialog() override;
    QString getLogin() const;
    QString getPassword() const;
    QString HashPassword(const QString password);
    QString getName() const;
protected:

private slots:
void on_buttonBox_accepted();
private:
    Ui::userdialog* ui;
};


#endif //PROJEKT_FRYZJER_USERDIALOG_H
