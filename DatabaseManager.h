#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H
#include "QSqlDatabase"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QString>

class DatabaseManager  {
public:
    static DatabaseManager& instance(){
        static DatabaseManager instance;
        return instance;
    }
    QSqlDatabase& getDatabase() {
        return db;
    }


    void closeConnection(){
        if(db.isOpen()){
            db.close();
        }
    }
private:
    QSqlDatabase db;
    DatabaseManager(){
        db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName("rack.db");
        if(!db.open()){
            qDebug() << "Unable to open database" << db.lastError().text();
        }else{
            qDebug() << "conneted to database";
        }
    }
    DatabaseManager(const DatabaseManager&) = delete; // dont allow copy constructor
    DatabaseManager& operator=(const DatabaseManager&) = delete;// dont allow asignment operator

};
#endif // DATABASEMANAGER_H
