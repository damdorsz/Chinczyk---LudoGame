#include "ValueError.h"

ValueError::ValueError() : reason("") {}

ValueError::ValueError(const QString& why) : reason(why) {}

void ValueError::raise() const {
    throw *this;
}

ValueError* ValueError::clone() const {
    return new ValueError(*this);
}

void ValueError::raise_new(const QString& why) {
    throw ValueError(why);
}

QString ValueError::why() const {
    return reason;
}
