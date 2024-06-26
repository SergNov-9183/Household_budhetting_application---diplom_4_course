#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QtQuick>

#include "AccountsModel.h"
#include "AnalyticsProxyModel.h"
#include "CategoriesModel.h"
#include "OperationsModel.h"
#include "OperationsProxyModel.h"
#include "PriceAnalyticsProxyModel.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    QScopedPointer<EditorController> editorController(new EditorController);

    editorController->sendRequest();

    const QUrl styleUrl(u"qrc:/HomeFinance/Style.qml"_qs);
    qmlRegisterSingletonType(styleUrl, "HomeFinance", 1, 0, "Style");

    qmlRegisterUncreatableMetaObject(PeriodType::staticMetaObject, "HomeFinance", 1, 0, "PeriodType", "Error!!");

    qmlRegisterType<EditorController>("HomeFinance", 1, 0, "EditorController");
    qmlRegisterType<CategoriesModel>("HomeFinance", 1, 0, "CategoriesModel");
    qmlRegisterType<AccountsModel>("HomeFinance", 1, 0, "AccountsModel");
    qmlRegisterType<OperationsModel>("HomeFinance", 1, 0, "OperationsModel");
    qmlRegisterType<OperationsProxyModel>("HomeFinance", 1, 0, "OperationsProxyModel");
    qmlRegisterType<AnalyticsProxyModel>("HomeFinance", 1, 0, "AnalyticsProxyModel");
    qmlRegisterType<PriceAnalyticsProxyModel>("HomeFinance", 1, 0, "PriceAnalyticsProxyModel");

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("globalController", editorController.data());

    const QUrl url(u"qrc:/HomeFinance/Main.qml"_qs);
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreationFailed, &app, []() { QCoreApplication::exit(-1); }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
