#ifndef GLOBALDEFINES_H
#define GLOBALDEFINES_H

#include <QObject>

namespace PeriodType {
    Q_NAMESPACE
    enum class Enum {
        Year = 0,
        Month,
        Week,
        Custom
    };
    Q_ENUM_NS(Enum)
};

struct IInvalidateFilter {
    virtual void invalidateData() = 0;
};

#endif // GLOBALDEFINES_H
