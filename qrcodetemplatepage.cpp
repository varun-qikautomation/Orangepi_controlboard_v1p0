#include "qrcodetemplatepage.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QLineEdit>
#include <QLabel>
#include <QRadioButton>
#include <QPushButton>
#include <QFrame>
#include <QButtonGroup>
#include <QSpacerItem>

QRCodeTemplatePage::QRCodeTemplatePage(QWidget *parent)
    : QWidget(parent)
{
    // === Page Title ===
    QLabel *pageTitle = new QLabel("QR Code Template");
    pageTitle->setStyleSheet("font-size: 22px; font-weight: bold;");

    // === Save Button ===
    saveButton = new QPushButton("SAVE");
    saveButton->setFixedWidth(100);
    saveButton->setStyleSheet("background-color: #007bff; color: white; font-weight: bold;");

    QHBoxLayout *topBarLayout = new QHBoxLayout;
    topBarLayout->addWidget(pageTitle);
    topBarLayout->addStretch();
    topBarLayout->addWidget(saveButton);

    // === Template Name Field ===
    QLabel *templateLabel = new QLabel("Template Name *");
    templateNameEdit = new QLineEdit;
    templateNameEdit->setPlaceholderText("Enter template name");
    templateNameEdit->setMinimumHeight(35);

    QVBoxLayout *templateNameLayout = new QVBoxLayout;
    templateNameLayout->addWidget(templateLabel);
    templateNameLayout->addWidget(templateNameEdit);

    // === Print Type Selection ===
    QLabel *selectLabel = new QLabel("Select The Type of Print");
    selectLabel->setStyleSheet("font-weight: bold; font-size: 15px;");

    // --- QR Code Option ---
    QLabel *qrIcon = new QLabel;
    qrIcon->setPixmap(QPixmap(":/icons/qrcode.png").scaled(64, 64, Qt::KeepAspectRatio));
    QLabel *qrText = new QLabel("QRcode");
    qrText->setAlignment(Qt::AlignCenter);
    qrCodeRadio = new QRadioButton;

    QVBoxLayout *qrLayout = new QVBoxLayout;
    qrLayout->addWidget(qrIcon, 0, Qt::AlignCenter);
    qrLayout->addWidget(qrText, 0, Qt::AlignCenter);
    qrLayout->addWidget(qrCodeRadio, 0, Qt::AlignCenter);

    QFrame *qrFrame = new QFrame;
    qrFrame->setLayout(qrLayout);
    qrFrame->setFrameShape(QFrame::StyledPanel);
    qrFrame->setStyleSheet("QFrame { border-radius: 10px; padding: 10px; }");

    // --- Barcode Option ---
    QLabel *barIcon = new QLabel;
    barIcon->setPixmap(QPixmap(":/icons/barcode.png").scaled(64, 64, Qt::KeepAspectRatio));
    QLabel *barText = new QLabel("Barcode");
    barText->setAlignment(Qt::AlignCenter);
    barcodeRadio = new QRadioButton;
    barcodeRadio->setChecked(true);

    QVBoxLayout *barLayout = new QVBoxLayout;
    barLayout->addWidget(barIcon, 0, Qt::AlignCenter);
    barLayout->addWidget(barText, 0, Qt::AlignCenter);
    barLayout->addWidget(barcodeRadio, 0, Qt::AlignCenter);

    QFrame *barFrame = new QFrame;
    barFrame->setLayout(barLayout);
    barFrame->setFrameShape(QFrame::StyledPanel);
    barFrame->setStyleSheet("QFrame { border-radius: 10px; padding: 10px; background-color: #eef4ff; border: 2px solid #007bff; }");

    // --- Data Matrix Option ---
    QLabel *dmIcon = new QLabel;
    dmIcon->setPixmap(QPixmap(":/icons/datamatrix.png").scaled(64, 64, Qt::KeepAspectRatio));
    QLabel *dmText = new QLabel("Data Matrix");
    dmText->setAlignment(Qt::AlignCenter);
    dataMatrixRadio = new QRadioButton;

    QVBoxLayout *dmLayout = new QVBoxLayout;
    dmLayout->addWidget(dmIcon, 0, Qt::AlignCenter);
    dmLayout->addWidget(dmText, 0, Qt::AlignCenter);
    dmLayout->addWidget(dataMatrixRadio, 0, Qt::AlignCenter);

    QFrame *dmFrame = new QFrame;
    dmFrame->setLayout(dmLayout);
    dmFrame->setFrameShape(QFrame::StyledPanel);
    dmFrame->setStyleSheet("QFrame { border-radius: 10px; padding: 10px; }");

    // Group radio buttons
    QButtonGroup *typeGroup = new QButtonGroup(this);
    typeGroup->addButton(qrCodeRadio);
    typeGroup->addButton(barcodeRadio);
    typeGroup->addButton(dataMatrixRadio);

    // Layout for 3 options
    QHBoxLayout *typeLayout = new QHBoxLayout;
    typeLayout->addWidget(qrFrame);
    typeLayout->addWidget(barFrame);
    typeLayout->addWidget(dmFrame);

    // === Card layout ===
    QVBoxLayout *cardLayout = new QVBoxLayout;
    cardLayout->addLayout(templateNameLayout);
    cardLayout->addSpacing(10);
    cardLayout->addWidget(selectLabel);
    cardLayout->addLayout(typeLayout);

    QFrame *cardFrame = new QFrame;
    cardFrame->setLayout(cardLayout);
    cardFrame->setFrameShape(QFrame::StyledPanel);
    cardFrame->setStyleSheet("QFrame { background-color: white; border-radius: 8px; padding: 20px; }");

    // === Main Layout ===
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(topBarLayout);
    mainLayout->addSpacing(10);
    mainLayout->addWidget(cardFrame);
    mainLayout->addStretch();

    setLayout(mainLayout);
    setStyleSheet("background-color: #f3f3f3;");
}
