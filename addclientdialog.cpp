//
// Created by Mati on 06.06.2026.
//

// You may need to build the project (run Qt uic code generator) to get "ui_addclientdialog.h" resolved

#include "addclientdialog.h"
#include <QMessageBox>
#include <QPushButton>
#include <QRegularExpressionValidator>
#include "ui_addclientdialog.h"

AddClientDialog::AddClientDialog(QWidget *parent) : QDialog(parent), ui(new Ui::AddClientDialog)
{
    ui->setupUi(this);

    ui->buttonBox->button(QDialogButtonBox::Cancel)->setText("Anuluj");

    // ui->lineEditPhone->setInputMask("999999999");
    QRegularExpression rxPhone("^[0-9]{9}$");
    QValidator *phoneValidator = new QRegularExpressionValidator(rxPhone, this);
    ui->lineEditPhone->setValidator(phoneValidator);

    QRegularExpression rxEmail("\\b[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\\.[A-Za-z]{2,4}\\b");
    QValidator *emailValidator = new QRegularExpressionValidator(rxEmail, this);
    ui->lineEditEmail->setValidator(emailValidator);

    ui->lineEditFirstName->setMaxLength(50);
    ui->lineEditLastName->setMaxLength(50);

    disconnect(ui->buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
}

AddClientDialog::~AddClientDialog()
{
    delete ui;
}

QString AddClientDialog::getFirstName() const
{
    return ui->lineEditFirstName->text();
}

QString AddClientDialog::getLastName() const
{
    return ui->lineEditLastName->text();
}

QString AddClientDialog::getEmail() const
{
    return ui->lineEditEmail->text();
}

QString AddClientDialog::getPhone() const
{
    return ui->lineEditPhone->text();
}

void AddClientDialog::on_buttonBox_accepted()
{
    // if (ui->lineEditFirstName->text().isEmpty() || ui->lineEditLastName->text().isEmpty() || ui->lineEditEmail->text().isEmpty() || ui->lineEditPhone->text().isEmpty())
    // {
    //     QMessageBox::warning(this, "Uwaga", "Uzupełnij wszystkie pola");
    //     return;
    // }

    //walidacja danych

    if (ui->lineEditFirstName->text().trimmed().isEmpty() || ui->lineEditLastName->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Błąd", "Imię i nazwisko nie mogą być puste!");
        return;
    }

    // if (ui->lineEditPhone->text().length() != 9) {
    //     QMessageBox::warning(this, "Błąd", "Podaj pełny, 9-cyfrowy numer telefonu!");
    //     return;
    // }

    if (!ui->lineEditPhone->text().isEmpty() && !ui->lineEditPhone->hasAcceptableInput()) {
        QMessageBox::warning(this, "Błąd", "Podaj pełny, 9-cyfrowy numer telefonu!");
        return;
    }

    if (!ui->lineEditEmail->text().isEmpty() && !ui->lineEditEmail->hasAcceptableInput()) {
        QMessageBox::warning(this, "Błąd", "Podany adres e-mail jest nieprawidłowy!");
        return;
    }

    QDialog::accept();
}

void AddClientDialog::setClientData(const QString &firstName, const QString &lastName, const QString &email, const QString &phone) {
    ui->lineEditFirstName->setText(firstName);
    ui->lineEditLastName->setText(lastName);
    ui->lineEditEmail->setText(email);
    ui->lineEditPhone->setText(phone);

    this->setWindowTitle("Edytuj dane klienta");
}
