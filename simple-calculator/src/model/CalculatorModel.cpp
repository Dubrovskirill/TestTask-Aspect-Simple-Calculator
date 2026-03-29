#include "CalculatorModel.h"

CalculatorModel::CalculatorModel()
    : m_leftOperand(0.0)
    , m_rightOperand(0.0)
    , m_currentStrategy(nullptr)
{
}

void CalculatorModel::setLeftOperand(double value) {
    m_leftOperand = value;
}

void CalculatorModel::setRightOperand(double value) {
    m_rightOperand = value;
}

void CalculatorModel::setStrategy(std::unique_ptr<IArithmeticStrategy> strategy) {
    m_currentStrategy = std::move(strategy);
}

double CalculatorModel::calculate() {
    if (!m_currentStrategy) {
        return m_leftOperand;
    }

    double result = m_currentStrategy->execute(m_leftOperand, m_rightOperand);

    m_leftOperand = result;
    return result;
}

void CalculatorModel::clear() {
    m_leftOperand = 0.0;
    m_rightOperand = 0.0;
    m_currentStrategy.reset(nullptr);
}


double CalculatorModel::calculatePreview() const {
    if (!m_currentStrategy) return m_leftOperand;
    return m_currentStrategy->execute(m_leftOperand, m_rightOperand);
}
