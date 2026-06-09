//
// Created by ms on 4.06.2026.
//

#ifndef PROJEKT_FRYZJER_USERS_H
#define PROJEKT_FRYZJER_USERS_H

#include <QWidget>
#include <QTableView>
#include <QSqlTableModel>

QT_BEGIN_NAMESPACE

namespace Ui
{
    class users;
}

QT_END_NAMESPACE

class users : public QWidget
{
    Q_OBJECT

public:
    explicit users(QWidget* parent = nullptr);
    ~users() override;

private:
    Ui::users* ui;
    QSqlTableModel *model;
    QTableView *tableView;
    void createModel();
    void createTable();
    void onAddClicked();
    void onDeleteClicked();
    void onPasswordChangeClicked();
    bool passwordCheck();
};


#endif //PROJEKT_FRYZJER_USERS_H
