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

#include <paint_helper.h>
#include <SaveGameEngine.h>

#include "ui_welcome.h"

#include "gameplaySettings.h"

void aboutClicked() {
    AboutDialog::show();
}

void loadClicked(QMainWindow &welcomeWindow) {
    GameWindow *gameWin;

    QString filename = QFileDialog::getOpenFileName(
        &welcomeWindow,
        QString("Load Game"),
        QString(),
        QString("Ludo Game save (*.lzs)"));

    if(filename == "")
        return; //Canceled

    SaveGameEngine *saveState = SaveGameEngine::deserialize(filename);

    gameWin = new GameWindow(saveState);
    gameWin->show();

    welcomeWindow.close();

    //Just for clearing the memory
    QObject::connect(gameWin, &GameWindow::exit, gameWin, [gameWin](){
        gameWin->close();
        delete gameWin;
        qDebug() << "Window deleted";
    });
}

void startClick(Ui::WelcomeWindow &ui, QMainWindow &welcomeWindow) {
    GameWindow *gameWin;
    gameplaySettings *m_gameplaySettings = new gameplaySettings();

    // Uruchomienie okna dialogowego i sprawdzenie wyniku
    if (m_gameplaySettings->exec() == QDialog::Accepted) {
        unsigned int players = m_gameplaySettings->getPlayers();
        QVector<PlayerColor> tablicaKolorowGraczy = m_gameplaySettings->getPlayerColors();
        QVector<QString> tablicaNazwGraczy = m_gameplaySettings->getNamePlayers();
        QVector<QString> tablicaModeGamers = m_gameplaySettings->getPlayerModes();
        // Kontynuacja tylko, jeśli liczba graczy jest poprawna
        if (players > 0) {
            gameWin = new GameWindow(players,tablicaNazwGraczy,tablicaKolorowGraczy,tablicaModeGamers);
            gameWin->show();
            welcomeWindow.close();

            // Czyszczenie pamięci po zamknięciu okna gry
            QObject::connect(gameWin, &GameWindow::exit, gameWin, [gameWin]() {
                gameWin->close();
                delete gameWin;
                qDebug() << "Window deleted";
            });
        }
    }

    // Zwolnienie pamięci po dialogu
    delete m_gameplaySettings;
}

int main(int argc, char *argv[]) {
    // // Włączenie wyświetlania wszystkich komunikatów debugowania
    // QLoggingCategory::setFilterRules("*.debug=true");

    // // Przekierowanie komunikatów do konsoli
    // qSetMessagePattern("[%{type}] %{message}");

    QApplication app(argc, argv);

    QMainWindow welcomeWindow {};
    Ui::WelcomeWindow ui {};
    ui.setupUi(&welcomeWindow);

    ui.logo->setPixmap(QPixmap(QString(":/images/images/ludoGameLogo.png")).scaledToHeight(ui.logo->height()));
    welcomeWindow.show();

    QObject::connect(ui.startButton, &QPushButton::clicked, &welcomeWindow, [&ui, &welcomeWindow](){
        startClick(ui, welcomeWindow);
    });

    QObject::connect(ui.loadButton, &QPushButton::clicked, &welcomeWindow, [&welcomeWindow]() {
        loadClicked(welcomeWindow);
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
