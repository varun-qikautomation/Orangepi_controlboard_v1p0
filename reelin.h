#ifndef REELIN_H
#define REELIN_H

#include <QDialog>

namespace Ui {
class reelin;
}

class reelin : public QDialog
{
    Q_OBJECT

public:
    explicit reelin(QWidget *parent = nullptr);
    ~reelin();

private:
    Ui::reelin *ui;
};

#endif // REELIN_H
