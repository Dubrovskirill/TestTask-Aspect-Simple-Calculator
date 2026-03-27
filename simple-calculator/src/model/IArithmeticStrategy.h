#ifndef IARITHMETICSTRATEGY_H
#define IARITHMETICSTRATEGY_H

enum class OperationType {
    None,
    Add,
    Subtract,
    Multiply,
    Divide,
    Percent
};


class IArithmeticStrategy {
public:
    virtual ~IArithmeticStrategy() = default;

    virtual double execute(double left, double right) = 0;
};
#endif // IARITHMETICSTRATEGY_H
