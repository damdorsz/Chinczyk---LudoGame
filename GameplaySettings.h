#ifndef GAMEPLAYSETTINGS_H
#define GAMEPLAYSETTINGS_H

#include <QDialog>
#include <QString>
#include <QComboBox>
#include <QVector>
#include <QSet>
#include <QMessageBox>
#include <QLineEdit>
#include "PaintHelper.h"
#include <QButtonGroup>

namespace Ui {
class GameplaySettings;
}

class GameplaySettings : public QDialog
{
    Q_OBJECT

public:
    explicit GameplaySettings(QWidget *parent = nullptr);
    ~GameplaySettings();

    void settingColoursDefault();
    void updateAvailableColors();
    void updateControlsForP3(int state);
    void updateControlsForP4(int state);
    void validateSettings();

    int getPlayers()const{return m_players;}
    QVector<PlayerColor> getPlayerColors() const;
    QVector<QString> getNamePlayers() const ;
    QVector<QString> getPlayerModes() const ;

private:
    Ui::GameplaySettings *ui;

    int m_players;
    QVector<PlayerColor>m_playerColors;
    QVector<QString> m_namePlayers;
    QVector<QString> m_playerModes;
    QButtonGroup *buttonGroupP2;
    QButtonGroup *buttonGroupP3;
    QButtonGroup *buttonGroupP4;
};

#endif // GAMEPLAYSETTINGS_H
