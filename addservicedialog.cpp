//
// Created by ms on 30.05.2026.
//

// You may need to build the project (run Qt uic code generator) to get "ui_AddServiceDialog.h" resolved

#include "addservicedialog.h"
#include "addservicedialog.h"
#include "ui_addservicedialog.h"
#include <QMessageBox>

//okno dla obsługi modyfikacji usługi oraz jej dodawania

QString AddServiceDialog::getName() const //zwróć podane imie w oknie wprowadzania usługi
{
  return ui->ServiceName->text();
}
double AddServiceDialog::getPrice() const //zwróć cenę
{
    return ui->ServicePrice->value();
}
int AddServiceDialog::getDuration() const //zwróć czas trwania
{
    return ui->ServiceDuration->value();
}
void AddServiceDialog::setName(const QString& name) //ustaw imię (w przypadku modyfikacji usługi)
{
    ui->ServiceName->setText(name);
}
void AddServiceDialog::setPrice(double price) //ustaw cenę
{
    ui->ServicePrice->setValue(price);
}
void AddServiceDialog::setDuration(int duration) //ustaw czas trwania
{
    ui->ServiceDuration->setValue(duration);
}

void AddServiceDialog::closeEvent(QCloseEvent* event) //nie można kliknąć w krzyżyk zamknięcia okna
{
    event->ignore();
}
void AddServiceDialog::on_buttonBox_accepted() //logika przycisku akceptacji
{
    if (ui->ServiceName->text().isEmpty() || ui->ServicePrice->value() == 0.00 || ui->ServiceDuration->value() == 0)
    {
        QMessageBox::warning(this, "Uwaga", "Brakuje danych dla usługi.");
        return;
    }
    QDialog::accept();
}
AddServiceDialog::AddServiceDialog(dialogMode mode,QWidget* parent) :
    QDialog(parent), ui(new Ui::AddServiceDialog)
{
        ui->setupUi(this);
        ui->ServicePrice->setMaximum(9999.99);
        connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    switch (mode) //zmiana tytułu okna w zależności od przeznaczenia
    {
    case dialogMode::add: setWindowTitle("Dodaj usługę"); break;
    case dialogMode::edit: setWindowTitle("Modyfikuj usługę"); break;
    }
    setFixedSize(this->size());

}

AddServiceDialog::~AddServiceDialog()
{
    delete ui;
}
