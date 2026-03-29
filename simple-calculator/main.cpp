#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "CalculatorPresenter.h"

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QGuiApplication app(argc, argv);


    qmlRegisterType<CalculatorPresenter>("com.calculator.backend", 1, 0, "CalculatorPresenter");

    QQmlApplicationEngine engine;

    const QUrl url(QStringLiteral("qrc:/gui/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
        &app, [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        }, Qt::QueuedConnection);

    engine.load(url);

    return app.exec();
}
