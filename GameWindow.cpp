#include <GameWindow.h>

#include <QDebug>

#include <Game.h>
#include <Board.h>
#include <Pawn.h>
#include <PlayerColor.h>
#include <Dice.h>
#include <paint_helper.h>
#include <ValueError.h>
#include <About.h>
#include <SaveGameEngine.h>

const QColor GameWindow::COLOR_RED = QColor (255,0,0);
const QColor GameWindow::COLOR_YELLOW = QColor (255,255,0);
const QColor GameWindow::COLOR_BLUE = QColor (0,0,255);
const QColor GameWindow::COLOR_GREEN = QColor (0,255,0);
const QColor GameWindow::BG_COLOR = QColor (251,248,248);
const QColor GameWindow::STROKE_COLOR = QColor (128,128,128);

const QColor GameWindow::COLOR_RED_LIGHT = QColor (255,131,131);
const QColor GameWindow::COLOR_YELLOW_LIGHT = QColor (255,250,153);
const QColor GameWindow::COLOR_BLUE_LIGHT = QColor (99,218,220);
const QColor GameWindow::COLOR_GREEN_LIGHT = QColor (148,237,148);

GameWindow::GameWindow(unsigned int players_count, QVector<QString> tablicaNazwGraczy, QVector<PlayerColor> tablicaGraczyKolor, QVector<QString> tablicaModeGamers) :
    mGame {new Game(players_count,tablicaGraczyKolor,tablicaModeGamers)},
    mBoard {mGame->getGameBoard()},
    state {ROLLING},
    mScreen (new GameScreen(this, tablicaGraczyKolor)),
    footer {new QWidget(this)},
    footerLayout {new QVBoxLayout()},
    dice {new Dice(nullptr, 6)},
    hintLabel {new QLabel()},
    mGraczyKolor(tablicaGraczyKolor),
    mNazwGraczy(tablicaNazwGraczy){

    init();
}

GameWindow::GameWindow(SaveGameEngine *save) :
    state {ROLLING},
    //mScreen (new GameScreen(this),mGraczyKolor),
    footer {new QWidget(this)},
    footerLayout {new QVBoxLayout()},
    dice {new Dice(nullptr, 6)},
    hintLabel {new QLabel()} {

    this->mBoard = save->getBoard();
    this->mGame = save->getGame();

    init();
}

void GameWindow::init() {

    this->setCentralWidget(this->mScreen);
    this->setWindowTitle("Ludo Game by Damian Dorsz & Damian Zimnol");

    //Setup menu bar
    QMenuBar *menuBar = this->menuBar();
    QMenu *gameMenu = menuBar->addMenu("&Game");
    QAction *saveAction = gameMenu->addAction(QIcon::fromTheme("file-save"), "&Save");
    QAction *loadAction = gameMenu->addAction(QIcon::fromTheme("file-save"), "&Load");
    gameMenu->addSeparator();
    QAction *aboutAction = gameMenu->addAction(QIcon::fromTheme("file-save"), "&About");
    QAction *exitAction = gameMenu->addAction(QIcon::fromTheme("file-save"), "&Exit");
    connect(saveAction, &QAction::triggered, this, &GameWindow::saveRequested);
    connect(loadAction, &QAction::triggered, this, &GameWindow::loadRequested);
    connect(aboutAction, &QAction::triggered, this, &GameWindow::aboutRequested);
    connect(exitAction, &QAction::triggered, this, &GameWindow::exitRequested);

    dice->setVisualSize(DICE_SIZE);
    hintLabel->setFixedHeight(CELL_SIZE);

    footer->setFixedSize(CELL_SIZE * 5, dice->height()+hintLabel->height());
    footer->move((CELL_SIZE * 12),
        BOARD_BOUND + (this->height() / 4)); //To center-right

    hintLabel->setWordWrap(true);
    dice->setColor(COLOR_RED_LIGHT);
    hintLabel->setFixedWidth(footer->geometry().width());
    hintLabel->setAlignment(Qt::AlignCenter);

    footerLayout->addWidget(dice, 0, Qt::AlignCenter);
    footerLayout->addWidget(hintLabel, 0, Qt::AlignCenter);
    footer->setLayout(footerLayout);

    this->mScreen->setFixedSize(
        (BOARD_BOUND * 2) + (CELL_SIZE * 15) + footer->width(), //Extra space for interaction widget,
        (BOARD_BOUND * 2) + (CELL_SIZE * 11)
    );

    QObject::connect(dice, SIGNAL(clicked()), this, SLOT(rollDiceClicked()));

    //Attach all pawns with the game window
    for(auto pawn : mBoard->getAllPawns()) {
        pawn->attatchWindow(this);
        pawn->setEnabled(false); //Disable all the pawn visuals (buttons)
    }

    updateUi();
}

GameWindow::~GameWindow() {
    delete dice;
    delete hintLabel;
    delete footerLayout;
    delete footer;
    delete mGame;

    delete mScreen;
}


QString GameWindow::getUserName(PlayerColor color) {
    //TODO: Replace this temporary code with something more sensible
    if(mGraczyKolor[0] == color)
    {
        return mNazwGraczy[0];
    } else if (mGraczyKolor[1] == color)
    {
        return mNazwGraczy[1];
    } else if (mGraczyKolor[2] == color)
    {
        return mNazwGraczy[2];
    } else if (mGraczyKolor[3] == color)
    {
        return mNazwGraczy[3];
    }
    return "Nieznany gracz";
}

void GameWindow::updateUi() {
    qInfo() << "GameWindow::updateUi()";

    //Set appropriate color for dice
    switch (mGame->getCurrentPlayer()) {
        case PlayerColor::RED:
            dice->setColor(COLOR_RED_LIGHT);
            break;
        case PlayerColor::YELLOW:
            dice->setColor(COLOR_YELLOW_LIGHT);
            break;
        case PlayerColor::BLUE:
            dice->setColor(COLOR_BLUE_LIGHT);
            break;
        case PlayerColor::GREEN:
            dice->setColor(COLOR_GREEN_LIGHT);
            break;
    }

        // if (state == ROLLING) {
        //     if(mGame->isCurrentPlayerAI()) {
        //         hintLabel->setText(QString("CPU wykonuje ruch..."));
        //         dice->setEnabled(false);
        //         // Dodajemy małe opóźnienie przed rzutem
        //         QTimer::singleShot(500, this, [this]() {
        //             animateDiceRoll();
        //         });
        //     } else {
        //         hintLabel->setText(
        //             QString("%1: Roll the dice!")
        //                 .arg(getUserName(mGame->getCurrentPlayer()))
        //             );
        //         dice->setEnabled(true);
        //     }
        //     for (auto p : mBoard->getAllPawns())
        //         p->setEnabled(false);
        // } else if (state == MOVING) {
        //     if(mGame->isCurrentPlayerAI()) {
        //         // Pobierz wartość kostki i wykonaj ruch AI
        //         int diceValue = dice->getValue();
        //         // Dodajemy małe opóźnienie przed wykonaniem ruchu
        //         QTimer::singleShot(500, this, [this, diceValue]() {
        //             //mGameAI->makeMove(diceValue);
        //             // Po wykonaniu ruchu AI, zaktualizuj stan gry
        //             state = ROLLING;
        //             updateUi();
        //         });
        //     } else {
        //         hintLabel->setText(
        //             QString("%1: Choose a pawn to move!")
        //                 .arg(getUserName(mGame->getCurrentPlayer()))
        //             );
        //     }
        //     dice->setEnabled(false);
        // }
    if (state == ROLLING) {
        hintLabel->setText(
            QString("%1: Roll the dice!")
                .arg(getUserName(mGame->getCurrentPlayer()))
            );
        for (auto p : mBoard->getAllPawns())
            p->setEnabled(false);
        dice->setEnabled(true);
        if(mGame->isCurrentPlayerAI())
        {
            hintLabel->setText(QString("CPU is making a move..."));
            QTimer::singleShot(1000, this, [this]() {
                animateDiceRoll();
            });/*
            int diceValue = dice->getValue();
            mGameAI->makeMove(diceValue);*/
        }

    } else if (state == MOVING) {
        hintLabel->setText(
            QString("%1: Choose a pawn to move!")
                .arg(getUserName(mGame->getCurrentPlayer()))
            );
        dice->setEnabled(false);

    } else if (state == ANIMATING) {
        hintLabel->setText(QString(""));
        for (auto p : mBoard->getAllPawns())
            p->setEnabled(false);
        dice->setEnabled(false);

    } else
        ValueError::raise_new(QString("GameWindow::updateUi() : Invalid value for GameWindow::state"));
}

void GameWindow::rollDiceClicked() {

    hintLabel->setText(QString(""));
    animateDiceRoll();
}

void GameWindow::animateDiceRoll() {
    state = ANIMATING;
    updateUi();

    if(diceAnimationGroup != 0)
        delete diceAnimationGroup;

    diceAnimationGroup = new QParallelAnimationGroup {};

    QPropertyAnimation *rollanimation = new QPropertyAnimation {dice, "rotation", diceAnimationGroup};
    rollanimation->setStartValue(0);
    rollanimation->setEndValue(360*8);
    rollanimation->setDuration(DICE_ANIMATION_DURATION);
    rollanimation->setEasingCurve(QEasingCurve::InOutQuad);

    QSequentialAnimationGroup *shake = new QSequentialAnimationGroup {diceAnimationGroup};

    for (int i = 0; i < 5; i++) {
        QPropertyAnimation *s = new QPropertyAnimation {dice, "pos", shake};
        s->setStartValue(QPoint(dice->x(), dice->y()));
        s->setKeyValueAt(0.25, QPoint(dice->x() - DICE_SHAKE_DISTANCE, dice->y()));
        s->setKeyValueAt(0.50, QPoint(dice->x(), dice->y()));
        s->setKeyValueAt(0.75, QPoint(dice->x() + DICE_SHAKE_DISTANCE, dice->y()));
        s->setEndValue(QPoint(dice->x(), dice->y()));
        s->setDuration(DICE_ANIMATION_DURATION / 5);
        s->setEasingCurve(QEasingCurve::InOutQuad);
        shake->addAnimation(s);
    }

    diceAnimationGroup->addAnimation(rollanimation);
    diceAnimationGroup->addAnimation(shake);
    diceAnimationGroup->start();

    connect(diceAnimationGroup, &QParallelAnimationGroup::finished,
        this,&GameWindow::diceAnimationFinished);
}

void GameWindow::diceAnimationFinished() {
    qDebug() << "GameWindow::diceAnimationFinished()";
    dice->setValue(mGame->rollDice());
    int diceValue = mGame->getLastDiceValue();
    QVector<Pawn*> playables = mGame->getPlayablePawns(diceValue);

    if (playables.isEmpty()) {
        mGame->changeCurrentPlayer();
        QTimer::singleShot(700, this, [this]() {
            this->state = ROLLING;
            this->updateUi();
        });
        return;
    }

    if (mGame->isCurrentPlayerAI()) {
        state = MOVING;

        QTimer::singleShot(500, this, [this, diceValue]() {
            GameAI* ai = new GameAI(mGame);
            Pawn* selectedPawn = ai->makeMove(diceValue);

            if (selectedPawn) {
                // Zamiast bezpośrednio wywoływać playMove, użyj pawnChosen
                pawnChosen(selectedPawn);
                qDebug() << "GameWindow: AI wykonało ruch pionkiem:" << (int)selectedPawn->getColor();
            } else {
                mGame->changeCurrentPlayer();
                state = ROLLING;
                updateUi();
                dice->setEnabled(true);
            }

            delete ai;
        });
    } else {
        if (playables.size() == 1) {
            pawnChosen(playables[0]);
        } else {
            for (auto p : playables)
                p->setEnabled(true);
            state = MOVING;
        }
    }
    updateUi();
}

void GameWindow::pawnChosen(Pawn *p) {
    qInfo() << "GameWindow::pawnChosen()";

    qDebug() << "mGame->getLastDiceValue()" << mGame->getLastDiceValue();
    qDebug() << "Pawn::getRelPosition() == " << p->getRelPosition();
    needs_to_save = true; //Changes made to game
    p->raise(); //So that it is visible on top
    movePawnVisual(p, mGame->predictRel(p, mGame->getLastDiceValue()));
}

void GameWindow::movePawnVisual(Pawn *p, int newrel) {
    qInfo() << "GameWindow::movePawnVisual()";

    if(p->isAtHome()) {
        QRect geom = painthelp::getPawnGeometry(Board::getPawnCoordinates(
            p->getColor(), newrel
        ));
        p->setGeometry(geom);
        pawnAnimationFinished(p); //No animation, so no wait
        state = ROLLING;

    } else {
        //Our magic spell.. Ha..ha..
        state = ANIMATING;
        animateVisualMovement(p, newrel);
    }

    updateUi();
}

void GameWindow::animateVisualMovement(Pawn* p, int endRel) {
    qInfo() << "GameWindow::animateVisualMovement() endRel == " << endRel;

    if(animationGroup != 0)
        delete animationGroup;

    animationGroup = new QSequentialAnimationGroup();
    int startRel = p->getRelPosition();

    for (int i = startRel + 1; i <= endRel; i++) {
        QRect iniCell = painthelp::getPawnGeometry(mBoard->getPawnCoordinates(p->getColor(), i-1));
        QRect destCell;

        if(i == Pawn::DEST)
            //It's stepping into destination, so...
            destCell = painthelp::getPawnDestGeometry(p);
        else
            destCell = painthelp::getPawnGeometry(mBoard->getPawnCoordinates(p->getColor(), i));

        QPropertyAnimation *animation = new QPropertyAnimation (p, "geometry");
        animation->setDuration(ANIMATION_DURATION);
        animation->setStartValue(iniCell);
        animation->setEndValue(destCell);
        animationGroup->addAnimation(animation);

        animationGroup->addPause(100); //This feels smooth
    }

    connect(animationGroup, &QSequentialAnimationGroup::finished, this, [this, p](){this->pawnAnimationFinished(p);});
    animationGroup->start();
}

void GameWindow::pawnAnimationFinished(Pawn *p) {
    if (!mGame->playMove(p, mGame->getLastDiceValue()))
        mGame->changeCurrentPlayer();

    state = ROLLING;
    updateUi();
}

void GameWindow::pawnClashed(Pawn *p) { // komenatrz 260125_1310
    p->changePosition(Pawn::HOME);
    //This line is used in the the Constructor of Pawn and has details in the comment there
    PlayerColor color = p->getColor();
    int which = 0;
    if(mGraczyKolor[0] == color)
    {
        which = 1;
    } else if (mGraczyKolor[1] == color)
    {
        which = 2;
    } else if (mGraczyKolor[2] == color)
    {
        which = 3;
    } else if (mGraczyKolor[3] == color)
    {
        which = 4;
    }
    p->setGeometry(painthelp::getPawnHomePosGeometry(p->getColor(), ((p->getId()+1) % 4) + 1,which));
}

void GameWindow::saveRequested() {
    saveGame();
}

bool GameWindow::saveGame() {
    QString filename = QFileDialog::getSaveFileName(this,
        "Save Game",
        "",
        "Ludo Z+ game save (*.lzs)");
    if(filename == "") //Canceled
        return false;
    SaveGameEngine::serialize(filename, mBoard->getAllPawns(), mBoard, mGame);
    return true;
}

void GameWindow::loadRequested() {
    qDebug() << "Load game requested";

    if(this->needs_to_save) {

        QMessageBox dialog {};
        dialog.setIcon(QMessageBox::Question);
        dialog.setText("Confirm load");
        dialog.setInformativeText("Your current game has some unsaved changes. \
        Are you sure to discard those and load a new game?");

        dialog.addButton(QMessageBox::Yes);
        dialog.addButton(QMessageBox::No);

        int ret = dialog.exec();

        switch (ret) {
            case QMessageBox::Yes:
                break; //Let this game be discarded
            case QMessageBox::No:
                return; //Skip loading
        }
    }

    QString filename = QFileDialog::getOpenFileName(this,
        "Load Game",
        "",
        "Ludo Z+ game save (*.lzs)");
    if(filename == "")
        return; //Canceled
    SaveGameEngine *saveState = SaveGameEngine::deserialize(filename);

    delete this->mBoard;

    this->mGame = saveState->getGame();
    this->mBoard = saveState->getBoard();
    this->state = ROLLING;

    for(auto p : this->mBoard->getAllPawns())
        p->attatchWindow(this);

    needs_to_save = false;
    updateUi();
}

void GameWindow::aboutRequested() {
    AboutDialog::show();
}

void GameWindow::exitRequested() {
    this->close(); //It automatically goes to GameWindow::closeEvent()
}

void GameWindow::closeEvent(QCloseEvent* event) {
    if(!needs_to_save)
        return;

    QMessageBox dialog {};
    dialog.setIcon(QMessageBox::Question);
    dialog.setText("Confirm exit");
    dialog.setInformativeText("Would you like to save the game before exiting?");

    dialog.addButton(QMessageBox::Save);
    dialog.addButton(QMessageBox::Discard);
    dialog.addButton(QMessageBox::Cancel);

    int ret = dialog.exec();

    switch (ret) {
        case QMessageBox::Cancel:
            event->ignore(); //Do nothing
            break;
        case QMessageBox::Save:
            //TODO: Open save dialog here
            event->accept();
            emit exit();
            break;
        case QMessageBox::Discard:
            event->accept();
            break;
    }
}

void GameWindow::endTurn() {
    // Przejdź do następnego gracza w grze

    //Zaktualizuj UI (np. wyświetlenie, który gracz jest teraz aktywny)
    updateUi();
}
