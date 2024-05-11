#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QtQuick>

#include "CategoriesModel.h"
#include "AccountsModel.h"
#include "OperationsModel.h"

int main(int argc, char *argv[]) {
    QGuiApplication app(argc, argv);
    QScopedPointer<EditorController> editorController(new EditorController);

    const QUrl styleUrl(u"qrc:/HomeFinance/Style.qml"_qs);
    qmlRegisterSingletonType(styleUrl, "HomeFinance", 1, 0, "Style");

    qmlRegisterType<EditorController>("HomeFinance", 1, 0, "EditorController");
    qmlRegisterType<CategoriesModel>("HomeFinance", 1, 0, "CategoriesModel");
    qmlRegisterType<AccountsModel>("HomeFinance", 1, 0, "AccountsModel");
    qmlRegisterType<OperationsModel>("HomeFinance", 1, 0, "OperationsModel");
    qmlRegisterType<OperationsProxyModel>("HomeFinance", 1, 0, "OperationsProxyModel");

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("globalController", editorController.data());

    const QUrl url(u"qrc:/HomeFinance/Main.qml"_qs);
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreationFailed, &app, []() { QCoreApplication::exit(-1); }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
