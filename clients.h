#ifndef PROJEKT_FRYZJER_CLIENTS_H
#define PROJEKT_FRYZJER_CLIENTS_H

#include <QWidget>
#include <QSqlTableModel>

QT_BEGIN_NAMESPACE
namespace Ui {
    class Clients;
}
QT_END_NAMESPACE

class Clients : public QWidget {
    Q_OBJECT

public:
    explicit Clients(QWidget *parent = nullptr);
    ~Clients() override;

private slots:
    void on_btnAdd_clicked();
    void on_btnDelete_clicked();
    void on_btnSave_clicked();

private:
    Ui::Clients *ui;
    QSqlTableModel *clientModel;
};

#endif //PROJEKT_FRYZJER_CLIENTS_H