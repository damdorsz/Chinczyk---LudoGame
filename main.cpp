#include <QApplication>
#include <QMainWindow>
#include <QDialog>
#include <QObject>
#include <QFileDialog>
#include <QPushButton>
#include <QString>
#include <QPixmap>
#include <QDebug>
#include <QVector>
#include <QMessageBox>
#include <QLoggingCategory>


#include <GameWindow.h>
#include <ValueError.h>
#include <About.h>

#include <PaintHelper.h>

#include "ui_welcome.h"

#include "GameplaySettings.h"

void aboutClicked() {
    AboutDialog::show();
}

void startClick(Ui::WelcomeWindow &ui, QMainWindow &welcomeWindow) {
    GameWindow *gameWin;
    GameplaySettings *m_gameplaySettings = new GameplaySettings();
    if (m_gameplaySettings->exec() == QDialog::Accepted) {
        unsigned int players = m_gameplaySettings->getPlayers();
        QVector<PlayerColor> playersColours = m_gameplaySettings->getPlayerColors();
        QVector<QString> playersNames = m_gameplaySettings->getNamePlayers();
        QVector<QString> playerModes = m_gameplaySettings->getPlayerModes();
        if (players > 0) {
            gameWin = new GameWindow(players,playersNames,playersColours,playerModes);
            gameWin->show();
            welcomeWindow.close();
            QObject::connect(gameWin, &GameWindow::exit, gameWin, [gameWin]() {
                gameWin->close();
                delete gameWin;
                qDebug() << "Window deleted";
            });
        }
    }

    delete m_gameplaySettings;
}

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    QMainWindow welcomeWindow {};
    Ui::WelcomeWindow ui {};
    ui.setupUi(&welcomeWindow);

    ui.logo->setPixmap(QPixmap(QString(":/images/images/ludoGameLogo.png")).scaledToHeight(ui.logo->height()));
    welcomeWindow.show();

    QObject::connect(ui.startButton, &QPushButton::clicked, &welcomeWindow, [&ui, &welcomeWindow](){
        startClick(ui, welcomeWindow);
    });

    QObject::connect(ui.aboutButton, &QPushButton::clicked, &welcomeWindow, [](){
        aboutClicked();
    });

    //Test
    QRect cr = painthelp::getCellRect(0,0);
    qDebug() << cr.x() << cr.y();
    QRect g = painthelp::getPawnGeometry(cr);
    qDebug() << g.x() << g.y();

    try {
        return app.exec();
    } catch (ValueError *e) {
        qDebug() << e->why();
    }
}
