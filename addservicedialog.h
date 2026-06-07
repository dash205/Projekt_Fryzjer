//
// Created by ms on 30.05.2026.
//

#ifndef PROJEKT_FRYZJER_ADDSERVICEDIALOG_H
#define PROJEKT_FRYZJER_ADDSERVICEDIALOG_H

#include <QDialog>
#include <QCloseEvent>

QT_BEGIN_NAMESPACE

namespace Ui
{
    class AddServiceDialog;
}

QT_END_NAMESPACE

class AddServiceDialog : public QDialog
{
    Q_OBJECT

public:
    enum class dialogMode{add, edit};
    
    explicit AddServiceDialog(dialogMode mode, QWidget* parent = nullptr);
    ~AddServiceDialog() override;
    QString getName() const;
    double getPrice() const;
    int getDuration() const;
    void setName(const QString& name);
    void setPrice(double price);
    void setDuration(int duration);
    Ui::AddServiceDialog* ui;
protected:
    void closeEvent(QCloseEvent* event) override;
private:
    private slots:
    void on_buttonBox_accepted();

};


#endif //PROJEKT_FRYZJER_ADDSERVICEDIALOG_H
