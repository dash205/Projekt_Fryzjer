//
// Created by ms on 3.06.2026.
//

// You may need to build the project (run Qt uic code generator) to get "ui_LoginDialog.h" resolved

#include "logindialog.h"

#include <QMessageBox>

#include "ui_logindialog.h"

QString LoginDialog::GetLogin() const
{
   return ui->login_line->text();
}
QString LoginDialog::GetPassword() const
{
    return ui->password_line->text();
}
void LoginDialog::on_buttonBox_clicked()
{
    if (!DatabaseConnection::instance().sign_correctness(GetLogin(), GetPassword()))
    {
        QMessageBox::warning(this, tr("Uwaga"), tr("Błędny login lub hasło!"));
        return;
    }
    QDialog::accept();
}
LoginDialog::LoginDialog(QWidget* parent) :
    QDialog(parent), ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
}

LoginDialog::~LoginDialog()
{
    delete ui;
}
