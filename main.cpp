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
    app.setOrganizationName("Sillicool");
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
    engine.rootContext()->setContextProperty("aaCore", highSprrow.logicManager->aaCore);
    engine.rootContext()->setContextProperty("dataFromCpp", &highSprrow.logicManager->aaCore->aaData_1);
    engine.rootContext()->setContextProperty("dataFromCpp2", &highSprrow.logicManager->aaCore->aaData_2);

    //Module
    engine.rootContext()->setContextProperty("sutModule", &highSprrow.baseModuleManager->sut_module);
    engine.rootContext()->setContextProperty("lutModule", &highSprrow.baseModuleManager->lut_module);
    engine.rootContext()->setContextProperty("aaHeadModule", &highSprrow.baseModuleManager->aa_head_module);

    //Position
    engine.rootContext()->setContextProperty("aaHeadMushroomPosition", &highSprrow.baseModuleManager->aa_head_module.mushroom_position);
    engine.rootContext()->setContextProperty("sutLoadPosition", &highSprrow.baseModuleManager->sut_module.load_position);

    engine.rootContext()->setContextProperty("sutToolUplookPosition", &highSprrow.baseModuleManager->sut_module.tool_uplook_positon);
    engine.rootContext()->setContextProperty("sutToollookPosition", &highSprrow.baseModuleManager->sut_module.tool_downlook_position);
    engine.rootContext()->setContextProperty("sutDownlookPosition", &highSprrow.baseModuleManager->sut_module.downlook_position);
    engine.rootContext()->setContextProperty("sutMushroomPosition", &highSprrow.baseModuleManager->sut_module.mushroom_positon);

    engine.rootContext()->setContextProperty("lutLoadPosition", &highSprrow.baseModuleManager->lut_module.load_position);
    engine.rootContext()->setContextProperty("lutUnloadPosition", &highSprrow.baseModuleManager->lut_module.unload_position);
    engine.rootContext()->setContextProperty("lutLoadUplookPosition", &highSprrow.baseModuleManager->lut_module.load_uplook_position);

    engine.rootContext()->setContextProperty("lutUpDownlookPositionAA1", &highSprrow.baseModuleManager->lut_module.aa1_updownlook_position);
    engine.rootContext()->setContextProperty("lutPickLensPositionAA1", &highSprrow.baseModuleManager->lut_module.aa1_picklens_position);
    engine.rootContext()->setContextProperty("lutUnPickLensPositionAA1", &highSprrow.baseModuleManager->lut_module.aa1_unpicklens_position);
    engine.rootContext()->setContextProperty("lutUplookPositionAA1", &highSprrow.baseModuleManager->lut_module.aa1_uplook_position);

    engine.rootContext()->setContextProperty("lutUpDownlookPositionAA2", &highSprrow.baseModuleManager->lut_module.aa2_updownlook_position);
    engine.rootContext()->setContextProperty("lutPickLensPositionAA2", &highSprrow.baseModuleManager->lut_module.aa2_picklens_position);
    engine.rootContext()->setContextProperty("lutUnPickLensPositionAA2", &highSprrow.baseModuleManager->lut_module.aa2_unpicklens_position);
    engine.rootContext()->setContextProperty("lutUplookPositionAA2", &highSprrow.baseModuleManager->lut_module.aa2_uplook_position);

    //Device
    engine.rootContext()->setContextProperty("imageGrabber", highSprrow.baseModuleManager->dothinkey);
    engine.rootContext()->setContextProperty("imageGrabberThread", highSprrow.baseModuleManager->imageGrabberThread);

    engine.rootContext()->setContextProperty("lightingController", highSprrow.baseModuleManager->lightingModule);

    //PR Params
    engine.rootContext()->setContextProperty("prSUTDownlookParams", &highSprrow.baseModuleManager->vision_locations[PR_SUT_DOWNLOOK]->parameters);
    engine.rootContext()->setContextProperty("prAA1LUTUplookParams", &highSprrow.baseModuleManager->vision_locations[PR_AA1_LUT_UPLOOK]->parameters);
    engine.rootContext()->setContextProperty("prAA1MushroomParams",  &highSprrow.baseModuleManager->vision_locations[PR_AA1_MUSHROOMHEAD]->parameters);
//    engine.rootContext()->setContextProperty("prDownlookParams", &highSprrow.baseModuleManager->vision_locations[PR_SUT_DOWNLOOK]->parameters);
//    engine.rootContext()->setContextProperty("prDownlookParams", &highSprrow.baseModuleManager->vision_locations[PR_SUT_DOWNLOOK]->parameters);

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
    engine.addImageProvider(QLatin1String("ocImage1"), highSprrow.logicManager->aaCore->ocImageProvider_1);

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
