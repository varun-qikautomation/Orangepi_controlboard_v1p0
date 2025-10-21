#ifndef SETTINGS_H
#define SETTINGS_H

#include <QWidget>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>
#include <QTreeWidget>

class Settings : public QWidget
{
    Q_OBJECT

public:
    explicit Settings(QWidget *parent = nullptr);

private slots:
    void onSaveClicked();

private:
    QComboBox *orderCombo;
    QComboBox *groupCombo;
    QComboBox *exchangeCombo;
};

#endif // SETTINGS_H*/
