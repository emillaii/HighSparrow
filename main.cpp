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
    //Module
    engine.rootContext()->setContextProperty("sh_lsut_Module", &highSprrow.baseModuleManager->sh_lsut_module);

    engine.rootContext()->setContextProperty("aaHeadModule", &highSprrow.baseModuleManager->aa_head_module);
    engine.rootContext()->setContextProperty("dispenseModule", &highSprrow.baseModuleManager->dispense_module);
    engine.rootContext()->setContextProperty("workersManager", highSprrow.worker_manager);
    engine.rootContext()->setContextProperty("sensorLoaderModule", &highSprrow.baseModuleManager->sensor_loader_module);
    engine.rootContext()->setContextProperty("lutClient", highSprrow.baseModuleManager->lutClient);
    engine.rootContext()->setContextProperty("sensorTrayLoaderModule", &highSprrow.baseModuleManager->sensor_tray_loder_module);
    engine.rootContext()->setContextProperty("sh_materialLoaderModule",&highSprrow.baseModuleManager->single_station_material_loader_module);
    engine.rootContext()->setContextProperty("materialLoaderParameter",&highSprrow.baseModuleManager->single_station_material_loader_module.parameters);
    //Register QML - C++ ENUM
    qmlRegisterType<SensorLoaderModule>("SomeLib",1,0,"SensorLoaderModule");
    qmlRegisterType<SingleheadLSutModule>("SingleheadLSutModuleLib",1,0,"SingleheadLSutModule");
    qmlRegisterType<SingleHeadMachineMaterialLoaderModule>("SingleHeadMaterialLoaderLib",1,0,"MaterialLoaderModule");
    qmlRegisterType<AACoreNew>("AACoreNew",1,1,"AACoreNew");
    //AA Head Position
    engine.rootContext()->setContextProperty("aaHeadMushroomPosition", &highSprrow.baseModuleManager->aa_head_module.mushroom_position);

    //LSUT Position
    engine.rootContext()->setContextProperty("lsutMushroomPosition", &highSprrow.baseModuleManager->sh_lsut_module.mushroom_position);
    engine.rootContext()->setContextProperty("lsutLoadPosition", &highSprrow.baseModuleManager->sh_lsut_module.load_position);
    engine.rootContext()->setContextProperty("lsutPRPosition", &highSprrow.baseModuleManager->sh_lsut_module.downlook_position);
    engine.rootContext()->setContextProperty("lsutGripperPosition", &highSprrow.baseModuleManager->sh_lsut_module.gripper_position);
    engine.rootContext()->setContextProperty("lsutCalibrationPosition", &highSprrow.baseModuleManager->sh_lsut_module.calibration_position);
    engine.rootContext()->setContextProperty("lsutSafetyPosition", &highSprrow.baseModuleManager->sh_lsut_module.safety_position);
    engine.rootContext()->setContextProperty("lsutParameter", &highSprrow.baseModuleManager->sh_lsut_module.parameters);

    //Material Loader Position
    engine.rootContext()->setContextProperty("sh_sut_pr_position",&highSprrow.baseModuleManager->single_station_material_loader_module.sut_pr_position);
    engine.rootContext()->setContextProperty("sh_lut_pr_position",&highSprrow.baseModuleManager->single_station_material_loader_module.lut_pr_position);
    engine.rootContext()->setContextProperty("sh_updownlook_up_position",&highSprrow.baseModuleManager->single_station_material_loader_module.updownlook_up_position);
    engine.rootContext()->setContextProperty("sh_updownlook_down_position",&highSprrow.baseModuleManager->single_station_material_loader_module.updownlook_down_position);
    engine.rootContext()->setContextProperty("sh_lens_suction_offset",&highSprrow.baseModuleManager->single_station_material_loader_module.lens_suction_offset);
    engine.rootContext()->setContextProperty("sh_sensor_suction_offset",&highSprrow.baseModuleManager->single_station_material_loader_module.sensor_suction_offset);

    engine.rootContext()->setContextProperty("sh_sensor_tray",&highSprrow.baseModuleManager->sensor_tray);
    engine.rootContext()->setContextProperty("sh_sensor_first_tray_end_position",&highSprrow.baseModuleManager->sensor_tray.first_tray_end_position);
    engine.rootContext()->setContextProperty("sh_sensor_tray_start_point1",&highSprrow.baseModuleManager->sensor_tray.parameters[0]->tray_start_position);
    //engine.rootContext()->setContextProperty("tray_start_point2",&highSprrow.baseModuleManager->material_tray.parameters[1]->tray_start_position);
    engine.rootContext()->setContextProperty("sh_sensor_tray_standard_parameter",&highSprrow.baseModuleManager->sensor_tray.standards_parameters);

    engine.rootContext()->setContextProperty("sh_lens_tray",&highSprrow.baseModuleManager->lens_tray);
    engine.rootContext()->setContextProperty("sh_lens_first_tray_end_position",&highSprrow.baseModuleManager->lens_tray.first_tray_end_position);
    engine.rootContext()->setContextProperty("sh_lens_tray_start_point1",&highSprrow.baseModuleManager->lens_tray.parameters[0]->tray_start_position);
    engine.rootContext()->setContextProperty("sh_lens_tray_start_point2",&highSprrow.baseModuleManager->lens_tray.parameters[1]->tray_start_position);
    engine.rootContext()->setContextProperty("sh_lens_tray_standard_parameter",&highSprrow.baseModuleManager->lens_tray.standards_parameters);

    QString dirPath = app.applicationDirPath();
    dirPath += "/..";
    engine.rootContext()->setContextProperty("dirPath",dirPath);
    //Device
    engine.rootContext()->setContextProperty("imageGrabber", highSprrow.baseModuleManager->dothinkey);
    engine.rootContext()->setContextProperty("imageGrabberThread", highSprrow.baseModuleManager->imageGrabberThread);
    engine.rootContext()->setContextProperty("lightingController", highSprrow.baseModuleManager->lightingModule);
    QList<QString> temp_names = highSprrow.baseModuleManager->motors.keys();
    engine.rootContext()->setContextProperty("motorsNames", QVariant::fromValue(highSprrow.baseModuleManager->motors.keys()));

    QStringList vacuums_name = highSprrow.baseModuleManager->vacuums.keys();
    QList<QObject*> vacuums_list;
    for(QString key:vacuums_name){
        vacuums_list<<&(highSprrow.baseModuleManager->vacuums[key]->parameters);
    }
    engine.rootContext()->setContextProperty("vacuums_list",QVariant::fromValue(vacuums_list));

    QStringList cylinders_name = highSprrow.baseModuleManager->cylinder.keys();
    QList<QObject*> cylinders_list;
    for(QString key:cylinders_name){
        cylinders_list<<&(highSprrow.baseModuleManager->cylinder[key]->parameters);
    }
    engine.rootContext()->setContextProperty("cylinders_list",QVariant::fromValue(cylinders_list));

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

    engine.rootContext()->setContextProperty("lsutParams", &highSprrow.baseModuleManager->sh_lsut_module.parameters);

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
    engine.rootContext()->setContextProperty("lpa_camera_tip_offset",
                                             &highSprrow.baseModuleManager->lens_loader_module.camera_to_picker_offset);

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
    engine.rootContext()->setContextProperty("materialPickArmParams",&highSprrow.baseModuleManager->single_station_material_pickarm.parameters);
    engine.rootContext()->setContextProperty("systerm_param",&highSprrow.baseModuleManager->paramers);

    engine.rootContext()->setContextProperty("sensor_picker_offset",
                                             &highSprrow.baseModuleManager->single_station_material_loader_module.sensor_suction_offset);
    engine.rootContext()->setContextProperty("lens_picker_offset",
                                             &highSprrow.baseModuleManager->single_station_material_loader_module.lens_suction_offset);

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
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
