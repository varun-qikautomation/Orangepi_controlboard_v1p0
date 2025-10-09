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


        //Rack details table
            const QString createRackDetailsTable = R"(CREATE TABLE IF NOT EXISTS rackDetails(
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            logicalName TEXT UNIQUE NOT NULL,
            rackId TEXT UNIQUE NOT NULL,
            numberOfSlots INTEGER UNIQUE NOT NULL,
            rackIp TEXT UNIQUE NOT NULL,
            rackPort TEXT UNIQUE NOT NULL,
            type TEXT UNIQUE NOT NULL,
            rackStatus TEXT UNIQUE NOT NULL,
            serverIp TEXT UNIQUE NOT NULL,
            serverPort TEXT UNIQUE NOT NULL
            ))";
             executeQuery(query,createRackDetailsTable,"rackDetails");

        // default rack details
            const QString defaultRackDetails = R"(INSERT OR IGNORE INTO rackDetails(logicalName,rackId,numberOfSlots,
            rackIp,rackPort,type,rackStatus,serverIp,serverPort) VALUES("S001","QIKKRACKTEST1","15","192.168.0.1","8877","Stationary",
            "Offline","192.168.0.2","8100")  )";
            executeQuery(query,defaultRackDetails,"update rackDetails");

            const QString createStocksTable = R"(
CREATE TABLE IF NOT EXISTS stocks(
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    uniqueId TEXT UNIQUE NOT NULL,
    manufacturer TEXT NOT NULL,
    quantity INTEGER NOT NULL,
    manufactureDate TEXT NOT NULL,
    lotNumber TEXT,
    partLocation TEXT NOT NULL,
    isScraped BOOL NOT NULL DEFAULT FALSE,
    partNumber TEXT NOT NULL,
    invoiceNumber TEXT,
    description TEXT,
    internalPN TEXT NOT NULL,
    expireDate TEXT,
    invoiceDate TEXT,
    createdAt DATETIME DEFAULT (datetime('now','localtime')),
    updatedAt DATETIME DEFAULT (datetime('now','localtime'))
)
)";

executeQuery(query,createStocksTable,"stocks");

            const QString createTrigger = R"(
CREATE TRIGGER IF NOT EXISTS trigger_stocks_updatedAt
AFTER UPDATE ON stocks
FOR EACH ROW
BEGIN
    UPDATE stocks
    SET updatedAt = datetime('now','localtime')
    WHERE id = OLD.id;
END;
)";
executeQuery(query,createTrigger,"update stocks");

//stores table create
const QString createStoresTable = R"(CREATE TABLE IF NOT EXISTS stores(id INTEGER PRIMARY KEY AUTOINCREMENT,
        uniqueId TEXT UNIQUE NOT NULL,
        quantity TEXT NOT NULL,
        partNumber TEXT NOT NULL,
        internalPN TEXT NOT NULL,
        reelPlaced BOOL NOT NULL,
        isWarning BOOL NOT NULL DEFAULT FALSE,
        isPickup BOOL NOT NULL DEFAULT FALSE,
        createdAt DATETIME DEFAULT (datetime('now','localtime')),
        updatedAt DATETIME DEFAULT (datetime('now','localtime'))
)   )";
executeQuery(query,createStoresTable,"stores");

const QString createTriggerStores = R"(
CREATE TRIGGER IF NOT EXISTS trigger_stores_updatedAt
AFTER UPDATE ON stores
FOR EACH ROW
BEGIN
    UPDATE stores
    SET updatedAt = datetime('now','localtime')
    WHERE id = OLD.id;
END;
)";
executeQuery(query,createTriggerStores,"update stores");

//settings
    const QString createBarcodeTempletTable = R"(CREATE TABLE IF NOT EXISTS barcodeTemplet(
        id INTEGER PRIMARY KEY AUTOINCREMENT,
        type TEXT UNIQUE,
        groupSeperator TEXT UNIQUE,
        templete text,
        createdAt DATETIME DEFAULT (datetime('now','localtime')),
        updatedAt DATETIME DEFAULT (datetime('now','localtime'))))";
executeQuery(query,createBarcodeTempletTable,"barcodeTemplet");

    const QString createBarcodeTempletTrigger = R"(
CREATE TRIGGER IF NOT EXISTS trigger_barcodeTemplet_updatedAt
AFTER UPDATE ON stores
FOR EACH ROW
BEGIN
    UPDATE barcodeTemplet
    SET updatedAt = datetime('now','localtime')
    WHERE id = OLD.id;
END;
)";
    executeQuery(query,createBarcodeTempletTrigger,"update barcodeTemplet");
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
