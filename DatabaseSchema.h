#ifndef DATABASESCHEMA_H
#define DATABASESCHEMA_H
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QString>

class DatabaseSchema {
public:
    static void initializeDatabase(){
        QSqlQuery query;

        const QString createUserTable = R"(
CREATE TABLE IF NOT EXISTS users(
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    username TEXT NOT NULL,
    password TEXT NOT NULL,
    UNIQUE(username, password)
)
)";

        executeQuery(query,createUserTable,"user table");

        // insert default user
        const QString insertDefaultUser = R"(
        INSERT OR IGNORE INTO users(username,password)
        VALUES('qik','qik')
)";
        executeQuery(query,insertDefaultUser,"Default user");
    }

private:
    static void executeQuery(QSqlQuery &query,const QString &sql,const QString &discription){
        if(!query.exec(sql)){
            qDebug() <<"Failed to create " << discription << query.lastError().text();
        }else{
            qDebug() <<"created " << discription << "created or already";
        }

    }
};

#endif // DATABASESCHEMA_H
