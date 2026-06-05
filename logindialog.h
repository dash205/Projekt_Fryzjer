//
// Created by ms on 3.06.2026.
//

#ifndef PROJEKT_FRYZJER_LOGINDIALOG_H
#define PROJEKT_FRYZJER_LOGINDIALOG_H

#include <QDialog>
#include  "databaseconnection.h"

QT_BEGIN_NAMESPACE
namespace Ui
{
    class LoginDialog;
}

QT_END_NAMESPACE

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget* parent = nullptr);
    ~LoginDialog() override;
    QString GetLogin() const;
    QString GetPassword() const;

private:
    Ui::LoginDialog* ui;
    private slots:
    void on_buttonBox_clicked();
};


#endif //PROJEKT_FRYZJER_LOGINDIALOG_H
