#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QApplication>
#include <QQmlContext>
#include <highsprrow.h>
#include <QIcon>
#include <logger.h>
#include "filecontent.h"
#include <QtWebEngine/QtWebEngine>

#include "aadata.h"
int main(int argc, char *argv[])
{
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    qRegisterMetaType<cv::Mat>("cv::Mat");
    qRegisterMetaType<std::vector<Sfr_entry>>("std::vector<Sfr_entry>");
    qRegisterMetaType<std::vector<std::vector<Sfr_entry>>>("vector<vector<Sfr_entry>>");
    qRegisterMetaType<sfr::EdgeFilter>("sfr::EdgeFilter");
    qmlRegisterType<FileContent>("FileContentItem", 1, 0, "FileContentItem");
    QApplication app(argc, argv);
    QApplication::setApplicationName("High Sparrow");
    app.setOrganizationName("Sillicool"); //1
    app.setOrganizationDomain("sillicool.com");
    //qInstallMessageHandler(sparrowLogOutput);
    qSetMessagePattern("%{time yyyy-MM-dd hh:mm:ss.zzz} [%{type}] %{file}:%{line}(%{function}):%{message}");
    app.setWindowIcon(QIcon(ICON_SPARROW));

    HighSprrow highSprrow;

    QtWebEngine::initialize();
    QQmlApplicationEngine engine;


    //Object Property Definition
    engine.rootContext()->setContextProperty("highSprrow", &highSprrow);
    engine.rootContext()->setContextProperty("visionModule", highSprrow.baseModuleManager->visionModule);
    engine.rootContext()->setContextProperty("uplookCamera", highSprrow.baseModuleManager->pylonUplookCamera);
    engine.rootContext()->setContextProperty("downlookCamera", highSprrow.baseModuleManager->pylonDownlookCamera);

    engine.rootContext()->setContextProperty("baseModuleManager", highSprrow.baseModuleManager);
    engine.rootContext()->setContextProperty("logicManager", highSprrow.logicManager);
    engine.rootContext()->setContextProperty("dataFromCpp", &highSprrow.logicManager->aaCore->aaData_1);

    //Module
    engine.rootContext()->setContextProperty("sutModule", &highSprrow.baseModuleManager->sut_module);
    engine.rootContext()->setContextProperty("lutModule", &highSprrow.baseModuleManager->lut_module);
    engine.rootContext()->setContextProperty("aaHeadModule", &highSprrow.baseModuleManager->aa_head_module);

    engine.rootContext()->setContextProperty("downlookCalibration", highSprrow.baseModuleManager->calibrations[AA1_DOWNLOOK_CALIBRATION]);
    engine.rootContext()->setContextProperty("uplookCalibration", highSprrow.baseModuleManager->calibrations[AA1_UPLOOK_CALIBRATION]);
    engine.rootContext()->setContextProperty("upDownlookCalibration_up", highSprrow.baseModuleManager->calibrations[AA1_UPDownLOOK_UP_CALIBRATION]);
    engine.rootContext()->setContextProperty("upDownlookCalibration_down", highSprrow.baseModuleManager->calibrations[AA1_UPDownLOOK_DOWN_CALIBRATION]);

    //Device
    engine.rootContext()->setContextProperty("imageGrabber", highSprrow.baseModuleManager->dothinkey);
    engine.rootContext()->setContextProperty("imageGrabberThread", highSprrow.baseModuleManager->imageGrabberThread);

    engine.rootContext()->setContextProperty("lightingController", highSprrow.baseModuleManager->lightingModule);
    //Params
    engine.rootContext()->setContextProperty("aaHeadParams", &highSprrow.baseModuleManager->aa_head_module.parameters);

    engine.rootContext()->setContextProperty("sutParams", &highSprrow.baseModuleManager->sut_module.parameters);
    engine.rootContext()->setContextProperty("sutCarrierParams",  &highSprrow.baseModuleManager->sut_carrier.parameters);

    engine.rootContext()->setContextProperty("lutParams", &highSprrow.baseModuleManager->lut_module.parameters);
    engine.rootContext()->setContextProperty("lutCarrierParams", &highSprrow.baseModuleManager->lut_carrier.parameters);

    //QImage Provider
    engine.addImageProvider(QLatin1String("uplookCameraImage"), highSprrow.baseModuleManager->pylonUplookCamera);
    engine.addImageProvider(QLatin1String("downlookCameraImage"), highSprrow.baseModuleManager->pylonDownlookCamera);
    engine.addImageProvider(QLatin1String("preview1"), highSprrow.baseModuleManager->visionModule);
    engine.addImageProvider(QLatin1String("imageGrabberLiveImage"), highSprrow.baseModuleManager->imageGrabberThread->m_pImgProvider);

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
