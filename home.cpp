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

Home::Home(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Home)
{
    ui->setupUi(this);

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

    // ============================
    // 2️⃣ CARDS CONTAINER SECTION
    // ============================
    QWidget *cardsContainer = new QWidget(this);
    cardsContainer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    cardsContainer->setStyleSheet("background-color: #ecf0f1;"); // light background

    // ✅ Give margin only to the cards container
    QVBoxLayout *cardsLayout = new QVBoxLayout(cardsContainer);
    cardsLayout->setContentsMargins(100, 40, 80, 100);  // margin for all sides
    cardsLayout->setSpacing(0);

    // Flow layout for cards
    FlowLayout *flowLayout = new FlowLayout(nullptr, 0, 30, 30); // 30px spacing between cards

    QStringList cardTitles = {"Reels In", "Pick up", "Settings","Warnings","Stores Out","Stores In"};
    for (const QString &title : cardTitles) {
        CardFrame *card = new CardFrame();
        card->setFixedSize(250, 200);

        // Shadow for nice look
        QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect();
        shadow->setBlurRadius(15);
        shadow->setOffset(3, 3);
        shadow->setColor(QColor(0, 0, 0, 100));
        card->setGraphicsEffect(shadow);

        // Card content
        QVBoxLayout *cardContent = new QVBoxLayout(card);
        QLabel *titleLabel = new QLabel(title);
        titleLabel->setAlignment(Qt::AlignCenter);
        titleLabel->setStyleSheet("font-weight: bold; font-size: 16px;");
        cardContent->addStretch();
        cardContent->addWidget(titleLabel);
        cardContent->addStretch();
        cardContent->setContentsMargins(10, 10, 10, 10);

        flowLayout->addWidget(card);


        // ✅ Connect click using lambda
        connect(card, &CardFrame::clicked, [this, title]() {
            if (title == "Reels In") {
                reelin *dialog = new reelin();
                dialog->setModal(true);
                dialog->showMaximized();
                this->hide();
            }
        });
    }

    // Add the flow layout to the cards container
    cardsLayout->addLayout(flowLayout);

    // Finally, add cards container below navbar
    ui->mainLayout->addWidget(cardsContainer);
}

Home::~Home()
{
    delete ui;
}
