#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QApplication>
#include <QQmlContext>
#include <highsprrow.h>
#include <QIcon>
#include <logger.h>
#include "filecontent.h"
#include <QtWebEngine/QtWebEngine>

#include <QDebug>

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
    app.setOrganizationName("Silicool");
    app.setOrganizationDomain("silicool.com");
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
    engine.rootContext()->setContextProperty("pickarmCamera", highSprrow.baseModuleManager->pylonPickarmCamera);

    engine.rootContext()->setContextProperty("baseModuleManager", highSprrow.baseModuleManager);
    engine.rootContext()->setContextProperty("logicManager", highSprrow.logicManager);
    engine.rootContext()->setContextProperty("aaCore", highSprrow.logicManager->aaCore);
    engine.rootContext()->setContextProperty("dataFromCpp", &highSprrow.logicManager->aaCore->aaData_1);
    engine.rootContext()->setContextProperty("dataFromCpp2", &highSprrow.logicManager->aaCore->aaData_2);
    engine.rootContext()->setContextProperty("dataFromCpp3", &highSprrow.logicManager->aaCore->mtf_log);

    //Module
    engine.rootContext()->setContextProperty("sutModule", &highSprrow.baseModuleManager->sut_module);
    engine.rootContext()->setContextProperty("lutModule", &highSprrow.baseModuleManager->lut_module);
    engine.rootContext()->setContextProperty("aaHeadModule", &highSprrow.baseModuleManager->aa_head_module);
    engine.rootContext()->setContextProperty("dispenseModule", &highSprrow.baseModuleManager->dispense_module);
    engine.rootContext()->setContextProperty("workersManager", highSprrow.worker_manager);

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
    QList<QString> temp_names = highSprrow.baseModuleManager->motors.keys();
    engine.rootContext()->setContextProperty("motorsNames", QVariant::fromValue(highSprrow.baseModuleManager->motors.keys()));

    QStringList vision_locations_list = highSprrow.baseModuleManager->vision_locations.keys();
    QList<QObject*> vl_parameter_list;
    for(QString key:vision_locations_list){
        vl_parameter_list<<&(highSprrow.baseModuleManager->vision_locations[key]->parameters);
    }
    //QStringList calibrations_list = highSprrow.baseModuleManager->calibrations.keys();
    //engine.rootContext()->setContextProperty("vision_locations_list",QVariant::fromValue(vision_locations_list));
    engine.rootContext()->setContextProperty("vl_parameter_list",QVariant::fromValue(vl_parameter_list));
    engine.rootContext()->setContextProperty("tray_clipin_parameter",&highSprrow.baseModuleManager->trayClipIn.standards_parameters);
    engine.rootContext()->setContextProperty("tray_clipout_parameter",&highSprrow.baseModuleManager->trayClipOut.standards_parameters);
    //engine.rootContext()->setContextProperty("tray_clipout_parameter",&highSprrow.baseModuleManager->tray_loader_module.tray_clip_out->standards_parameters);
    //qDebug()<<highSprrow.baseModuleManager->tray_loader_module.parameters.ltlPressPos();
    engine.rootContext()->setContextProperty("tray_loader_module_parameters",&highSprrow.baseModuleManager->tray_loader_module.parameters);
    engine.rootContext()->setContextProperty("tray_loader_module",&highSprrow.baseModuleManager->tray_loader_module);
    //PR Params
//    engine.rootContext()->setContextProperty("prSUTDownlookParams", &highSprrow.baseModuleManager->vision_locations[PR_SUT_DOWNLOOK]->parameters);
//    engine.rootContext()->setContextProperty("prAA1LUTUplookParams", &highSprrow.baseModuleManager->vision_locations[PR_AA1_LUT_UPLOOK]->parameters);
//    engine.rootContext()->setContextProperty("prAA1MushroomParams",  &highSprrow.baseModuleManager->vision_locations[PR_AA1_MUSHROOMHEAD]->parameters);
//    engine.rootContext()->setContextProperty("prAA1ToolDownlookParams", &highSprrow.baseModuleManager->vision_locations[PR_AA1_TOOL_DOWNLOOK]->parameters);
//    engine.rootContext()->setContextProperty("prAA1ToolUplookParams", &highSprrow.baseModuleManager->vision_locations[PR_AA1_TOOL_UPLOOK]->parameters);

//    engine.rootContext()->setContextProperty("prLENSLpaLook",&highSprrow.baseModuleManager->vision_locations[PR_LENS_LPALOOK]->parameters);
//    engine.rootContext()->setContextProperty("prVACANCYLpaLook",&highSprrow.baseModuleManager->vision_locations[PR_VACANCY_LPALOOK]->parameters);
//    engine.rootContext()->setContextProperty("prLENSLutLook",&highSprrow.baseModuleManager->vision_locations[PR_LENS_LUTLOOK]->parameters);


    //Params
    engine.rootContext()->setContextProperty("aaHeadParams", &highSprrow.baseModuleManager->aa_head_module.parameters);
    engine.rootContext()->setContextProperty("aaCoreParams", &highSprrow.logicManager->aaCore->parameters);

    engine.rootContext()->setContextProperty("sutParams", &highSprrow.baseModuleManager->sut_module.parameters);
    engine.rootContext()->setContextProperty("sutCarrierParams",  &highSprrow.baseModuleManager->sut_carrier.parameters);

    engine.rootContext()->setContextProperty("lutParams", &highSprrow.baseModuleManager->lut_module.parameters);
    engine.rootContext()->setContextProperty("lutCarrierParams", &highSprrow.baseModuleManager->lut_carrier.parameters);
    engine.rootContext()->setContextProperty("dispenserParams",&highSprrow.baseModuleManager->dispenser.parameters);
    engine.rootContext()->setContextProperty("dispenseParams",&highSprrow.baseModuleManager->dispense_module.parameters);
    engine.rootContext()->setContextProperty("lensPickArmParams",&highSprrow.baseModuleManager->lens_pick_arm.parameters);
//*
    engine.rootContext()->setContextProperty("material_tray",&highSprrow.baseModuleManager->material_tray);
    engine.rootContext()->setContextProperty("first_tray_end_position",&highSprrow.baseModuleManager->material_tray.first_tray_end_position);
    engine.rootContext()->setContextProperty("tray_start_point1",&highSprrow.baseModuleManager->material_tray.parameters[0]->tray_start_position);
    engine.rootContext()->setContextProperty("tray_start_point2",&highSprrow.baseModuleManager->material_tray.parameters[1]->tray_start_position);
    engine.rootContext()->setContextProperty("tray_standard_parameter",&highSprrow.baseModuleManager->material_tray.standards_parameters);
    engine.rootContext()->setContextProperty("lensPickArmModuleParameter",
                                             &highSprrow.baseModuleManager->lens_loader_module.parameters);
    engine.rootContext()->setContextProperty("lut_pr_position1",
                                             &highSprrow.baseModuleManager->lens_loader_module.lut_pr_position1);
    engine.rootContext()->setContextProperty("lut_pr_position2",
                                             &highSprrow.baseModuleManager->lens_loader_module.lut_pr_position2);
    engine.rootContext()->setContextProperty("lut_camera_position",
                                             &highSprrow.baseModuleManager->lens_loader_module.lut_camera_position);
    engine.rootContext()->setContextProperty("lut_picker_position",
                                             &highSprrow.baseModuleManager->lens_loader_module.lut_picker_position);
//*/
    QStringList data = highSprrow.baseModuleManager->motors.keys();
    engine.rootContext()->setContextProperty("motorsNames",data);

    QStringList outputListRaw = highSprrow.baseModuleManager->output_ios.keys();
    QStringList outputList;
    for(QString output:outputListRaw){
        if(output.contains('.')||output.startsWith("SUT1补充")||output.contains("OUT"))
            continue;
        outputList<<output;
    }

    engine.rootContext()->setContextProperty("outputList",outputList);

    QStringList inputListRaw = highSprrow.baseModuleManager->input_ios.keys();
    QStringList inputList;
    for(QString input:inputListRaw){
        if(input.contains('.')||input.contains("IN"))
            continue;
        inputList<<input;
    }
    engine.rootContext()->setContextProperty("inputList",inputList);



    //QImage Provider
    engine.addImageProvider(QLatin1String("uplookCameraImage"), highSprrow.baseModuleManager->pylonUplookCamera);
    engine.addImageProvider(QLatin1String("downlookCameraImage"), highSprrow.baseModuleManager->pylonDownlookCamera);
    engine.addImageProvider(QLatin1String("pickarmCameraImage"), highSprrow.baseModuleManager->pylonPickarmCamera);
    engine.addImageProvider(QLatin1String("preview1"), highSprrow.baseModuleManager->visionModule);
    engine.addImageProvider(QLatin1String("imageGrabberLiveImage"), highSprrow.baseModuleManager->imageGrabberThread->m_pImgProvider);
    engine.addImageProvider(QLatin1String("ocImage1"), highSprrow.logicManager->aaCore->ocImageProvider_1);
    engine.addImageProvider(QLatin1String("sfrImage"), highSprrow.logicManager->aaCore->sfrImageProvider);


    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
