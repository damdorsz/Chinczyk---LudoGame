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
    this->setWindowTitle("Ludo Game gameplay settings");
    settingColoursDefault();
}

GameplaySettings::~GameplaySettings()
{
    delete ui;
}

QVector<PlayerColor> GameplaySettings::getPlayerColors() const {
    return m_playerColors;
}

QVector<QString> GameplaySettings::getNamePlayers() const {
    return m_namePlayers;
}

QVector<QString> GameplaySettings::getPlayerModes() const {
    return  m_playerModes;
}

void GameplaySettings::settingColoursDefault()
{
    QVector<QString> colors = {"Red", "Blue", "Green", "Yellow"};
    for (int licznik = 1; licznik <= 4; licznik++) {
        QString nazwaComboBoxa = QString("comboBoxP%1").arg(licznik);
        QComboBox *comboBox = findChild<QComboBox*>(nazwaComboBoxa);

        if (comboBox) {
            comboBox->clear();
            comboBox->addItem("-");

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
    QVector<PlayerColor> playerColors = {PlayerColor::WHITE, PlayerColor::WHITE, PlayerColor::WHITE, PlayerColor::WHITE};
    QVector<QString> playerModes;
    int numberOfPlayers = 0;

    for (int i = 1; i <= 4; i++) {
        if (i >= 3 && !findChild<QCheckBox*>(QString("checkBoxP%1").arg(i))->isChecked()) {
            continue;
        }

        QComboBox* comboBox = findChild<QComboBox*>(QString("comboBoxP%1").arg(i));
        if (comboBox) {
            QString selectedColor = comboBox->currentText();
            if (selectedColor == "-") {
                QMessageBox::warning(this, "Invalid Selection", "Please select a color");
                return;
            }


            static const QMap<QString, PlayerColor> colorMap = {
                {"Red", PlayerColor::RED},
                {"Yellow", PlayerColor::YELLOW},
                {"Green", PlayerColor::GREEN},
                {"Blue", PlayerColor::BLUE}
            };
            playerColors[i - 1] = colorMap.value(selectedColor, PlayerColor::WHITE);
        }

        QString playerMode;
        if (i == 1) {
            playerMode = "HUMAN";
        } else {
            QButtonGroup* currentGroup = nullptr;
            switch (i) {
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
                playerMode = selectedButton->text();
            }
        }
        playerModes.append(playerMode);

        QLineEdit* lineEdit = findChild<QLineEdit*>(QString("lineEditP%1").arg(i));
        if (lineEdit) {
            QString namePlayer = lineEdit->text().trimmed();
            if (namePlayer.isEmpty()) {
                QMessageBox::warning(this, "Invalid Selection", "Please write a player name");
                return;
            }
            namePlayers.append(namePlayer);
        }
    }

    numberOfPlayers = ui->checkBoxP4->isChecked() ? 4 :
                          ui->checkBoxP3->isChecked() ? 3 : 2;

    m_players = numberOfPlayers;
    m_playerColors = playerColors;
    m_namePlayers = namePlayers;
    m_playerModes = playerModes;
    accept();
}


