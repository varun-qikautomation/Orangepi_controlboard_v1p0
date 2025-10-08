#include "warning.h"
#include "ui_warning.h"
#include <QLabel>

warning::warning(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::warning)
{
    ui->setupUi(this);

    // user code
    // Navbar
    QWidget *navbar = new QWidget(this);
    navbar->setFixedHeight(70);
    navbar->setStyleSheet("background-color: #FFFFFF;");

    QHBoxLayout *navLayout = new QHBoxLayout(navbar);
    navLayout->setContentsMargins(20, 0, 20, 0);
    navLayout->setSpacing(25);

    // Adding Logo
    QLabel *logoLabel = new QLabel;
    QPixmap logoPixmap(":/logo/logoWNavbar.png");
    logoLabel->setPixmap(logoPixmap.scaled(250,250,Qt::KeepAspectRatio,Qt::SmoothTransformation));
    navLayout->addWidget(logoLabel);

    // Insert navbar at top of layout
    ui->mainLayout->insertWidget(0, navbar);

    // No margin for main layout (navbar and content separate)
    ui->mainLayout->setContentsMargins(0, 0, 0, 0);
    ui->mainLayout->setSpacing(0);

    // warning table section


}

warning::~warning()
{
    delete ui;
}
