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

    // Сценарии цепочек и истории
    void flow_simpleChain();      // 10 / 2 + 5 = 15 (с проверкой истории)
    void flow_multiChainNoEqual(); // 75 * 4 * 4 (без промежуточного =)
    void flow_resetAfterEqual();   // Ввод цифры после "=" должен стереть историю
    void flow_continueAfterEqual(); // Ввод знака после "=" должен продолжить счет
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
    presenter->processInput(".");
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
}

// Проверка 10 / 2 + 5 = 10
void TestCalculator::flow_simpleChain() {
    presenter->processInput("1");
    presenter->processInput("0");
    presenter->processInput("/");
    presenter->processInput("2");
    presenter->processInput("=");

    QCOMPARE(presenter->display(), QString("5"));
    QCOMPARE(presenter->history(), QString("10 / 2")); // История сохраняется после "="

    presenter->processInput("+"); // Нажали плюс после равно
    QCOMPARE(presenter->history(), QString("5 +")); // История обновилась, дублей нет

    presenter->processInput("5");
    presenter->processInput("=");
    QCOMPARE(presenter->display(), QString("10"));
}

// Тест кейса 75 * 4 * 4 = 1200 (без промежуточных =)
void TestCalculator::flow_multiChainNoEqual() {
    presenter->processInput("7");
    presenter->processInput("5");
    presenter->processInput("*");
    presenter->processInput("4");

    // Нажимаем второй раз "*" -> должен появиться промежуточный итог 300
    presenter->processInput("*");
    QCOMPARE(presenter->display(), QString("300"));
    QCOMPARE(presenter->history(), QString("75 * 4 *"));

    presenter->processInput("4");
    presenter->processInput("=");

    QCOMPARE(presenter->display(), QString("1200"));
    QCOMPARE(presenter->history(), QString("75 * 4 * 4"));
}

// Проверка: цифра после "=" стирает всё
void TestCalculator::flow_resetAfterEqual() {
    presenter->processInput("5");
    presenter->processInput("+");
    presenter->processInput("5");
    presenter->processInput("="); // Дисплей 10, История 5 + 5

    presenter->processInput("9"); // Новое число
    QCOMPARE(presenter->display(), QString("9"));
    QCOMPARE(presenter->history(), QString("")); // История стерта
}

// Проверка: знак после "=" продолжает цепочку
void TestCalculator::flow_continueAfterEqual() {
    presenter->processInput("1");
    presenter->processInput("0");
    presenter->processInput("+");
    presenter->processInput("2");
    presenter->processInput("="); // 12

    presenter->processInput("*");
    QCOMPARE(presenter->history(), QString("12 *")); // Взяли результат и приставили знак

    presenter->processInput("3");
    presenter->processInput("=");
    QCOMPARE(presenter->display(), QString("36"));
}

void TestCalculator::flow_errorStateLock() {
    presenter->processInput("5");
    presenter->processInput("/");
    presenter->processInput("0");
    presenter->processInput("=");
    QVERIFY(presenter->isError());

    presenter->processInput("9");
    QVERIFY(presenter->display() != "9");

    presenter->processInput("CE");
    QCOMPARE(presenter->isError(), false);
    QCOMPARE(presenter->display(), QString("0"));
    QCOMPARE(presenter->history(), QString(""));
}

QTEST_MAIN(TestCalculator)
#include "TestCalculator.moc"
