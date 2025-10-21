#include "storesout.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QHeaderView>
#include <QPixmap>
#include <QTableWidgetItem>
#include <QCheckBox>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QPushButton>
#include <QDateTime>


StoreOut::StoreOut(QWidget *parent)
    : QDialog(parent)
{
    this->setWindowTitle("Stores Out");
    this->setStyleSheet("background-color: #f4f4f4;");

    QVBoxLayout *mainVLayout = new QVBoxLayout(this);
    mainVLayout->setContentsMargins(0, 0, 0, 0);
    mainVLayout->setSpacing(0);

    // Navbar
    QWidget *navbar = new QWidget(this);
    navbar->setFixedHeight(70);
    navbar->setStyleSheet("background-color: #FFFFFF; border-bottom: 1px solid #e0e0e0;");

    QHBoxLayout *navLayout = new QHBoxLayout(navbar);
    navLayout->setContentsMargins(20, 0, 20, 0);
    navLayout->setSpacing(25);

    QLabel *logoLabel = new QLabel;
    QPixmap logoPixmap(":/logo/logoA.png"); // PNG file in resources
    logoLabel->setPixmap(logoPixmap.scaled(250, 250, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    navLayout->addWidget(logoLabel);

    navLayout->addStretch(1);

    QLabel *breadcrumb = new QLabel();
    breadcrumb->setStyleSheet("font-size:14px; color:#555;");
    navLayout->addWidget(breadcrumb);

    mainVLayout->addWidget(navbar);

    // Content Area
    QHBoxLayout *contentLayout = new QHBoxLayout();
    contentLayout->setContentsMargins(10, 10, 10, 10);
    contentLayout->setSpacing(0);

    // Sidebar
    leftPanel = new QWidget(this);
    leftPanel->setFixedWidth(180);
    leftPanel->setStyleSheet("background-color: #f4f4f4;");

    QVBoxLayout *leftLayout = new QVBoxLayout(leftPanel);

    QLabel *title = new QLabel("Stores Out");
    QFont titleFont;
    titleFont.setBold(true);
    titleFont.setPointSize(12);
    title->setFont(titleFont);

    btnDirectPickup = new QPushButton("DIRECT PICKUP");
    btnWorkOrderPickup = new QPushButton("WORK ORDER PICKUP");

    btnDirectPickup->setCheckable(true);
    btnWorkOrderPickup->setCheckable(true);
    btnDirectPickup->setAutoExclusive(true);
    btnWorkOrderPickup->setAutoExclusive(true);
    btnDirectPickup->setChecked(true);

    QString btnStyle = R"(
        QPushButton {
            background-color: transparent;
            border: none;
            color: #333;
            font-size: 14px;
            padding: 10px;
            text-align: left;
        }
        QPushButton:checked {
            background-color: #dbe8ff;
            border-left: 3px solid #0078d7;
            font-weight: bold;
            color: #0056b3;
        }
    )";

    btnDirectPickup->setStyleSheet(btnStyle);
    btnWorkOrderPickup->setStyleSheet(btnStyle);

    leftLayout->addWidget(title);
    leftLayout->addSpacing(20);
    leftLayout->addWidget(btnDirectPickup);
    leftLayout->addWidget(btnWorkOrderPickup);
    leftLayout->addStretch();

    // Stacked Pages
    stackedWidget = new QStackedWidget(this);
    directPickupPage = createPickupPage(false);
    workOrderPickupPage = createPickupPage(true);
    stackedWidget->addWidget(directPickupPage);
    stackedWidget->addWidget(workOrderPickupPage);

    contentLayout->addWidget(leftPanel);
    contentLayout->addWidget(stackedWidget);
    contentLayout->setStretch(1, 1);

    mainVLayout->addLayout(contentLayout);

    // Connect buttons
    connect(btnDirectPickup, &QPushButton::clicked, this, &StoreOut::showDirectPickup);
    connect(btnWorkOrderPickup, &QPushButton::clicked, this, &StoreOut::showWorkOrderPickup);

    // Connect search boxes for real-time filtering
    connect(searchDirect, &QLineEdit::textChanged, [=](){ filterTable(searchDirect, tableDirect); });
    connect(searchWorkOrder, &QLineEdit::textChanged, [=](){ filterTable(searchWorkOrder, tableWorkOrder); });

    // Connect download combo boxes
    connect(downloadComboDirect, QOverload<int>::of(&QComboBox::activated), this, [=](int index){
        if(index == 1) saveTableToCSV(tableDirect, "direct_pickup.csv");
    });
    connect(downloadComboWorkOrder, QOverload<int>::of(&QComboBox::activated), this, [=](int index){
        if(index == 1) saveTableToCSV(tableWorkOrder, "workorder_pickup.csv");
    });
}

StoreOut::~StoreOut() {}

// =============================
// Create Pickup Pages
// =============================
QWidget *StoreOut::createPickupPage(bool isWorkOrder)
{
    QWidget *page = new QWidget(this);
    QVBoxLayout *vbox = new QVBoxLayout(page);

    QHBoxLayout *topBar = new QHBoxLayout();
    QLineEdit *search = new QLineEdit();
    search->setPlaceholderText("Search");
    search->setFixedWidth(250);

    QComboBox *downloadCombo = new QComboBox();
    downloadCombo->addItem("Download");
    downloadCombo->addItem("Excel");
    downloadCombo->addItem("PDF");
    downloadCombo->setFixedWidth(120);

    topBar->addWidget(search);
    topBar->addStretch();
    topBar->addWidget(downloadCombo);

    QTableWidget *table = createTable(isWorkOrder);

    vbox->addLayout(topBar);
    vbox->addWidget(table);

    if(isWorkOrder){
        searchWorkOrder = search;
        downloadComboWorkOrder = downloadCombo;
        tableWorkOrder = table;
    } else {
        searchDirect = search;
        downloadComboDirect = downloadCombo;
        tableDirect = table;
    }

    return page;
}

// =============================
// Create Table with checkbox, image, buttons, and dummy data
// =============================
QTableWidget *StoreOut::createTable(bool isWorkOrder)
{
    QTableWidget *table = new QTableWidget();
    table->setColumnCount(isWorkOrder ? 8 : 7);

    QStringList headers;
    if(isWorkOrder)
        headers << "Select" << "Pickup ID" << "Work Order Number" << "Product Name"
                << "Total Parts" << "Picked Parts" << "Pending Parts" << "Actions";
    else
        headers << "Select" << "Pickup ID" << "Total Parts" << "Picked Parts"
                << "Pending Parts" << "Creation Date/Time" << "Actions";

    table->setHorizontalHeaderLabels(headers);
    table->horizontalHeader()->setStretchLastSection(true);
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    table->setStyleSheet(R"(
        QTableWidget { background-color: #ffffff; gridline-color: #d9d9d9; border: 1px solid #d9d9d9; font-size: 13px; }
        QHeaderView::section { background-color: #f0f0f0; border: 1px solid #d9d9d9; font-weight: bold; padding: 4px; }
        QTableWidget::item { border: 1px solid #d9d9d9; padding: 4px; }
        QTableWidget::item:selected { background-color: #cce4ff; color: #000; }
        QTableWidget::item:hover { background-color: #e6f2ff; }
    )");

    table->setAlternatingRowColors(true);
    table->setRowCount(0);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);

    // Dummy data
    int rows = 5;
    table->setRowCount(rows);

    for(int i=0; i<rows; ++i){
        // ✅ Checkbox with right-tick PNG
        QWidget *chkWidget = new QWidget();
        QCheckBox *chkBox = new QCheckBox();
        chkBox->setStyleSheet(R"(
            QCheckBox::indicator {
                width: 20px;
                height: 20px;
            }
            QCheckBox::indicator:unchecked {
                image: url(:/checkBox/unchecked.png); /* optional */
            }
            QCheckBox::indicator:checked {
                image: url(:/checkBox/right.png);
            }
        )");
        QHBoxLayout *chkLayout = new QHBoxLayout(chkWidget);
        chkLayout->addWidget(chkBox);
        chkLayout->setAlignment(Qt::AlignCenter);
        chkLayout->setContentsMargins(0,0,0,0);
        chkWidget->setLayout(chkLayout);
        table->setCellWidget(i,0,chkWidget);

        // Pickup ID
        table->setItem(i,1,new QTableWidgetItem(QString("PICK%1").arg(1000+i)));

        if(isWorkOrder){
            // Work Order Number
            table->setItem(i,2,new QTableWidgetItem(QString("WO%1").arg(2000+i)));

            // ✅ Product Name
            QStringList productNames = {"Resistor", "Capacitor", "Inductor", "Diode", "Transistor"};
            table->setItem(i,3,new QTableWidgetItem(productNames[i % productNames.size()]));

            table->setItem(i,4,new QTableWidgetItem(QString::number(10+i)));
            table->setItem(i,5,new QTableWidgetItem(QString::number(5+i)));
            table->setItem(i,6,new QTableWidgetItem(QString::number(5+i)));

            // Action buttons
            QWidget *actionWidget = new QWidget();
            QPushButton *btnPickup = new QPushButton("Pickup");
            QPushButton *btnCancel = new QPushButton("Cancel");
            QHBoxLayout *actionLayout = new QHBoxLayout(actionWidget);
            actionLayout->addWidget(btnPickup);
            actionLayout->addWidget(btnCancel);
            actionLayout->setContentsMargins(0,0,0,0);
            actionWidget->setLayout(actionLayout);
            table->setCellWidget(i,7,actionWidget);

            connect(btnPickup, &QPushButton::clicked, this, [=](){
                QMessageBox::information(this,"Pickup","Pickup initialized for "+table->item(i,1)->text());
            });
            connect(btnCancel, &QPushButton::clicked, this, [=](){
                QMessageBox::warning(this,"Pickup","Pickup canceled for "+table->item(i,1)->text());
            });

        } else {
            // Direct Pickup - Parts
            table->setItem(i,2,new QTableWidgetItem(QString::number(10+i)));
            table->setItem(i,3,new QTableWidgetItem(QString::number(5+i)));
            table->setItem(i,4,new QTableWidgetItem(QString::number(5+i)));

            // ✅ Creation Date/Time
            QLabel *dateLabel = new QLabel();
            QString dateTimeStr = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
            dateLabel->setText(dateTimeStr);
            dateLabel->setAlignment(Qt::AlignCenter);
            table->setCellWidget(i,5,dateLabel);

            // Action buttons
            QWidget *actionWidget = new QWidget();
            QPushButton *btnPickup = new QPushButton("Pickup");
            QPushButton *btnCancel = new QPushButton("Cancel");
            QHBoxLayout *actionLayout = new QHBoxLayout(actionWidget);
            actionLayout->addWidget(btnPickup);
            actionLayout->addWidget(btnCancel);
            actionLayout->setContentsMargins(0,0,0,0);
            actionWidget->setLayout(actionLayout);
            table->setCellWidget(i,6,actionWidget);

            connect(btnPickup, &QPushButton::clicked, this, [=](){
                QMessageBox::information(this,"Pickup","Pickup initialized for "+table->item(i,1)->text());
            });
            connect(btnCancel, &QPushButton::clicked, this, [=](){
                QMessageBox::warning(this,"Pickup","Pickup canceled for "+table->item(i,1)->text());
            });
        }
    }

    return table;
}

// =============================
// Switch Pages
// =============================
void StoreOut::showDirectPickup(){ stackedWidget->setCurrentIndex(0); }
void StoreOut::showWorkOrderPickup(){ stackedWidget->setCurrentIndex(1); }

// =============================
// Real-time search filter
// =============================
void StoreOut::filterTable(QLineEdit *searchEdit, QTableWidget *table)
{
    QString text = searchEdit->text().toLower();
    for(int i=0;i<table->rowCount();i++){
        bool match = false;
        for(int j=0;j<table->columnCount();j++){
            QTableWidgetItem *item = table->item(i,j);
            if(item && item->text().toLower().contains(text)){
                match = true;
                break;
            }
        }
        table->setRowHidden(i,!match);
    }
}

// =============================
// Save table to CSV
// =============================
void StoreOut::saveTableToCSV(QTableWidget *table, const QString &filename)
{
    QFile file(filename);
    if(file.open(QFile::WriteOnly | QFile::Text)){
        QTextStream out(&file);

        // headers
        QStringList headers;
        for(int i=0;i<table->columnCount();i++){
            headers << table->horizontalHeaderItem(i)->text();
        }
        out << headers.join(",") << "\n";

        // rows
        for(int i=0;i<table->rowCount();i++){
            QStringList rowData;
            for(int j=0;j<table->columnCount();j++){
                QTableWidgetItem *item = table->item(i,j);
                if(item) rowData << item->text();
                else rowData << "";
            }
            out << rowData.join(",") << "\n";
        }
        file.close();
        QMessageBox::information(this,"Export","Table saved as "+filename);
    }
}


