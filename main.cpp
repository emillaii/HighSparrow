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
#include "checkprocessmodel.h"
#include "logmodel.h"
#include "traymapmodel.h"

#include <QtWidgets/QApplication>
#include <windows.h>
#include <DbgHelp.h>
#pragma comment(lib,"Dbghelp.lib")

long  __stdcall CrashInfocallback(_EXCEPTION_POINTERS *pexcp)
{
    //创建 Dump 文件
    HANDLE hDumpFile = ::CreateFile(
        L"MEMORY.DMP",
        GENERIC_WRITE,
        0,
        NULL,
        CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );
    if (hDumpFile != INVALID_HANDLE_VALUE)
    {
        //Dump信息
        MINIDUMP_EXCEPTION_INFORMATION dumpInfo;
        dumpInfo.ExceptionPointers = pexcp;
        dumpInfo.ThreadId = GetCurrentThreadId();
        dumpInfo.ClientPointers = TRUE;
        //写入Dump文件内容
        ::MiniDumpWriteDump(
            GetCurrentProcess(),
            GetCurrentProcessId(),
            hDumpFile,
            MiniDumpNormal,
            &dumpInfo,
            NULL,
            NULL
        );
    }
    return 0;
}
int main(int argc, char *argv[])
{
    ::SetUnhandledExceptionFilter((LPTOP_LEVEL_EXCEPTION_FILTER)CrashInfocallback);
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

    // initialize logging system
    initLoggingSystem();
//    qInstallMessageHandler(messageLogger);
	//qInstallMessageHandler(sparrowLogOutput);
    qSetMessagePattern("%{time yyyy-MM-dd hh:mm:ss.zzz} [%{type}] %{file}:%{line}(%{function}):%{message}");

    app.setWindowIcon(QIcon(ICON_SPARROW));

    HighSprrow highSprrow;

    QtWebEngine::initialize();
    QQmlApplicationEngine engine;

    qmlRegisterUncreatableType<TrayMapModel>("HighSprrow.Models", 1, 0, "TrayMapModel", "Tray map model should only be created in cpp code");

    //Object Property Definition
    engine.rootContext()->setContextProperty("lensTrayModel", TrayMapModel::instance(TrayMapModel::LensTray));
    engine.rootContext()->setContextProperty("sensorTrayModel", TrayMapModel::instance(TrayMapModel::SensorTray));
    engine.rootContext()->setContextProperty("productTrayModel", TrayMapModel::instance(TrayMapModel::ProductTray));
    engine.rootContext()->setContextProperty("rejectTrayModel", TrayMapModel::instance(TrayMapModel::RejectTray));
    engine.rootContext()->setContextProperty("logModel", LogModel::instance());
    engine.rootContext()->setContextProperty("highSprrow", &highSprrow);
    engine.rootContext()->setContextProperty("visionModule", highSprrow.baseModuleManager->visionModule);
    engine.rootContext()->setContextProperty("uplookCamera", highSprrow.baseModuleManager->pylonUplookCamera);
    engine.rootContext()->setContextProperty("downlookCamera", highSprrow.baseModuleManager->pylonDownlookCamera);
    engine.rootContext()->setContextProperty("pickarmCamera", highSprrow.baseModuleManager->pylonPickarmCamera);
    QStringList workerNameList;
    for(QString output:highSprrow.worker_manager->getWorkersNames()){
        workerNameList<<output;
    }

    engine.rootContext()->setContextProperty("workerNameList",workerNameList);
    engine.rootContext()->setContextProperty("baseModuleManager", highSprrow.baseModuleManager);

    engine.rootContext()->setContextProperty("logicManager", highSprrow.logicManager);
    engine.rootContext()->setContextProperty("aaNewCore", &highSprrow.baseModuleManager->aaCoreNew);
    engine.rootContext()->setContextProperty("dataFromCpp3", &highSprrow.baseModuleManager->aaCoreNew.mtf_log);
    engine.rootContext()->setContextProperty("dataFromCpp", &highSprrow.baseModuleManager->aaCoreNew.aaData_1);
    engine.rootContext()->setContextProperty("dataFromCpp2", &highSprrow.baseModuleManager->aaCoreNew.aaData_2);
    engine.rootContext()->setContextProperty("dataFromIntensityProfile", &highSprrow.baseModuleManager->aaCoreNew.intensity_profile);
    //Module
    engine.rootContext()->setContextProperty("sutModule", &highSprrow.baseModuleManager->sut_module);
    engine.rootContext()->setContextProperty("lutModule", &highSprrow.baseModuleManager->lut_module);
    engine.rootContext()->setContextProperty("aaHeadModule", &highSprrow.baseModuleManager->aa_head_module);
    engine.rootContext()->setContextProperty("dispenseModule", &highSprrow.baseModuleManager->dispense_module);
    engine.rootContext()->setContextProperty("workersManager", highSprrow.worker_manager);
    qmlRegisterType<SensorLoaderModule>("SomeLib",1,1,"SensorLoaderModule");
    qmlRegisterType<AACoreNew>("AACoreNew",1,1,"AACoreNew");
    engine.rootContext()->setContextProperty("sensorLoaderModule", &highSprrow.baseModuleManager->sensor_loader_module);
    engine.rootContext()->setContextProperty("lutClient", highSprrow.baseModuleManager->lutClient);
    engine.rootContext()->setContextProperty("sensorTrayLoaderModule", &highSprrow.baseModuleManager->sensor_tray_loder_module);
    engine.rootContext()->setContextProperty("dothinkey", highSprrow.baseModuleManager->dothinkey);



    //Position
    engine.rootContext()->setContextProperty("aaHeadMushroomPosition", &highSprrow.baseModuleManager->aa_head_module.mushroom_position);
    engine.rootContext()->setContextProperty("sutLoadPosition", &highSprrow.baseModuleManager->sut_module.load_position);

    engine.rootContext()->setContextProperty("sutToolUplookPosition", &highSprrow.baseModuleManager->sut_module.tool_uplook_positon);
    engine.rootContext()->setContextProperty("sutToollookPosition", &highSprrow.baseModuleManager->sut_module.tool_downlook_position);
    engine.rootContext()->setContextProperty("sutDownlookPosition", &highSprrow.baseModuleManager->sut_module.downlook_position);
    engine.rootContext()->setContextProperty("sutMushroomPosition", &highSprrow.baseModuleManager->sut_module.mushroom_positon);

    engine.rootContext()->setContextProperty("lutLoadPosition", &highSprrow.baseModuleManager->lut_module.load_position);
    engine.rootContext()->setContextProperty("lutUnloadPosition", &highSprrow.baseModuleManager->lut_module.lut_downlook_unload_position);
    engine.rootContext()->setContextProperty("lutLoadUplookPosition", &highSprrow.baseModuleManager->lut_module.load_uplook_position);
    engine.rootContext()->setContextProperty("lutDownlookLoadPosition", &highSprrow.baseModuleManager->lut_module.lut_downlook_load_position);
    engine.rootContext()->setContextProperty("lutDownlookUnloadPosition", &highSprrow.baseModuleManager->lut_module.lut_downlook_unload_position);

    engine.rootContext()->setContextProperty("lutUpDownlookPositionAA1", &highSprrow.baseModuleManager->lut_module.aa1_updownlook_position);
    engine.rootContext()->setContextProperty("lutPickLensPositionAA1", &highSprrow.baseModuleManager->lut_module.aa1_picklens_position);
    engine.rootContext()->setContextProperty("lutUnPickLensPositionAA1", &highSprrow.baseModuleManager->lut_module.aa1_unpicklens_position);
    engine.rootContext()->setContextProperty("lutUplookPositionAA1", &highSprrow.baseModuleManager->lut_module.aa1_uplook_position);

    engine.rootContext()->setContextProperty("lutUpDownlookPositionAA2", &highSprrow.baseModuleManager->lut_module.aa2_updownlook_position);
    engine.rootContext()->setContextProperty("lutPickLensPositionAA2", &highSprrow.baseModuleManager->lut_module.aa2_picklens_position);
    engine.rootContext()->setContextProperty("lutUnPickLensPositionAA2", &highSprrow.baseModuleManager->lut_module.aa2_unpicklens_position);
    engine.rootContext()->setContextProperty("lutUplookPositionAA2", &highSprrow.baseModuleManager->lut_module.aa2_uplook_position);

    QString dirPath = app.applicationDirPath();
    dirPath += "/..";
    engine.rootContext()->setContextProperty("dirPath",dirPath);
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

    //Params
    engine.rootContext()->setContextProperty("aaHeadParams", &highSprrow.baseModuleManager->aa_head_module.parameters);
    engine.rootContext()->setContextProperty("aaCoreParams", &highSprrow.baseModuleManager->aaCoreNew.parameters);
    engine.rootContext()->setContextProperty("aaCoreStates", &highSprrow.baseModuleManager->aaCoreNew.states);

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

    engine.rootContext()->setContextProperty("lens_loader_parameter",
                                             &highSprrow.baseModuleManager->lens_loader_module.parameters);

    engine.rootContext()->setContextProperty("sensorPickArmParams",&highSprrow.baseModuleManager->sensor_pickarm.parameters);
    engine.rootContext()->setContextProperty("sensorLoaderParameter",
                                             &highSprrow.baseModuleManager->sensor_loader_module.parameters);
    engine.rootContext()->setContextProperty("sut_pr_position1",
                                             &highSprrow.baseModuleManager->sensor_loader_module.sut1_pr_position);
    engine.rootContext()->setContextProperty("sut_pr_position2",
                                             &highSprrow.baseModuleManager->sensor_loader_module.sut2_pr_position);
    engine.rootContext()->setContextProperty("pick1_offset",
                                             &highSprrow.baseModuleManager->sensor_loader_module.picker1_offset);
    engine.rootContext()->setContextProperty("pick2_offset",
                                             &highSprrow.baseModuleManager->sensor_loader_module.picker2_offset);
    engine.rootContext()->setContextProperty("sensor_uph",&highSprrow.baseModuleManager->sensor_loader_module.sensor_uph);
    engine.rootContext()->setContextProperty("left_sensor_uph",&highSprrow.baseModuleManager->sensor_loader_module.left_sensor_uph);
    engine.rootContext()->setContextProperty("right_sensor_uph",&highSprrow.baseModuleManager->sensor_loader_module.right_sensor_uph);
    engine.rootContext()->setContextProperty("product_uph",&highSprrow.baseModuleManager->sensor_loader_module.product_uph);
    engine.rootContext()->setContextProperty("left_product_uph",&highSprrow.baseModuleManager->sensor_loader_module.left_product_uph);
    engine.rootContext()->setContextProperty("right_product_uph",&highSprrow.baseModuleManager->sensor_loader_module.right_product_uph);
    engine.rootContext()->setContextProperty("comprehensive_uph",&highSprrow.baseModuleManager->sensor_loader_module.comprehensive_uph);
    engine.rootContext()->setContextProperty("left_comprehensive_uph",&highSprrow.baseModuleManager->sensor_loader_module.left_comprehensive_uph);
    engine.rootContext()->setContextProperty("right_comprehensive_uph",&highSprrow.baseModuleManager->sensor_loader_module.right_comprehensive_uph);
    engine.rootContext()->setContextProperty("lens_uph",&highSprrow.baseModuleManager->lut_module.lens_uph);
    engine.rootContext()->setContextProperty("left_lens_uph",&highSprrow.baseModuleManager->lut_module.left_lens_uph);
    engine.rootContext()->setContextProperty("right_lens_uph",&highSprrow.baseModuleManager->lut_module.right_lens_uph);

    engine.rootContext()->setContextProperty("systerm_param",&highSprrow.baseModuleManager->paramers);

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


    QStringList logList;
    QFile file("./log/system_log/log.txt");
    if(!file.open(QIODevice::ReadOnly|QIODevice::Text)){
        qDebug()<<"找不到log文件";
    }else{
        QTextStream in(&file);
        QString line = in.readLine();
        while(!line.isNull()){
            logList<<line;
            line = in.readLine();
        }
    }
    engine.rootContext()->setContextProperty("logList",logList);

    engine.rootContext()->setContextProperty("sensor_clip_standard_parameter",
                                             &highSprrow.baseModuleManager->sensor_clip_stand);
    engine.rootContext()->setContextProperty("sensor_exit_clip_parameter",
                                             &highSprrow.baseModuleManager->sensor_tray_loder_module.exit_clip->parameters);
    engine.rootContext()->setContextProperty("sensor_entrance_clip_parameter",
                                             &highSprrow.baseModuleManager->sensor_tray_loder_module.entrance_clip->parameters);
    engine.rootContext()->setContextProperty("sensor_tray_loader_module_parameter",
                                             &highSprrow.baseModuleManager->sensor_tray_loder_module.parameters);


    CheckProcessModel checkProcessModel;
    engine.rootContext()->setContextProperty("checkProcessModel",&checkProcessModel);
//    QStringList vacuumNames = highSprrow.baseModuleManager->vacuums.keys();
//    QStringList cylinderNames = highSprrow.baseModuleManager->cylinder.keys();
//    engine.rootContext()->setContextProperty("vacuumNames",vacuumNames);
//    engine.rootContext()->setContextProperty("cylinderNames",cylinderNames);

    //QImage Provider
    engine.addImageProvider(QLatin1String("uplookCameraImage"), highSprrow.baseModuleManager->pylonUplookCamera);
    engine.addImageProvider(QLatin1String("downlookCameraImage"), highSprrow.baseModuleManager->pylonDownlookCamera);
    engine.addImageProvider(QLatin1String("pickarmCameraImage"), highSprrow.baseModuleManager->pylonPickarmCamera);
    engine.addImageProvider(QLatin1String("preview1"), highSprrow.baseModuleManager->visionModule);
    engine.addImageProvider(QLatin1String("imageGrabberLiveImage"), highSprrow.baseModuleManager->imageGrabberThread->m_pImgProvider);
    engine.addImageProvider(QLatin1String("ocImage1"), highSprrow.baseModuleManager->aaCoreNew.ocImageProvider_1);
    engine.addImageProvider(QLatin1String("sfrImage"), highSprrow.baseModuleManager->aaCoreNew.sfrImageProvider);
engine.addImageProvider(QLatin1String("dispenseImage"), highSprrow.baseModuleManager->aaCoreNew.dispenseImageProvider);
    engine.addImageProvider(QLatin1String("aaCoreTuningImage"), highSprrow.baseModuleManager->aaCoreNew.aaCoreTuningProvider);
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;


    // test code just to demonstrate how to use these models
    TrayMapModel* model = TrayMapModel::instance(TrayMapModel::LensTray);
    model->resetModel(8, 5, TrayMapModel::StatusIdle);
    model->setUnitStatus(1, TrayMapModel::StatusGood);
    model->setUnitStatus(4, TrayMapModel::StatusNG);

    model = TrayMapModel::instance(TrayMapModel::SensorTray);
    model->resetModel(8, 5, TrayMapModel::StatusIdle);
    model->setUnitStatus(2, TrayMapModel::StatusGood);
    model->setUnitStatus(14, TrayMapModel::StatusNG);

    model = TrayMapModel::instance(TrayMapModel::ProductTray);
    model->resetModel(8, 5, TrayMapModel::StatusIdle);
    model->setUnitStatus(9, TrayMapModel::StatusGood);
    model->setUnitStatus(21, TrayMapModel::StatusNG);

    model = TrayMapModel::instance(TrayMapModel::RejectTray);
    model->resetModel(8, 5, TrayMapModel::StatusIdle);
    model->setUnitStatus(27, TrayMapModel::StatusGood);
    model->setUnitStatus(34, TrayMapModel::StatusNG);

    return app.exec();
}
