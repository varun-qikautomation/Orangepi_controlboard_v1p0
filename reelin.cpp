#include "reelin.h"
#include "ui_reelin.h"
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QStackedWidget>
#include <QString>
#include <QDebug>
#include <QGraphicsDropShadowEffect>
#include <QTableWidget>
#include <QHeaderView>
#include <QSqlQuery>
#include <QStandardItemModel>
reelin::reelin(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::reelin)
{
    ui->setupUi(this);

    // ===== Navbar =====
    QWidget *navBar = new QWidget(this);
    navBar->setFixedHeight(70);
    navBar->setStyleSheet("background-color: #FFFFFF;");

    QHBoxLayout *navbarLayout = new QHBoxLayout(navBar);
    navbarLayout->setSpacing(0);
    navbarLayout->setContentsMargins(20, 0, 20, 0);

    QLabel *logo = new QLabel;
    QPixmap logopixmap(":logo/logoWNavbar.png");
    logo->setPixmap(logopixmap.scaled(250, 250, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    navbarLayout->addWidget(logo);

    QPushButton *backButton = new QPushButton("← Back");
    backButton->setFixedHeight(40);
    backButton->setFixedWidth(100);
    navbarLayout->addWidget(backButton);

    ui->mainLayout->insertWidget(0, navBar);

    connect(backButton, &QPushButton::clicked, this, &reelin::backToHome);

    // ===== Middle Section =====
    QWidget *middleWidget = new QWidget(this);
    middleWidget->setStyleSheet("background-color: #ecf0f1;");
    QHBoxLayout *middleLayout = new QHBoxLayout(middleWidget);
    middleLayout->setSpacing(20);
    middleLayout->setContentsMargins(20, 30, 20, 30);

    // ---- Left Card: reelsIn ----
    QWidget *reelsIn = new QWidget(middleWidget);
    reelsIn->setStyleSheet("background-color: #ffffff; border-radius: 5px;");
    reelsIn->setMinimumSize(300, 150); // set fixed minimum sizesetStyleSheet("background-color: #ecf0f1;");

    // Add shadow effect
    QGraphicsDropShadowEffect *shadow1 = new QGraphicsDropShadowEffect;
    shadow1->setBlurRadius(15);
    shadow1->setXOffset(0);
    shadow1->setYOffset(5);
    shadow1->setColor(QColor(0, 0, 0, 80));
    reelsIn->setGraphicsEffect(shadow1);

    QVBoxLayout *reelsInLayout = new QVBoxLayout(reelsIn);
    reelsInLayout->setSpacing(5);
    reelsInLayout->setContentsMargins(80, 80, 80, 80);

    m_uniqueId = new QLineEdit;
    m_uniqueId->setPlaceholderText("Unique Id");
    m_uniqueId->setMaximumHeight(120);
    m_uniqueId->setStyleSheet(" border:1px solid;");
    reelsInLayout->addWidget(m_uniqueId);

    QWidget *buttonsWidget = new QWidget(this);
    QHBoxLayout *buttonsLayout = new QHBoxLayout(buttonsWidget);
    buttonsLayout->setSpacing(10);
    buttonsLayout->setContentsMargins(0, 0, 0, 0);

    QPushButton *inButton = new QPushButton("IN");
    inButton->setStyleSheet("background-color: #1976d2; color: white; font-weight: bold;");
    inButton->setMinimumSize(120, 50); // set fixed minimum size
    buttonsLayout->addWidget(inButton);

    QPushButton *cancelButton = new QPushButton("CANCEL");
    cancelButton->setMinimumSize(120, 50); // set fixed minimum size
    cancelButton->setStyleSheet("background-color:  #1976d2; color: white; font-weight: bold;");
    buttonsLayout->addWidget(cancelButton);

    reelsInLayout->addWidget(buttonsWidget);

    connect(inButton, &QPushButton::clicked, this, &reelin::handleInButton);
    connect(cancelButton, &QPushButton::clicked, this, &reelin::handleCancelButton);

    middleLayout->addWidget(reelsIn, 1); // stretch factor 1

    // ---- Right Card: Recently Scanned Reels ----
    QWidget *recentReels = new QWidget(middleWidget);
    recentReels->setStyleSheet("background-color: #FFFFFF; border-radius: 10px;");
    recentReels->setMinimumSize(300, 400); // same size as left card

    QGraphicsDropShadowEffect *shadow2 = new QGraphicsDropShadowEffect;
    shadow2->setBlurRadius(15);
    shadow2->setXOffset(0);
    shadow2->setYOffset(5);
    shadow2->setColor(QColor(0, 0, 0, 80));
    recentReels->setGraphicsEffect(shadow2);

    QVBoxLayout *recentReelsLayout = new QVBoxLayout(recentReels);
    recentReelsLayout->setSpacing(15);
    recentReelsLayout->setContentsMargins(15, 15, 15, 15);

    QLabel *recentLabel = new QLabel("Recently Scanned Reels");
    recentLabel->setStyleSheet("font-weight: bold; font-size: 5px;");
    recentReelsLayout->addWidget(recentLabel);

    middleLayout->addWidget(recentReels, 1); // same stretch factor for equal width

    ui->mainLayout->insertWidget(1, middleWidget);

    // ===== Bottom Section: Rack Details =====
    QWidget *rackDetails = new QWidget(this);
    rackDetails->setStyleSheet("background-color: #ffffff; border-radius: 5px;");
    rackDetails->setMaximumHeight(300);

    // Add shadow effect
    QGraphicsDropShadowEffect *shadow3 = new QGraphicsDropShadowEffect;
    shadow3->setBlurRadius(15);
    shadow3->setXOffset(0);
    shadow3->setYOffset(5);
    shadow3->setColor(QColor(0, 0, 0, 80));
    rackDetails->setGraphicsEffect(shadow3);

    // Layout for the rackDetails card
    QVBoxLayout *rackLayout = new QVBoxLayout(rackDetails);
    rackLayout->setContentsMargins(20, 30, 20, 30);
    rackLayout->setSpacing(10);

    // Label
    QLabel *rackLabel = new QLabel("Rack Details");
    rackLabel->setStyleSheet("font-weight: bold; font-size: 16px;");
    rackLayout->addWidget(rackLabel);

    // ===== TableView instead of QTableWidget =====
    QTableView *rackView = new QTableView(rackDetails);
    rackView->verticalHeader()->setVisible(false);
    rackView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    rackView->setSelectionMode(QAbstractItemView::NoSelection);
    rackView->setFocusPolicy(Qt::NoFocus);
    rackView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    rackView->setShowGrid(true);                // show grid lines
    rackView->setGridStyle(Qt::SolidLine);      // solid border style
    rackView->setStyleSheet("QHeaderView::section { background-color: #f0f0f0; border: 1px solid #ccc; }");


    // Model
    QStandardItemModel *model = new QStandardItemModel(rackView);
    model->setColumnCount(7);
    model->setHorizontalHeaderLabels(QStringList() << "#"
                                                   << "Logical Name"
                                                   << "Rack ID"
                                                   << "Number of Slots"
                                                   << "Filled Slots"
                                                   << "Remaining Slots"
                                                   << "Status");

    // Populate model from DB
    QSqlQuery query("SELECT logicalName, rackId, numberOfSlots, rackStatus FROM rackDetails");
    int row = 0;
    while(query.next()) {
        QList<QStandardItem*> items;
        items << new QStandardItem(QString::number(row + 1));
        items << new QStandardItem(query.value("logicalName").toString());
        items << new QStandardItem(query.value("rackId").toString());

        int slot = query.value("numberOfSlots").toInt();
        items << new QStandardItem(QString::number(slot));
        items << new QStandardItem("0"); // Filled Slots placeholder
        items << new QStandardItem(QString::number(slot)); // Remaining Slots

        QString status = query.value("rackStatus").toString();
        QStandardItem *statusItem = new QStandardItem(status);

        // Set background color for status
        if(status.compare("Online", Qt::CaseInsensitive) == 0)
            statusItem->setBackground(QBrush(Qt::green));
        else if(status.compare("Offline", Qt::CaseInsensitive) == 0)
            statusItem->setBackground(QBrush(Qt::red));

        statusItem->setTextAlignment(Qt::AlignCenter);
        items << statusItem;

        // Center align all other columns
        for(auto &item : items)
            item->setTextAlignment(Qt::AlignCenter);

        model->appendRow(items);
        row++;
    }

    // If no data, show placeholder
    if(row == 0) {
        QList<QStandardItem*> items;
        items << new QStandardItem("1");
        items << new QStandardItem("No data available");
        for(int i=2;i<7;i++)
            items << new QStandardItem("");
        for(auto &item : items)
            item->setTextAlignment(Qt::AlignCenter);
        model->appendRow(items);
    }

    rackView->setModel(model);

    // Add table view to layout
    rackLayout->addWidget(rackView);
    // Add rackDetails card to main layout
    ui->mainLayout->insertWidget(2, rackDetails);

}


void reelin::handleInButton(){
    QString scannedData = m_uniqueId->text();
    qDebug() << "scannedData : " <<scannedData;
}
void reelin::handleCancelButton(){

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
