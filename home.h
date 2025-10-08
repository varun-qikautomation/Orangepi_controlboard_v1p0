#ifndef HOME_H
#define HOME_H

#include <QDialog>
#include "reelin.h"
#include "warning.h"

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
};

#endif // HOME_H
