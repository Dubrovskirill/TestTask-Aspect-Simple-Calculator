#include "ArithmeticFactory.h"
#include "ArithmeticStrategies.h"

std::unique_ptr<IArithmeticStrategy> ArithmeticFactory::create(const QString &op) {
    const auto& creators = getCreatorsMap();
    auto creator = creators.value(op, nullptr);
    if (creator) {
        return creator();
    }
    return nullptr;
}

const QMap<QString, ArithmeticFactory::StrategyCreator>& ArithmeticFactory::getCreatorsMap() {
    static const QMap<QString, StrategyCreator> creators = {
        {"+", []() { return std::make_unique<AddStrategy>(); }},
        {"-", []() { return std::make_unique<SubStrategy>(); }},
        {"*", []() { return std::make_unique<MulStrategy>(); }},
        {"/", []() { return std::make_unique<DivStrategy>(); }},
        {"%", []() { return std::make_unique<PercentStrategy>(); }}
    };
    return creators;
}
