#include "reelin.h"
#include "ui_reelin.h"

reelin::reelin(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::reelin)
{
    ui->setupUi(this);
}

reelin::~reelin()
{
    delete ui;
}
