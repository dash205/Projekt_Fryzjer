//
// Created by ms on 4.06.2026.
//

// You may need to build the project (run Qt uic code generator) to get "ui_users.h" resolved

#include "users.h"
#include "databaseconnection.h"
#include "ui_users.h"
#include "userdialog.h"
#include <QMessageBox>
#include <QInputDialog>

QByteArray generateSalt(int length = 16) {
    QByteArray salt = QByteArray(length, '\0');
    for (int i = 0; i < length; i+=sizeof(quint32))
    {
        quint32 val = QRandomGenerator::global()->generate();
        memcpy(salt.data() + i, &val, qMin(static_cast<int>(sizeof(quint32)), length-i));
    }
    return salt;
}
QString HashPassword(const QString password)
{
    QByteArray salt = generateSalt();

    QByteArray data = salt + password.toUtf8();
    QByteArray hash = QCryptographicHash::hash(data, QCryptographicHash::Sha256);

    return QString("%1:%2")
            .arg(salt.toBase64())
            .arg(hash.toBase64());
}

void users::createModel()
{
    model = new QSqlTableModel(this);
    model->setTable("users");
    model->setTable("users");
    model->setHeaderData(1, Qt::Horizontal, "Login");
    model->setHeaderData(3, Qt::Horizontal, "Typ");
    model->setHeaderData(4, Qt::Horizontal, "Imie");
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model->select();
    qDebug() << model->lastError().text();
    qDebug() << model->rowCount();
}
void users::createTable()
{
    ui->UsersTable->setModel(model);
    ui->UsersTable->setColumnHidden(0, true);
    ui->UsersTable->setColumnHidden(2, true);
    ui->UsersTable->setFocusPolicy(Qt::NoFocus);
    ui->UsersTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    ui->UsersTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->UsersTable->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->UsersTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->UsersTable->setAlternatingRowColors(true);
    ui->UsersTable->verticalHeader()->setDefaultSectionSize(40);
    ui->UsersTable->setShowGrid(false);
}

void users::onAddClicked()
{
    addUserDialog dialog(this);

    if (dialog.exec() == QDialog::Accepted && passwordCheck())
    {
        int row = model->rowCount();
        model->insertRow(row);
        model->setData(model->index(row, 1), dialog.getLogin());
        model->setData(model->index(row, 2), HashPassword(dialog.getPassword()));
        model->setData(model->index(row, 4), dialog.getName());
        model->submitAll();
        model->select();
    }
}

bool users::passwordCheck()
{
    bool ok = false;
    QString currentPassword = QInputDialog::getText(
        this,
        "Potwierdzenie tożsamości",
        "Podaj swoje hasło administratora, aby kontynuować:",
        QLineEdit::Password,
        "",
        &ok
        );
    if (!ok || currentPassword.isEmpty())
    {
        return false;
    }

    if (!DatabaseConnection::instance().verifyPassword(currentPassword, DatabaseConnection::instance().getCurrentAdminPassword(DatabaseConnection::instance().currentUserId)))
    {
        QMessageBox::critical(this, "Błąd autoryzacji",
            "Nieprawidłowe hasło. Operacja odrzucona.");
        return false;
    }
    return true;
}

void users::onDeleteClicked()
{
    QModelIndex index = ui->UsersTable->currentIndex();
    if (!index.isValid())
    {
        QMessageBox::warning(this, tr("Błąd"), tr("Użytkownik nie została wybrany."));
        return;
    }
    if (passwordCheck())
    {
        model->removeRow(index.row());
        model->submitAll();
        model->select();
    }
}

void users::onEditClicked()
{
    QModelIndex index = ui->UsersTable->currentIndex();
    if (!index.isValid())
    {
        QMessageBox::warning(this, tr("Błąd"), tr("Użytkownik nie została wybrany."));
        return;
    }
    bool ok = false;
    QString currentPassword = QInputDialog::getText(
        this,
        "Nowe hasło",
        "Podaj nowe hasło dla tego użytkownika:",
        QLineEdit::Password,
        "",
        &ok
        );
    if (!ok || currentPassword.isEmpty())
    {
        return;
    }
    if (passwordCheck())
    {
        model->setData(model->index(index.row(), 2), HashPassword(currentPassword));
        model->submitAll();
        model->select();
        QMessageBox::information(this, "Nowe hasło", "Hasło zostało zmienione");
    }
}

users::users(QWidget* parent) :
    QWidget(parent), ui(new Ui::users)
{
    ui->setupUi(this);
    connect(ui->addUser, &QPushButton::clicked, this, &users::onAddClicked);
    connect(ui->deleteUser, &QPushButton::clicked, this, &users::onDeleteClicked);
    connect(ui->editUser, &QPushButton::clicked, this, &users::onEditClicked);

    createModel();
    createTable();
}

users::~users()
{
    delete ui;
}
