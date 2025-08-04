#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "DatabaseManager.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    // Create and open the DatabaseManager
    DatabaseManager dbManager;
    dbManager.open("localhost", 5432, "narrative_db", "postgres", "2204");

    // Register dbManager as a context property for QML
    engine.rootContext()->setContextProperty("dbManager", &dbManager);


    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);

    engine.loadFromModule("NarrativeDesignApp", "Main");

    return app.exec();
}
