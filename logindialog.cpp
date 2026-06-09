//
// Created by ms on 3.06.2026.
//

// You may need to build the project (run Qt uic code generator) to get "ui_LoginDialog.h" resolved

#include "logindialog.h"

#include <QMessageBox>
#include <QPushButton>

#include "ui_logindialog.h"

QString LoginDialog::GetLogin() const //zwróć podane hasło w menu logowania
{
    return ui->login_line->text();
}
QString LoginDialog::GetPassword() const //zwróć hasłoo
{
    return ui->password_line->text();
}
void LoginDialog::on_buttonBox_clicked() //logika przycisku - sprawdzamy poprawność hasła
{
    if (!DatabaseConnection::instance().sign_correctness(GetLogin(), GetPassword()))
    {
        QMessageBox::warning(this, tr("Uwaga"), tr("Błędny login lub hasło!"));
        ui->login_line->setText("");
        ui->password_line->setText("");
        return;
    }
    QDialog::accept();
}
LoginDialog::LoginDialog(QWidget* parent) :
    QDialog(parent), ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
    ui->buttonBox->button(QDialogButtonBox::Apply)->setText("Zaloguj się");
    setFixedSize(this->size());
}

LoginDialog::~LoginDialog()
{
    delete ui;
}
