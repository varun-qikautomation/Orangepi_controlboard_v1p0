#include "settings.h"
#include "qrcodetemplate.h"
#include "productionline.h"
#include <QMessageBox>
#include <QSplitter>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFrame>
#include <QPixmap>
#include <QTreeWidgetItem>
#include <QStackedWidget>
#include <QLineEdit>
#include <QCheckBox>
#include <QPropertyAnimation>
#include <QEasingCurve>
#include <QTableWidget>
#include <QHeaderView>
#include <QPushButton>
#include <QComboBox>
#include <QLabel>

Settings::Settings(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0,0,0,0);

    // ---------- NAVBAR ----------
    QWidget *navbar = new QWidget(this);
    navbar->setFixedHeight(70);
    navbar->setStyleSheet("background-color: white; border-bottom: 1px solid #ccc;");

    QHBoxLayout *navLayout = new QHBoxLayout(navbar);
    navLayout->setContentsMargins(20, 0, 20, 0);
    navLayout->setSpacing(25);

    QLabel *logoLabel = new QLabel;
    QPixmap logoPixmap(":/logo/logoA.png");
    logoLabel->setPixmap(logoPixmap.scaled(200, 50, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    navLayout->addWidget(logoLabel);
    navLayout->addStretch();

    QLabel *title = new QLabel("");
    title->setStyleSheet("font-size: 22px; font-weight: bold; color: #333;");
    navLayout->addWidget(title);

    mainLayout->addWidget(navbar);

    // ---------- SPLITTER ----------
    QSplitter *splitter = new QSplitter(Qt::Horizontal, this);
    splitter->setHandleWidth(2);

    // ---------- SIDEBAR ----------
    QTreeWidget *sidebar = new QTreeWidget;
    sidebar->setHeaderHidden(true);
    sidebar->setStyleSheet(R"(
        QTreeWidget {
            background-color: #f8f8f8;
            border-right: 1px solid #ccc;
            font-size: 15px;
        }
        QTreeWidget::item {
            padding: 6px 8px;
        }
        QTreeWidget::item:selected {
            background-color: #0078d7;
            color: white;
        }
    )");

    // Top-level items
    QTreeWidgetItem *partsInConf = new QTreeWidgetItem(QStringList() << "Parts In Configurations");
    QTreeWidgetItem *systemConfigs = new QTreeWidgetItem(QStringList() << "System Configs");
    QTreeWidgetItem *smartRack = new QTreeWidgetItem(QStringList() << "Smart Rack");
    QTreeWidgetItem *printers = new QTreeWidgetItem(QStringList() << "Printers");
    QTreeWidgetItem *qrCode = new QTreeWidgetItem(QStringList() << "QR Code Template");

    // Smart Rack children
    QTreeWidgetItem *prodLineItem = new QTreeWidgetItem(QStringList() << "Production Line");
    smartRack->addChild(prodLineItem);
    smartRack->addChild(new QTreeWidgetItem(QStringList() << "Smart Rack"));
    smartRack->addChild(new QTreeWidgetItem(QStringList() << "Rack Configs"));
    smartRack->addChild(new QTreeWidgetItem(QStringList() << "Products"));
    smartRack->addChild(new QTreeWidgetItem(QStringList() << "Threshold Limit"));

    // Add top-level items to sidebar
    sidebar->addTopLevelItem(partsInConf);
    sidebar->addTopLevelItem(systemConfigs);
    sidebar->addTopLevelItem(smartRack);
    sidebar->addTopLevelItem(printers);
    sidebar->addTopLevelItem(qrCode);

    sidebar->expandAll();

    // ---------- STACKED WIDGET ----------
    QStackedWidget *stacked = new QStackedWidget;

    // ===== PAGE: Parts Configs =====
    QWidget *partsPage = new QWidget;
    QVBoxLayout *partsLayout = new QVBoxLayout(partsPage);
    partsLayout->setContentsMargins(40, 30, 40, 30);

    QLabel *partsTitle = new QLabel("Parts Configs");
    partsTitle->setStyleSheet("font-size: 20px; font-weight: bold;");
    partsLayout->addWidget(partsTitle);

    QFrame *partsFrame = new QFrame;
    partsFrame->setStyleSheet("background: white; border: 1px solid #ddd; border-radius: 6px;");
    QVBoxLayout *partsForm = new QVBoxLayout(partsFrame);
    partsForm->setContentsMargins(25,25,25,25);

    orderCombo = new QComboBox;
    groupCombo = new QComboBox;
    exchangeCombo = new QComboBox;

    partsForm->addWidget(new QLabel("Select Part Order"));
    partsForm->addWidget(orderCombo);
    partsForm->addWidget(new QLabel("Group Separator"));
    partsForm->addWidget(groupCombo);
    partsForm->addWidget(new QLabel("Work Order Parts Exchange Enable"));
    partsForm->addWidget(exchangeCombo);

    QPushButton *partsSave = new QPushButton("SAVE");
    partsSave->setFixedWidth(120);
    partsSave->setStyleSheet("background-color:#0078d7;color:white;padding:8px 20px;border-radius:5px;");
    connect(partsSave, &QPushButton::clicked, this, &Settings::onSaveClicked);

    QHBoxLayout *partsSaveLayout = new QHBoxLayout;
    partsSaveLayout->addStretch();
    partsSaveLayout->addWidget(partsSave);
    partsForm->addLayout(partsSaveLayout);

    partsLayout->addWidget(partsFrame);
    partsLayout->addStretch();
    stacked->addWidget(partsPage);

    // ===== PAGE: System Configs =====
    QWidget *systemPage = new QWidget;
    QVBoxLayout *sysLayout = new QVBoxLayout(systemPage);
    sysLayout->setContentsMargins(40,30,40,30);

    QLabel *sysTitle = new QLabel("System Configs");
    sysLayout->addWidget(sysTitle);

    QFrame *sysFrame = new QFrame;
    sysFrame->setStyleSheet("background:white;border:1px solid #ddd;border-radius:6px;");
    QVBoxLayout *sysForm = new QVBoxLayout(sysFrame);
    sysForm->setContentsMargins(25,25,25,25);

    QCheckBox *usbToggle = new QCheckBox("USB printer enable");
    usbToggle->setChecked(true);
    sysForm->addWidget(usbToggle);

    QLineEdit *vendorEdit = new QLineEdit;
    vendorEdit->setPlaceholderText("Vendor ID");
    QLineEdit *productEdit = new QLineEdit;
    productEdit->setPlaceholderText("Product ID");
    sysForm->addWidget(vendorEdit);
    sysForm->addWidget(productEdit);

    QPushButton *sysSave = new QPushButton("SAVE");
    sysSave->setFixedWidth(120);
    sysSave->setStyleSheet("background-color:#0078d7;color:white;padding:8px 20px;border-radius:5px;");
    connect(sysSave, &QPushButton::clicked, this, [=](){
        QString msg = QString("USB: %1\nVendor: %2\nProduct: %3")
        .arg(usbToggle->isChecked() ? "ON" : "OFF")
            .arg(vendorEdit->text())
            .arg(productEdit->text());
        QMessageBox::information(this, "Saved", msg);
    });
    QHBoxLayout *sysSaveLayout = new QHBoxLayout;
    sysSaveLayout->addStretch();
    sysSaveLayout->addWidget(sysSave);
    sysForm->addLayout(sysSaveLayout);

    sysLayout->addWidget(sysFrame);
    sysLayout->addStretch();
    stacked->addWidget(systemPage);

    // ===== PAGE: Printers =====
    QWidget *printerPage = new QWidget;
    QVBoxLayout *pLayout = new QVBoxLayout(printerPage);
    pLayout->setContentsMargins(40,30,40,30);

    QLabel *pTitle = new QLabel("Printers");
    pLayout->addWidget(pTitle);

    QFrame *pFrame = new QFrame;
    QVBoxLayout *pForm = new QVBoxLayout(pFrame);
    QLineEdit *printerName = new QLineEdit; printerName->setPlaceholderText("Printer Name *");
    QLineEdit *ipAddress = new QLineEdit; ipAddress->setPlaceholderText("IP Address *");
    QLineEdit *port = new QLineEdit; port->setPlaceholderText("Port *");
    QPushButton *saveBtn = new QPushButton("SAVE");
    QPushButton *clearBtn = new QPushButton("CLEAR");
    QHBoxLayout *row1 = new QHBoxLayout;
    row1->addWidget(printerName); row1->addWidget(ipAddress); row1->addWidget(port);
    row1->addWidget(saveBtn); row1->addWidget(clearBtn);
    pForm->addLayout(row1);

    QTableWidget *printerTable = new QTableWidget(5,5);
    QStringList headers = {"#", "Printer Name", "IP Address", "Port", "Action"};
    printerTable->setHorizontalHeaderLabels(headers);
    pForm->addWidget(printerTable);

    pLayout->addWidget(pFrame);
    stacked->addWidget(printerPage);

    // ===== PAGE: QR Code Template =====
    QRCodeTemplate *qrPage = new QRCodeTemplate;
    stacked->addWidget(qrPage);

    // ===== PAGE: Production Line =====
    ProductionLine *productionPage = new ProductionLine;
    stacked->addWidget(productionPage);

    // ---------- SIDEBAR NAVIGATION ----------
    connect(sidebar, &QTreeWidget::itemClicked, this, [=](QTreeWidgetItem *item, int){
        QString text = item->text(0);

        // Expand parent if child clicked
        QTreeWidgetItem *parent = item->parent();
        if(parent) parent->setExpanded(true);

        // Deselect all first
        for(int i=0; i<sidebar->topLevelItemCount(); i++)
            sidebar->topLevelItem(i)->setSelected(false);
        item->setSelected(true);

        if(text == "Parts In Configurations") stacked->setCurrentWidget(partsPage);
        else if(text == "System Configs") stacked->setCurrentWidget(systemPage);
        else if(text == "Printers") stacked->setCurrentWidget(printerPage);
        else if(text == "QR Code Template") stacked->setCurrentWidget(qrPage);
        else if(text == "Production Line") stacked->setCurrentWidget(productionPage);
    });

    splitter->addWidget(sidebar);
    splitter->addWidget(stacked);
    splitter->setStretchFactor(1, 1);
    mainLayout->addWidget(splitter);
}

// ---------- SAVE BUTTON ----------
void Settings::onSaveClicked()
{
    QString order = orderCombo->currentText();
    QString group = groupCombo->currentText();
    QString exchange = exchangeCombo->currentText();

    QString msg = QString("Part Order: %1\nGroup Separator: %2\nExchange Enable: %3")
                      .arg(order, group, exchange);

    QMessageBox::information(this, "Saved", msg);
}
