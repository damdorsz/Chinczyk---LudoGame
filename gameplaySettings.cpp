#include "gameplaySettings.h"
#include "ui_gameplaSettings.h"
#include "board.h"

gameplaySettings::gameplaySettings(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::gameplaySettings),
    m_board(nullptr)
{
    ui->setupUi(this);

     ui->radioButtonP2_2->setChecked(true);

    connect(ui->checkBoxP3, &QCheckBox::stateChanged, this, &gameplaySettings::updateControlsForP3);
    connect(ui->checkBoxP4, &QCheckBox::stateChanged, this, &gameplaySettings::updateControlsForP4);
    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &gameplaySettings::validateSettings);


    updateControlsForP3(ui->checkBoxP3->isChecked() ? Qt::Checked : Qt::Unchecked);
    updateControlsForP4(ui->checkBoxP4->isChecked() ? Qt::Checked : Qt::Unchecked);

    for (int i = 1; i <= 4; i++) {
        QString comboName = QString("comboBoxP%1").arg(i);
        QComboBox *comboBox = findChild<QComboBox*>(comboName);

        if (comboBox) {
            connect(comboBox, &QComboBox::currentIndexChanged, this, &gameplaySettings::updateAvailableColors);
        }
    }
    ustawienieKolorowDomyslne();
}

void gameplaySettings::setBoard(Board* board)
{
    m_board = board; // Przypisanie wskaźnika
}

gameplaySettings::~gameplaySettings()
{
    delete ui;
}

void gameplaySettings::ustawienieKolorowDomyslne()
{
    QVector<QString> colors = {"Red", "Blue", "Green", "Yellow"};

    // Iteracja przez ComboBoxy (comboBoxP1, comboBoxP2, ...)
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

void gameplaySettings::updateAvailableColors()
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

            // Blokowanie sygnałów na czas aktualizacji
            bool signalsBlocked = comboBox->blockSignals(true);

            comboBox->clear();
            comboBox->addItem("-");

            for (const auto& color : colors) {
                if (!usedColors.contains(color) || color == currentSelection) {
                    comboBox->addItem(color);
                }
            }

            comboBox->setCurrentText(currentSelection);

            // Przywrócenie sygnałów
            comboBox->blockSignals(signalsBlocked);
        }
    }
}

void gameplaySettings::updateControlsForP3(int state)
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
}

void gameplaySettings::updateControlsForP4(int state)
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
}

// void gameplaySettings::validateSettings()
// {
//     QVector<QString> requiredPlayers = {"Player 1", "Player 2", "Player 3", "Player 4"};

//     // Sprawdzenie kolorów
//     QSet<QString> selectedColors;
//     for (int i = 1; i <= 4; i++) {
//         QString comboName = QString("comboBoxP%1").arg(i);
//         QComboBox *comboBox = findChild<QComboBox*>(comboName);
//         QString checkBoxName = QString("checkBoxP%1").arg(i);
//         QCheckBox *checkBox = findChild<QCheckBox*>(checkBoxName);

//         if ((i >= 3 && checkBox && !checkBox->isChecked())) {
//             continue;
//         }

//         if (comboBox) {
//             QString selectedColor = comboBox->currentText();
//             if (selectedColor == "-") {
//                 QMessageBox::warning(this, "Invalid Selection", QString("Please select a color for %1").arg(requiredPlayers[i - 1]));
//                 return;
//             }
//             selectedColors.insert(selectedColor);
//         }
//     }

//     // Sprawdzenie trybów gry
//     for (int i = 1; i <= 4; i++) {
//         QString checkBoxName = QString("checkBoxP%1").arg(i);
//         QCheckBox *checkBox = findChild<QCheckBox*>(checkBoxName);

//         if (checkBox && !checkBox->isChecked() && i >= 3) {
//             // Jeśli gracz 3 lub 4 jest nieaktywny, pomijamy sprawdzanie
//             continue;
//         }

//         QString radioButton1Name = QString("radioButtonP%1").arg(i);
//         QString radioButton2Name = QString("radioButtonP%1_2").arg(i);

//         QRadioButton *radioButton1 = findChild<QRadioButton*>(radioButton1Name);
//         QRadioButton *radioButton2 = findChild<QRadioButton*>(radioButton2Name);

//         if (radioButton1 && radioButton2) {
//             if (!radioButton1->isChecked() && !radioButton2->isChecked()) {
//                 QMessageBox::warning(this, "Invalid Selection", QString("Please select a game mode for %1").arg(requiredPlayers[i - 1]));
//                 return;
//             }
//         }

//     }

//     // Jeśli wszystko jest poprawne, akceptujemy ustawienia
//     accept();
// }

void gameplaySettings::validateSettings()
{
    QVector<QString> requiredPlayers = {"Player 1", "Player 2", "Player 3", "Player 4"};

    // Kolekcje do przechowywania ustawień graczy
    QList<QString> playerColors; // Lista kolorów graczy
    QList<QString> playerModes;  // Lista trybów graczy (human/cpu)
    QSet<QString> selectedColors;

    for (int i = 1; i <= 4; i++) {
        QString comboName = QString("comboBoxP%1").arg(i);
        QComboBox *comboBox = findChild<QComboBox*>(comboName);
        QString checkBoxName = QString("checkBoxP%1").arg(i);
        QCheckBox *checkBox = findChild<QCheckBox*>(checkBoxName);

        if ((i >= 3 && checkBox && !checkBox->isChecked())) {
            continue; // Pomijamy graczy 3 i 4, jeśli są nieaktywni
        }

        if (comboBox) {
            QString selectedColor = comboBox->currentText();
            if (selectedColor == "-") {
                QMessageBox::warning(this, "Invalid Selection",
                                     QString("Please select a color for %1").arg(requiredPlayers[i - 1]));
                return;
            }
            if (selectedColors.contains(selectedColor)) {
                QMessageBox::warning(this, "Duplicate Color",
                                     QString("Color %1 is already selected for another player.").arg(selectedColor));
                return;
            }

            selectedColors.insert(selectedColor);
            playerColors.append(selectedColor);
        }
    }

    for (int i = 1; i <= 4; i++) {
        QString checkBoxName = QString("checkBoxP%1").arg(i);
        QCheckBox *checkBox = findChild<QCheckBox*>(checkBoxName);

        if (checkBox && !checkBox->isChecked() && i >= 3) {
            continue; // Pomijamy nieaktywnych graczy
        }

        QString radioButton1Name = QString("radioButtonP%1").arg(i);
        QString radioButton2Name = QString("radioButtonP%1_2").arg(i);

        QRadioButton *radioButton1 = findChild<QRadioButton*>(radioButton1Name);
        QRadioButton *radioButton2 = findChild<QRadioButton*>(radioButton2Name);

        if (radioButton1 && radioButton2) {
            if (radioButton1->isChecked()) {
                playerModes.append("human");
            } else if (radioButton2->isChecked()) {
                playerModes.append("cpu");
            } else {
                QMessageBox::warning(this, "Invalid Selection",
                                     QString("Please select a game mode for %1").arg(requiredPlayers[i - 1]));
                return;
            }
        }
    }

    // Przekazanie ustawień do obiektu Board
    //if (m_board) {
        m_board->setPlayerSettings(playerColors, playerModes);
        m_board->updateBoard();
    //}

    accept(); // Akceptacja ustawień
}

