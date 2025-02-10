#include "GameplaySettings.h"
#include "ui_GameplaySettings.h"

GameplaySettings::GameplaySettings(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::GameplaySettings)
{
    ui->setupUi(this);

    buttonGroupP2 = new QButtonGroup(this);
    buttonGroupP3 = new QButtonGroup(this);
    buttonGroupP4 = new QButtonGroup(this);
    buttonGroupP2->addButton(ui->radioButtonP2);
    buttonGroupP2->addButton(ui->radioButtonP2_2);
    buttonGroupP3->addButton(ui->radioButtonP3);
    buttonGroupP3->addButton(ui->radioButtonP3_2);
    buttonGroupP4->addButton(ui->radioButtonP4);
    buttonGroupP4->addButton(ui->radioButtonP4_2);
    ui->radioButtonP2_2->setChecked(true);
    ui->radioButtonP3_2->setChecked(true);
    ui->radioButtonP4_2->setChecked(true);
    connect(ui->checkBoxP3, &QCheckBox::stateChanged, this, &GameplaySettings::updateControlsForP3);
    connect(ui->checkBoxP4, &QCheckBox::stateChanged, this, &GameplaySettings::updateControlsForP4);
    connect(ui->buttonBox_2, &QDialogButtonBox::accepted, this, &GameplaySettings::validateSettings);
    updateControlsForP3(ui->checkBoxP3->isChecked() ? Qt::Checked : Qt::Unchecked);
    updateControlsForP4(ui->checkBoxP4->isChecked() ? Qt::Checked : Qt::Unchecked);

    for (int i = 1; i <= 4; i++) {
        QString comboName = QString("comboBoxP%1").arg(i);
        QComboBox *comboBox = findChild<QComboBox*>(comboName);

        if (comboBox) {
            connect(comboBox, &QComboBox::currentIndexChanged, this, &GameplaySettings::updateAvailableColors);
        }
    }
    ustawienieKolorowDomyslne();
}

GameplaySettings::~GameplaySettings()
{
    delete ui;
}

void GameplaySettings::ustawienieKolorowDomyslne()
{
    QVector<QString> colors = {"Red", "Blue", "Green", "Yellow"};
    for (int licznik = 1; licznik <= 4; licznik++) {
        QString nazwaComboBoxa = QString("comboBoxP%1").arg(licznik);
        QComboBox *comboBox = findChild<QComboBox*>(nazwaComboBoxa);

        if (comboBox) {
            comboBox->clear();  // Czyść poprzednie elementy
            comboBox->addItem("-");  // Opcja domyślna

            // Dodawanie pozostałych kolorów
            for (const auto& color : colors) {
                comboBox->addItem(color);
            }
        }
    }
}

void GameplaySettings::updateAvailableColors()
{
    QSet<QString> usedColors;

    for (int i = 1; i <= 4; i++) {
        QString comboName = QString("comboBoxP%1").arg(i);
        QComboBox *comboBox = findChild<QComboBox*>(comboName);

        if (comboBox) {
            QString selectedColor = comboBox->currentText();
            if (selectedColor != "-") {
                usedColors.insert(selectedColor);
            }
        }
    }

    QVector<QString> colors = {"Red", "Blue", "Green", "Yellow"};

    for (int i = 1; i <= 4; i++) {
        QString comboName = QString("comboBoxP%1").arg(i);
        QComboBox *comboBox = findChild<QComboBox*>(comboName);

        if (comboBox) {
            QString currentSelection = comboBox->currentText();
            bool signalsBlocked = comboBox->blockSignals(true);

            comboBox->clear();
            comboBox->addItem("-");

            for (const auto& color : colors) {
                if (!usedColors.contains(color) || color == currentSelection) {
                    comboBox->addItem(color);
                }
            }

            comboBox->setCurrentText(currentSelection);
            comboBox->blockSignals(signalsBlocked);
        }
    }
}

void GameplaySettings::updateControlsForP3(int state)
{
    bool isEnabled = (state == Qt::Checked);
    if (ui->radioButtonP3) {
        ui->radioButtonP3->setEnabled(isEnabled);
    }
    if (ui->radioButtonP3_2) {
        ui->radioButtonP3_2->setEnabled(isEnabled);
    }
    if (ui->comboBoxP3) {
        ui->comboBoxP3->setEnabled(isEnabled);
    }
    if(ui->lineEditP3) {
        ui->lineEditP3->setEnabled(isEnabled);
    }
}

void GameplaySettings::updateControlsForP4(int state)
{
    bool isEnabled = (state == Qt::Checked);
    if (ui->radioButtonP4) {
        ui->radioButtonP4->setEnabled(isEnabled);
    }
    if (ui->radioButtonP4_2) {
        ui->radioButtonP4_2->setEnabled(isEnabled);
    }
    if (ui->comboBoxP4) {
        ui->comboBoxP4->setEnabled(isEnabled);
    }
    if(ui->lineEditP4) {
        ui->lineEditP4->setEnabled(isEnabled);
    }
}

void GameplaySettings::validateSettings() {

    QVector<QString> namePlayers;
    QVector<PlayerColor> playerColors = {PlayerColor::WHITE,PlayerColor::WHITE,PlayerColor::WHITE,PlayerColor::WHITE};
    QVector<QString> playerModes;
    int numberOfPlayers = 0;

    for (int i = 1; i <= 4; i++) {

        QString checkBoxName = QString("checkBoxP%1").arg(i);
        QCheckBox *checkBox = findChild<QCheckBox *>(checkBoxName);

        if ((i >= 3 && checkBox && !checkBox->isChecked())) {
            continue;
        }

        QString comboName = QString("comboBoxP%1").arg(i);
        QComboBox *comboBox = findChild<QComboBox *>(comboName);

        if (comboBox) {
            QString selectedColor = comboBox->currentText();
            if (selectedColor == "-") {
                QMessageBox::warning(this, "Invalid Selection",
                                     QString("Please select a color"));
                return;
            }
            if(selectedColor == "Red")
                playerColors[i - 1] = PlayerColor::RED;
            if(selectedColor == "Yellow")
                playerColors[i - 1] = PlayerColor::YELLOW;
            if(selectedColor == "Green")
                playerColors[i - 1] = PlayerColor::GREEN;
            if(selectedColor == "Blue")
                playerColors[i - 1] = PlayerColor::BLUE;
        }

        if(i == 1){
            playerModes.append("HUMAN");
        } else {
            QButtonGroup* currentGroup = nullptr;
            switch(i) {
            case 2: currentGroup = buttonGroupP2; break;
            case 3: currentGroup = buttonGroupP3; break;
            case 4: currentGroup = buttonGroupP4; break;
            }

            if (currentGroup) {
                QAbstractButton* selectedButton = currentGroup->checkedButton();
                if (!selectedButton) {
                    QMessageBox::warning(this, "Invalid Selection",
                                         QString("Please select a game mode for Player %1").arg(i));
                    return;
                }
                playerModes.append(selectedButton->text());
            }
        }

        QString lineEditName = QString("lineEditP%1").arg(i);
        QLineEdit *lineEdit = findChild<QLineEdit *>(lineEditName);

        if (lineEdit) {
            QString namePlayer = lineEdit->text();
            if (namePlayer == "") {
                QMessageBox::warning(this, "Invalid Selection",
                                     QString("Please write a name player"));
                return;
            }
            else {
                namePlayers.append(namePlayer);
            }
        }
    }

    if (ui->checkBoxP4->isChecked()) {
        numberOfPlayers = 4;
    } else if (ui->checkBoxP3->isChecked()) {
        numberOfPlayers = 3;
    } else {
        numberOfPlayers = 2;
    }

    m_players = numberOfPlayers;
    m_playerColors = playerColors;
    m_namePlayers = namePlayers;
    m_playerModes = playerModes;
    accept();
}

