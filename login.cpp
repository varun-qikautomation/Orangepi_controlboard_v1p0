#include "login.h"
#include "ui_login.h"
#include <QDebug>
#include <QString>
#include <QSqlQuery>
#include <QMessageBox>
Login::Login(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Login)
{
    ui->setupUi(this);
}

Login::~Login()
{
    delete ui;
}

void Login::on_pushButton_login_clicked()
{
    hide();
  homepage = new Home(this);
    homepage->showMaximized();
//     QString username = ui->lineEdit_username->text();
//     QString password = ui->lineEdit_2_password->text();

//     QSqlQuery query;

//       query.prepare("SELECT * FROM users WHERE username = :username AND password = :password");
//     query.bindValue(":username",username);
//       query.bindValue(":password",password);

//     if(!query.exec()){
//           QMessageBox::critical(this,"Error","Database Error");
// return;    }

//     if(query.next()){
//         QMessageBox::information(this,"Success","Login successfull");
//         hide();
//         homepage = new Home(this);
//         homepage->showMaximized();

//     }else{
//         QMessageBox::warning(this,"Login Failed","Invalid username or password");
//     }
 }

