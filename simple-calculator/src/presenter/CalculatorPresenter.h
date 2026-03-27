#ifndef CALCULATORPRESENTER_H
#define CALCULATORPRESENTER_H

#include <QObject>
#include <QString>
#include <memory>
#include <QMap>
#include <functional>
#include "CalculatorModel.h"


class CalculatorPresenter : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString display READ display NOTIFY displayChanged)
    Q_PROPERTY(QString history READ history NOTIFY historyChanged)
    Q_PROPERTY(bool isError READ isError NOTIFY isErrorChanged)

public:
    explicit CalculatorPresenter(QObject *parent = nullptr);

    QString display() const { return m_display; }
    QString history() const { return m_history; }
    bool isError() const { return m_isError; }


    Q_INVOKABLE void processInput(const QString &key);

signals:
    void displayChanged();
    void historyChanged();
    void isErrorChanged();

private:
    void handleDigit(const QString &digit);
    void handleOperation(const QString &op);
    void handleCalculate();
    void handleClear();
    void handleSignChange();

    void updateDisplay(const QString &value);
    void updateHistory(const QString &value);
    void setError(bool error);

private:
    CalculatorModel m_model;

    QString m_display;
    QString m_history;
    bool m_isError;
    bool m_waitingForNewOperand;

    using ActionHandler = std::function<void()>;
    QMap<QString, ActionHandler> m_actions;
    void initializeActions();
};

#endif // CALCULATORPRESENTER_H
