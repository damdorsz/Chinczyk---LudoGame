#include <Dice.h>

#include <QString>
#include <QDebug>

#include <GameWindow.h>
#include <ValueError.h>

const QColor Dice::COLOR_DICE_SECONDARY = QColor (57, 60, 63);

Dice::Dice(QWidget *parent, unsigned int init_val) :
QWidget(parent), current_value(init_val) {}

Dice::~Dice() {}

unsigned int Dice::getValue() const {
    return current_value;
}

void Dice::setValue(unsigned int v) {
    if(v < 1 || v > 6)
        ValueError::raise_new(QString("Dice::setValue : Invalid value for dice face : ")
            .arg(v));

    current_value = v;
    this->repaint();
}

QColor Dice::getColor() const {
    return backColor;
}

void Dice::setColor(const QColor &c) {
    this->backColor = c;
    this->repaint();
}

void Dice::setVisualSize(qreal size) {
    qreal abs_size = 2 * (qSqrt((size*size)/2));
    this->setFixedSize(abs_size, abs_size);
    this->size = size;
}

qreal Dice::getVisualSize() const {
    return size;
}

void Dice::rotate(int degres) {
    this->rotation = degres;
    repaint();
}

int Dice::getRotation() const {
    return this->rotation;
}

void Dice::setEnabled(bool e) {
    this->enabled = e;
}

bool Dice::isEnabled() const {
    return this->enabled;
}

void Dice::mousePressEvent(QMouseEvent*) {
    if(!this->isEnabled())
        return;

    emit clicked();
}

void Dice::paintEvent(QPaintEvent*) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    painter.translate(this->width() / 2, this->height() / 2);
    painter.rotate(rotation);

    painter.translate(this->width() / -2 + (this->width() - this->getVisualSize()) / 2,
        this->height() / -2 + (this->height() - this->getVisualSize())/2);

    QPainterPath outline {};

    outline.addRoundedRect(
        QRectF(
            QPointF(
                GameWindow::STROKE_WIDTH / 2.0,
                GameWindow::STROKE_WIDTH / 2.0),
            QPointF(
                this->getVisualSize() - (GameWindow::STROKE_WIDTH / 2.0),
                this->getVisualSize() - (GameWindow::STROKE_WIDTH / 2.0)
            )),
        10, 10
    );

    QPen pen(COLOR_DICE_SECONDARY, GameWindow::STROKE_WIDTH);

    painter.setPen(pen);
    painter.setBrush(backColor);
    painter.drawPath(outline);

    painter.setBrush(COLOR_DICE_SECONDARY);
    qreal dot_rad = this->getVisualSize() / 12.0;
    QPointF center = QPointF {getVisualSize() / 2, getVisualSize() / 2};

    QPointF left = QPointF {center.x() - (dot_rad * 3), center.y()};
    QPointF right = QPointF {center.x() + (dot_rad * 3), center.y()};

    QPointF top = QPointF {center.x(), center.y() - (dot_rad * 3)};
    QPointF bottom = QPointF {center.x(), center.y() + (dot_rad * 3)};

    QPointF topleft = QPointF {left.x(), top.y()};
    QPointF topright = QPointF {right.x(), top.y()};

    QPointF bottomleft = QPointF {left.x(), bottom.y()};
    QPointF bottomright = QPointF {right.x(), bottom.y()};

    switch (current_value) {
        case 1:
            painter.drawEllipse(center, dot_rad, dot_rad);
            break;

        case 2:
            painter.drawEllipse(left, dot_rad, dot_rad);
            painter.drawEllipse(right, dot_rad, dot_rad);
            break;

        case 3:
            painter.drawEllipse(center, dot_rad, dot_rad);
            painter.drawEllipse(left, dot_rad, dot_rad);
            painter.drawEllipse(right, dot_rad, dot_rad);
            break;

        case 4:
            painter.drawEllipse(topleft, dot_rad, dot_rad);
            painter.drawEllipse(topright, dot_rad, dot_rad);
            painter.drawEllipse(bottomleft, dot_rad, dot_rad);
            painter.drawEllipse(bottomright, dot_rad, dot_rad);
            break;

        case 5:
            painter.drawEllipse(center, dot_rad, dot_rad);
            painter.drawEllipse(topleft, dot_rad, dot_rad);
            painter.drawEllipse(topright, dot_rad, dot_rad);
            painter.drawEllipse(bottomleft, dot_rad, dot_rad);
            painter.drawEllipse(bottomright, dot_rad, dot_rad);
            break;

        case 6:
            painter.drawEllipse(top, dot_rad, dot_rad);
            painter.drawEllipse(bottom, dot_rad, dot_rad);
            painter.drawEllipse(topleft, dot_rad, dot_rad);
            painter.drawEllipse(topright, dot_rad, dot_rad);
            painter.drawEllipse(bottomleft, dot_rad, dot_rad);
            painter.drawEllipse(bottomright, dot_rad, dot_rad);
            break;

        default:
            break;
    }
}
