#include "login.h"
#include "ui_login.h"
#include <QDebug>
#include <QString>
#include <QSqlQuery>
#include <QMessageBox>
#include <QWidget>
#include <QLabel>
#include <QPixmap>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QPushButton>

Login::Login(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Login)
{
    ui->setupUi(this);

    // ===== Central Widget =====
    QWidget *central = new QWidget(this);
    QHBoxLayout *mainLayout = new QHBoxLayout(central);
    mainLayout->setSpacing(200);                 // space between logo and login
    mainLayout->setContentsMargins(0,50,0,50);
    mainLayout->setAlignment(Qt::AlignCenter);  // center everything
    setCentralWidget(central);

    // ===== Logo =====
    QWidget *logoWidget = new QWidget(this);

    logoWidget->setMaximumWidth(600);  // max width for logo
    logoWidget->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);


    QVBoxLayout *logoLayout = new QVBoxLayout(logoWidget);
    logoLayout->setContentsMargins(0,0,0,0);
    logoLayout->setAlignment(Qt::AlignLeft | Qt::AlignVCenter); // logo left, vertically centered

    m_mainLogo = new QLabel;
    m_mainLogo->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    m_pixmap = QPixmap(":logo/logoB.png");
    m_mainLogo->setPixmap(m_pixmap.scaled(500,500,Qt::KeepAspectRatio,Qt::SmoothTransformation));

    logoLayout->addWidget(m_mainLogo);

    // ===== Login Form =====
    QWidget *loginWidget = new QWidget(this);
    loginWidget->setFixedSize(400, 500); // slightly bigger


    QVBoxLayout *loginLayout = new QVBoxLayout(loginWidget);
    loginLayout->setAlignment(Qt::AlignCenter);
    loginLayout->setSpacing(30);

    m_usernameEdit = new QLineEdit;
    m_passwordEdit = new QLineEdit;

    m_usernameEdit->setObjectName("usernameEdit");
    m_passwordEdit->setObjectName("passwordEdit");

    // Username

    m_usernameEdit->setPlaceholderText("Username");
    m_usernameEdit->setFixedHeight(40);
    m_usernameEdit->setStyleSheet(
        "border:1px solid #ccc; border-radius:5px; padding-left:10px; font-size:14px;"
        );
    loginLayout->addWidget(m_usernameEdit);

    // Password

    m_passwordEdit->setEchoMode(QLineEdit::Password);
    m_passwordEdit->setPlaceholderText("Password");
    m_passwordEdit->setFixedHeight(40);
    m_passwordEdit->setStyleSheet(
        "border:1px solid #ccc; border-radius:5px; padding-left:10px; font-size:14px;"
        );
    loginLayout->addWidget(m_passwordEdit);

    // Login Button
    QPushButton *loginButton = new QPushButton("Login");
    loginButton->setFixedHeight(45);
    loginButton->setStyleSheet(
        "background-color:#1976d2; color:white; font-size:16px; border-radius:8px; border:none;"
        );
    loginLayout->addWidget(loginButton);

    // ===== Add widgets to main layout =====
    mainLayout->addWidget(logoWidget);
    mainLayout->addWidget(loginWidget);

    // Optional: stretch to control space distribution
    mainLayout->setStretch(0,1); // logo
    mainLayout->setStretch(1,3); // login

    connect(loginButton, &QPushButton::clicked, this, &Login::handleLogin);


    // ===== Responsive logo =====
    this->installEventFilter(this);
}

// ===== Event filter for logo resizing =====
bool Login::eventFilter(QObject *obj, QEvent *event)
{
    if(event->type() == QEvent::Resize)
    {
        int logoWidth = m_mainLogo->parentWidget()->width();   // available width
        int logoHeight = m_mainLogo->parentWidget()->height(); // available height
        int size = qMin(600, qMin(logoWidth, logoHeight)); // ✅ correct

        m_mainLogo->setPixmap(m_pixmap.scaled(size, size, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
    return QMainWindow::eventFilter(obj, event);
}



Login::~Login()
{
    delete ui;
}

void Login::handleLogin()
{
    hide();
    homepage = new Home(this);
    homepage->showMaximized();
    // Example: get username and password
//     QString username = m_usernameEdit->text();
//     QString password = m_passwordEdit->text();


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

