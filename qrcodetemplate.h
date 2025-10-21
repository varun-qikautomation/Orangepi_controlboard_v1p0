// #ifndef QRCODETEMPLATE_H
// #define QRCODETEMPLATE_H

// #include <QWidget>
// #include <QButtonGroup>

// class QLineEdit;
// class QPushButton;
// class QGroupBox;
// class QRadioButton;
// class QLabel;

// class QRCodeTemplate : public QWidget
// {
//     Q_OBJECT

// public:
//     explicit QRCodeTemplate(QWidget *parent = nullptr);

// private:
//     QLineEdit *templateNameEdit;
//     QPushButton *saveButton;
//     QButtonGroup *typeGroup;
// };

// #endif // QRCODETEMPLATE_H



// trial-2
#ifndef QRCODETEMPLATE_H
#define QRCODETEMPLATE_H

#include <QWidget>

class QRCodeTemplate : public QWidget
{
    Q_OBJECT
public:
    explicit QRCodeTemplate(QWidget *parent = nullptr);
};

#endif // QRCODETEMPLATE_H
