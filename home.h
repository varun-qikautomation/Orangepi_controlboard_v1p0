#ifndef HOME_H
#define HOME_H

#include <QDialog>
#include "reelin.h"
#include "warning.h"
#include "pickup.h"
#include "storesout.h"
#include "partsin.h"
#include "settings.h"
#include <QStackedWidget>


namespace Ui {
class Home;
}

class Home : public QDialog
{
    Q_OBJECT

public:
    explicit Home(QWidget *parent = nullptr);
    ~Home();



private:
    Ui::Home *ui;
    QStackedWidget *stackedWidget;

    QWidget *homePage;
    QWidget *reelsInPage;
    QWidget *pickupPage;
    QWidget *storesoutPage;
    QWidget *partsinPage;
    QWidget *settingsPage;


    void setupPages();
    void setupHomePage();
};

#endif // HOME_H
