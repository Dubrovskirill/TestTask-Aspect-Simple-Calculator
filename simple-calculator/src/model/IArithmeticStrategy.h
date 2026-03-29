#ifndef IARITHMETICSTRATEGY_H
#define IARITHMETICSTRATEGY_H


class IArithmeticStrategy {
public:
    virtual ~IArithmeticStrategy() = default;

    virtual double execute(double left, double right) = 0;
};
#endif // IARITHMETICSTRATEGY_H
