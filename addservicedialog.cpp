//
// Created by ms on 30.05.2026.
//

// You may need to build the project (run Qt uic code generator) to get "ui_AddServiceDialog.h" resolved

#include "addservicedialog.h"
#include "ui_addservicedialog.h"
#include <QMessageBox>

QString AddServiceDialog::getName() const
{
  return ui->ServiceName->text();
}
double AddServiceDialog::getPrice() const
{
    return ui->ServicePrice->value();
}
int AddServiceDialog::getDuration() const
{
    return ui->ServiceDuration->value();
}
void AddServiceDialog::setName(const QString& name)
{
    ui->ServiceName->setText(name);
}
void AddServiceDialog::setPrice(double price)
{
    ui->ServicePrice->setValue(price);
}
void AddServiceDialog::setDuration(int duration)
{
    ui->ServiceDuration->setValue(duration);
}

void AddServiceDialog::closeEvent(QCloseEvent* event)
{
    event->ignore();
}
void AddServiceDialog::on_buttonBox_accepted()
{
    if (ui->ServiceName->text().isEmpty() || ui->ServicePrice->value() == 0.00 || ui->ServiceDuration->value() == 0)
    {
        QMessageBox::warning(this, "Uwaga", "Brakuje danych dla usługi.");
        return;
    }
    QDialog::accept();
}
AddServiceDialog::AddServiceDialog(QWidget* parent) :
    QDialog(parent), ui(new Ui::AddServiceDialog)
{
    ui->setupUi(this);
    ui->ServicePrice->setMaximum(9999.99);
    /*connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &AddServiceDialog::on_buttonBox_accepted);*/
    connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
}

AddServiceDialog::~AddServiceDialog()
{
    delete ui;
}
