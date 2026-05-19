#include "clients.h"
#include "ui_clients.h"


Clients::Clients(QWidget *parent) : QWidget(parent), ui(new Ui::Clients) {
    ui->setupUi(this);
}

Clients::~Clients() {
    delete ui;
}
