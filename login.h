#ifndef LOGIN_H
#define LOGIN_H

#include <QMainWindow>
#include "home.h"
#include <QLabel>
#include <QLineEdit>


QT_BEGIN_NAMESPACE
namespace Ui {
class Login;
}
QT_END_NAMESPACE

class Login : public QMainWindow
{
    Q_OBJECT

public:
    Login(QWidget *parent = nullptr);
    ~Login();

    protected:
               bool eventFilter(QObject *obj, QEvent *event) override; // declare the event filter
private slots:
               void handleLogin();


private:
    Ui::Login *ui;
    Home *homepage;
    // For responsive logo
    QLabel *m_mainLogo;  // store QLabel pointer
    QPixmap m_pixmap;    // store original pixmap

    QLineEdit *m_usernameEdit;
    QLineEdit *m_passwordEdit;



};
#endif // LOGIN_H
