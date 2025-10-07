#include "login.h"

#include <QApplication>
#include "DatabaseManager.h"
#include "DatabaseSchema.h"
#include "QMessageBox"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // initalize and open db
    DatabaseManager &dbManager = DatabaseManager::instance();
    QSqlDatabase &db = const_cast<QSqlDatabase&>(dbManager.getDatabase());

    if (!db.isOpen()) {
        QMessageBox::critical(nullptr, "Database Error", "Failed to open connection");
        return 1;
    }

    // adding default user

    DatabaseSchema::initializeDatabase();

    // optional: check 'users' table
    QSqlQuery checkQuery(db);
    checkQuery.prepare("SELECT name FROM sqlite_master WHERE type='table' AND name='users';" );
    if(!checkQuery.exec()){
        qDebug() << "Error in checking" << checkQuery.lastError().text();
        return 1;
    }
    if(!checkQuery.next()){
        qDebug() <<  "users table not found";
    }


    Login w;
    w.setWindowTitle("rack software");
    w.showMaximized();

    return a.exec();
}
