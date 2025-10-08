#include "reelin.h"
#include "ui_reelin.h"
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QStackedWidget>

reelin::reelin(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::reelin)
{
    ui->setupUi(this);
    // Navbar
    QWidget *navBar = new QWidget(this);
    navBar->setFixedHeight(70);
    navBar->setStyleSheet("background-color: #FFFFFF;");

    QHBoxLayout *navbarLayout = new QHBoxLayout(navBar);
    navbarLayout->setSpacing(0);
    navbarLayout->setContentsMargins(20,0,20,0);

    // logo
    QLabel *logo = new QLabel;
    QPixmap logopixmap(":logo/logoWNavbar.png");
    logo->setPixmap(logopixmap.scaled(250,250,Qt::KeepAspectRatio,Qt::SmoothTransformation));
    navbarLayout->addWidget(logo);

    QPushButton *backButton = new QPushButton("← Back");
    backButton->setFixedHeight(40);
    backButton->setFixedWidth(100);
navbarLayout->addWidget(backButton);

    ui->mainLayout->insertWidget(0, navBar);


    // 3️⃣ Connect back button
    connect(backButton, &QPushButton::clicked, this, &reelin::backToHome);

}

void reelin::backToHome()
{
    // Start from this widget
    QWidget *p = this->parentWidget();

    // Walk up the parent hierarchy until we find a QStackedWidget
    while (p && !qobject_cast<QStackedWidget*>(p)) {
        p = p->parentWidget();
    }

    // If found, switch to the first page (home)
    if (QStackedWidget *stack = qobject_cast<QStackedWidget*>(p)) {
        stack->setCurrentIndex(0);
    }
}



reelin::~reelin()
{
    delete ui;
}
