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
    void input_startWithEmpty();
    void input_ignoreMultipleDots();
    void input_leadingZeros();
    void input_signToggle();
    void input_dotAsFirstChar();


    void flow_livePreview();        // Проверка: результат считается при наборе цифр
    void flow_simpleChain();       // 10 / 2 + 5 = 15
    void flow_multiChainNoEqual(); // 75 * 4 * 4
    void flow_resetAfterEqual();   // Ввод цифры после "="
    void flow_clearEverything();    // Проверка CE


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

void TestCalculator::input_startWithEmpty() {
    // Начальное состояние
    QCOMPARE(presenter->display(), QString(""));

    // Ввод первой цифры
    presenter->processInput("8");
    QCOMPARE(presenter->display(), QString("8"));
    QCOMPARE(presenter->history(), QString("8"));
}
void TestCalculator::input_ignoreMultipleDots() {
    presenter->processInput("1");
    presenter->processInput(".");
    presenter->processInput("2");
    presenter->processInput("."); // Должна игнорироваться
    presenter->processInput("3");


    QCOMPARE(presenter->display(), QString("1.23"));
    QCOMPARE(presenter->history(), QString("1.23"));
}

void TestCalculator::input_leadingZeros() {
    presenter->processInput("0");
    presenter->processInput("0");
    presenter->processInput("7");
    QCOMPARE(presenter->display(), QString("7"));
    QCOMPARE(presenter->history(), QString("7"));
}

void TestCalculator::input_signToggle() {
    presenter->processInput("5");
    presenter->processInput("+/-");
    // Если это первое число, оно меняет знак
    QCOMPARE(presenter->display(), QString("-5"));
}

void TestCalculator::input_dotAsFirstChar() {
    presenter->processInput(".");

    QCOMPARE(presenter->display(), QString("0."));
    QCOMPARE(presenter->history(), QString("0."));

    presenter->processInput("5");
    QCOMPARE(presenter->display(), QString("0.5"));
    QCOMPARE(presenter->history(), QString("0.5"));
}

void TestCalculator::flow_livePreview() {
    presenter->processInput("7");
    presenter->processInput("5");
    presenter->processInput("*");

    presenter->processInput("4");
    // Как только нажали "4", дисплей должен показать 300, а история 75 * 4
    QCOMPARE(presenter->display(), QString("300"));
    QCOMPARE(presenter->history(), QString("75 * 4"));
}

void TestCalculator::flow_simpleChain() {
    presenter->processInput("1");
    presenter->processInput("0");
    presenter->processInput("/");
    presenter->processInput("2");

    QCOMPARE(presenter->display(), QString("5"));

    presenter->processInput("=");
    QCOMPARE(presenter->display(), QString("5"));
    QCOMPARE(presenter->history(), QString("10 / 2"));
}

void TestCalculator::flow_multiChainNoEqual() {
    presenter->processInput("7");
    presenter->processInput("5");
    presenter->processInput("*");
    presenter->processInput("4");

    presenter->processInput("*");
    QCOMPARE(presenter->history(), QString("75 * 4 * "));
    QCOMPARE(presenter->display(), QString("300"));

    presenter->processInput("4");
    QCOMPARE(presenter->display(), QString("1200"));
    QCOMPARE(presenter->history(), QString("75 * 4 * 4"));

    presenter->processInput("=");
    QCOMPARE(presenter->display(), QString("1200"));
    QCOMPARE(presenter->history(), QString("75 * 4 * 4"));
}

void TestCalculator::flow_resetAfterEqual() {
    presenter->processInput("5");
    presenter->processInput("+");
    presenter->processInput("5");
    presenter->processInput("="); // 10

    presenter->processInput("9");
    QCOMPARE(presenter->display(), QString("9"));
    QCOMPARE(presenter->history(), QString("9")); // История начинается заново с "9"
}

void TestCalculator::flow_clearEverything() {
    presenter->processInput("1");
    presenter->processInput("0");
    presenter->processInput("+");
    presenter->processInput("5");

    presenter->processInput("CE");

    QCOMPARE(presenter->display(), QString(""));
    QCOMPARE(presenter->history(), QString(""));
    QCOMPARE(presenter->isError(), false);

    // Проверка, что после CE ввод чистый
    presenter->processInput("7");
    QCOMPARE(presenter->display(), QString("7"));
    QCOMPARE(presenter->history(), QString("7"));
}


QTEST_MAIN(TestCalculator)
#include "TestCalculator.moc"
