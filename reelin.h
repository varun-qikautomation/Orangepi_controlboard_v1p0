#ifndef REELIN_H
#define REELIN_H

#include <QDialog>
#include <QLineEdit>

namespace Ui {
class reelin;
}

class reelin : public QDialog
{
    Q_OBJECT

public:
    explicit reelin(QWidget *parent = nullptr);
    ~reelin();

public slots:         // 👈 use slots if you connect with signals
    void backToHome();
    void handleInButton();
    void handleCancelButton();
private:
    Ui::reelin *ui;
    QLineEdit *m_uniqueId;
    QWidget *homePage;

};

#endif // REELIN_H
