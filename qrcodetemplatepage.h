#ifndef QRCODETEMPLATEPAGE_H
#define QRCODETEMPLATEPAGE_H

#include <QWidget>

class QLineEdit;
class QRadioButton;
class QPushButton;
class QLabel;

class QRCodeTemplatePage : public QWidget
{
    Q_OBJECT

public:
    explicit QRCodeTemplatePage(QWidget *parent = nullptr);

private:
    QLineEdit *templateNameEdit;
    QRadioButton *qrCodeRadio;
    QRadioButton *barcodeRadio;
    QRadioButton *dataMatrixRadio;
    QPushButton *saveButton;
};

#endif // QRCODETEMPLATEPAGE_H
