#ifndef WARNING_H
#define WARNING_H

#include <QDialog>
#include <QList>
#include <QStringList>

namespace Ui {
class warning;
}

class warning : public QDialog
{
    Q_OBJECT

public:
    explicit warning(QWidget *parent = nullptr);
    ~warning();

private:
    Ui::warning *ui;
      QList<QStringList> dataList; // now a class member
    void setupWarningSection();
};

#endif // WARNING_H

//-----------------ends-------------------

