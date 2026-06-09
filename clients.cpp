#include "clients.h"
#include "ui_clients.h"
#include <QMessageBox>
#include <QSqlTableModel>

#include "addclientdialog.h"
#include "addservicedialog.h"
#include "databaseconnection.h"

Clients::Clients(QWidget *parent) : QWidget(parent), ui(new Ui::Clients) {
    ui->setupUi(this);

    // konfiguracja modelu bazy danych
    clientModel = new QSqlTableModel(this);
    clientModel->setTable("clients");
    clientModel->setEditStrategy(QSqlTableModel::OnManualSubmit);

    clientModel->setHeaderData(1, Qt::Horizontal, "Imię");
    clientModel->setHeaderData(2, Qt::Horizontal, "Nazwisko");
    clientModel->setHeaderData(3, Qt::Horizontal, "Telefon");
    clientModel->setHeaderData(4, Qt::Horizontal, "E-mail");
    clientModel->select();

    // konfiguracja widoku tabeli
    ui->tableView->setModel(clientModel);
    ui->tableView->hideColumn(0);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView->setSortingEnabled(true);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    //ustawiamy zeby przyciski zapisu i anulowania zmian byly domyslnie wylaczone
    ui->btnSave->setEnabled(false);
    ui->btnRevert->setEnabled(false);
}

Clients::~Clients() {
    delete ui;
}
// obsluga przycisku dodawania klientow
void Clients::on_btnAdd_clicked() {
    AddClientDialog dialog(this);

    if (dialog.exec() == QDialog::Accepted) {
        int row = clientModel->rowCount();
        clientModel->insertRow(row);
        //pobieranie danych z okna dialogowego
        clientModel->setData(clientModel->index(row, 1), dialog.getFirstName());
        clientModel->setData(clientModel->index(row, 2), dialog.getLastName());
        clientModel->setData(clientModel->index(row, 3), dialog.getPhone());
        clientModel->setData(clientModel->index(row, 4), dialog.getEmail());

        clientModel->submitAll();
        // ui->btnSave->setEnabled(true);
        // ui->btnRevert->setEnabled(true);
    }
}
//oblsluga przycisku usuwania klienta
void Clients::on_btnDelete_clicked() {
    int selectedRow = ui->tableView->currentIndex().row();
    if (selectedRow >= 0) {
        int clientId = clientModel->data(clientModel->index(selectedRow, 0)).toInt();

        QString imie = clientModel->data(clientModel->index(selectedRow, 1)).toString();
        QString nazwisko = clientModel->data(clientModel->index(selectedRow, 2)).toString();

        QString alert = QString("Czy na pewno chcesz usunąć klienta: <b>%1 %2</b>?").arg(imie, nazwisko);


        if (confirmation("Potwierdzenie usunięcia", alert)) {
            //przekazujemy id klienta do funkcji odpowiedzialnej za archiwizacje jego wizyt
            if (DatabaseConnection::instance().archiveAllClientApointments(clientId)) {
                clientModel->removeRow(selectedRow);

                if (clientModel->submitAll()) {
                    QMessageBox::information(this, "Sukces", "Klient został usunięty z bazy danych, a jego wizyty zarchiwizowane.");
                }
                else {
                    QMessageBox::critical(this, "Błąd", "Wystąpił problem podczas archiwizacji wizyt klienta.");
                }
            }
        }
    }
    else {
        QMessageBox::warning(this, "Błąd", "Wybierz klienta do usunięcia!");
    }
}
//obsluga przycisku zapisu zmian
void Clients::on_btnSave_clicked() {
    QString changes = QString("Czy na pewno chcesz zapisać wszystkie zmiany?");

    if (confirmation("Potwierdzenie zapisu",changes)) {
        if (clientModel->submitAll()) {
            QMessageBox::information(this, "Sukces", "Zmiany zostały zapisane w bazie.");

            ui->btnSave->setEnabled(false);
            ui->btnRevert->setEnabled(false);
        } else {
            QMessageBox::critical(this, "Błąd zapisu", clientModel->lastError().text());
        }
    }
}
//obsluga przycisku anulowania zmian
void Clients::on_btnRevert_clicked() {
    QString cancel = QString("Czy na pewno chcesz cofnąć zmiany?");

    if (confirmation("Potwierdzenie anulowania", cancel)) {
        clientModel->revertAll();

        ui->btnSave->setEnabled(false);
        ui->btnRevert->setEnabled(false);
    }
}

//customowe okienko potwierdzenia
bool Clients::confirmation(const QString &title, const QString &message) {
    QMessageBox msgBox(this);
    msgBox.setWindowTitle(title);
    msgBox.setText(message);
    msgBox.setIcon(QMessageBox::Question);

    QPushButton *btnTak = msgBox.addButton("Tak", QMessageBox::YesRole);
    QPushButton *btnNie = msgBox.addButton("Nie", QMessageBox::NoRole);
    msgBox.exec();

    return (msgBox.clickedButton() == btnTak);
}
//filtrowanie
void Clients::on_lineEditSearch_textChanged(const QString &text) const {
    if (text.isEmpty()) {
        clientModel->setFilter("");
    }
    else {
        QString filtr = QString("first_name LIKE '%%1%' OR last_name LIKE '%%1%' OR phone LIKE '%%1%' OR email LIKE '%%1%'").arg(text);
        clientModel->setFilter(filtr);
    }
    clientModel->select();
}
//edycja danych w tabeli
void Clients::on_tableView_doubleClicked(const QModelIndex &index) {
    int selectedRow = ui->tableView->currentIndex().row();
    //pobieramy obecne dane klienta
    QString currentFirstName = clientModel->data(clientModel->index(selectedRow, 1)).toString();
    QString currentLastName = clientModel->data(clientModel->index(selectedRow, 2)).toString();
    QString currentPhone = clientModel->data(clientModel->index(selectedRow, 3)).toString();
    QString currentEmail = clientModel->data(clientModel->index(selectedRow, 4)).toString();

    AddClientDialog dialog(this);
    //wstawiamy w oknie dialogowym obecne dane klienta
    dialog.setClientData(currentFirstName, currentLastName, currentEmail, currentPhone);

    //zmieniamy dane klienta w tabeli na nowe
    if (dialog.exec() == QDialog::Accepted) {
        clientModel->setData(clientModel->index(selectedRow, 1), dialog.getFirstName());
        clientModel->setData(clientModel->index(selectedRow, 2), dialog.getLastName());
        clientModel->setData(clientModel->index(selectedRow, 3), dialog.getPhone());
        clientModel->setData(clientModel->index(selectedRow, 4), dialog.getEmail());

        ui->btnSave->setEnabled(true);
        ui->btnRevert->setEnabled(true);
    }
}