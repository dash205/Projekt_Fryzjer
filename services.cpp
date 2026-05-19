//
// Created by Artur on 19.05.2026.
//

// You may need to build the project (run Qt uic code generator) to get "ui_services.h" resolved

#include "services.h"
#include "ui_services.h"


Services::Services(QWidget *parent) : QWidget(parent), ui(new Ui::Services) {
    ui->setupUi(this);
}

Services::~Services() {
    delete ui;
}
