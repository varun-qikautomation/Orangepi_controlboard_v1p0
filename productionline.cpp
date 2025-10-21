#include "productionline.h"
#include "addproductiondialog.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QMessageBox>
#include <QDate>
#include <QDebug>

ProductionLine::ProductionLine(QWidget *parent)
    : QWidget(parent)
{
    setupUI();
    populateDummyData();
    populateTable();
}

void ProductionLine::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // 🔹 Top Bar: Search + Add + Download
    QHBoxLayout *topBar = new QHBoxLayout();

    searchEdit = new QLineEdit();
    searchEdit->setPlaceholderText("Search");
    searchEdit->setFixedWidth(250);

    addButton = new QPushButton("ADD");
    addButton->setStyleSheet("background-color: #1976D2; color: white; font-weight: bold; padding: 6px 16px;");

    downloadCombo = new QComboBox();
    downloadCombo->addItem("Download");
    downloadCombo->addItem("Excel");
    downloadCombo->addItem("PDF");
    downloadCombo->setFixedWidth(120);

    topBar->addWidget(searchEdit);
    topBar->addStretch();
    topBar->addWidget(addButton);
    topBar->addWidget(downloadCombo);

    // 🔹 Table
    table = new QTableWidget();
    table->setColumnCount(5);
    QStringList headers = {"#", "Production Line", "Description", "Created On", "Action"};
    table->setHorizontalHeaderLabels(headers);
    table->horizontalHeader()->setStretchLastSection(true);
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table->setStyleSheet("QHeaderView::section { background-color: #f0f0f0; font-weight: bold; }");

    // 🔹 Footer
    footerLabel = new QLabel("Rows per page: 10   1–3 of 3");
    footerLabel->setStyleSheet("color: gray;");

    QHBoxLayout *footerLayout = new QHBoxLayout();
    footerLayout->addStretch();
    footerLayout->addWidget(footerLabel);

    mainLayout->addLayout(topBar);
    mainLayout->addWidget(table);
    mainLayout->addLayout(footerLayout);

    connect(addButton, &QPushButton::clicked, this, &ProductionLine::onAddClicked);
    connect(downloadCombo, &QComboBox::currentTextChanged, this, &ProductionLine::onDownloadClicked);
}

void ProductionLine::populateDummyData()
{
    ProductionItem a{1, "Line A", "Main assembly line", "2025-01-15"};
    ProductionItem b{2, "Line B", "Secondary production line", "2025-02-20"};
    ProductionItem c{3, "Line C", "Testing line", "2025-03-05"};

    productionList = {a, b, c};
}

void ProductionLine::populateTable()
{
    table->clearContents();

    if (productionList.isEmpty()) {
        table->setRowCount(1);
        QTableWidgetItem *noData = new QTableWidgetItem("No Data Found");
        noData->setTextAlignment(Qt::AlignCenter);
        table->setItem(0, 0, noData);
        table->setSpan(0, 0, 1, 5);
        return;
    }

    table->setRowCount(productionList.size());

    for (int i = 0; i < productionList.size(); ++i) {
        const ProductionItem &item = productionList[i];

        table->setItem(i, 0, new QTableWidgetItem(QString::number(item.id)));
        table->setItem(i, 1, new QTableWidgetItem(item.name));
        table->setItem(i, 2, new QTableWidgetItem(item.description));
        table->setItem(i, 3, new QTableWidgetItem(item.createdOn));

        QPushButton *editBtn = new QPushButton("Edit");
        editBtn->setStyleSheet("background-color: #4CAF50; color: white; padding: 4px 10px; border-radius: 4px;");
        table->setCellWidget(i, 4, editBtn);
    }
}

void ProductionLine::onAddClicked()
{
    AddProductionDialog dlg(this);

    connect(&dlg, &AddProductionDialog::productionAdded,
            this, [=](const ProductionItem &itm) {
                productionList.append(itm);
                populateTable();
            });

    dlg.exec();
}

void ProductionLine::onDownloadClicked(const QString &format)
{
    if (format == "Excel") {
        QMessageBox::information(this, "Download", "Downloading as Excel...");
    } else if (format == "PDF") {
        QMessageBox::information(this, "Download", "Downloading as PDF...");
    }
    downloadCombo->setCurrentIndex(0);
}
