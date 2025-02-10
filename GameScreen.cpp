#include <GameScreen.h>

#include <GameWindow.h>

GameScreen::GameScreen(GameWindow *parent, const QVector<PlayerColor> &coloursPlayers) : QWidget(parent),coloursPlayers(coloursPlayers) {}

GameScreen::~GameScreen() {}

void GameScreen::paintEvent(QPaintEvent*) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QBrush backBrush (GameWindow::BG_COLOR);
    painter.setBrush(backBrush);

    QRect outer(GameWindow::BOARD_BOUND, GameWindow::BOARD_BOUND, GameWindow::CELL_SIZE * 11, GameWindow::CELL_SIZE * 11);
    painter.drawRect(outer);

    painter.setPen(QPen(QBrush(GameWindow::STROKE_COLOR), GameWindow::STROKE_WIDTH));

    for (int i = 0; i < 11; i++) {
        for (int j = 0; j < 11; j++) {
            QRect g (GameWindow::BOARD_BOUND + (i * GameWindow::CELL_SIZE), GameWindow::BOARD_BOUND + (j * GameWindow::CELL_SIZE), GameWindow::CELL_SIZE, GameWindow::CELL_SIZE);
            painter.drawRect(g);
        }
    }

    painter.drawRect(painthelp::getDestRect());

    drawHomes(painter);
    drawGuidePaths(painter);
}

QColor GameScreen::whichColour(PlayerColor searchColour)
{
    if(searchColour == PlayerColor::RED)
        return QColor (255,0,0);
    if(searchColour == PlayerColor::BLUE)
        return QColor (0,0,255);
    if(searchColour == PlayerColor::YELLOW)
        return QColor (255,255,0);
    if(searchColour == PlayerColor::GREEN)
        return QColor (0,255,0);
    if(searchColour == PlayerColor::WHITE)
        return QColor (0,0,0);
}

void GameScreen::drawHomes(QPainter &painter) {
    QBrush tlBrush (whichColour(coloursPlayers[0]));
    painter.setBrush(tlBrush);
    painter.drawRect(painthelp::getTopLeftHomeRect());

    QBrush trBrush (whichColour(coloursPlayers[2]));
    painter.setBrush(trBrush);
    painter.drawRect(painthelp::getTopRightHomeRect());

    QBrush brBrush (whichColour(coloursPlayers[1]));
    painter.setBrush(brBrush);
    painter.drawRect(painthelp::getBottomRightHomeRect());

    QBrush blBrush (whichColour(coloursPlayers[3]));
    painter.setBrush(blBrush);
    painter.drawRect(painthelp::getBottomLeftHomeRect());

    painter.setBrush(QBrush(GameWindow::BG_COLOR));
    for (auto e : painthelp::getHomeCircleRects())
        painter.drawEllipse(e);
}

void GameScreen::drawGuidePaths(QPainter &painter) {
    QPoint topLeft {0, 4}; //
    painter.setBrush(whichColour(coloursPlayers[0]));
    painter.drawEllipse(painthelp::getCellRect(topLeft).center(), GameWindow::GUIDER_DOT_SIZE, GameWindow::GUIDER_DOT_SIZE);
    topLeft = painthelp::pointBelow(topLeft);
    topLeft = painthelp::pointRight(topLeft);

    for (int i = 0; i < 4; i++) {
        painter.drawRect(QRect(painthelp::getCellRect(topLeft).center() - QPoint(10 / 2, 10 / 2),QSize(10, 10)));
        topLeft = painthelp::pointRight(topLeft);
    }

    QPoint topRight {6, 0};
    painter.setBrush(whichColour(coloursPlayers[2]));
    painter.drawEllipse(painthelp::getCellRect(topRight).center(), GameWindow::GUIDER_DOT_SIZE, GameWindow::GUIDER_DOT_SIZE);
    topRight = painthelp::pointLeft(topRight);
    topRight = painthelp::pointBelow(topRight);

    for (int i = 0; i < 4; i++) {
        painter.drawRect(QRect(painthelp::getCellRect(topRight).center() - QPoint(10 / 2, 10 / 2),QSize(10, 10)));
        topRight = painthelp::pointBelow(topRight);
    }

    QPoint bottomRight {10, 6};
    painter.setBrush(whichColour(coloursPlayers[1]));
    painter.drawEllipse(painthelp::getCellRect(bottomRight).center(), GameWindow::GUIDER_DOT_SIZE, GameWindow::GUIDER_DOT_SIZE);
    bottomRight = painthelp::pointAbove(bottomRight);
    bottomRight = painthelp::pointLeft(bottomRight);

    for (int i = 0; i < 4; i++) {
        painter.drawRect(QRect(painthelp::getCellRect(bottomRight).center() - QPoint(10 / 2, 10 / 2),QSize(10, 10)));
        bottomRight = painthelp::pointLeft(bottomRight);
    }

    QPoint bottomLeft {4, 10};
    painter.setBrush(whichColour(coloursPlayers[3]));
    painter.drawEllipse(painthelp::getCellRect(bottomLeft).center(), GameWindow::GUIDER_DOT_SIZE, GameWindow::GUIDER_DOT_SIZE);
    bottomLeft = painthelp::pointRight(bottomLeft);
    bottomLeft = painthelp::pointAbove(bottomLeft);

    for (int i = 0; i < 4; i++) {
        painter.drawRect(QRect(painthelp::getCellRect(bottomLeft).center() - QPoint(10 / 2, 10 / 2),QSize(10, 10)));
        bottomLeft = painthelp::pointAbove(bottomLeft);
    }
}
