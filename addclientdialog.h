//
// Created by Mati on 06.06.2026.
//

#ifndef PROJEKT_FRYZJER_ADDCLIENTDIALOG_H
#define PROJEKT_FRYZJER_ADDCLIENTDIALOG_H

#include <QDialog>
#include <QString>
#include <QCloseEvent>


QT_BEGIN_NAMESPACE

namespace Ui {
    class AddClientDialog;
}

QT_END_NAMESPACE

class AddClientDialog : public QDialog {
    Q_OBJECT

public:
    explicit AddClientDialog(QWidget *parent = nullptr);

    ~AddClientDialog() override;

    QString getFirstName() const;
    QString getLastName() const;
    QString getEmail() const;
    QString getPhone() const;
    void setClientData(const QString &firstName, const QString &lastName, const QString &email, const QString &phone);

private:
    Ui::AddClientDialog *ui;

    private slots:
    void on_buttonBox_accepted();
};


#endif //PROJEKT_FRYZJER_ADDCLIENTDIALOG_H
