#include <QtTest>
#include "../src/presenter/CalculatorPresenter.h"
#include "../src/model/ArithmeticFactory.h"

class TestCalculator : public QObject
{
    Q_OBJECT

private slots:
    void init();
    void cleanup();

    // Математика
    void math_addition();
    void math_divisionByZero();

    // Ввод
    void input_ignoreMultipleDots();
    void input_leadingZeros();
    void input_signToggle();

    // Сценарии
    void flow_simpleChain();
    void flow_errorStateLock();

private:
    CalculatorPresenter* presenter;
};

void TestCalculator::init() {
    presenter = new CalculatorPresenter();
}

void TestCalculator::cleanup() {
    delete presenter;
}

// --- ТЕСТЫ ---

void TestCalculator::math_addition() {
    auto strategy = ArithmeticFactory::create("+");
    QCOMPARE(strategy->execute(5.5, 4.5), 10.0);
}

void TestCalculator::math_divisionByZero() {
    auto strategy = ArithmeticFactory::create("/");
    QVERIFY_EXCEPTION_THROWN(strategy->execute(10, 0), std::runtime_error);
}

void TestCalculator::input_ignoreMultipleDots() {
    presenter->processInput("1");
    presenter->processInput(".");
    presenter->processInput("2");
    presenter->processInput("."); // Вторая точка
    presenter->processInput("3");

    QCOMPARE(presenter->display(), QString("1.23"));
}

void TestCalculator::input_leadingZeros() {
    presenter->processInput("0");
    presenter->processInput("0");
    presenter->processInput("7");

    QCOMPARE(presenter->display(), QString("7"));
}

void TestCalculator::input_signToggle() {
    presenter->processInput("5");
    presenter->processInput("+/-");
    QCOMPARE(presenter->display(), QString("-5"));
    presenter->processInput("+/-");
    QCOMPARE(presenter->display(), QString("5"));
}

void TestCalculator::flow_simpleChain() {
    // 10 / 2 + 5 = 10
    presenter->processInput("1");
    presenter->processInput("0");
    presenter->processInput("/");
    presenter->processInput("2");
    presenter->processInput("="); // Результат 5

    presenter->processInput("+");
    presenter->processInput("5");
    presenter->processInput("=");

    QCOMPARE(presenter->display(), QString("10"));
}

void TestCalculator::flow_errorStateLock() {
    // Делим на 0
    presenter->processInput("5");
    presenter->processInput("/");
    presenter->processInput("0");
    presenter->processInput("=");

    QVERIFY(presenter->isError());

    // Пробуем нажать "9" — дисплей не должен меняться (заблокирован)
    presenter->processInput("9");
    QCOMPARE(presenter->isError(), true);
    QVERIFY(presenter->display() != "9");

    // Нажимаем CE — разблокировка
    presenter->processInput("CE");
    QCOMPARE(presenter->isError(), false);
    QCOMPARE(presenter->display(), QString("0"));
}

QTEST_MAIN(TestCalculator)
#include "TestCalculator.moc"
