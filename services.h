#ifndef PROJEKT_FRYZJER_SERVICES_H
#define PROJEKT_FRYZJER_SERVICES_H

#include <QWidget>


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
};


#endif //PROJEKT_FRYZJER_SERVICES_H
