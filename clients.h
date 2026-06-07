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
    void on_btnRevert_clicked();
    void on_lineEditSearch_textChanged(const QString &text) const;
    void on_tableView_doubleClicked(const QModelIndex &index);

private:
    Ui::Clients *ui;
    QSqlTableModel *clientModel;

    bool confirmation(const QString &title, const QString &message);
};

#endif //PROJEKT_FRYZJER_CLIENTS_H