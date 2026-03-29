#ifndef ARITHMETICFACTORY_H
#define ARITHMETICFACTORY_H

#include "IArithmeticStrategy.h"
#include <memory>
#include <functional>
#include <QMap>
#include <QString>

class ArithmeticFactory {
public:
    using StrategyCreator = std::function<std::unique_ptr<IArithmeticStrategy>()>;

    static std::unique_ptr<IArithmeticStrategy> create(const QString &op);

private:
    static const QMap<QString, StrategyCreator>& getCreatorsMap();
};

#endif // ARITHMETICFACTORY_H
