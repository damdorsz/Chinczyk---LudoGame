#include <GameWindow.h>

#include <QDebug>

#include <Game.h>
#include <Board.h>
#include <Pawn.h>
#include <PlayerColor.h>
#include <Dice.h>
#include <PaintHelper.h>
#include <ValueError.h>
#include <About.h>

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

GameWindow::GameWindow(unsigned int players_count,const QVector<QString>& playersNames,const QVector<PlayerColor>& playersColours, const QVector<QString>& playerModes) :
    mGame {new Game(players_count,playersColours,playerModes)},
    mBoard {mGame->getGameBoard()},
    state {ROLLING},
    mScreen (new GameScreen(this, playersColours)),
    footer {new QWidget(this)},
    footerLayout {new QVBoxLayout()},
    dice {new Dice(nullptr, 6)},
    hintLabel {new QLabel()},
    mPlayersColours(playersColours),
    mPlayersNames(playersNames){

    init();
}


void GameWindow::init() {
    this->setCentralWidget(this->mScreen);
    this->setWindowTitle("Ludo Game by Damian Dorsz & Damian Zimnol");
    QMenuBar *menuBar = this->menuBar();
    QMenu *gameMenu = menuBar->addMenu("&Game");
    gameMenu->addSeparator();
    QAction *aboutAction = gameMenu->addAction(QIcon::fromTheme("file-save"), "&About");
    QAction *exitAction = gameMenu->addAction(QIcon::fromTheme("file-save"), "&Exit");
    connect(aboutAction, &QAction::triggered, this, &GameWindow::aboutRequested);
    connect(exitAction, &QAction::triggered, this, &GameWindow::exitRequested);
    dice->setVisualSize(DICE_SIZE *2);
    dice->setFixedWidth(dice->width() * 2);


    footer->setFixedSize(
        CELL_SIZE * 5 * 4,
        dice->height()  + (CELL_SIZE / 20)
        );

    footer->move(
        (CELL_SIZE * 9),
        BOARD_BOUND + (this->height() / 30)
        );

    hintLabel->setFixedWidth(footer->geometry().width());
    hintLabel->setWordWrap(true);
    hintLabel->setAlignment(Qt::AlignCenter);

    QFont labelFont = hintLabel->font();
    labelFont.setPointSize(labelFont.pointSize() * 3);
    labelFont.setBold(true);
    hintLabel->setFont(labelFont);

    dice->setColor(COLOR_RED_LIGHT);

    footerLayout->addWidget(dice, 0, Qt::AlignCenter);
    footerLayout->addWidget(hintLabel, 0, Qt::AlignCenter);
    footerLayout->setContentsMargins(0, 0, 0, 0);
    footerLayout->setSpacing(1);
    footer->setLayout(footerLayout);

    this->mScreen->setFixedSize(
        (BOARD_BOUND * 2) + (CELL_SIZE * 16) + footer->width() /2,
        (BOARD_BOUND * 2) + (CELL_SIZE * 11)
        );

    QObject::connect(dice, SIGNAL(clicked()), this, SLOT(rollDiceClicked()));

    for(auto pawn : mBoard->getAllPawns()) {
        pawn->attatchWindow(this);
        pawn->setEnabled(false);
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
    if(mPlayersColours[0] == color)
    {
        return mPlayersNames[0];
    }
    else if (mPlayersColours[1] == color)
    {
        return mPlayersNames[1];
    }
    else if (mPlayersColours[2] == color)
    {
        return mPlayersNames[2];
    }
    else if (mPlayersColours[3] == color)
    {
        return mPlayersNames[3];
    }
    return "Unknown player";
}

void GameWindow::updateUi() {
    qInfo() << "GameWindow::updateUi()";

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
            hintLabel->setText(QString("CPU(%1) is making a move...")
                               .arg(getUserName(mGame->getCurrentPlayer()))
                               );
            QTimer::singleShot(1000, this, [this]() {
                animateDiceRoll();
            });
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

void GameWindow::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Space) {
        if (state == GameState::ROLLING) {
            animateDiceRoll();
        }
    }
    QMainWindow::keyPressEvent(event);
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
                pawnChosen(selectedPawn);
                qDebug() << "GameWindow: AI made a move with a pawn:" << (int)selectedPawn->getColor();
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
    needs_to_save = true;
    p->raise();
    movePawnVisual(p, mGame->predictRel(p, mGame->getLastDiceValue()));
}

void GameWindow::movePawnVisual(Pawn *p, int newrel) {
    qInfo() << "GameWindow::movePawnVisual()";

    if(p->isAtHome()) {
        QRect geom = painthelp::getPawnGeometry(Board::getPawnCoordinates(
            p->getColor(), newrel
        ));
        p->setGeometry(geom);
        pawnAnimationFinished(p);
        state = ROLLING;

    } else {
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
            destCell = painthelp::getPawnDestGeometry(p);
        else
            destCell = painthelp::getPawnGeometry(mBoard->getPawnCoordinates(p->getColor(), i));

        QPropertyAnimation *animation = new QPropertyAnimation (p, "geometry");
        animation->setDuration(ANIMATION_DURATION);
        animation->setStartValue(iniCell);
        animation->setEndValue(destCell);
        animationGroup->addAnimation(animation);

        animationGroup->addPause(100);
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

void GameWindow::pawnClashed(Pawn *p) {
    p->changePosition(Pawn::HOME);
    PlayerColor color = p->getColor();
    int which = 0;
    if(mPlayersColours[0] == color)
    {
        which = 1;
    } else if (mPlayersColours[1] == color)
    {
        which = 2;
    } else if (mPlayersColours[2] == color)
    {
        which = 3;
    } else if (mPlayersColours[3] == color)
    {
        which = 4;
    }
    p->setGeometry(painthelp::getPawnHomePosGeometry(p->getColor(), ((p->getId()+1) % 4) + 1,which));
}


void GameWindow::aboutRequested() {
    AboutDialog::show();
}

void GameWindow::exitRequested() {
    this->close();
}

void GameWindow::closeEvent(QCloseEvent* event) {
    if(!needs_to_save)
        return;

    QMessageBox dialog {};
    dialog.setIcon(QMessageBox::Question);
    dialog.setText("Confirm exit");
    dialog.setInformativeText("Do you really want to close the game?");

    dialog.addButton(QMessageBox::Close);
    dialog.addButton(QMessageBox::Cancel);

    int ret = dialog.exec();

    switch (ret) {
        case QMessageBox::Cancel:
            event->ignore();
            break;
        case QMessageBox::Close:
            event->accept();
            break;
    }
}

void GameWindow::endTurn() {
    updateUi();
}
