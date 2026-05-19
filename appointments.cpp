//
// Created by Artur on 19.05.2026.
//

// You may need to build the project (run Qt uic code generator) to get "ui_Appointments.h" resolved

#include "appointments.h"
#include "ui_Appointments.h"


Appointments::Appointments(QWidget *parent) : QWidget(parent), ui(new Ui::Appointments) {
    ui->setupUi(this);
}

Appointments::~Appointments() {
    delete ui;
}
