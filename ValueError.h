#ifndef VALUE_ERROR_H
#define VALUE_ERROR_H

#include <QException>
#include <QString>

class ValueError : public QException {
public:
    ValueError();

    void raise() const override;
    ValueError *clone() const override;

    ValueError(QString why);
    static void raise_new(QString why);

    QString why();
private:
    QString reason {};
};

#endif //VALUE_ERROR_H
