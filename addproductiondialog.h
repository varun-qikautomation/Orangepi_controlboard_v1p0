#ifndef ADDPRODUCTIONDIALOG_H
#define ADDPRODUCTIONDIALOG_H

#include <QDialog>
#include "productionline.h"

class QLineEdit;
class QTextEdit;
class QPushButton;

class AddProductionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddProductionDialog(QWidget *parent = nullptr);
    void setEditingItem(const ProductionItem &item); // For edit mode

signals:
    void productionAdded(const ProductionItem &item);
    void productionEdited(const ProductionItem &item);

private slots:
    void onSaveClicked();

private:
    QLineEdit *nameEdit;
    QTextEdit *descEdit;
    QPushButton *saveBtn;

    bool isEditing = false;
    int editingId = -1;
};

#endif // ADDPRODUCTIONDIALOG_H
