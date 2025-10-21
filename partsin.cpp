
#include "partsin.h"
#include <QHBoxLayout>
#include <QGridLayout>
#include <QHeaderView>
#include <QDate>
#include <QLabel>
#include <QPixmap>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QTableWidget>
#include <QMessageBox>
#include <QRandomGenerator>
#include <QSqlQuery>
#include <QSqlDatabase>

PartsIn::PartsIn(QWidget *parent) : QWidget(parent)
{
    // ---------- NAVBAR ----------
    QWidget *navbar = new QWidget(this);
    navbar->setFixedHeight(70);
    navbar->setStyleSheet("background-color: #FFFFFF;");

    QHBoxLayout *navLayout = new QHBoxLayout(navbar);
    navLayout->setContentsMargins(20, 0, 20, 0);
    navLayout->setSpacing(25);

    QLabel *logoLabel = new QLabel;
    QPixmap logoPixmap(":/logo/logoA.png");
    logoLabel->setPixmap(logoPixmap.scaled(250, 50, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    navLayout->addWidget(logoLabel);
    navLayout->addStretch();

    // ---------- SCAN SECTION ----------
    scanDataEdit = new QLineEdit();
    scanDataEdit->setPlaceholderText("Scan Data Matrix/ECIA Standards");

    clearScanBtn = new QPushButton("CLEAR");
    generateStickersBtn = new QPushButton("Generate Multiple Stickers");

    QHBoxLayout *scanLayout = new QHBoxLayout();
    scanLayout->addWidget(scanDataEdit);
    scanLayout->addWidget(clearScanBtn);
    scanLayout->addWidget(generateStickersBtn);

    // ---------- MANUAL ENTRY ----------
    QLabel *manualLabel = new QLabel("Manual Entry");
    manualLabel->setStyleSheet("font-weight: bold; font-size: 14px;");

    invoiceEdit = new QLineEdit();
    invoiceEdit->setPlaceholderText("Invoice Number");

    grnDateEdit = new QDateEdit(QDate::currentDate());
    grnDateEdit->setDisplayFormat("MM/dd/yyyy");
    grnDateEdit->setCalendarPopup(true);

    partNumberEdit = new QLineEdit();
    partNumberEdit->setPlaceholderText("Part Number *");

    quantityEdit = new QLineEdit();
    quantityEdit->setPlaceholderText("Quantity *");

    lotNumberEdit = new QLineEdit();
    lotNumberEdit->setPlaceholderText("Lot Number *");

    manufactureDateEdit = new QDateEdit(QDate::currentDate());
    manufactureDateEdit->setDisplayFormat("MM/dd/yyyy");
    manufactureDateEdit->setCalendarPopup(true);

    manufacturerEdit = new QLineEdit();
    manufacturerEdit->setPlaceholderText("Manufacturer");

    descriptionEdit = new QLineEdit();
    descriptionEdit->setPlaceholderText("Part Description");

    expiryEdit = new QLineEdit();
    expiryEdit->setPlaceholderText("Expiry Date");

    locationEdit = new QLineEdit();
    locationEdit->setPlaceholderText("Part Location");

    internalPartEdit = new QLineEdit();
    internalPartEdit->setPlaceholderText("Internal Part Number");
    internalPartEdit->setReadOnly(true); // Auto-generated

    // ---------- GRID LAYOUT FOR FORM ----------
    QGridLayout *formGrid = new QGridLayout();
    formGrid->addWidget(invoiceEdit, 0, 0);
    formGrid->addWidget(grnDateEdit, 0, 1);
    formGrid->addWidget(partNumberEdit, 1, 0);
    formGrid->addWidget(quantityEdit, 1, 1);
    formGrid->addWidget(lotNumberEdit, 2, 0);
    formGrid->addWidget(manufactureDateEdit, 2, 1);
    formGrid->addWidget(manufacturerEdit, 3, 0);
    formGrid->addWidget(descriptionEdit, 3, 1);
    formGrid->addWidget(expiryEdit, 4, 0);
    formGrid->addWidget(locationEdit, 4, 1);
    formGrid->addWidget(internalPartEdit, 5, 0, 1, 2);

    // ---------- BUTTONS ----------
    submitBtn = new QPushButton("SUBMIT");
    clearBtn = new QPushButton("CLEAR");

    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnLayout->addStretch();
    btnLayout->addWidget(submitBtn);
    btnLayout->addWidget(clearBtn);

    // ---------- SEARCH & TABLE ----------
    searchEdit = new QLineEdit();
    searchEdit->setPlaceholderText("Search");

    table = new QTableWidget();
    table->setColumnCount(9);
    QStringList headers = {
        "#", "Unique Id", "Internal Part Number", "Part Number",
        "Quantity", "Receipt Number", "Manufacturer", "GRN Date",
        "Manufacture Date"
    };
    table->setHorizontalHeaderLabels(headers);
    table->horizontalHeader()->setStretchLastSection(true);
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table->setRowCount(0);

    // ---------- MAIN LAYOUT ----------
    mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(navbar);
    mainLayout->addLayout(scanLayout);
    mainLayout->addSpacing(10);
    mainLayout->addWidget(manualLabel);
    mainLayout->addLayout(formGrid);
    mainLayout->addLayout(btnLayout);
    mainLayout->addSpacing(10);
    mainLayout->addWidget(searchEdit);
    mainLayout->addWidget(table);
    mainLayout->addStretch();

    setLayout(mainLayout);

    // ---------- BASIC STYLE ----------
    setStyleSheet(R"(
        QLineEdit {
            border: 1px solid #ccc;
            border-radius: 4px;
            padding: 4px;
        }
        QPushButton {
            background-color: #1976d2;
            color: white;
            padding: 6px 12px;
            border: none;
            border-radius: 4px;
        }
        QPushButton:hover {
            background-color: #1565c0;
        }
        QPushButton#clearScanBtn, QPushButton#clearBtn {
            background-color: #f44336;
        }
        QPushButton#clearScanBtn:hover, QPushButton#clearBtn:hover {
            background-color: #d32f2f;
        }
    )");

    // ---------- DUMMY DATA ----------
    loadStocksFromDB();

    // ---------- BUTTON CONNECTIONS ----------
    connect(submitBtn, &QPushButton::clicked, this, &PartsIn::addManualEntry);
    connect(clearBtn, &QPushButton::clicked, this, &PartsIn::clearForm);
}

// ---------------- HELPER FUNCTIONS ----------------

// Add some initial dummy data
void PartsIn::addDummyData()
{
    for(int i = 0; i < 5; ++i) {
        int row = table->rowCount();
        table->insertRow(row);
        table->setItem(row, 0, new QTableWidgetItem(QString::number(row+1)));
        table->setItem(row, 1, new QTableWidgetItem(QString("UID%1").arg(1000+i)));
        table->setItem(row, 2, new QTableWidgetItem(QString("INT%1").arg(5000+i)));
        table->setItem(row, 3, new QTableWidgetItem(QString("PN%1").arg(200+i)));
        table->setItem(row, 4, new QTableWidgetItem(QString::number(10+i)));
        table->setItem(row, 5, new QTableWidgetItem(QString("INV%1").arg(300+i)));
        table->setItem(row, 6, new QTableWidgetItem(QString("Manufacturer %1").arg(i+1)));
        table->setItem(row, 7, new QTableWidgetItem(QDate::currentDate().toString("MM/dd/yyyy")));
        table->setItem(row, 8, new QTableWidgetItem(QDate::currentDate().toString("MM/dd/yyyy")));
    }
}

void PartsIn::loadStocksFromDB()
{
    qDebug() << "loadStocksFromDB";

    table->setRowCount(0); // Clear existing rows

    // Correct query: select only existing columns
    QSqlQuery query(
        "SELECT id, uniqueId, internalPN, partNumber, quantity, invoiceNumber, "
        "manufacturer, manufactureDate, lotNumber, partLocation FROM stocks"
        );

    int row = 0;
    while(query.next()) {
        table->insertRow(row);

        // Set table items
        table->setItem(row, 0, new QTableWidgetItem(QString::number(row + 1))); // Row number
        table->setItem(row, 1, new QTableWidgetItem(query.value("uniqueId").toString()));
        table->setItem(row, 2, new QTableWidgetItem(query.value("internalPN").toString()));
        table->setItem(row, 3, new QTableWidgetItem(query.value("partNumber").toString()));
        table->setItem(row, 4, new QTableWidgetItem(query.value("quantity").toString()));
        table->setItem(row, 5, new QTableWidgetItem(query.value("invoiceNumber").toString()));
        table->setItem(row, 6, new QTableWidgetItem(query.value("manufacturer").toString()));
        table->setItem(row, 7, new QTableWidgetItem(query.value("manufactureDate").toString()));
        table->setItem(row, 8, new QTableWidgetItem(query.value("lotNumber").toString()));
        table->setItem(row, 9, new QTableWidgetItem(query.value("partLocation").toString()));

        row++;
    }

    qDebug() << "Loaded rows:" << row;
}



void PartsIn::addManualEntry()
{
    qDebug() << "addManualEntry";

    // Check mandatory fields
    if(partNumberEdit->text().isEmpty() || quantityEdit->text().isEmpty() || lotNumberEdit->text().isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please fill all mandatory fields (*)");
        return;
    }

    int row = table->rowCount();
    table->insertRow(row);

    // Auto-generate internal part number
    QString internalPart = QString("INT%1").arg(QRandomGenerator::global()->bounded(1000, 9999));
    internalPartEdit->setText(internalPart);

    // Generate a uniqueId
    QString uniqueId = QString("UID%1").arg(1000 + row);

    // Insert into database
    QSqlQuery query;
    query.prepare(R"(
        INSERT OR IGNORE INTO stocks (
            uniqueId, internalPN, partNumber, quantity, invoiceNumber,
            manufacturer, manufactureDate, lotNumber, partLocation, createdAt
        ) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, datetime('now','localtime'))
    )");

    query.addBindValue(uniqueId);
    query.addBindValue(internalPart);
    query.addBindValue(partNumberEdit->text());
    query.addBindValue(quantityEdit->text().toInt());
    query.addBindValue(invoiceEdit->text());
    query.addBindValue(manufacturerEdit->text());
    query.addBindValue(manufactureDateEdit->date().toString("yyyy-MM-dd"));
    query.addBindValue(lotNumberEdit->text());
    query.addBindValue(locationEdit->text());

    if(!query.exec()) {
        qDebug() << "Database insert failed:" ;
        QMessageBox::critical(this, "Database Error", "Failed to insert entry into database.");
        return;
    }

    // Insert into table widget
    table->setItem(row, 0, new QTableWidgetItem(QString::number(row+1)));
    table->setItem(row, 1, new QTableWidgetItem(uniqueId));
    table->setItem(row, 2, new QTableWidgetItem(internalPart));
    table->setItem(row, 3, new QTableWidgetItem(partNumberEdit->text()));
    table->setItem(row, 4, new QTableWidgetItem(quantityEdit->text()));
    table->setItem(row, 5, new QTableWidgetItem(invoiceEdit->text()));
    table->setItem(row, 6, new QTableWidgetItem(manufacturerEdit->text()));
    table->setItem(row, 7, new QTableWidgetItem(grnDateEdit->date().toString("MM/dd/yyyy")));
    table->setItem(row, 8, new QTableWidgetItem(manufactureDateEdit->date().toString("MM/dd/yyyy")));
    table->setItem(row, 9, new QTableWidgetItem(lotNumberEdit->text()));
    table->setItem(row, 10, new QTableWidgetItem(locationEdit->text()));

    clearForm();
}

// Clear the form fields
void PartsIn::clearForm()
{
    invoiceEdit->clear();
    partNumberEdit->clear();
    quantityEdit->clear();
    lotNumberEdit->clear();
    manufacturerEdit->clear();
    descriptionEdit->clear();
    expiryEdit->clear();
    locationEdit->clear();
    internalPartEdit->clear();
    grnDateEdit->setDate(QDate::currentDate());
    manufactureDateEdit->setDate(QDate::currentDate());
}

