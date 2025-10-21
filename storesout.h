#ifndef STOREOUT_H
#define STOREOUT_H

#include <QDialog>
#include <QPushButton>
#include <QStackedWidget>
#include <QTableWidget>
#include <QLineEdit>
#include <QComboBox>

class StoreOut : public QDialog
{
    Q_OBJECT

public:
    explicit StoreOut(QWidget *parent = nullptr);
    ~StoreOut();

private slots:
    void showDirectPickup();
    void showWorkOrderPickup();

private:
    // UI elements
    QWidget *leftPanel;
    QPushButton *btnDirectPickup;
    QPushButton *btnWorkOrderPickup;
    QStackedWidget *stackedWidget;

    QWidget *directPickupPage;
    QLineEdit *searchDirect;
    QComboBox *downloadComboDirect;
    QTableWidget *tableDirect;

    QWidget *workOrderPickupPage;
    QLineEdit *searchWorkOrder;
    QComboBox *downloadComboWorkOrder;
    QTableWidget *tableWorkOrder;

    // Helper functions
    QWidget *createPickupPage(bool isWorkOrder);
    QTableWidget *createTable(bool isWorkOrder);

    // ✅ Added declarations
    void filterTable(QLineEdit *searchEdit, QTableWidget *table);
    void saveTableToCSV(QTableWidget *table, const QString &filename);
};

#endif // STOREOUT_H
