//
// Created by ms on 4.06.2026.
//

// You may need to build the project (run Qt uic code generator) to get "ui_userdialog.h" resolved
#include "userdialog.h"
#include "ui_userdialog.h"
#include <QMessageBox>

QString addUserDialog::getLogin() const //pobierz wpisany login dla nowo tworzonego użytkownika
{
    return ui->login_line->text();
}
QString addUserDialog::getPassword() const //pobierz wpisane hasło
{
    return ui->password_line->text();
}
QString addUserDialog::getName() const //pobierz imie użytkownika
{
    return ui->name_line->text();
}
void addUserDialog::on_buttonBox_accepted() //logika dla przycisku zatwierdzenia w oknie dodawania użytkownika
{
    if (ui->login_line->text().isEmpty() || ui->password_line->text().isEmpty() || ui->name_line->text().isEmpty())
    {
        QMessageBox::warning(this, "Uwaga", "Brakuje danych dla użytkownika.");
        return;
    }
    if (DatabaseConnection::instance().LoginExist(ui->login_line->text()))
    {
        QMessageBox::warning(this, "Uwaga", "Podany login już istnieje.");
        return;
    }
    QDialog::accept();
}
addUserDialog::addUserDialog(QWidget* parent) :
    QDialog(parent), ui(new Ui::userdialog)
{
    ui->setupUi(this);
    connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    setFixedSize(this->size());
}

addUserDialog::~addUserDialog()
{
    delete ui;
}
