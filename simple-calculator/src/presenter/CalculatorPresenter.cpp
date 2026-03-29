#include "CalculatorPresenter.h"
#include "ArithmeticFactory.h"
#include <QDebug>

CalculatorPresenter::CalculatorPresenter(QObject *parent)
    : QObject(parent)
    , m_display("")
    , m_history("")
    , m_currentInput("")
    , m_isError(false)
    , m_waitingForNewOperand(true)
{
    initializeActions();
}

void CalculatorPresenter::initializeActions() {
    m_actions.insert("=",   [this]() { handleCalculate(); });
    m_actions.insert("CE",  [this]() { handleClear(); });
    m_actions.insert("+/-", [this]() { handleSignChange(); });
}

void CalculatorPresenter::processInput(const QString &key) {
    if (m_isError && key != "CE") return;

    if ((key >= "0" && key <= "9") || key == ".") {
        if (m_isFinalResult) {
            updateDisplay("0");
            updateHistory("");
            m_currentInput = "";
            m_model.clear();
            m_isFinalResult = false;
        }
        handleDigit(key);
        return;
    }

    if (m_actions.contains(key)) {
        if (key == "=") {
            if (!m_isFinalResult) {
                m_actions[key]();
                m_isFinalResult = true;
            }
        } else {
            m_isFinalResult = false;
            m_actions[key]();
        }
        return;
    }

    if (m_isFinalResult) {
        QString currentResult = m_display;
        updateHistory(currentResult + " " + key + " ");

        m_model.clear();
        m_model.setLeftOperand(currentResult.toDouble());

        auto strategy = ArithmeticFactory::create(key);
        m_model.setStrategy(std::move(strategy));

        m_waitingForNewOperand = true;
        m_isFinalResult = false;

    } else {
        handleOperation(key);
    }
}

void CalculatorPresenter::handleDigit(const QString &digit) {
    if (digit == "." && m_currentInput.contains(".")) {
        return;
    }

    QString oldInput = m_currentInput;

    if (m_currentInput.isEmpty() && digit == ".") {
        m_currentInput = "0.";
    } else if (m_currentInput == "0") {
        if (digit == "0") {
            return;
        } else if (digit != ".") {
            m_currentInput = digit;
        } else {
            m_currentInput += digit;
        }
    } else {
        m_currentInput += digit;
    }


    if (m_history.endsWith(oldInput) && !oldInput.isEmpty()) {
        QString baseHistory = m_history.left(m_history.length() - oldInput.length());
        updateHistory(baseHistory + m_currentInput);
    } else {
        updateHistory(m_history + m_currentInput);
    }

    if (m_model.hasStrategy()) {
        double val = m_currentInput.isEmpty() ? 0.0 : m_currentInput.toDouble();
        m_model.setRightOperand(val);
        try {
            double res = m_model.calculatePreview();
            updateDisplay(QString::number(res, 'g', 10));
        } catch (...) {}
    } else {
        updateDisplay(m_currentInput);
    }

    m_waitingForNewOperand = false;
}

void CalculatorPresenter::handleOperation(const QString &op) {

    if (m_currentInput.isEmpty() && !m_history.isEmpty()) {
        QString tempHistory = m_history.trimmed();
        tempHistory.chop(1);
        updateHistory(tempHistory.trimmed() + " " + op + " ");

        auto strategy = ArithmeticFactory::create(op);
        m_model.setStrategy(std::move(strategy));
        return;
    }

    if (!m_currentInput.isEmpty()) {
        if (m_model.hasStrategy()) {
            m_model.setRightOperand(m_currentInput.toDouble());
            try {
                double result = m_model.calculate();
                updateDisplay(QString::number(result, 'g', 10));
            } catch (...) {
                setError(true);
                return;
            }
        } else {
            m_model.setLeftOperand(m_currentInput.toDouble());
        }

        updateHistory(m_history + " " + op + " ");

        auto strategy = ArithmeticFactory::create(op);
        m_model.setStrategy(std::move(strategy));

        m_currentInput = "";
        m_waitingForNewOperand = true;
    }
}

void CalculatorPresenter::handleCalculate() {
    if (!m_model.hasStrategy()) return;

    try {

        m_model.setRightOperand(m_currentInput.toDouble());
        double result = m_model.calculate();
        updateDisplay(QString::number(result, 'g', 10));

        m_model.clear();
        m_currentInput = "";
        m_waitingForNewOperand = true;

    } catch (const std::runtime_error& e) {
        setError(true);
        updateDisplay(e.what());
    }
}

void CalculatorPresenter::handleClear() {

    m_model.clear();
    m_currentInput = "";
    updateDisplay("");
    updateHistory("");
    setError(false);
    m_waitingForNewOperand = true;
    m_isFinalResult = false;
}

void CalculatorPresenter::handleSignChange() {
    if (m_isError) return;


    QString target = m_currentInput;
    if (target.isEmpty()) {
        target = m_display.isEmpty() ? "0" : m_display;
    }

    double val = target.toDouble();
    if (val == 0.0) return;

    val *= -1;
    QString oldInput = m_currentInput;
    m_currentInput = QString::number(val, 'g', 10);

    if (!oldInput.isEmpty() && m_history.endsWith(oldInput)) {
        QString baseHistory = m_history.left(m_history.length() - oldInput.length());
        updateHistory(baseHistory + m_currentInput);
    } else if (m_isFinalResult) {

        updateHistory(m_currentInput);
        m_isFinalResult = false;
    } else if (m_currentInput != "0") {

        updateHistory(m_currentInput);
    }

    if (m_model.hasStrategy()) {
        m_model.setRightOperand(val);
        try {
            double res = m_model.calculatePreview();
            updateDisplay(QString::number(res, 'g', 10));
        } catch (...) {}
    } else {
        updateDisplay(m_currentInput);
    }
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
