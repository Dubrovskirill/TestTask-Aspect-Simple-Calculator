#include "CalculatorPresenter.h"
#include "ArithmeticFactory.h"
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

    if ((key >= "0" && key <= "9") || key == ".") {
        if (m_isFinalResult) {
            updateDisplay("0");
            updateHistory("");
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
        updateHistory(currentResult + " " + key);

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
    if (m_waitingForNewOperand) {
        if (digit == ".") {
            updateDisplay("0.");
        } else {
            updateDisplay(digit);
        }
        m_waitingForNewOperand = false;
        return;
    }

    if (digit == "." && m_display.contains(".")) {
        return;
    }

    if (m_display == "0" && digit != ".") {
        updateDisplay(digit);
    } else {
        updateDisplay(m_display + digit);
    }
}

void CalculatorPresenter::handleOperation(const QString &op) {
    QString newHistory = m_history;

    if (!newHistory.isEmpty()) {
        newHistory += " ";
    }
    newHistory += m_display + " " + op;

    updateHistory(newHistory);

    if (m_model.hasStrategy() && !m_waitingForNewOperand) {
        m_model.setRightOperand(m_display.toDouble());
        try {
            double result = m_model.calculate();
            updateDisplay(QString::number(result, 'g', 10));
        } catch (...) {
            setError(true);
            return;
        }
    }

    auto strategy = ArithmeticFactory::create(op);
    if (strategy) {
        m_model.setLeftOperand(m_display.toDouble());
        m_model.setStrategy(std::move(strategy));
        m_waitingForNewOperand = true;
    }
}

void CalculatorPresenter::handleCalculate() {
    if (!m_model.hasStrategy()) return;

    try {
        updateHistory(m_history + " " + m_display);

        m_model.setRightOperand(m_display.toDouble());
        double result = m_model.calculate();

        updateDisplay(QString::number(result, 'g', 10));
        m_model.clear();
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
