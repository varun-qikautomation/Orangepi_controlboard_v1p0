#include "home.h"
#include "ui_home.h"
#include <QFrame>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGraphicsDropShadowEffect>
#include <QPushButton>
#include "cardframe.h"
#include "flowlayout.h"
#include <QDir>
#include <QStackedWidget>

Home::Home(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Home)
{
    ui->setupUi(this);
    // Create stacked widget
    stackedWidget = new QStackedWidget(this);
    stackedWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // Setup individual pages
    setupPages();

    // Add stacked widget to main layout
    ui->mainLayout->addWidget(stackedWidget);

}
void Home::setupPages()
{
    // 1️⃣ Home page (cards)
    homePage = new QWidget();
    setupHomePage();
    stackedWidget->addWidget(homePage);

    // 2️⃣ Reel In page
    reelsInPage = new reelin();   // your existing ReelIn widget
    stackedWidget->addWidget(reelsInPage);

    // ✅ Connect back signals to go to Home
    connect((reelin*)reelsInPage, &reelin::backToHome, [this]() {
        stackedWidget->setCurrentWidget(homePage);
    });

    // Show Home page initially
    stackedWidget->setCurrentWidget(homePage);
}

void Home::setupHomePage()
{
    // ✅ Create layout for the home page itself
    QVBoxLayout *homeLayout = new QVBoxLayout(homePage);
    homeLayout->setContentsMargins(0, 0, 0, 0);
    homeLayout->setSpacing(0);

    // Navbar
    QWidget *navbar = new QWidget(homePage);
    navbar->setFixedHeight(70);
    navbar->setStyleSheet("background-color: #FFFFFF;");

    QHBoxLayout *navLayout = new QHBoxLayout(navbar);
    navLayout->setContentsMargins(20, 0, 20, 0);
    navLayout->setSpacing(25);

    QLabel *logoLabel = new QLabel;
    QPixmap logoPixmap(":/logo/logoWNavbar.png");
    logoLabel->setPixmap(logoPixmap.scaled(250,250,Qt::KeepAspectRatio,Qt::SmoothTransformation));
    navLayout->addWidget(logoLabel);

    homeLayout->addWidget(navbar); //  add to home page layout

    // Cards container
    QWidget *cardsContainer = new QWidget(homePage);
    cardsContainer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    cardsContainer->setStyleSheet("background-color: #ecf0f1;");

    QVBoxLayout *cardsLayout = new QVBoxLayout(cardsContainer);
    cardsLayout->setContentsMargins(100, 40, 80, 100);
    cardsLayout->setSpacing(0);

    FlowLayout *flowLayout = new FlowLayout(nullptr, 0, 30, 30);

    QStringList cardTitles = {"Reels In", "Pick up", "Settings", "Warnings", "Stores Out", "Stores In"};
    for (const QString &title : cardTitles) {
        CardFrame *card = new CardFrame();
        card->setFixedSize(250, 200);

        QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect();
        shadow->setBlurRadius(15);
        shadow->setOffset(3, 3);
        shadow->setColor(QColor(0, 0, 0, 100));
        card->setGraphicsEffect(shadow);

        QVBoxLayout *cardContent = new QVBoxLayout(card);
        QLabel *titleLabel = new QLabel(title);
        titleLabel->setAlignment(Qt::AlignCenter);
        titleLabel->setStyleSheet("font-weight: bold; font-size: 16px;");
        cardContent->addStretch();
        cardContent->addWidget(titleLabel);
        cardContent->addStretch();
        cardContent->setContentsMargins(10, 10, 10, 10);

        flowLayout->addWidget(card);

        // Connect card click to switch page
        connect(card, &CardFrame::clicked, [this, title]() {
            if (title == "Reels In") {
                reelin *dialog = new reelin();
                dialog->setModal(true);
                dialog->showMaximized();
                this->hide();
            }else if(title == "Warnings"){
                warning *dialog = new warning();
                dialog->setModal(true);
                dialog->showMaximized();
                this->hide();
                stackedWidget->setCurrentWidget(reelsInPage);
            }
        });
    }

    cardsLayout->addLayout(flowLayout);
    homeLayout->addWidget(cardsContainer); // ✅ add below navbar
}

Home::~Home()
{
    delete ui;
}
