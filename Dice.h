#ifndef DICE_H
#define DICE_H

#include <QWidget>
#include <QPainter>
#include <QPen>
#include <QPainterPath>
#include <QPointF>
#include <QRectF>
#include <QColor>
#include <QMouseEvent>
#include <QtGlobal>
#include <QtMath>

class GameWindow;
class Dice : public QWidget {
    Q_OBJECT
    Q_PROPERTY(unsigned int value READ getValue WRITE setValue)
    Q_PROPERTY(QColor backColor READ getColor WRITE setColor)
    Q_PROPERTY(qreal rotation READ getRotation WRITE rotate)

public:

    static const QColor COLOR_DICE_SECONDARY;
    explicit Dice(QWidget *parent = nullptr, unsigned int init_val = 3);
    ~Dice();
    unsigned int getValue() const;
    void setValue(unsigned int v);
    QColor getColor() const;
    void setColor(const QColor& c);
    void setVisualSize(qreal size);
    qreal getVisualSize() const;
    void rotate(int degrees);
    int getRotation() const;
    void setEnabled(bool e);
    bool isEnabled() const;

signals:
    void clicked();

private:
    unsigned int current_value {};
    QColor backColor {};
    qreal size {};
    bool enabled {true};
    qreal rotation {0};
    void paintEvent(QPaintEvent *e) override;
    void mousePressEvent(QMouseEvent *event) override;
};
#endif //DICE_H
