// #include "qrcodetemplate.h"
// #include <QVBoxLayout>
// #include <QHBoxLayout>
// #include <QLabel>
// #include <QLineEdit>
// #include <QPushButton>
// #include <QGroupBox>
// #include <QRadioButton>
// #include <QFrame>
// #include <QButtonGroup>
// #include <QSpacerItem>
// #include <QStyle>
// #include <QApplication>

// QRCodeTemplate::QRCodeTemplate(QWidget *parent)
//     : QWidget(parent)
// {
//     // ====== Page Title ======
//     QLabel *titleLabel = new QLabel("QR Code Template");
//     titleLabel->setStyleSheet("font-size: 22px; font-weight: 600;");

//     // ====== Input Field ======
//     QLabel *nameLabel = new QLabel("Template Name*");
//     templateNameEdit = new QLineEdit();
//     templateNameEdit->setPlaceholderText("Enter template name");
//     templateNameEdit->setMinimumWidth(400);

//     QVBoxLayout *nameLayout = new QVBoxLayout();
//     nameLayout->addWidget(nameLabel);
//     nameLayout->addWidget(templateNameEdit);

//     // ====== Type Selection ======
//     QLabel *typeLabel = new QLabel("Select The Type of Print");
//     typeLabel->setStyleSheet("font-weight: 600; font-size: 15px;");

//     QRadioButton *qrButton = new QRadioButton("QRcode");
//     QRadioButton *barcodeButton = new QRadioButton("Barcode");
//     QRadioButton *dataMatrixButton = new QRadioButton("Data Matrix");
//     barcodeButton->setChecked(true);

//     typeGroup = new QButtonGroup(this);
//     typeGroup->addButton(qrButton, 0);
//     typeGroup->addButton(barcodeButton, 1);
//     typeGroup->addButton(dataMatrixButton, 2);

//     // --- Card Style ---
//     QString cardStyle = R"(
//         QGroupBox {
//             border: 1px solid #d0d0d0;
//             border-radius: 8px;
//             background-color: #fff;
//             padding: 10px;
//         }
//         QGroupBox:hover {
//             border: 1px solid #0078d7;
//         }
//         QRadioButton {
//             font-size: 14px;
//         }
//     )";

//     // Create visual card boxes
//     QGroupBox *qrBox = new QGroupBox();
//     QVBoxLayout *qrLayout = new QVBoxLayout();
//     QLabel *qrIcon = new QLabel("🔲"); // you can replace with QPixmap icon
//     qrIcon->setAlignment(Qt::AlignCenter);
//     qrLayout->addWidget(qrIcon);
//     qrLayout->addWidget(qrButton, 0, Qt::AlignCenter);
//     qrBox->setLayout(qrLayout);
//     qrBox->setStyleSheet(cardStyle);

//     QGroupBox *barcodeBox = new QGroupBox();
//     QVBoxLayout *barcodeLayout = new QVBoxLayout();
//     QLabel *barcodeIcon = new QLabel("▮▯▮▮▯▯▮");
//     barcodeIcon->setAlignment(Qt::AlignCenter);
//     barcodeLayout->addWidget(barcodeIcon);
//     barcodeLayout->addWidget(barcodeButton, 0, Qt::AlignCenter);
//     barcodeBox->setLayout(barcodeLayout);
//     barcodeBox->setStyleSheet(cardStyle + "background-color: #f5f9ff; border: 1px solid #0078d7;");

//     QGroupBox *dataMatrixBox = new QGroupBox();
//     QVBoxLayout *dataLayout = new QVBoxLayout();
//     QLabel *dataIcon = new QLabel("▣▢▣▢");
//     dataIcon->setAlignment(Qt::AlignCenter);
//     dataLayout->addWidget(dataIcon);
//     dataLayout->addWidget(dataMatrixButton, 0, Qt::AlignCenter);
//     dataMatrixBox->setLayout(dataLayout);
//     dataMatrixBox->setStyleSheet(cardStyle);

//     // Horizontal layout for selection
//     QHBoxLayout *typeLayout = new QHBoxLayout();
//     typeLayout->setSpacing(20);
//     typeLayout->addWidget(qrBox);
//     typeLayout->addWidget(barcodeBox);
//     typeLayout->addWidget(dataMatrixBox);

//     // ====== Save Button ======
//     saveButton = new QPushButton("SAVE");
//     saveButton->setFixedWidth(100);
//     saveButton->setStyleSheet("background-color: #1976d2; color: white; padding: 6px 15px; font-weight: bold; border-radius: 4px;");

//     // ====== Main Layout ======
//     QVBoxLayout *mainLayout = new QVBoxLayout(this);
//     mainLayout->setContentsMargins(30, 20, 30, 20);
//     mainLayout->setSpacing(20);

//     mainLayout->addWidget(titleLabel);
//     mainLayout->addLayout(nameLayout);
//     mainLayout->addWidget(typeLabel);
//     mainLayout->addLayout(typeLayout);
//     mainLayout->addSpacing(10);
//     mainLayout->addWidget(saveButton, 0, Qt::AlignRight);

//     setStyleSheet("background-color: #f4f4f4;");
// }





// trial-2
#include "qrcodetemplate.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QGroupBox>
#include <QRadioButton>
#include <QButtonGroup>
#include <QFrame>

QRCodeTemplate::QRCodeTemplate(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(40, 30, 40, 30);
    mainLayout->setSpacing(20);

    QLabel *title = new QLabel("QR Code Template");
    title->setStyleSheet("font-size: 22px; font-weight: bold;");
    mainLayout->addWidget(title);

    QFrame *frame = new QFrame;
    frame->setStyleSheet("background: white; border: 1px solid #ddd; border-radius: 6px;");
    QVBoxLayout *frameLayout = new QVBoxLayout(frame);
    frameLayout->setContentsMargins(25, 25, 25, 25);
    frameLayout->setSpacing(20);

    QLineEdit *templateName = new QLineEdit;
    templateName->setPlaceholderText("Template Name *");
    frameLayout->addWidget(templateName);

    QLabel *typeLabel = new QLabel("Select The Type of Print");
    typeLabel->setStyleSheet("font-size: 15px; font-weight: 600;");
    frameLayout->addWidget(typeLabel);

    QRadioButton *qrButton = new QRadioButton("QRcode");
    QRadioButton *barButton = new QRadioButton("Barcode");
    QRadioButton *dataButton = new QRadioButton("Data Matrix");
    barButton->setChecked(true);

    QButtonGroup *group = new QButtonGroup(this);
    group->addButton(qrButton);
    group->addButton(barButton);
    group->addButton(dataButton);

    QGroupBox *qrBox = new QGroupBox;
    QVBoxLayout *qrLayout = new QVBoxLayout(qrBox);
    qrLayout->addWidget(new QLabel("🔲"), 0, Qt::AlignCenter);
    qrLayout->addWidget(qrButton, 0, Qt::AlignCenter);

    QGroupBox *barBox = new QGroupBox;
    QVBoxLayout *barLayout = new QVBoxLayout(barBox);
    barLayout->addWidget(new QLabel("▮▯▮▯▮"), 0, Qt::AlignCenter);
    barLayout->addWidget(barButton, 0, Qt::AlignCenter);
    barBox->setStyleSheet("background-color:#f5f9ff; border:1px solid #0078d7; border-radius:8px;");

    QGroupBox *dataBox = new QGroupBox;
    QVBoxLayout *dataLayout = new QVBoxLayout(dataBox);
    dataLayout->addWidget(new QLabel("▣▢▣▢"), 0, Qt::AlignCenter);
    dataLayout->addWidget(dataButton, 0, Qt::AlignCenter);

    QHBoxLayout *typeLayout = new QHBoxLayout;
    typeLayout->setSpacing(20);
    typeLayout->addWidget(qrBox);
    typeLayout->addWidget(barBox);
    typeLayout->addWidget(dataBox);

    frameLayout->addLayout(typeLayout);

    QPushButton *saveBtn = new QPushButton("SAVE");
    saveBtn->setFixedWidth(120);
    saveBtn->setStyleSheet("background-color:#1976d2;color:white;padding:8px 20px;border-radius:5px;");
    QHBoxLayout *saveLayout = new QHBoxLayout;
    saveLayout->addStretch();
    saveLayout->addWidget(saveBtn);
    frameLayout->addLayout(saveLayout);

    mainLayout->addWidget(frame);
    mainLayout->addStretch();
}
