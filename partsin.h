// #ifndef PARTSIN_H
// #define PARTSIN_H

// #include <QWidget>
// #include <QLineEdit>
// #include <QPushButton>
// #include <QTableWidget>
// #include <QLabel>
// #include <QDateEdit>
// #include <QComboBox>
// #include <QVBoxLayout>

// class PartsIn : public QWidget
// {
//     Q_OBJECT
// public:
//     explicit PartsIn(QWidget *parent = nullptr);

// private:
//     // Scan section
//     QLineEdit *scanDataEdit;
//     QPushButton *clearScanBtn;
//     QPushButton *generateStickersBtn;

//     // Manual entry fields
//     QLineEdit *invoiceEdit;
//     QDateEdit *grnDateEdit;
//     QLineEdit *partNumberEdit;
//     QLineEdit *quantityEdit;
//     QLineEdit *lotNumberEdit;
//     QDateEdit *manufactureDateEdit;
//     QLineEdit *manufacturerEdit;
//     QLineEdit *descriptionEdit;
//     QLineEdit *expiryEdit;
//     QLineEdit *locationEdit;
//     QLineEdit *internalPartEdit;

//     QPushButton *submitBtn;
//     QPushButton *clearBtn;

//     // Search & Table
//     QLineEdit *searchEdit;
//     QTableWidget *table;

//     // Main layout
//     QVBoxLayout *mainLayout;
// };

// #endif // PARTSIN_H




// trial-2
#ifndef PARTSIN_H
#define PARTSIN_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QDateEdit>
#include <QTableWidget>
#include <QVBoxLayout>

class PartsIn : public QWidget
{
    Q_OBJECT
public:
    explicit PartsIn(QWidget *parent = nullptr);


private:

    // Form fields
    QLineEdit *scanDataEdit;
    QLineEdit *invoiceEdit;
    QLineEdit *partNumberEdit;
    QLineEdit *quantityEdit;
    QLineEdit *lotNumberEdit;
    QDateEdit *grnDateEdit;
    QDateEdit *manufactureDateEdit;
    QLineEdit *manufacturerEdit;
    QLineEdit *descriptionEdit;
    QLineEdit *expiryEdit;
    QLineEdit *locationEdit;
    QLineEdit *internalPartEdit;

    // Buttons
    QPushButton *submitBtn;
    QPushButton *clearBtn;
    QPushButton *clearScanBtn;
    QPushButton *generateStickersBtn;

    // Table and search
    QLineEdit *searchEdit;
    QTableWidget *table;

    QVBoxLayout *mainLayout;

    // ---------- Declare helper functions ----------
    void addDummyData();           // Populate table with dummy data
    void addManualEntry();         // Add form data to table
    void clearForm();              // Clear form fields
    void loadStocksFromDB();
};

#endif // PARTSIN_H
