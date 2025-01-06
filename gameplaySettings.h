#ifndef GAMEPLAYSETTINGS_H
#define GAMEPLAYSETTINGS_H

#include <QDialog>
#include <QString>
#include <QComboBox>
#include <QVector>
#include <QSet>
#include <QMessageBox>
#include "board.h"

namespace Ui {
class gameplaySettings;
}

class gameplaySettings : public QDialog
{
    Q_OBJECT

public:
    explicit gameplaySettings(QWidget *parent = nullptr);
    ~gameplaySettings();
    void ustawienieKolorowDomyslne();
    void updateAvailableColors();
    void updateControlsForP3(int state);
    void updateControlsForP4(int state);
    void validateSettings();

    void setBoard(Board* board);
private:
    Ui::gameplaySettings *ui;
    Board* m_board;
};

#endif // GAMEPLAYSETTINGS_H
