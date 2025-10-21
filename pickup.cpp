#include "pickup.h"
#include "ui_pickup.h"
#include <QLabel>
#include <QTabWidget>
#include <QTableWidget>
#include <QHeaderView>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QSpacerItem>
#include <QDate>
#include <QCheckBox>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QRandomGenerator>
#include <algorithm>
#include <memory>
#include <QFileDialog>
#include <QFileInfo>


struct PickupItem {
    int id;
    QString workOrder;
    QString productName;
    int panelCount;
    int childPanelCount;
    int itemsToProduce;
    QString description;
    QDate expiryDate;
    bool isChecked = false;
};

pickup::pickup(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::pickup)
{
    ui->setupUi(this);

    // ===== NAVBAR =====
    QWidget *navbar = new QWidget(this);
    navbar->setFixedHeight(70);
    navbar->setStyleSheet("background-color: #FFFFFF; border-bottom: 1px solid #ccc;");
    QHBoxLayout *navLayout = new QHBoxLayout(navbar);
    navLayout->setContentsMargins(20,0,20,0);
    QLabel *logoLabel = new QLabel;
    QPixmap logoPixmap(":/logo/logoA.png");
    logoLabel->setPixmap(logoPixmap.scaled(250,250,Qt::KeepAspectRatio,Qt::SmoothTransformation));
    navLayout->addWidget(logoLabel);
    ui->mainLayout->insertWidget(0,navbar);
    ui->mainLayout->setContentsMargins(0,0,0,0);
    ui->mainLayout->setSpacing(5);

    // ===== MAIN CONTENT =====
    QWidget *contentWidget = new QWidget(this);
    QVBoxLayout *contentLayout = new QVBoxLayout(contentWidget);
    contentLayout->setContentsMargins(15,15,15,15);

    // ===== TAB WIDGET =====
    QTabWidget *tabWidget = new QTabWidget(this);
    tabWidget->setStyleSheet(
        "QTabWidget::pane { border: 1px solid #ccc; background: #fff; }"
        "QTabBar::tab { background: #f5f5f5; padding: 10px 20px; }"
        "QTabBar::tab:selected { background: white; color: #0078d7; border-bottom: 2px solid #0078d7; }"
        );

    QString tableStyle = R"(
    QTableWidget {
        border: 1px solid #ccc;
        alternate-background-color: #f9f9f9;
        background-color: white;
        selection-background-color: transparent; /* No blue selection */
        selection-color: black; /* Keep text readable */
    }
    QTableView::item:hover {
        background: transparent; /* No hover color */
    }
    QTableView::item:selected {
        background: transparent; /* No blue select color */
        color: black;
    }
    QHeaderView::section {
        background-color: #f2f2f2;
        font-weight: bold;
        border: 1px solid #ccc;
    }
)";


    // ==============================================================
    // ===================== DIRECT PICKUP TAB ======================
    // ==============================================================

    QWidget *directPickupTab = new QWidget();
    QVBoxLayout *directLayout = new QVBoxLayout(directPickupTab);

    QHBoxLayout *directHeader = new QHBoxLayout();
    QLineEdit *directSearch = new QLineEdit();
    directSearch->setPlaceholderText("Search Work Order...");
    directSearch->setFixedWidth(250);

    QPushButton *allBtn = new QPushButton("All");
    QPushButton *validBtn = new QPushButton("Valid");
    QPushButton *expiredBtn = new QPushButton("Expired");
    QPushButton *scannerBtn = new QPushButton("Scanner");
    QPushButton *addPickupBtn = new QPushButton("ADD PICKUP");
    QPushButton *downloadBtn = new QPushButton("Download");

    QList<QPushButton*> buttons = {allBtn, validBtn, expiredBtn, scannerBtn, downloadBtn};
    for (auto *btn : buttons)
        btn->setStyleSheet("background-color:#f5f5f5;border:1px solid #ccc;border-radius:5px;padding:6px 12px;");
    addPickupBtn->setStyleSheet("background-color:#0078d7;color:white;border-radius:5px;padding:6px 12px;");

    QSpacerItem *directSpacer = new QSpacerItem(20,20,QSizePolicy::Expanding,QSizePolicy::Minimum);
    directHeader->addWidget(directSearch);
    directHeader->addSpacing(15);
    directHeader->addWidget(allBtn);
    directHeader->addWidget(validBtn);
    directHeader->addWidget(expiredBtn);
    directHeader->addWidget(scannerBtn);
    directHeader->addItem(directSpacer);
    directHeader->addWidget(addPickupBtn);
    directHeader->addWidget(downloadBtn);

    QTableWidget *directTable = new QTableWidget();
    directTable->setColumnCount(7);
    directTable->setHorizontalHeaderLabels({"#","Work Order","Product","Panel","Child Panel","Items","Status"});
    directTable->horizontalHeader()->setStretchLastSection(true);
    directTable->setAlternatingRowColors(true);
    directTable->setStyleSheet(tableStyle);

    // Shared data
    auto directData = std::make_shared<QVector<PickupItem>>(QVector<PickupItem>{
        {1,"WO1001","Product A",10,2,50,"",QDate::currentDate().addDays(5)},
        {2,"WO1002","Product B",5,1,20,"",QDate::currentDate().addDays(-2)},
        {3,"WO1003","Product C",8,2,30,"",QDate::currentDate().addDays(3)}
    });
    auto currentDirectFilter = std::make_shared<QString>("All");

    auto populateDirectTable = [directTable, directData, currentDirectFilter](const QString &filter, const QString &searchText = "") {
        *currentDirectFilter = filter;
        QString trimmedSearch = searchText.trimmed();

        QVector<PickupItem> sorted = *directData;
        std::sort(sorted.begin(), sorted.end(), [](const PickupItem &a, const PickupItem &b){
            return a.expiryDate < b.expiryDate;
        });

        directTable->blockSignals(true);
        directTable->clearContents();
        directTable->setRowCount(0);

        int row = 0;
        for (const auto &item : sorted) {
            bool filterMatch =
                (filter == "All") ||
                (filter == "Valid" && item.expiryDate >= QDate::currentDate()) ||
                (filter == "Expired" && item.expiryDate < QDate::currentDate());
            bool searchMatch = trimmedSearch.isEmpty() ||
                               item.workOrder.contains(trimmedSearch, Qt::CaseInsensitive) ||
                               item.productName.contains(trimmedSearch, Qt::CaseInsensitive);
            if (!filterMatch || !searchMatch) continue;

            directTable->insertRow(row);
            auto *check = new QTableWidgetItem();
            check->setCheckState(item.isChecked ? Qt::Checked : Qt::Unchecked);
            check->setData(Qt::UserRole, item.id);
            directTable->setItem(row,0,check);
            directTable->setItem(row,1,new QTableWidgetItem(item.workOrder));
            directTable->setItem(row,2,new QTableWidgetItem(item.productName));
            directTable->setItem(row,3,new QTableWidgetItem(QString::number(item.panelCount)));
            directTable->setItem(row,4,new QTableWidgetItem(QString::number(item.childPanelCount)));
            directTable->setItem(row,5,new QTableWidgetItem(QString::number(item.itemsToProduce)));

            QString status = (item.expiryDate >= QDate::currentDate()) ? "Valid" : "Expired";
            auto *statusItem = new QTableWidgetItem(status);
            directTable->setItem(row,6,statusItem);

            QColor bg = (status=="Expired") ? QColor(255,200,200) : Qt::white;
            for (int c=0; c<directTable->columnCount(); ++c)
                directTable->item(row,c)->setBackground(bg);

            row++;
        }

        if (row == 0) {
            directTable->setRowCount(1);
            directTable->setSpan(0,0,1,directTable->columnCount());
            auto *noData = new QTableWidgetItem("No Data Found");
            noData->setTextAlignment(Qt::AlignCenter);
            directTable->setItem(0,0,noData);
        }

        directTable->blockSignals(false);
    };

    populateDirectTable("All");

    connect(directSearch, &QLineEdit::textChanged, this, [=](){
        populateDirectTable(*currentDirectFilter, directSearch->text());
    });
    connect(allBtn, &QPushButton::clicked, this, [=](){
        populateDirectTable("All", directSearch->text());
    });
    connect(validBtn, &QPushButton::clicked, this, [=](){
        populateDirectTable("Valid", directSearch->text());
    });
    connect(expiredBtn, &QPushButton::clicked, this, [=](){
        populateDirectTable("Expired", directSearch->text());
    });
    connect(scannerBtn, &QPushButton::clicked, this, [=](){
        int newId = directData->size()+1;
        QDate expiry = QDate::currentDate().addDays(QRandomGenerator::global()->bounded(-3,10));
        directData->append({newId, QString("WO%1").arg(1000+newId),
                            QString("Product %1").arg(QChar('A'+newId)), 5, 1, 20, "", expiry, false});
        populateDirectTable(*currentDirectFilter, directSearch->text());
    });
    connect(addPickupBtn, &QPushButton::clicked, this, [=](){
        bool anyChecked = false;
        for (int r=0; r<directTable->rowCount(); ++r) {
            if (directTable->item(r,0) && directTable->item(r,0)->checkState() == Qt::Checked)
                anyChecked = true;
        }
        if (anyChecked)
            QMessageBox::information(nullptr,"Pickup","Pickup created successfully!");
        else
            QMessageBox::warning(nullptr,"Pickup","No rows selected!");
    });
    connect(downloadBtn, &QPushButton::clicked, this, [=]() {
        QString fileName = QFileDialog::getSaveFileName(nullptr,
                                                        "Save Direct Pickup Data",
                                                        "direct_pickup.csv",
                                                        "CSV Files (*.csv)");
        if (fileName.isEmpty())
            return;

        QFile file(fileName);
        if (file.open(QIODevice::WriteOnly)) {
            QTextStream out(&file);
            QStringList headers;
            for (int i = 0; i < directTable->columnCount(); i++)
                headers << directTable->horizontalHeaderItem(i)->text();
            out << headers.join(",") << "\n";

            for (int r = 0; r < directTable->rowCount(); r++) {
                if (directTable->item(r, 0) && directTable->item(r, 0)->text() == "No Data Found")
                    continue;
                QStringList rowData;
                for (int c = 0; c < directTable->columnCount(); c++) {
                    QTableWidgetItem *item = directTable->item(r, c);
                    rowData << (item ? item->text() : "");
                }
                out << rowData.join(",") << "\n";
            }

            file.close();
            QMessageBox::information(nullptr, "Download", "Data saved to " + fileName);
        }
    });


    directLayout->addLayout(directHeader);
    directLayout->addWidget(directTable);
    tabWidget->addTab(directPickupTab,"Direct Pickup");

    // ==============================================================
    // =============== WORK ORDER PICKUP TAB ========================
    // ==============================================================

    QWidget *workOrderTab = new QWidget();
    QVBoxLayout *workLayout = new QVBoxLayout(workOrderTab);

    QHBoxLayout *workHeader = new QHBoxLayout();
    QLineEdit *workSearch = new QLineEdit();
    workSearch->setPlaceholderText("Search Work Order...");
    workSearch->setFixedWidth(250);

    // 🔹  Add Work Order
    QPushButton *addWorkOrderBtn = new QPushButton("Add Work Order");
    QPushButton *downloadWorkBtn = new QPushButton("Download");
    addWorkOrderBtn->setStyleSheet("background-color:#0078d7;color:white;border-radius:5px;padding:6px 12px;");
    downloadWorkBtn->setStyleSheet("background-color:#f5f5f5;border:1px solid #ccc;border-radius:5px;padding:6px 12px;");

    QSpacerItem *workSpacer = new QSpacerItem(20,20,QSizePolicy::Expanding,QSizePolicy::Minimum);
    workHeader->addWidget(workSearch);
    workHeader->addSpacing(15);
    workHeader->addWidget(addWorkOrderBtn);
    workHeader->addItem(workSpacer);
    workHeader->addWidget(downloadWorkBtn);

    QTableWidget *workTable = new QTableWidget();
    workTable->setColumnCount(6);
    workTable->setHorizontalHeaderLabels({"Work Order","Product","Panel","Child Panel","Items","Status"});
    workTable->horizontalHeader()->setStretchLastSection(true);
    workTable->setAlternatingRowColors(true);
    workTable->setStyleSheet(tableStyle);

    auto workData = std::make_shared<QVector<PickupItem>>(QVector<PickupItem>{
        {1,"WO2001","Widget A",4,2,40,"",QDate::currentDate().addDays(2)},
        {2,"WO2002","Widget B",7,1,60,"",QDate::currentDate().addDays(-1)},
        {3,"WO2003","Widget C",3,3,25,"",QDate::currentDate().addDays(6)}
    });

    auto populateWorkTable = [workTable, workData](const QString &searchText = "") {
        QString trimmed = searchText.trimmed();
        QVector<PickupItem> sorted = *workData;
        std::sort(sorted.begin(), sorted.end(), [](const PickupItem &a, const PickupItem &b){
            return a.expiryDate < b.expiryDate;
        });

        workTable->blockSignals(true);
        workTable->clearContents();
        workTable->setRowCount(0);
        int row = 0;

        for (const auto &item : sorted) {
            if (!trimmed.isEmpty() && !item.workOrder.contains(trimmed, Qt::CaseInsensitive)
                && !item.productName.contains(trimmed, Qt::CaseInsensitive))
                continue;

            workTable->insertRow(row);
            workTable->setItem(row,0,new QTableWidgetItem(item.workOrder));
            workTable->setItem(row,1,new QTableWidgetItem(item.productName));
            workTable->setItem(row,2,new QTableWidgetItem(QString::number(item.panelCount)));
            workTable->setItem(row,3,new QTableWidgetItem(QString::number(item.childPanelCount)));
            workTable->setItem(row,4,new QTableWidgetItem(QString::number(item.itemsToProduce)));

            QString status = (item.expiryDate >= QDate::currentDate()) ? "Valid" : "Expired";
            auto *statusItem = new QTableWidgetItem(status);
            workTable->setItem(row,5,statusItem);

            QColor bg = (status=="Expired") ? QColor(255,220,220) : Qt::white;
            for (int c=0; c<workTable->columnCount(); ++c)
                workTable->item(row,c)->setBackground(bg);

            row++;
        }

        if (row == 0) {
            workTable->setRowCount(1);
            workTable->setSpan(0,0,1,workTable->columnCount());
            auto *noData = new QTableWidgetItem("No Data Found");
            noData->setTextAlignment(Qt::AlignCenter);
            workTable->setItem(0,0,noData);
        }
        workTable->blockSignals(false);
    };

    populateWorkTable();

    connect(workSearch, &QLineEdit::textChanged, this, [=](){
        populateWorkTable(workSearch->text());
    });

    // New Add Work Order button logic
    connect(addWorkOrderBtn, &QPushButton::clicked, this, [=](){
        QString fileName = QFileDialog::getOpenFileName(nullptr,
                                                        "Upload Work Order File",
                                                        "",
                                                        "CSV Files (*.csv);;Excel Files (*.xlsx *.xls);;Text Files (*.txt);;All Files (*.*)");

        if (!fileName.isEmpty()) {
            QMessageBox::information(nullptr, "File Uploaded",
                                     QString("File '%1' uploaded successfully!").arg(QFileInfo(fileName).fileName()));
            // Optional: Later, you can parse this file to update the table
        }
    });

    // connect(downloadWorkBtn, &QPushButton::clicked, this, [=](){
    //     QFile file("work_order_pickup.csv");
    //     if (file.open(QIODevice::WriteOnly)) {
    //         QTextStream out(&file);
    //         QStringList headers;
    //         for (int i=0;i<workTable->columnCount();i++)
    //             headers << workTable->horizontalHeaderItem(i)->text();
    //         out << headers.join(",") << "\n";
    //         for (int r=0; r<workTable->rowCount(); r++) {
    //             if (workTable->item(r,0) && workTable->item(r,0)->text()=="No Data Found") continue;
    //             QStringList rowData;
    //             for (int c=0;c<workTable->columnCount();c++)
    //                 rowData << workTable->item(r,c)->text();
    //             out << rowData.join(",") << "\n";
    //         }
    //         file.close();
    //         QMessageBox::information(nullptr,"Download","Data saved to work_order_pickup.csv");
    //     }
    // });

    connect(downloadWorkBtn, &QPushButton::clicked, this, [=]() {
        QString fileName = QFileDialog::getSaveFileName(nullptr,
                                                        "Save Work Order Pickup Data",
                                                        "work_order_pickup.csv",
                                                        "CSV Files (*.csv)");
        if (fileName.isEmpty())
            return;

        QFile file(fileName);
        if (file.open(QIODevice::WriteOnly)) {
            QTextStream out(&file);
            QStringList headers;
            for (int i = 0; i < workTable->columnCount(); i++)
                headers << workTable->horizontalHeaderItem(i)->text();
            out << headers.join(",") << "\n";

            for (int r = 0; r < workTable->rowCount(); r++) {
                if (workTable->item(r, 0) && workTable->item(r, 0)->text() == "No Data Found")
                    continue;
                QStringList rowData;
                for (int c = 0; c < workTable->columnCount(); c++) {
                    QTableWidgetItem *item = workTable->item(r, c);
                    rowData << (item ? item->text() : "");
                }
                out << rowData.join(",") << "\n";
            }

            file.close();
            QMessageBox::information(nullptr, "Download", "Data saved to " + fileName);
        }
    });


    workLayout->addLayout(workHeader);
    workLayout->addWidget(workTable);
    tabWidget->addTab(workOrderTab,"Work Order Pickup");


    // ==============================================================
    // Final layout setup
    // ==============================================================
    contentLayout->addWidget(tabWidget);
    ui->mainLayout->addWidget(contentWidget);
}

pickup::~pickup()
{
    delete ui;
}



