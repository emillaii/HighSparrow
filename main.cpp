#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <highsprrow.h>
#include <QIcon>
#include <logger.h>
#include "filecontent.h"
#include <QtWebEngine/QtWebEngine>

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    qRegisterMetaType<cv::Mat>("cv::Mat");
    qRegisterMetaType<std::vector<Sfr_entry>>("std::vector<Sfr_entry>");
    qRegisterMetaType<std::vector<std::vector<Sfr_entry>>>("vector<vector<Sfr_entry>>");
    qRegisterMetaType<sfr::EdgeFilter>("sfr::EdgeFilter");
    qmlRegisterType<FileContent>("FileContentItem", 1, 0, "FileContentItem");
    QGuiApplication app(argc, argv);
    QGuiApplication::setApplicationName("High Sparrow");
    //qInstallMessageHandler(sparrowLogOutput);
    qSetMessagePattern("%{time yyyy-MM-dd hh:mm:ss.zzz} [%{type}] %{file}:%{line}(%{function}):%{message}");
    QIcon::setThemeName("automotive");
    app.setWindowIcon(QIcon(ICON_SPARROW));

    HighSprrow highSprrow;

    QtWebEngine::initialize();
    QQmlApplicationEngine engine;

    //Object Property Definition
    engine.rootContext()->setContextProperty("highSprrow", &highSprrow);
    engine.rootContext()->setContextProperty("visionModule", highSprrow.baseModuleManager->visionModule);
    engine.rootContext()->setContextProperty("uplookCamera", highSprrow.baseModuleManager->pylonUplookCamera);
    engine.rootContext()->setContextProperty("downlookCamera", highSprrow.baseModuleManager->pylonDownlookCamera);
    engine.rootContext()->setContextProperty("aaHeadParams", &highSprrow.aaHeadModule->aaModuleParams);
    engine.rootContext()->setContextProperty("baseModuleManager", highSprrow.baseModuleManager);
    engine.rootContext()->setContextProperty("logicManager", highSprrow.logicManager);

    //QImage Provider
    engine.addImageProvider(QLatin1String("uplookCameraImage"), highSprrow.baseModuleManager->pylonUplookCamera);
    engine.addImageProvider(QLatin1String("downlookCameraImage"), highSprrow.baseModuleManager->pylonDownlookCamera);
    engine.addImageProvider(QLatin1String("preview1"), highSprrow.baseModuleManager->visionModule);

    engine.rootContext()->setContextProperty("motorModels", QVariant::fromValue(highSprrow.aaHeadModule));

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;
//    QObject::connect(engine.rootObjects().first(), SIGNAL(qmlSignal(QString)), &highSprrow, SLOT(cppSlot(QString)));
//    QObject* tabHomeButton = engine.rootObjects().first()->findChild<QObject*>("HomeButtonObject");
//    QObject::connect(tabHomeButton, SIGNAL(clicked()), &highSprrow, SLOT(homeAllMotors()));


    return app.exec();
}
