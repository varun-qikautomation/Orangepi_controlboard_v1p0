#include "addproductiondialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>
#include <QMessageBox>
#include <QDate>

AddProductionDialog::AddProductionDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("Add Production Line");
    setModal(true);
    setFixedSize(400, 220);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    QLabel *lblName = new QLabel("Production Line Name:");
    nameEdit = new QLineEdit();
    nameEdit->setPlaceholderText("Enter line name");

    QLabel *lblDesc = new QLabel("Description:");
    descEdit = new QTextEdit();
    descEdit->setPlaceholderText("Enter description");
    descEdit->setFixedHeight(80);

    saveBtn = new QPushButton("Save");
    saveBtn->setStyleSheet("background-color: #1976D2; color: white; padding: 6px 14px;");

    mainLayout->addWidget(lblName);
    mainLayout->addWidget(nameEdit);
    mainLayout->addWidget(lblDesc);
    mainLayout->addWidget(descEdit);
    mainLayout->addStretch();
    mainLayout->addWidget(saveBtn, 0, Qt::AlignRight);

    connect(saveBtn, &QPushButton::clicked, this, &AddProductionDialog::onSaveClicked);
}

void AddProductionDialog::setEditingItem(const ProductionItem &item)
{
    isEditing = true;
    editingId = item.id;
    nameEdit->setText(item.name);
    descEdit->setText(item.description);
    setWindowTitle("Edit Production Line");
}

void AddProductionDialog::onSaveClicked()
{
    QString name = nameEdit->text().trimmed();
    QString desc = descEdit->toPlainText().trimmed();

    if (name.isEmpty()) {
        QMessageBox::warning(this, "Validation", "Please enter a production line name.");
        return;
    }

    ProductionItem item;
    item.id = isEditing ? editingId : (rand() % 10000 + 100);
    item.name = name;
    item.description = desc;
    item.createdOn = QDate::currentDate().toString("yyyy-MM-dd");

    if (isEditing)
        emit productionEdited(item);
    else
        emit productionAdded(item);

    accept();
}
