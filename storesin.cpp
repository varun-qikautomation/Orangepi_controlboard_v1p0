// #include "storesin.h"
// #include <QGroupBox>
// #include <QHeaderView>
// #include <QScrollArea>
// #include <QFrame>
// #include <QDebug>
// #include <QSpacerItem>
// #include <QVBoxLayout>
// #include <QHBoxLayout>
// #include <QLabel>
// #include <QLineEdit>
// #include <QPushButton>
// #include <QTableWidget>
// #include <QMessageBox>


// StoresIn::StoresIn(QWidget *parent) : QWidget(parent)
// {
//     setupUI();

//     connect(inButton, &QPushButton::clicked, this, &StoresIn::handleInButton);
//     connect(clearButton, &QPushButton::clicked, this, &StoresIn::handleClearButton);
//     connect(nextButton, &QPushButton::clicked, this, &StoresIn::nextPage);
//     connect(prevButton, &QPushButton::clicked, this, &StoresIn::prevPage);
// }

// void StoresIn::setupUI()
// {
//     // ---- Title ----
//     titleLabel = new QLabel("Stores In");
//     titleLabel->setStyleSheet("font-size: 20px; font-weight: bold;");

//     // ---- Scan Section ----
//     scanInput = new QLineEdit();
//     scanInput->setPlaceholderText("Scan Slot / Bin / Part");
//     scanInput->setMinimumHeight(35);
//     scanInput->setStyleSheet("QLineEdit { border: 1px solid #0078D7; border-radius: 5px; padding: 6px; }");

//     inButton = new QPushButton("IN");
//     clearButton = new QPushButton("CLEAR");

//     QString btnStyle = R"(
//         QPushButton {
//             background-color: #0078D7;
//             color: white;
//             font-weight: bold;
//             border-radius: 5px;
//             padding: 8px 16px;
//         }
//         QPushButton:hover {
//             background-color: #005fa3;
//         }
//     )";

//     inButton->setStyleSheet(btnStyle);
//     clearButton->setStyleSheet(btnStyle);

//     QHBoxLayout *scanBtnLayout = new QHBoxLayout();
//     scanBtnLayout->addWidget(inButton);
//     scanBtnLayout->addWidget(clearButton);

//     QVBoxLayout *scanLayout = new QVBoxLayout();
//     scanLayout->addWidget(scanInput);
//     scanLayout->addLayout(scanBtnLayout);

//     QGroupBox *scanBox = new QGroupBox();
//     scanBox->setLayout(scanLayout);
//     scanBox->setStyleSheet("QGroupBox { border: none; background-color: #f9f9f9; padding: 10px; }");

//     // ---- Unmapped Bins ----
//     unmappedLabel = new QLabel("Unmapped Bins");
//     unmappedLabel->setStyleSheet("font-size: 18px; font-weight: bold;");

//     unmappedTable = new QTableWidget(0, 3);
//     QStringList headers = {"#", "Bin Id", "Parts Count"};
//     unmappedTable->setHorizontalHeaderLabels(headers);
//     unmappedTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
//     unmappedTable->verticalHeader()->setVisible(false);
//     unmappedTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
//     unmappedTable->setSelectionBehavior(QAbstractItemView::SelectRows);
//     unmappedTable->setStyleSheet("QTableWidget { background: #fff; }");

//     // Pagination controls
//     prevButton = new QPushButton("◀");
//     nextButton = new QPushButton("▶");
//     pageLabel = new QLabel("Page 1");

//     QHBoxLayout *paginationLayout = new QHBoxLayout();
//     paginationLayout->addWidget(prevButton);
//     paginationLayout->addWidget(pageLabel);
//     paginationLayout->addWidget(nextButton);
//     paginationLayout->addStretch();

//     QVBoxLayout *unmappedLayout = new QVBoxLayout();
//     unmappedLayout->addWidget(unmappedLabel);
//     unmappedLayout->addWidget(unmappedTable);
//     unmappedLayout->addLayout(paginationLayout);

//     // ---- Recently Scanned Reels ----
//     recentLabel = new QLabel("Recently Scanned Reels");
//     recentLabel->setStyleSheet("font-size: 18px; font-weight: bold;");

//     recentContent = new QWidget();
//     recentLayout = new QVBoxLayout(recentContent);
//     recentLayout->setSpacing(12);
//     recentLayout->addStretch();

//     recentScrollArea = new QScrollArea();
//     recentScrollArea->setWidgetResizable(true);
//     recentScrollArea->setWidget(recentContent);
//     recentScrollArea->setStyleSheet("QScrollArea { background-color: #f9f9f9; }");

//     QVBoxLayout *recentContainerLayout = new QVBoxLayout();
//     recentContainerLayout->addWidget(recentLabel);
//     recentContainerLayout->addWidget(recentScrollArea);

//     // ---- Bottom Layout ----
//     QHBoxLayout *bottomLayout = new QHBoxLayout();
//     bottomLayout->addLayout(unmappedLayout, 1);
//     bottomLayout->addLayout(recentContainerLayout, 1);

//     // ---- Main Layout ----
//     QVBoxLayout *mainLayout = new QVBoxLayout(this);
//     mainLayout->addWidget(titleLabel);
//     mainLayout->addWidget(scanBox);
//     mainLayout->addLayout(bottomLayout);
//     mainLayout->setSpacing(15);
//     setLayout(mainLayout);

//     // Initial placeholder
//     updateUnmappedTable();
// }

// void StoresIn::addRecentItem(const QString &uniqueId, const QString &rack, const QString &partNumber)
// {
//     QLabel *itemLabel = new QLabel(
//         QString("<b>Unique Id :</b> %1<br>"
//                 "<b>Rack :</b> %2<br>"
//                 "<b>Part Number :</b> %3")
//             .arg(uniqueId, rack, partNumber));

//     itemLabel->setStyleSheet("background-color: #fff; border-radius: 5px; padding: 8px;");
//     recentLayout->insertWidget(recentLayout->count() - 1, itemLabel);
// }

// void StoresIn::updateUnmappedTable()
// {
//     unmappedTable->clearContents();

//     int totalRows = binList.size();
//     if (totalRows == 0) {
//         unmappedTable->setRowCount(1);
//         QTableWidgetItem *noData = new QTableWidgetItem("No Data Found");
//         noData->setTextAlignment(Qt::AlignCenter);
//         unmappedTable->setSpan(0, 0, 1, 3);
//         unmappedTable->setItem(0, 0, noData);
//         pageLabel->setText("Page 1");
//         return;
//     }

//     unmappedTable->setRowCount(0);
//     unmappedTable->setColumnCount(3);
//     unmappedTable->setHorizontalHeaderLabels({"#", "Bin Id", "Parts Count"});

//     int start = currentPage * rowsPerPage;
//     int end = qMin(start + rowsPerPage, totalRows);

//     for (int i = start; i < end; ++i) {
//         const BinData &bin = binList[i];
//         int row = unmappedTable->rowCount();
//         unmappedTable->insertRow(row);
//         unmappedTable->setItem(row, 0, new QTableWidgetItem(QString::number(i + 1)));
//         unmappedTable->setItem(row, 1, new QTableWidgetItem(bin.binId));
//         unmappedTable->setItem(row, 2, new QTableWidgetItem(QString::number(bin.partsCount)));
//     }

//     int totalPages = qMax(1, (totalRows + rowsPerPage - 1) / rowsPerPage);
//     pageLabel->setText(QString("Page %1 / %2").arg(currentPage + 1).arg(totalPages));
// }

// void StoresIn::handleInButton()
// {
//     QString text = scanInput->text().trimmed();
//     if (text.isEmpty()) {
//         QMessageBox::warning(this, "Input Required", "Please scan or enter a Slot / Bin / Part first.");
//         return;
//     }

//     // Add new reel to the right panel
//     QString uniqueId = QString("TIT%1").arg(++reelCounter);
//     QString rack = QString("RACK~STORE-RE~%1").arg((reelCounter % 5) + 1);
//     addRecentItem(uniqueId, rack, text);

//     // Add or update bin on the left
//     QString binId = QString("BIN-%1").arg((reelCounter % 15) + 1);

//     bool found = false;
//     for (auto &bin : binList) {
//         if (bin.binId == binId) {
//             bin.partsCount++;
//             found = true;
//             break;
//         }
//     }
//     if (!found) {
//         binList.append({binId, 1});
//     }

//     scanInput->clear();
//     updateUnmappedTable();
// }

// void StoresIn::handleClearButton()
// {
//     scanInput->clear();
// }

// void StoresIn::nextPage()
// {
//     int totalPages = qMax(1, (binList.size() + rowsPerPage - 1) / rowsPerPage);
//     if (currentPage < totalPages - 1) {
//         currentPage++;
//         updateUnmappedTable();
//     }
// }

// void StoresIn::prevPage()
// {
//     if (currentPage > 0) {
//         currentPage--;
//         updateUnmappedTable();
//     }
// }






// trial -2

// #include "storesin.h"
// #include <QGroupBox>
// #include <QHeaderView>
// #include <QScrollArea>
// #include <QFrame>
// #include <QSpacerItem>
// #include <QVBoxLayout>
// #include <QHBoxLayout>
// #include <QLabel>
// #include <QLineEdit>
// #include <QPushButton>
// #include <QTableWidget>
// #include <QMessageBox>
// #include <QDebug>
// #include <QStringList>

// StoresIn::StoresIn(QWidget *parent) : QWidget(parent)
// {
//     // ---------------- Navbar ----------------
//     QWidget *navbar = new QWidget(this);
//     navbar->setFixedHeight(70);
//     navbar->setStyleSheet("background-color: #FFFFFF;");

//     QHBoxLayout *navLayout = new QHBoxLayout(navbar);
//     navLayout->setContentsMargins(20, 0, 20, 0);
//     navLayout->setSpacing(25);

//     QLabel *logoLabel = new QLabel;
//     QPixmap logoPixmap(":/logo/logoWNavbar.png");
//     logoLabel->setPixmap(logoPixmap.scaled(250, 250, Qt::KeepAspectRatio, Qt::SmoothTransformation));
//     navLayout->addWidget(logoLabel);

//     // ---------------- Main Layout ----------------
//     QVBoxLayout *homeLayout = new QVBoxLayout(this);
//     homeLayout->addWidget(navbar);

//     // Setup UI
//     setupUI();

//     // Add to main layout
//     homeLayout->addWidget(titleLabel);
//     homeLayout->addWidget(scanBox);
//     homeLayout->addLayout(bottomLayout);
//     homeLayout->setSpacing(15);
//     setLayout(homeLayout);

//     // Connect buttons
//     connect(inButton, &QPushButton::clicked, this, &StoresIn::handleInButton);
//     connect(clearButton, &QPushButton::clicked, this, &StoresIn::handleClearButton);
//     connect(nextButton, &QPushButton::clicked, this, &StoresIn::nextPage);
//     connect(prevButton, &QPushButton::clicked, this, &StoresIn::prevPage);
// }

// void StoresIn::setupUI()
// {
//     // ---- Title ----
//     titleLabel = new QLabel("Stores In");
//     titleLabel->setStyleSheet("font-size: 20px; font-weight: bold;");

//     // ---- Scan Section ----
//     scanInput = new QLineEdit();
//     scanInput->setPlaceholderText("Scan Slot / Bin / Part");
//     scanInput->setMinimumHeight(35);
//     scanInput->setStyleSheet("QLineEdit { border: 1px solid #0078D7; border-radius: 5px; padding: 6px; }");

//     inButton = new QPushButton("IN");
//     clearButton = new QPushButton("CLEAR");

//     QString btnStyle = R"(
//         QPushButton {
//             background-color: #0078D7;
//             color: white;
//             font-weight: bold;
//             border-radius: 5px;
//             padding: 8px 16px;
//         }
//         QPushButton:hover {
//             background-color: #005fa3;
//         }
//     )";

//     inButton->setStyleSheet(btnStyle);
//     clearButton->setStyleSheet(btnStyle);

//     QHBoxLayout *scanBtnLayout = new QHBoxLayout();
//     scanBtnLayout->addWidget(inButton);
//     scanBtnLayout->addWidget(clearButton);

//     QVBoxLayout *scanLayout = new QVBoxLayout();
//     scanLayout->addWidget(scanInput);
//     scanLayout->addLayout(scanBtnLayout);

//     scanBox = new QGroupBox();
//     scanBox->setLayout(scanLayout);
//     scanBox->setStyleSheet("QGroupBox { border: none; background-color: #f9f9f9; padding: 10px; }");

//     // ---- Unmapped Bins Table ----
//     unmappedLabel = new QLabel("Unmapped Bins");
//     unmappedLabel->setStyleSheet("font-size: 18px; font-weight: bold;");

//     unmappedTable = new QTableWidget(0, 3);
//     QStringList headers = {"#", "Bin Id", "Parts Count"};
//     unmappedTable->setHorizontalHeaderLabels(headers);
//     unmappedTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
//     unmappedTable->verticalHeader()->setVisible(false);
//     unmappedTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
//     unmappedTable->setSelectionBehavior(QAbstractItemView::SelectRows);
//     unmappedTable->setStyleSheet("QTableWidget { background: #fff; }");

//     // Pagination controls
//     prevButton = new QPushButton("◀");
//     nextButton = new QPushButton("▶");
//     pageLabel = new QLabel("Page 1");

//     QHBoxLayout *paginationLayout = new QHBoxLayout();
//     paginationLayout->addWidget(prevButton);
//     paginationLayout->addWidget(pageLabel);
//     paginationLayout->addWidget(nextButton);
//     paginationLayout->addStretch();

//     QVBoxLayout *unmappedLayout = new QVBoxLayout();
//     unmappedLayout->addWidget(unmappedLabel);
//     unmappedLayout->addWidget(unmappedTable);
//     unmappedLayout->addLayout(paginationLayout);

//     // ---- Recently Scanned Reels ----
//     recentLabel = new QLabel("Recently Scanned Reels");
//     recentLabel->setStyleSheet("font-size: 18px; font-weight: bold;");

//     recentContent = new QWidget();
//     recentLayout = new QVBoxLayout(recentContent);
//     recentLayout->setSpacing(12);
//     recentLayout->addStretch();

//     recentScrollArea = new QScrollArea();
//     recentScrollArea->setWidgetResizable(true);
//     recentScrollArea->setWidget(recentContent);
//     recentScrollArea->setStyleSheet("QScrollArea { background-color: #f9f9f9; }");

//     QVBoxLayout *recentContainerLayout = new QVBoxLayout();
//     recentContainerLayout->addWidget(recentLabel);
//     recentContainerLayout->addWidget(recentScrollArea);

//     // ---- Bottom Layout ----
//     bottomLayout = new QHBoxLayout();
//     bottomLayout->addLayout(unmappedLayout, 1);
//     bottomLayout->addLayout(recentContainerLayout, 1);

//     // Initial placeholder
//     updateUnmappedTable();
// }

// void StoresIn::addRecentItem(const QString &uniqueId, const QString &rack, const QString &partNumber)
// {
//     QLabel *itemLabel = new QLabel(
//         QString("<b>Unique Id :</b> %1<br>"
//                 "<b>Rack :</b> %2<br>"
//                 "<b>Part Number :</b> %3")
//             .arg(uniqueId, rack, partNumber));

//     itemLabel->setStyleSheet("background-color: #fff; border-radius: 5px; padding: 8px;");
//     recentLayout->insertWidget(recentLayout->count() - 1, itemLabel);
// }

// void StoresIn::updateUnmappedTable()
// {
//     unmappedTable->clearContents();

//     int totalRows = binList.size();
//     if (totalRows == 0) {
//         unmappedTable->setRowCount(1);
//         QTableWidgetItem *noData = new QTableWidgetItem("No Data Found");
//         noData->setTextAlignment(Qt::AlignCenter);
//         unmappedTable->setSpan(0, 0, 1, 3);
//         unmappedTable->setItem(0, 0, noData);
//         pageLabel->setText("Page 1");
//         return;
//     }

//     unmappedTable->setRowCount(0);
//     unmappedTable->setColumnCount(3);
//     unmappedTable->setHorizontalHeaderLabels({"#", "Bin Id", "Parts Count"});

//     int start = currentPage * rowsPerPage;
//     int end = qMin(start + rowsPerPage, totalRows);

//     for (int i = start; i < end; ++i) {
//         const BinData &bin = binList[i];
//         int row = unmappedTable->rowCount();
//         unmappedTable->insertRow(row);
//         unmappedTable->setItem(row, 0, new QTableWidgetItem(QString::number(i + 1)));
//         unmappedTable->setItem(row, 1, new QTableWidgetItem(bin.binId));
//         unmappedTable->setItem(row, 2, new QTableWidgetItem(QString::number(bin.partsCount)));
//     }

//     int totalPages = qMax(1, (totalRows + rowsPerPage - 1) / rowsPerPage);
//     pageLabel->setText(QString("Page %1 / %2").arg(currentPage + 1).arg(totalPages));
// }

// void StoresIn::handleInButton()
// {
//     QString text = scanInput->text().trimmed();
//     if (text.isEmpty()) {
//         QMessageBox::warning(this, "Input Required", "Please scan or enter a Slot / Bin / Part first.");
//         return;
//     }

//     // Add new reel to the right panel
//     QString uniqueId = QString("TIT%1").arg(++reelCounter);
//     QString rack = QString("RACK~STORE-RE~%1").arg((reelCounter % 5) + 1);
//     addRecentItem(uniqueId, rack, text);

//     // Add or update bin on the left
//     QString binId = QString("BIN-%1").arg((reelCounter % 15) + 1);

//     bool found = false;
//     for (auto &bin : binList) {
//         if (bin.binId == binId) {
//             bin.partsCount++;
//             found = true;
//             break;
//         }
//     }
//     if (!found) {
//         binList.append({binId, 1});
//     }

//     scanInput->clear();
//     updateUnmappedTable();
// }

// void StoresIn::handleClearButton()
// {
//     scanInput->clear();
// }

// void StoresIn::nextPage()
// {
//     int totalPages = qMax(1, (binList.size() + rowsPerPage - 1) / rowsPerPage);
//     if (currentPage < totalPages - 1) {
//         currentPage++;
//         updateUnmappedTable();
//     }
// }

// void StoresIn::prevPage()
// {
//     if (currentPage > 0) {
//         currentPage--;
//         updateUnmappedTable();
//     }
// }



// trial -3

#include "storesin.h"
#include <QGroupBox>
#include <QHeaderView>
#include <QScrollArea>
#include <QFrame>
#include <QSpacerItem>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QTableWidget>
#include <QMessageBox>
#include <QDebug>
#include <QStringList>

StoresIn::StoresIn(QWidget *parent) : QWidget(parent)
{
    // ---------------- Navbar ----------------
    QWidget *navbar = new QWidget(this);
    navbar->setFixedHeight(70);
    navbar->setStyleSheet("background-color: #FFFFFF;");

    QHBoxLayout *navLayout = new QHBoxLayout(navbar);
    navLayout->setContentsMargins(20, 0, 20, 0);
    navLayout->setSpacing(25);

    QLabel *logoLabel = new QLabel;
    QPixmap logoPixmap(":/logo/logoA.png");
    logoLabel->setPixmap(logoPixmap.scaled(250, 250, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    navLayout->addWidget(logoLabel);

    // ---------------- Main Layout ----------------
    QVBoxLayout *homeLayout = new QVBoxLayout(this);
    homeLayout->addWidget(navbar);

    // Setup UI
    setupUI();

    // Add to main layout
    homeLayout->addWidget(titleLabel);
    homeLayout->addWidget(scanBox);
    homeLayout->addLayout(bottomLayout);
    homeLayout->setSpacing(15);
    setLayout(homeLayout);

    // Connect buttons
    connect(inButton, &QPushButton::clicked, this, &StoresIn::handleInButton);
    connect(clearButton, &QPushButton::clicked, this, &StoresIn::handleClearButton);
    connect(nextButton, &QPushButton::clicked, this, &StoresIn::nextPage);
    connect(prevButton, &QPushButton::clicked, this, &StoresIn::prevPage);
}

void StoresIn::setupUI()
{
    // ---- Title ----
    titleLabel = new QLabel("Stores In");
    titleLabel->setStyleSheet("font-size: 20px; font-weight: bold;");

    // ---- Scan Section ----
    scanInput = new QLineEdit();
    scanInput->setPlaceholderText("Scan Slot / Bin / Part");
    scanInput->setMinimumHeight(35);
    scanInput->setStyleSheet("QLineEdit { border: 1px solid #0078D7; border-radius: 5px; padding: 6px; }");

    inButton = new QPushButton("IN");
    clearButton = new QPushButton("CLEAR");

    QString btnStyle = R"(
        QPushButton {
            background-color: #0078D7;
            color: white;
            font-weight: bold;
            border-radius: 5px;
            padding: 8px 16px;
        }
        QPushButton:hover {
            background-color: #005fa3;
        }
    )";

    inButton->setStyleSheet(btnStyle);
    clearButton->setStyleSheet(btnStyle);

    QHBoxLayout *scanBtnLayout = new QHBoxLayout();
    scanBtnLayout->addWidget(inButton);
    scanBtnLayout->addWidget(clearButton);

    QVBoxLayout *scanLayout = new QVBoxLayout();
    scanLayout->addWidget(scanInput);
    scanLayout->addLayout(scanBtnLayout);

    scanBox = new QGroupBox();
    scanBox->setLayout(scanLayout);
    scanBox->setStyleSheet("QGroupBox { border: none; background-color: #f9f9f9; padding: 10px; }");

    // ---- Unmapped Bins Table ----
    unmappedLabel = new QLabel("Unmapped Bins");
    unmappedLabel->setStyleSheet("font-size: 18px; font-weight: bold;");

    unmappedTable = new QTableWidget(0, 3);
    QStringList headers = {"#", "Bin Id", "Parts Count"};
    unmappedTable->setHorizontalHeaderLabels(headers);
    unmappedTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    unmappedTable->verticalHeader()->setVisible(false);
    unmappedTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    unmappedTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    unmappedTable->setStyleSheet("QTableWidget { background: #fff; }");

    // Pagination controls
    prevButton = new QPushButton("◀");
    nextButton = new QPushButton("▶");
    pageLabel = new QLabel("Page 1");

    QHBoxLayout *paginationLayout = new QHBoxLayout();
    paginationLayout->addWidget(prevButton);
    paginationLayout->addWidget(pageLabel);
    paginationLayout->addWidget(nextButton);
    paginationLayout->addStretch();

    QVBoxLayout *unmappedLayout = new QVBoxLayout();
    unmappedLayout->addWidget(unmappedLabel);
    unmappedLayout->addWidget(unmappedTable);
    unmappedLayout->addLayout(paginationLayout);

    // ---- Recently Scanned Reels ----
    recentLabel = new QLabel("Recently Scanned Reels");
    recentLabel->setStyleSheet("font-size: 18px; font-weight: bold;");

    recentContent = new QWidget();
    recentLayout = new QVBoxLayout(recentContent);
    recentLayout->setSpacing(12);
    recentLayout->addStretch();

    recentScrollArea = new QScrollArea();
    recentScrollArea->setWidgetResizable(true);
    recentScrollArea->setWidget(recentContent);
    recentScrollArea->setStyleSheet("QScrollArea { background-color: #f9f9f9; }");

    QVBoxLayout *recentContainerLayout = new QVBoxLayout();
    recentContainerLayout->addWidget(recentLabel);
    recentContainerLayout->addWidget(recentScrollArea);

    // ---- Bottom Layout ----
    bottomLayout = new QHBoxLayout();
    bottomLayout->addLayout(unmappedLayout, 1);
    bottomLayout->addLayout(recentContainerLayout, 1);

    // ---------------- Dummy Data for Testing ----------------
    for (int i = 1; i <= 15; ++i) {  // 15 bins for pagination
        binList.append({QString("BIN-%1").arg(i), (i * 2) % 10 + 1});
    }
    updateUnmappedTable();

    for (int i = 101; i <= 110; ++i) { // 10 recent scanned reels
        addRecentItem(QString("TIT%1").arg(i),
                      QString("RACK~STORE-RE~%1").arg((i % 5) + 1),
                      QString("PART-%1").arg(i));
    }
}

void StoresIn::addRecentItem(const QString &uniqueId, const QString &rack, const QString &partNumber)
{
    QLabel *itemLabel = new QLabel(
        QString("<b>Unique Id :</b> %1<br>"
                "<b>Rack :</b> %2<br>"
                "<b>Part Number :</b> %3")
            .arg(uniqueId, rack, partNumber));

    itemLabel->setStyleSheet("background-color: #fff; border-radius: 5px; padding: 8px;");
    recentLayout->insertWidget(recentLayout->count() - 1, itemLabel);
}

void StoresIn::updateUnmappedTable()
{
    unmappedTable->clearContents();

    int totalRows = binList.size();
    if (totalRows == 0) {
        unmappedTable->setRowCount(1);
        QTableWidgetItem *noData = new QTableWidgetItem("No Data Found");
        noData->setTextAlignment(Qt::AlignCenter);
        unmappedTable->setSpan(0, 0, 1, 3);
        unmappedTable->setItem(0, 0, noData);
        pageLabel->setText("Page 1");
        return;
    }

    unmappedTable->setRowCount(0);
    unmappedTable->setColumnCount(3);
    unmappedTable->setHorizontalHeaderLabels({"#", "Bin Id", "Parts Count"});

    int start = currentPage * rowsPerPage;
    int end = qMin(start + rowsPerPage, totalRows);

    for (int i = start; i < end; ++i) {
        const BinData &bin = binList[i];
        int row = unmappedTable->rowCount();
        unmappedTable->insertRow(row);
        unmappedTable->setItem(row, 0, new QTableWidgetItem(QString::number(i + 1)));
        unmappedTable->setItem(row, 1, new QTableWidgetItem(bin.binId));
        unmappedTable->setItem(row, 2, new QTableWidgetItem(QString::number(bin.partsCount)));
    }

    int totalPages = qMax(1, (totalRows + rowsPerPage - 1) / rowsPerPage);
    pageLabel->setText(QString("Page %1 / %2").arg(currentPage + 1).arg(totalPages));
}

void StoresIn::handleInButton()
{
    QString text = scanInput->text().trimmed();
    if (text.isEmpty()) {
        QMessageBox::warning(this, "Input Required", "Please scan or enter a Slot / Bin / Part first.");
        return;
    }

    // Add new reel to the right panel
    QString uniqueId = QString("TIT%1").arg(++reelCounter);
    QString rack = QString("RACK~STORE-RE~%1").arg((reelCounter % 5) + 1);
    addRecentItem(uniqueId, rack, text);

    // Add or update bin on the left
    QString binId = QString("BIN-%1").arg((reelCounter % 15) + 1);

    bool found = false;
    for (auto &bin : binList) {
        if (bin.binId == binId) {
            bin.partsCount++;
            found = true;
            break;
        }
    }
    if (!found) {
        binList.append({binId, 1});
    }

    scanInput->clear();
    updateUnmappedTable();
}

void StoresIn::handleClearButton()
{
    scanInput->clear();
}

void StoresIn::nextPage()
{
    int totalPages = qMax(1, (binList.size() + rowsPerPage - 1) / rowsPerPage);
    if (currentPage < totalPages - 1) {
        currentPage++;
        updateUnmappedTable();
    }
}

void StoresIn::prevPage()
{
    if (currentPage > 0) {
        currentPage--;
        updateUnmappedTable();
    }
}
