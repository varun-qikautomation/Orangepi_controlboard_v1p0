#ifndef STORESIN_H
#define STORESIN_H

#include <QGroupBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QTableWidget>
#include <QScrollArea>
#include <QVector>

class StoresIn : public QWidget
{
    Q_OBJECT

public:
    explicit StoresIn(QWidget *parent = nullptr);
    ~StoresIn() {}

private:
    // Top UI
    QLabel *titleLabel;
    QLineEdit *scanInput;
    QPushButton *inButton;
    QPushButton *clearButton;

    // Pagination
    QPushButton *prevButton;
    QPushButton *nextButton;
    QLabel *pageLabel;

    // Left Table
    QLabel *unmappedLabel;
    QTableWidget *unmappedTable;

    // Right Recent Scanned
    QLabel *recentLabel;
    QWidget *recentContent;
    QVBoxLayout *recentLayout;
    QScrollArea *recentScrollArea;

    // Layouts
    QGroupBox *scanBox;
    QHBoxLayout *bottomLayout;

    // Data structures
    struct BinData { QString binId; int partsCount; };
    QVector<BinData> binList;

    int currentPage = 0;
    int rowsPerPage = 5;
    int reelCounter = 0;

    // Methods
    void setupUI();
    void addRecentItem(const QString &uniqueId, const QString &rack, const QString &partNumber);
    void updateUnmappedTable();

private slots:
    void handleInButton();
    void handleClearButton();
    void nextPage();
    void prevPage();
};

#endif // STORESIN_H

