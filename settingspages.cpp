#include "settingspage.h"

settingspage::SettingsPage(QWidget *parent) : QWidget(parent)
{
    // Main layout
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // Header label
    QLabel *header = new QLabel("Settings Page");
    header->setStyleSheet("font-size: 20px; font-weight: bold;");
    mainLayout->addWidget(header);

    // Form layout
    QVBoxLayout *formLayout = new QVBoxLayout();

    // Username
    QLabel *usernameLabel = new QLabel("Username:");
    usernameEdit = new QLineEdit();
    formLayout->addWidget(usernameLabel);
    formLayout->addWidget(usernameEdit);

    // Email
    QLabel *emailLabel = new QLabel("Email:");
    emailEdit = new QLineEdit();
    formLayout->addWidget(emailLabel);
    formLayout->addWidget(emailEdit);

    // Password
    QLabel *passwordLabel = new QLabel("Password:");
    passwordEdit = new QLineEdit();
    passwordEdit->setEchoMode(QLineEdit::Password);
    formLayout->addWidget(passwordLabel);
    formLayout->addWidget(passwordEdit);

    mainLayout->addLayout(formLayout);

    // Buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    saveButton = new QPushButton("Save");
    resetButton = new QPushButton("Reset");
    buttonLayout->addWidget(saveButton);
    buttonLayout->addWidget(resetButton);
    mainLayout->addLayout(buttonLayout);

    // Settings table
    settingsTable = new QTableWidget(3, 2);
    settingsTable->setHorizontalHeaderLabels(QStringList() << "Setting" << "Value");
    settingsTable->verticalHeader()->setVisible(false);
    settingsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    settingsTable->setItem(0, 0, new QTableWidgetItem("Theme"));
    settingsTable->setItem(0, 1, new QTableWidgetItem("Light"));
    settingsTable->setItem(1, 0, new QTableWidgetItem("Notifications"));
    settingsTable->setItem(1, 1, new QTableWidgetItem("Enabled"));
    settingsTable->setItem(2, 0, new QTableWidgetItem("Auto Update"));
    settingsTable->setItem(2, 1, new QTableWidgetItem("Enabled"));
    mainLayout->addWidget(settingsTable);

    // Connect signals
    connect(saveButton, &QPushButton::clicked, this, &SettingsPage::saveSettings);
    connect(resetButton, &QPushButton::clicked, this, &SettingsPage::resetSettings);
}

// Save settings slot
void SettingsPage::saveSettings()
{
    if(usernameEdit->text().isEmpty() || emailEdit->text().isEmpty() || passwordEdit->text().isEmpty()){
        QMessageBox::warning(this, "Warning", "Please fill all fields!");
        return;
    }
    QMessageBox::information(this, "Saved", "Settings have been saved!");
}

// Reset settings slot
void SettingsPage::resetSettings()
{
    usernameEdit->clear();
    emailEdit->clear();
    passwordEdit->clear();
    QMessageBox::information(this, "Reset", "Settings have been reset!");
}
