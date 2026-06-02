#ifndef PROJEKT_FRYZJER_SERVICES_H
#define PROJEKT_FRYZJER_SERVICES_H

#include <QWidget>
#include <QMainWindow>
#include <QTableView>
#include <QSqlTableModel>
#include <QBoxLayout>


QT_BEGIN_NAMESPACE

namespace Ui {
    class Services;
}

QT_END_NAMESPACE

class Services : public QWidget {
    Q_OBJECT

public:
    explicit Services(QWidget *parent = nullptr);

    ~Services() override;

private:
    Ui::Services *ui;
    QSqlTableModel *model;
    QTableView *tableView;
    void createModel();
    void createTable();
    private slots:
    void onAddClicked();
    void onDeleteClicked();
    void onEditClicked();

};


#endif //PROJEKT_FRYZJER_SERVICES_H
