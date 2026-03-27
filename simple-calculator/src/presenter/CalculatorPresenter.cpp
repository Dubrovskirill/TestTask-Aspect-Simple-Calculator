#include "CalculatorPresenter.h"
#include "ArithmeticFactory.h" // Теперь подключаем только фабрику
#include <QDebug>

CalculatorPresenter::CalculatorPresenter(QObject *parent)
    : QObject(parent)
    , m_display("0")
    , m_history("")
    , m_isError(false)
    , m_waitingForNewOperand(true)
{
    initializeActions();
}

void CalculatorPresenter::initializeActions() {
    m_actions.insert("=",   [this]() { handleCalculate(); });
    m_actions.insert("CE",  [this]() { handleClear(); });
    m_actions.insert("+/-", [this]() { handleSignChange(); });
    m_actions.insert(".",   [this]() {
        if (!m_display.contains(".")) updateDisplay(m_display + ".");
    });
}

void CalculatorPresenter::processInput(const QString &key) {

    if (m_isError && key != "CE") return;


    if (key >= "0" && key <= "9") {
        handleDigit(key);
        return;
    }

    if (m_actions.contains(key)) {
        m_actions[key]();
        return;
    }

    handleOperation(key);
}

void CalculatorPresenter::handleDigit(const QString &digit) {
    if (m_waitingForNewOperand) {
        updateDisplay(digit);
        m_waitingForNewOperand = false;
    } else {
        if (m_display == "0") {
            updateDisplay(digit);
        } else {
            updateDisplay(m_display + digit);
        }
    }
}

void CalculatorPresenter::handleOperation(const QString &op) {

    auto strategy = ArithmeticFactory::create(op);

    if (strategy) {
        m_model.setLeftOperand(m_display.toDouble());

        m_model.setStrategy(std::move(strategy));

        updateHistory(m_display + " " + op);
        m_waitingForNewOperand = true;
    }
}

void CalculatorPresenter::handleCalculate() {
    try {
        m_model.setRightOperand(m_display.toDouble());
        double result = m_model.calculate();

        updateHistory("");
        updateDisplay(QString::number(result, 'g', 10));
        m_waitingForNewOperand = true;
    } catch (const std::runtime_error& e) {
        setError(true);
        updateDisplay(e.what());
    }
}

void CalculatorPresenter::handleClear() {
    m_model.clear();
    updateDisplay("0");
    updateHistory("");
    setError(false);
    m_waitingForNewOperand = true;
}

void CalculatorPresenter::handleSignChange() {
    double val = m_display.toDouble();
    updateDisplay(QString::number(val * -1));
}

void CalculatorPresenter::updateDisplay(const QString &value) {
    if (m_display != value) {
        m_display = value;
        emit displayChanged();
    }
}

void CalculatorPresenter::updateHistory(const QString &value) {
    if (m_history != value) {
        m_history = value;
        emit historyChanged();
    }
}

void CalculatorPresenter::setError(bool error) {
    if (m_isError != error) {
        m_isError = error;
        emit isErrorChanged();
    }
}
