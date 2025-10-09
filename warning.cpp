#include "warning.h"
#include "ui_warning.h"
#include <QLabel>
#include <QTableWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QCheckBox>
#include <QLineEdit>
#include <QSignalBlocker>
#include <QScrollBar>
#include <QDebug>

warning::warning(QWidget *parent)
    : QDialog(parent),
    ui(new Ui::warning)
{
    ui->setupUi(this);

    // Navbar
    QWidget *navbar = new QWidget(this);
    navbar->setFixedHeight(70);
    navbar->setStyleSheet("background-color: #FFFFFF; border-bottom: 1px solid #ccc;");

    QHBoxLayout *navLayout = new QHBoxLayout(navbar);
    navLayout->setContentsMargins(20, 0, 20, 0);
    navLayout->setSpacing(25);

    QLabel *logoLabel = new QLabel;
    QPixmap logoPixmap(":/logo/logoWNavbar.png");
    logoLabel->setPixmap(logoPixmap.scaled(250, 250, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    navLayout->addWidget(logoLabel);

    ui->mainLayout->insertWidget(0, navbar);
    ui->mainLayout->setContentsMargins(0, 0, 0, 0);
    ui->mainLayout->setSpacing(5);

    setupWarningSection();
}

warning::~warning()
{
    delete ui;
}

void warning::setupWarningSection()
{
    QWidget *card = new QWidget(this);
    card->setStyleSheet("background-color: #FFFFFF; border-radius: 10px;");
    QVBoxLayout *cardLayout = new QVBoxLayout(card);
    cardLayout->setContentsMargins(20, 20, 20, 20);
    cardLayout->setSpacing(10);

    QLabel *title = new QLabel("Warnings");
    QFont titleFont;
    titleFont.setPointSize(16);
    titleFont.setBold(true);
    title->setFont(titleFont);
    title->setStyleSheet("color: #000;");
    cardLayout->addWidget(title);

    // 🔍 Search bar
    QHBoxLayout *searchLayout = new QHBoxLayout();
    QLabel *searchLabel = new QLabel("Search:");
    searchLabel->setStyleSheet("color:#000;");
    QLineEdit *searchEdit = new QLineEdit();
    searchEdit->setPlaceholderText("Enter Unique Id");
    searchEdit->setFixedWidth(180);
    searchLayout->addWidget(searchLabel);
    searchLayout->addWidget(searchEdit);
    searchLayout->addStretch();
    cardLayout->addLayout(searchLayout);

    // Buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->setSpacing(10);
    buttonLayout->addStretch();

    QPushButton *removeBtn = new QPushButton("REMOVE");
    QPushButton *refreshBtn = new QPushButton("REFRESH");

    removeBtn->setEnabled(false);
    removeBtn->setFixedWidth(100);
    refreshBtn->setFixedWidth(100);

    removeBtn->setStyleSheet("background-color: #E0E0E0; color: #666; border-radius: 5px; padding: 6px;");
    refreshBtn->setStyleSheet("background-color: #007BFF; color: white; border-radius: 5px; padding: 6px;");

    buttonLayout->addWidget(removeBtn);
    buttonLayout->addWidget(refreshBtn);
    cardLayout->addLayout(buttonLayout);

    // Table
    QTableWidget *table = new QTableWidget();
    table->setColumnCount(7);
    QStringList headers = {"", "#", "Unique Id", "Part Number", "Internal Part Number", "Quantity", "Created On"};
    table->setHorizontalHeaderLabels(headers);
    table->horizontalHeader()->setStretchLastSection(true);
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    table->verticalHeader()->setVisible(false);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setSelectionMode(QAbstractItemView::NoSelection);
    table->setAlternatingRowColors(true);

    // 🧭 Style – white theme, dark borders, no blue highlight
    table->setStyleSheet(R"(
 QTableWidget {
            border: 2px solid #000;
            border-radius: 8px;
            gridline-color: #444;
            alternate-background-color: #f2f2f2;
            background-color: #fff;
            color: #000;
            selection-background-color: transparent;
            selection-color: black;
        }
        QHeaderView::section {
            background-color: #ccc;
            color: black;
            padding: 6px;
            border: 1px solid #000;
        }
        QTableWidget::item:selected {
            background-color: transparent;
            color: black;
        }
    )");

    cardLayout->addWidget(table);

    QLabel *noData = new QLabel("No Data Found");
    noData->setAlignment(Qt::AlignCenter);
    noData->setStyleSheet("color: #555;");
    cardLayout->addWidget(noData);

    ui->mainLayout->addWidget(card);

    // ✅ Checkbox style (black tick)
    auto createCheckbox = []() -> QCheckBox* {
        QCheckBox *chk = new QCheckBox();
        chk->setStyleSheet(R"(
            QCheckBox::indicator {
                width: 18px;
                height: 18px;
                border: 2px solid #888;
                border-radius: 4px;
                background-color: #fff;
            }
            QCheckBox::indicator:hover {
                border-color: #000;
            }
            QCheckBox::indicator:checked {
                background-color: #fff;
                border: 2px solid #000;
                image: url(:/checkBox/right.png);
                image-position: center;
            }
        )");
        return chk;
    };

    // 🔁 Populate table function
    auto populateTable = [=]() {
        table->setRowCount(2);

        auto addRow = [&](int row, const QStringList &data) {
            QCheckBox *chk = createCheckbox();
            QWidget *chkContainer = new QWidget();
            QHBoxLayout *lay = new QHBoxLayout(chkContainer);
            lay->setContentsMargins(0, 0, 0, 0);
            lay->setAlignment(Qt::AlignCenter);
            lay->addWidget(chk);
            chkContainer->setLayout(lay);
            table->setCellWidget(row, 0, chkContainer);

            for (int i = 1; i < data.size(); ++i) {
                QTableWidgetItem *item = new QTableWidgetItem(data[i]);
                item->setTextAlignment(Qt::AlignCenter);
                table->setItem(row, i, item);
            }

            connect(chk, &QCheckBox::checkStateChanged, this, [table, removeBtn]() {
                bool anyChecked = false;
                for (int r = 0; r < table->rowCount(); ++r) {
                    auto c = table->cellWidget(r, 0)->findChild<QCheckBox*>();
                    if (c && c->isChecked()) {
                        anyChecked = true;
                        break;
                    }
                }
                removeBtn->setEnabled(anyChecked);
            });
        };

        addRow(0, {"", "1", "U12345", "P-001", "INT-999", "25", "2025-10-08"});
        addRow(1, {"", "2", "U78910", "P-002", "INT-555", "10", "2025-10-07"});

        noData->hide();
        removeBtn->setEnabled(false);
    };

    populateTable();

    // 🔁 Refresh logic
    connect(refreshBtn, &QPushButton::clicked, this, populateTable);

    // 🗑️ Remove selected
    connect(removeBtn, &QPushButton::clicked, this, [this, populateTable, table]() {
        for (int row = table->rowCount() - 1; row >= 0; --row) {
            auto chk = qobject_cast<QCheckBox*>(table->cellWidget(row, 0)->findChild<QCheckBox*>());
            if (chk && chk->isChecked()) {
                table->removeRow(row);
            }
        }
        if (table->rowCount() == 0)
            populateTable();
    });

    // 🔍 Search logic
    connect(searchEdit, &QLineEdit::textChanged, this, [=](const QString &text) {
        for (int row = 0; row < table->rowCount(); ++row) {
            bool match = false;
            for (int col = 0; col < table->columnCount(); ++col) {
                QTableWidgetItem *item = table->item(row, col);
                if (item && item->text().contains(text, Qt::CaseInsensitive)) {
                    match = true;
                    break;
                }
            }
            table->setRowHidden(row, !match);
        }
    });
}
