//
// Created by Artur on 19.05.2026.
//

#ifndef PROJEKT_FRYZJER_CLIENTS_H
#define PROJEKT_FRYZJER_CLIENTS_H

#include <QWidget>


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

private:
    Ui::Clients *ui;
};


#endif //PROJEKT_FRYZJER_CLIENTS_H
