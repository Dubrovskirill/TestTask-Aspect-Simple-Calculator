#ifndef CALCULATORMODEL_H
#define CALCULATORMODEL_H

#include "IArithmeticStrategy.h"
#include <memory>


class CalculatorModel {
public:
    CalculatorModel();
    ~CalculatorModel() = default;

    void setLeftOperand(double value);
    void setRightOperand(double value);
    void setStrategy(std::unique_ptr<IArithmeticStrategy> strategy);

    double calculate();

    void clear();

    bool hasStrategy() const { return m_currentStrategy != nullptr; }

    double calculatePreview() const;

private:
    double m_leftOperand;
    double m_rightOperand;
    std::unique_ptr<IArithmeticStrategy> m_currentStrategy;
};

#endif // CALCULATORMODEL_H
