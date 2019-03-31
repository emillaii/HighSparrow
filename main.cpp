#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <highsprrow.h>
#include <QIcon>
#include <logger.h>
#include <motorspositionmodel.h>

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    QGuiApplication app(argc, argv);
    QGuiApplication::setApplicationName("High Sparrow");
    //qInstallMessageHandler(sparrowLogOutput);
    qSetMessagePattern("%{time yyyy-MM-dd hh:mm:ss.zzz} [%{type}] %{file}:%{line}(%{function}):%{message}");
    QIcon::setThemeName("automotive");
    app.setWindowIcon(QIcon(ICON_SPARROW));
    HighSprrow highSprrow;
    QQmlApplicationEngine engine;

    //Object Property Definition
    engine.rootContext()->setContextProperty("highSprrow", &highSprrow);
    engine.rootContext()->setContextProperty("visionModule", highSprrow.visionModule);
    engine.rootContext()->setContextProperty("uplookCamera", highSprrow.pylonUplookCamera);
    engine.rootContext()->setContextProperty("downlookCamera", highSprrow.pylonDownlookCamera);

    //QImage Provider
    engine.addImageProvider(QLatin1String("uplookCameraImage"), highSprrow.pylonUplookCamera);
    engine.addImageProvider(QLatin1String("downlookCameraImage"), highSprrow.pylonDownlookCamera);
    engine.addImageProvider(QLatin1String("preview1"), highSprrow.visionModule);

    //Model Loading
    QList<QObject*> dataList;
    dataList.append(new MotorsPositionModel("AA_X", 1));
    dataList.append(new MotorsPositionModel("AA_Y", 2));
    dataList.append(new MotorsPositionModel("AA_Z", 3));
    dataList.append(new MotorsPositionModel("AA_ROLL", 4));
    dataList.append(new MotorsPositionModel("AA_YAW", 5));
    dataList.append(new MotorsPositionModel("AA_PITCH", 6));

    engine.rootContext()->setContextProperty("motorModels", QVariant::fromValue(dataList));

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;
//    QObject* tabHomeButton = engine.rootObjects().first()->findChild<QObject*>("HomeButtonObject");
//    QObject::connect(tabHomeButton, SIGNAL(clicked()), &highSprrow, SLOT(homeAllMotors()));


    return app.exec();
}
