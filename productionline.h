#ifndef PRODUCTIONLINE_H
#define PRODUCTIONLINE_H

#include <QWidget>
#include <QTableWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QComboBox>
#include <QLabel>

// Data structure for a production line entry
struct ProductionItem {
    int id;
    QString name;
    QString description;
    QString createdOn;
};

class ProductionLine : public QWidget
{
    Q_OBJECT

public:
    explicit ProductionLine(QWidget *parent = nullptr);

private slots:
    void onAddClicked();
    void onDownloadClicked(const QString &format);
    void onEditClicked(int row);
    void onDeleteClicked(int row);

private:
    QLineEdit *searchEdit;
    QPushButton *addButton;
    QComboBox *downloadCombo;
    QTableWidget *table;
    QLabel *footerLabel;

    QList<ProductionItem> productionList;

    void setupUI();
    void populateDummyData();
    void populateTable();
};

#endif // PRODUCTIONLINE_H
