#ifndef VALUE_ERROR_H
#define VALUE_ERROR_H

#include <QException>
#include <QString>

class ValueError : public QException {
public:
    ValueError();
    explicit ValueError(const QString& why);

    void raise() const override;
    ValueError* clone() const override;

    static void raise_new(const QString& why);

    QString why() const;

private:
    QString reason;
};

#endif // VALUE_ERROR_H
