#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QtQuick>

#include "CategoriesModel.h"

static void initFonts() {
    auto addFont = [](const QString& name) {
        auto fontPath = QString(":/Fonts/%1.ttf").arg(name);
        auto fontId = QFontDatabase::addApplicationFont(fontPath);
        if (fontId < 0) {
            qDebug() << "Failed to add font from resource: " << fontPath.toStdString().c_str();
        }
        else {
            qDebug() << "Font successfully added, index: " << name.toStdString().c_str() << fontId;
        }
    };
    addFont("Roboto-Regular");
    addFont("Roboto-Bold");
    addFont("Roboto-Medium");
    addFont("Manrope-Bold");
    addFont("Manrope-ExtraBold");
    addFont("Manrope-ExtraLight");
    addFont("Manrope-Light");
    addFont("Manrope-Medium");
    addFont("Manrope-Regular");
    addFont("Manrope-SemiBold");
}


int main(int argc, char *argv[]) {
    QGuiApplication app(argc, argv);

    initFonts();

    QScopedPointer<EditorController> editorController(new EditorController);

    const QUrl styleUrl(u"qrc:/HomeFinance/Style.qml"_qs);
    qmlRegisterSingletonType(styleUrl, "HomeFinance", 1, 0, "Style");

    qmlRegisterType<EditorController>("HomeFinance", 1, 0, "EditorController");
    qmlRegisterType<CategoriesModel>("HomeFinance", 1, 0, "CategoriesModel");

    QQmlApplicationEngine engine;

    engine.rootContext()->setContextProperty("globalController", editorController.data());

    const QUrl url(u"qrc:/HomeFinance/Main.qml"_qs);
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreationFailed, &app, []() { QCoreApplication::exit(-1); }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
