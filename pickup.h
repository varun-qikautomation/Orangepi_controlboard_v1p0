#ifndef PICKUP_H
#define PICKUP_H

#include <QDialog>

namespace Ui {
class pickup;
}

class pickup : public QDialog
{
    Q_OBJECT

public:
    explicit pickup(QWidget *parent = nullptr);
    ~pickup();

signals:
    void backToHome();  // Signal to go back to home


private:
    Ui::pickup *ui;
};

#endif // PICKUP_H
