#include <QCoreApplication>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QStandardPaths>
#include <QDir>

class DatabaseConnection {
public:
    static DatabaseConnection& instance();

    bool openConnection();
    bool addClient(const QString &name);
private:
    DatabaseConnection() = default;
    QSqlDatabase m_db;
};
