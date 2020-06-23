#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QApplication>
#include <QQmlContext>
#include <highsprrow.h>
#include <QIcon>
#include "utils/filecontent.h"
#include <QtWebEngine/QtWebEngine>

#include "utils/loging/loging.h"
#include "utils/loging/logmodel.h"
#include "utils/singletoninstances.h"
#include "utils/userManagement/usermanagement.h"
#include "utils/userManagement/mysqltablemodel.h"
#include "UnitTest/SilicolMsgBoxTest.h"
#include "utils/languageManager/languagemanager.h"
#include "utils/configManager/configfile.h"
#include "basicconfig.h"

#include "AACore/aadata.h"
#include "checkprocessmodel.h"
#include "traymapmodel.h"

#include <QtWidgets/QApplication>
#include <windows.h>
#include <DbgHelp.h>

#include "utils/configManager/configobjectarray.h"

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
    app.setWindowIcon(QIcon(ICON_SPARROW));

    HighSprrow highSprrow;

    QtWebEngine::initialize();
    QQmlApplicationEngine engine;

    qmlRegisterType<ConfigElementInfo>("ConfigElementType", 1, 0, "ConfigElementType");
    qmlRegisterType<ConfigObject>("ConfigObject", 1, 0, "ConfigObject");
    qRegisterMetaType<ConfigElementInfo::Type>();
    engine.rootContext()->setContextProperty("cfgManager", &SI::cfgManager);

    BasicConfig basicConfig;
    basicConfig.setObjectName("basicConfig");
    ConfigFile basicConfigFile("basicConfig", &basicConfig, "basicConfig.json", false);
    basicConfigFile.populate();
    // initialize logging system
    LogManager logManager;
    logManager.initLogSystem(basicConfig.logConfig());
    engine.rootContext()->setContextProperty("logConfig", basicConfig.logConfig());
    engine.rootContext()->setContextProperty("logModel", &logManager.logModel);

    LanguageManager languageManager(basicConfig.languageConfig(), app, engine);
    engine.rootContext()->setContextProperty("languageConfig", basicConfig.languageConfig());
    qmlRegisterType<LanguageConfig>("LanguageConfig", 1, 0, "LanguageConfig");

    qRegisterMetaType<MsgBoxItem>();
    qRegisterMetaType<MsgBoxIcon::Icon>();
    qmlRegisterType<MsgBoxIcon>("MsgBoxIcon", 1, 0, "MsgBoxIcon");
    MsgBoxModel msgBoxModel;
    SI::ui.init(&msgBoxModel);
    engine.rootContext()->setContextProperty("msgBoxModel", &msgBoxModel);
    engine.rootContext()->setContextProperty("uiOperation", &SI::ui);

//    UserManagement userManagement;
//    userManagement.init();
    qmlRegisterType<UserManagement>("UserMng", 1, 0, "UserMng");
    engine.rootContext()->setContextProperty("userManagement", &highSprrow.baseModuleManager->userManagement);
    engine.rootContext()->setContextProperty("userModel", highSprrow.baseModuleManager->userManagement.userModel);

    MsgBoxTester msgBoxTester;
    engine.rootContext()->setContextProperty("msgBoxTester", &msgBoxTester);

    qmlRegisterUncreatableType<TrayMapModel>("HighSprrow.Models", 1, 0, "TrayMapModel", "Tray map model should only be created in cpp code");

    //Object Property Definition
    engine.rootContext()->setContextProperty("lensTrayModel", TrayMapModel::instance(TrayMapModel::LensTray));
    engine.rootContext()->setContextProperty("sensorTrayModel", TrayMapModel::instance(TrayMapModel::SensorTray));
    engine.rootContext()->setContextProperty("productTrayModel", TrayMapModel::instance(TrayMapModel::ProductTray));
    engine.rootContext()->setContextProperty("rejectTrayModel", TrayMapModel::instance(TrayMapModel::RejectTray));
    engine.rootContext()->setContextProperty("highSprrow", &highSprrow);
    engine.rootContext()->setContextProperty("visionModule", highSprrow.baseModuleManager->visionModule);
    engine.rootContext()->setContextProperty("uplookCamera", highSprrow.baseModuleManager->pylonUplookCamera);
    engine.rootContext()->setContextProperty("downlookCamera", highSprrow.baseModuleManager->pylonDownlookCamera);
    engine.rootContext()->setContextProperty("pickarmCamera", highSprrow.baseModuleManager->pylonPickarmCamera);
    engine.rootContext()->setContextProperty("aa2DownlookCamera", highSprrow.baseModuleManager->pylonAA2DownlookCamera);
    engine.rootContext()->setContextProperty("sensorPickarmCamera", highSprrow.baseModuleManager->pylonSensorPickarmCamera);
    engine.rootContext()->setContextProperty("pickarmULCamera", highSprrow.baseModuleManager->pylonPickarmULCamera);
    engine.rootContext()->setContextProperty("pickarmBarcodeCamera", highSprrow.baseModuleManager->pylonBarcodeCamera);
    engine.rootContext()->setContextProperty("sciencaLightSourceController", &highSprrow.baseModuleManager->chartBackgroundLightSourceController);
    QStringList workerNameList;
    for(QString output:highSprrow.worker_manager->getWorkersNames()){
        workerNameList<<output;
    }

    engine.rootContext()->setContextProperty("workerNameList",workerNameList);
    engine.rootContext()->setContextProperty("baseModuleManager", highSprrow.baseModuleManager);
    { //Display Log Level
        engine.rootContext()->setContextProperty("displayDebugLog", highSprrow.baseModuleManager->m_QtDebugMsgDisplay);
        engine.rootContext()->setContextProperty("displayWarningLog", highSprrow.baseModuleManager->m_QtWarningMsgDisplay);
        engine.rootContext()->setContextProperty("displayCriticalLog", highSprrow.baseModuleManager->m_QtCriticalMsgDisplay);
        engine.rootContext()->setContextProperty("displayFatalLog", highSprrow.baseModuleManager->m_QtFatalMsgDisplay);
        engine.rootContext()->setContextProperty("displayInfoLog", highSprrow.baseModuleManager->m_QtInfoMsgDisplay);
    }
    engine.rootContext()->setContextProperty("moduleManagerParam", &highSprrow.baseModuleManager->parameters);

    engine.rootContext()->setContextProperty("logicManager", highSprrow.logicManager);
    engine.rootContext()->setContextProperty("logicManagerState", &highSprrow.logicManager->states);
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
    engine.rootContext()->setContextProperty("alarmShower", &highSprrow.worker_manager->alarm_shower);
    //QML Library Module
    qmlRegisterType<LutModule>("LutModuleLib",1,1,"LutModule");
    qmlRegisterType<SutModule>("SutModuleLib",1,1,"SutModule");
    qmlRegisterType<LensLoaderModule>("SomeLib",1,1,"LensLoaderModule");
    qmlRegisterType<SensorLoaderModule>("SomeLib",1,1,"SensorLoaderModule");
    qmlRegisterType<AAHeadModule>("SomeLib",1,1,"AAHeadModule");
    qmlRegisterType<AACoreNew>("AACoreNew",1,1,"AACoreNew");
    qmlRegisterType<LogicManager>("LogicManagerLib",1,1,"LogicManager");
    qmlRegisterType<TrayLoaderModule>("TrayLoaderModuleLib",1,1,"TrayLoaderModule");
    qmlRegisterType<VisionModule>("VisionModuleLib",1,1,"VisionModule");
    qmlRegisterType<SensorTrayLoaderModule>("SomeLib",1,1,"SensorTrayLoaderModule");

    //
    engine.rootContext()->setContextProperty("sensorLoaderModule", &highSprrow.baseModuleManager->sensor_loader_module);
    engine.rootContext()->setContextProperty("sensorLoaderModuleState", &highSprrow.baseModuleManager->sensor_loader_module.states);
    engine.rootContext()->setContextProperty("lensLoaderModule", &highSprrow.baseModuleManager->lens_loader_module);
    engine.rootContext()->setContextProperty("sensorTrayLoaderModule", &highSprrow.baseModuleManager->sensor_tray_loder_module);
    engine.rootContext()->setContextProperty("dothinkey", highSprrow.baseModuleManager->dothinkey);
    engine.rootContext()->setContextProperty("tray_loader_module",&highSprrow.baseModuleManager->tray_loader_module);

    //Position
    {
        engine.rootContext()->setContextProperty("aaHeadParams", &highSprrow.baseModuleManager->aa_head_module.parameters);
        engine.rootContext()->setContextProperty("aaCoreParams", &highSprrow.baseModuleManager->aaCoreNew.parameters);
        engine.rootContext()->setContextProperty("aaCoreStates", &highSprrow.baseModuleManager->aaCoreNew.states);

        engine.rootContext()->setContextProperty("tcpAAHeadParams", &highSprrow.baseModuleManager->tcp_aaHeadModule.parameters);
        engine.rootContext()->setContextProperty("tcpAACoreParams", &highSprrow.baseModuleManager->tcp_aaCoreNew.parameters);
        engine.rootContext()->setContextProperty("tcpAACoreStates", &highSprrow.baseModuleManager->tcp_aaCoreNew.states);

        //Local AA Head Position
        engine.rootContext()->setContextProperty("aaHeadMushroomPosition", &highSprrow.baseModuleManager->aa_head_module.mushroom_position);
        engine.rootContext()->setContextProperty("aaHeadPickLensPosition", &highSprrow.baseModuleManager->aa_head_module.pick_lens_position);
        engine.rootContext()->setContextProperty("aaHeadPr2BondOffset", &highSprrow.baseModuleManager->aa_head_module.pr2Bond_offset);

        //Remote AA Head Position
        engine.rootContext()->setContextProperty("tcpAAHeadMushroomPosition", &highSprrow.baseModuleManager->tcp_aaHeadModule.mushroom_position);
        engine.rootContext()->setContextProperty("tcpAAHeadPickLensPosition", &highSprrow.baseModuleManager->tcp_aaHeadModule.pick_lens_position);
        engine.rootContext()->setContextProperty("tcpAAHeadPr2BondOffset", &highSprrow.baseModuleManager->tcp_aaHeadModule.pr2Bond_offset);
    }

    {   //Local SUT Position
        engine.rootContext()->setContextProperty("sutLoadPosition", &highSprrow.baseModuleManager->sut_module.load_position);
        engine.rootContext()->setContextProperty("sutToolUplookPosition", &highSprrow.baseModuleManager->sut_module.tool_uplook_positon);
        engine.rootContext()->setContextProperty("sutToollookPosition", &highSprrow.baseModuleManager->sut_module.tool_downlook_position);
        engine.rootContext()->setContextProperty("sutDownlookPosition", &highSprrow.baseModuleManager->sut_module.downlook_position);
        engine.rootContext()->setContextProperty("sutParticalCheckPosition", &highSprrow.baseModuleManager->sut_module.partical_check_position);
        engine.rootContext()->setContextProperty("sutMushroomPosition", &highSprrow.baseModuleManager->sut_module.mushroom_positon);
        engine.rootContext()->setContextProperty("sutDownlookFlyEndPosition", &highSprrow.baseModuleManager->sut_module.down_look_fly_end_position);


        //Local SUT Params
        engine.rootContext()->setContextProperty("sutParams", &highSprrow.baseModuleManager->sut_module.parameters);
        engine.rootContext()->setContextProperty("sutCarrierParams",  &highSprrow.baseModuleManager->sut_carrier.parameters);

        //Remote SUT Position
        engine.rootContext()->setContextProperty("tcpSUTLoadPosition", &highSprrow.baseModuleManager->tcp_sut.load_position);
        engine.rootContext()->setContextProperty("tcpSUTToolUplookPosition", &highSprrow.baseModuleManager->tcp_sut.tool_uplook_positon);
        engine.rootContext()->setContextProperty("tcpSUTToollookPosition", &highSprrow.baseModuleManager->tcp_sut.tool_downlook_position);
        engine.rootContext()->setContextProperty("tcpSUTDownlookPosition", &highSprrow.baseModuleManager->tcp_sut.downlook_position);
        engine.rootContext()->setContextProperty("tcpSUTParticalCheckPosition", &highSprrow.baseModuleManager->tcp_sut.partical_check_position);
        engine.rootContext()->setContextProperty("tcpSUTMushroomPosition", &highSprrow.baseModuleManager->tcp_sut.mushroom_positon);

        //Remote SUT Params
        engine.rootContext()->setContextProperty("tcpSUTParams", &highSprrow.baseModuleManager->tcp_sut.parameters);
        engine.rootContext()->setContextProperty("tcpSUTCarrierParams",  &highSprrow.baseModuleManager->tcp_sut_carrier.parameters);

        //SUT state
        engine.rootContext()->setContextProperty("sutState", &highSprrow.baseModuleManager->sut_module.states);
        //Remote SUT state
        engine.rootContext()->setContextProperty("tcpSUTState", &highSprrow.baseModuleManager->tcp_sut.states);
    }

    {   //Dispense Params
        engine.rootContext()->setContextProperty("dispenserParams",&highSprrow.baseModuleManager->dispenser.parameters);
        engine.rootContext()->setContextProperty("dispenseParams",&highSprrow.baseModuleManager->dispense_module.parameters);

        //Remote Dispense Params
        engine.rootContext()->setContextProperty("tcpDispenserParams",&highSprrow.baseModuleManager->tcp_dispenser.parameters);
        engine.rootContext()->setContextProperty("tcpDispenseParams",&highSprrow.baseModuleManager->tcp_dispenseModule.parameters);
    }

    {
        //Local LUT Position
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

        //Local LUT Params
        engine.rootContext()->setContextProperty("lutParams", &highSprrow.baseModuleManager->lut_module.parameters);
        engine.rootContext()->setContextProperty("lutCarrierParams", &highSprrow.baseModuleManager->lut_carrier.parameters);

        //Remote LUT Position
        engine.rootContext()->setContextProperty("tcpLUTLoadPosition", &highSprrow.baseModuleManager->tcp_lutModule.load_position);
        engine.rootContext()->setContextProperty("tcpLUTUnloadPosition", &highSprrow.baseModuleManager->tcp_lutModule.lut_downlook_unload_position);
        engine.rootContext()->setContextProperty("tcpLUTLoadUplookPosition", &highSprrow.baseModuleManager->tcp_lutModule.load_uplook_position);
        engine.rootContext()->setContextProperty("tcpLUTDownlookLoadPosition", &highSprrow.baseModuleManager->tcp_lutModule.lut_downlook_load_position);
        engine.rootContext()->setContextProperty("tcpLUTDownlookUnloadPosition", &highSprrow.baseModuleManager->tcp_lutModule.lut_downlook_unload_position);

        engine.rootContext()->setContextProperty("tcpLUTUpDownlookPositionAA1", &highSprrow.baseModuleManager->tcp_lutModule.aa1_updownlook_position);
        engine.rootContext()->setContextProperty("tcpLUTPickLensPositionAA1", &highSprrow.baseModuleManager->tcp_lutModule.aa1_picklens_position);
        engine.rootContext()->setContextProperty("tcpLUTUnPickLensPositionAA1", &highSprrow.baseModuleManager->tcp_lutModule.aa1_unpicklens_position);
        engine.rootContext()->setContextProperty("tcpLUTUplookPositionAA1", &highSprrow.baseModuleManager->tcp_lutModule.aa1_uplook_position);

        engine.rootContext()->setContextProperty("tcpLUTUpDownlookPositionAA2", &highSprrow.baseModuleManager->tcp_lutModule.aa2_updownlook_position);
        engine.rootContext()->setContextProperty("tcpLUTPickLensPositionAA2", &highSprrow.baseModuleManager->tcp_lutModule.aa2_picklens_position);
        engine.rootContext()->setContextProperty("tcpLUTUnPickLensPositionAA2", &highSprrow.baseModuleManager->tcp_lutModule.aa2_unpicklens_position);
        engine.rootContext()->setContextProperty("tcpLUTUplookPositionAA2", &highSprrow.baseModuleManager->tcp_lutModule.aa2_uplook_position);

        //Remote LUT Params
        engine.rootContext()->setContextProperty("tcpLUTParams", &highSprrow.baseModuleManager->tcp_lutModule.parameters);
        engine.rootContext()->setContextProperty("tcpLUTCarrierParams", &highSprrow.baseModuleManager->tcp_lutCarrier.parameters);

        //LUT State
        engine.rootContext()->setContextProperty("lutState", &highSprrow.baseModuleManager->lut_module.states);
        //Remote LUT State
        engine.rootContext()->setContextProperty("tcpLUTState", &highSprrow.baseModuleManager->tcp_lutModule.states);
    }

    {
        //Local lens tray loader
        engine.rootContext()->setContextProperty("tray_clipin_parameter",&highSprrow.baseModuleManager->trayClipIn.standards_parameters);
        engine.rootContext()->setContextProperty("tray_clipout_parameter",&highSprrow.baseModuleManager->trayClipOut.standards_parameters);
        engine.rootContext()->setContextProperty("tray_loader_module_parameters",&highSprrow.baseModuleManager->tray_loader_module.parameters);

        //Remote lens tray loader
        engine.rootContext()->setContextProperty("tcp_tray_clipin_parameter",&highSprrow.baseModuleManager->tcp_lensTrayClipIn.standards_parameters);
        engine.rootContext()->setContextProperty("tcp_tray_clipout_parameter",&highSprrow.baseModuleManager->tcp_lensTrayClipOut.standards_parameters);
        engine.rootContext()->setContextProperty("tcp_tray_loader_module_parameters",&highSprrow.baseModuleManager->tcp_lensTrayLoaderModule.parameters);
        engine.rootContext()->setContextProperty("tcp_tray_loader_module_states",&highSprrow.baseModuleManager->tcp_lensTrayLoaderModule.states);
    }

    {
        //Local lens loader
        engine.rootContext()->setContextProperty("lensPickArmModuleParameter", &highSprrow.baseModuleManager->lens_pick_arm.parameters);
        engine.rootContext()->setContextProperty("lut_pr_position1",&highSprrow.baseModuleManager->lens_loader_module.lut_pr_position1);
        engine.rootContext()->setContextProperty("lut_pr_position2",&highSprrow.baseModuleManager->lens_loader_module.lut_pr_position2);
        engine.rootContext()->setContextProperty("lut_camera_position",&highSprrow.baseModuleManager->lens_loader_module.lut_camera_position);
        engine.rootContext()->setContextProperty("lut_picker_position",&highSprrow.baseModuleManager->lens_loader_module.lut_picker_position);
        engine.rootContext()->setContextProperty("lens_loader_parameter",&highSprrow.baseModuleManager->lens_loader_module.parameters);

        engine.rootContext()->setContextProperty("picker_offset", &highSprrow.baseModuleManager->lens_loader_module.camera_to_picker_offset);
        //Remote lens loader
        engine.rootContext()->setContextProperty("tcpLensPickArmModuleParameter", &highSprrow.baseModuleManager->tcp_lensLoaderModule.parameters);
        engine.rootContext()->setContextProperty("tcp_lut_pr_position1",&highSprrow.baseModuleManager->tcp_lensLoaderModule.lut_pr_position1);
        engine.rootContext()->setContextProperty("tcp_lut_pr_position2",&highSprrow.baseModuleManager->tcp_lensLoaderModule.lut_pr_position2);
        engine.rootContext()->setContextProperty("tcp_lut_camera_position",&highSprrow.baseModuleManager->tcp_lensLoaderModule.lut_camera_position);
        engine.rootContext()->setContextProperty("tcp_lut_picker_position",&highSprrow.baseModuleManager->tcp_lensLoaderModule.lut_picker_position);
        engine.rootContext()->setContextProperty("tcp_lens_loader_parameter",&highSprrow.baseModuleManager->tcp_lensLoaderModule.parameters);

        engine.rootContext()->setContextProperty("tcp_picker_offset", &highSprrow.baseModuleManager->tcp_lensLoaderModule.camera_to_picker_offset);

        //Remote lens loader state
        engine.rootContext()->setContextProperty("tcpLensLoaderModuleState", &highSprrow.baseModuleManager->tcp_lensLoaderModule.states);
    }

    {
        //Material Tray
        engine.rootContext()->setContextProperty("material_tray",&highSprrow.baseModuleManager->material_tray);
        engine.rootContext()->setContextProperty("first_tray_end_position",&highSprrow.baseModuleManager->material_tray.first_tray_end_position);
        engine.rootContext()->setContextProperty("tray_start_point1",&highSprrow.baseModuleManager->material_tray.parameters[0]->tray_start_position);
        engine.rootContext()->setContextProperty("tray_start_point2",&highSprrow.baseModuleManager->material_tray.parameters[1]->tray_start_position);
        if(highSprrow.baseModuleManager->getServerMode()==1)
        {
            engine.rootContext()->setContextProperty("tray_start_point3",&highSprrow.baseModuleManager->material_tray.parameters[2]->tray_start_position);
            engine.rootContext()->setContextProperty("tray_start_point4",&highSprrow.baseModuleManager->material_tray.parameters[3]->tray_start_position);
        }
        engine.rootContext()->setContextProperty("tray_standard_parameter",&highSprrow.baseModuleManager->material_tray.standards_parameters);

        //Remote Material Tray
        engine.rootContext()->setContextProperty("tcp_material_tray",&highSprrow.baseModuleManager->tcp_lens_tray);
        engine.rootContext()->setContextProperty("tcp_first_tray_end_position",&highSprrow.baseModuleManager->tcp_lens_tray.first_tray_end_position);
        engine.rootContext()->setContextProperty("tcp_tray_start_point1",&highSprrow.baseModuleManager->tcp_lens_tray.parameters[0]->tray_start_position);
        engine.rootContext()->setContextProperty("tcp_tray_start_point2",&highSprrow.baseModuleManager->tcp_lens_tray.parameters[1]->tray_start_position);
        engine.rootContext()->setContextProperty("tcp_tray_standard_parameter",&highSprrow.baseModuleManager->tcp_lens_tray.standards_parameters);
    }

    QString dirPath = app.applicationDirPath();
    dirPath += "/..";
    engine.rootContext()->setContextProperty("dirPath",dirPath);
    //Device
    engine.rootContext()->setContextProperty("imageGrabber", highSprrow.baseModuleManager->dothinkey);
    engine.rootContext()->setContextProperty("imageGrabberThread", highSprrow.baseModuleManager->imageGrabberThread);
    engine.rootContext()->setContextProperty("lightingController", highSprrow.baseModuleManager->lightingModule);
    engine.rootContext()->setContextProperty("motorsNames", QVariant::fromValue(highSprrow.baseModuleManager->motors.keys()));

    engine.rootContext()->setContextProperty("scaleValue",highSprrow.scaleValue);
    engine.rootContext()->setContextProperty("scaleLevel",highSprrow.scaleLevel);

    //Vision Location
    QStringList vision_locations_list = highSprrow.baseModuleManager->vision_locations.keys();
    QList<QObject*> vl_parameter_list;
    for(QString key:vision_locations_list){
        vl_parameter_list<<&(highSprrow.baseModuleManager->vision_locations[key]->parameters);
        engine.rootContext()->setContextProperty(key, &(highSprrow.baseModuleManager->vision_locations[key]->parameters));
    }
    engine.rootContext()->setContextProperty("vl_parameter_list",QVariant::fromValue(vl_parameter_list));

    //Remote TCP vision location
    engine.rootContext()->setContextProperty("tcp_vision_location_aa1_downlook", &highSprrow.baseModuleManager->tcp_vision_location_aa1_downlook.parameters);
    engine.rootContext()->setContextProperty("tcp_vision_location_aa1_mushroomhead", &highSprrow.baseModuleManager->tcp_vision_location_aa1_mushroomhead.parameters);
    engine.rootContext()->setContextProperty("tcp_vision_location_aa1_uplook", &highSprrow.baseModuleManager->tcp_vision_location_aa1_uplook.parameters);
    engine.rootContext()->setContextProperty("tcp_vision_location_aa1_updownLook_down", &highSprrow.baseModuleManager->tcp_vision_location_aa1_updownlook_down.parameters);
    engine.rootContext()->setContextProperty("tcp_vision_location_aa1_updownlook_up", &highSprrow.baseModuleManager->tcp_vision_location_aa1_updownlook_up.parameters);
    engine.rootContext()->setContextProperty("tcp_vision_location_lpa_lens", &highSprrow.baseModuleManager->tcp_vision_location_lpa_lens.parameters);
    engine.rootContext()->setContextProperty("tcp_vision_location_lpa_lut_lens", &highSprrow.baseModuleManager->tcp_vision_location_lpa_lut_lens.parameters);
    engine.rootContext()->setContextProperty("tcp_vision_location_lpa_lut", &highSprrow.baseModuleManager->tcp_vision_location_lpa_lut.parameters);
    engine.rootContext()->setContextProperty("tcp_vision_location_lpa_lut_ng", &highSprrow.baseModuleManager->tcp_vision_location_lpa_lut_ng.parameters);
    engine.rootContext()->setContextProperty("tcp_vision_location_lpa_vacancy", &highSprrow.baseModuleManager->tcp_vision_location_lpa_vacancy.parameters);
    engine.rootContext()->setContextProperty("tcp_vision_location_lut_load", &highSprrow.baseModuleManager->tcp_vision_location_lut_load.parameters);
    engine.rootContext()->setContextProperty("tcp_vision_location_lut_uplook_picker", &highSprrow.baseModuleManager->tcp_vision_location_lut_uplook_picker.parameters);

    //Params
    engine.rootContext()->setContextProperty("lensPickArmParams",&highSprrow.baseModuleManager->lens_pick_arm.parameters);

    engine.rootContext()->setContextProperty("sensorPickArmParams",&highSprrow.baseModuleManager->sensor_pickarm.parameters);
    engine.rootContext()->setContextProperty("sensorLoaderParameter",
                                             &highSprrow.baseModuleManager->sensor_loader_module.parameters);
    engine.rootContext()->setContextProperty("sut_pr_position1",
                                             &highSprrow.baseModuleManager->sensor_loader_module.sut1_pr_position);
    engine.rootContext()->setContextProperty("sut_pr_position2",
                                             &highSprrow.baseModuleManager->sensor_loader_module.sut2_pr_position);
    engine.rootContext()->setContextProperty("spa_standby_position",
                                             &highSprrow.baseModuleManager->sensor_loader_module.spa_standby_position);
    engine.rootContext()->setContextProperty("pick1_offset",
                                             &highSprrow.baseModuleManager->sensor_loader_module.picker1_offset);
    engine.rootContext()->setContextProperty("pick2_offset",
                                             &highSprrow.baseModuleManager->sensor_loader_module.picker2_offset);
    engine.rootContext()->setContextProperty("sensorTray1PickOffset",
                                             &highSprrow.baseModuleManager->sensor_loader_module.sensorTray1PickOffset);
    engine.rootContext()->setContextProperty("sensorTray2PickOffset",
                                             &highSprrow.baseModuleManager->sensor_loader_module.sensorTray2PickOffset);
    engine.rootContext()->setContextProperty("sut1PickOffset",
                                             &highSprrow.baseModuleManager->sensor_loader_module.sut1PickOffset);
    engine.rootContext()->setContextProperty("sut2PickOffset",
                                             &highSprrow.baseModuleManager->sensor_loader_module.sut2PickOffset);
    engine.rootContext()->setContextProperty("sut1PlaceOffset",
                                             &highSprrow.baseModuleManager->sensor_loader_module.sut1PlaceOffset);
    engine.rootContext()->setContextProperty("sut2PlaceOffset",
                                             &highSprrow.baseModuleManager->sensor_loader_module.sut2PlaceOffset);
	engine.rootContext()->setContextProperty("sensorTray1PlaceOffset",
                                             &highSprrow.baseModuleManager->sensor_loader_module.sensorTray1PlaceOffset);
    engine.rootContext()->setContextProperty("sensorTray2PlaceOffset",
                                             &highSprrow.baseModuleManager->sensor_loader_module.sensorTray2PlaceOffset);
    engine.rootContext()->setContextProperty("ngTrayPlaceOffset",
                                             &highSprrow.baseModuleManager->sensor_loader_module.ngTrayPlaceOffset);
    engine.rootContext()->setContextProperty("bufferTrayPlaceOffset",
                                             &highSprrow.baseModuleManager->sensor_loader_module.bufferTrayPlaceOffset);
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


    QStringList data = highSprrow.baseModuleManager->motors.keys();
    engine.rootContext()->setContextProperty("motorsNames", data);
    //ToDo: Need to add the tcpMotorNames dynamically from the inquiryAllMotorNames in tcp
    if (highSprrow.baseModuleManager->ServerMode() == 1) {
        highSprrow.baseModuleManager->tcpMotorNames.append("AA1_A");
        highSprrow.baseModuleManager->tcpMotorNames.append("AA1_B");
        highSprrow.baseModuleManager->tcpMotorNames.append("AA1_C");
        highSprrow.baseModuleManager->tcpMotorNames.append("LPA_R");
        highSprrow.baseModuleManager->tcpMotorNames.append("LPA_X");
        highSprrow.baseModuleManager->tcpMotorNames.append("LPA_Y");
        highSprrow.baseModuleManager->tcpMotorNames.append("LPA_Z");
        highSprrow.baseModuleManager->tcpMotorNames.append("LTIE");
        highSprrow.baseModuleManager->tcpMotorNames.append("LTK_X1");
        highSprrow.baseModuleManager->tcpMotorNames.append("LTK_X2");
        highSprrow.baseModuleManager->tcpMotorNames.append("LTL_X");
        highSprrow.baseModuleManager->tcpMotorNames.append("LTOE");
        highSprrow.baseModuleManager->tcpMotorNames.append("LUT_X");
        highSprrow.baseModuleManager->tcpMotorNames.append("LUT_Y");
        highSprrow.baseModuleManager->tcpMotorNames.append("LUT_Z");
        highSprrow.baseModuleManager->tcpMotorNames.append("SUT1_X");
        highSprrow.baseModuleManager->tcpMotorNames.append("SUT1_Y");
        highSprrow.baseModuleManager->tcpMotorNames.append("SUT1_Z");
        engine.rootContext()->setContextProperty("tcpMotorNames", highSprrow.baseModuleManager->tcpMotorNames);
    }

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
        qDebug()<<"Missing the log.txt";
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
    engine.rootContext()->setContextProperty("sensorTrayLoaderModuleState",
                                             &highSprrow.baseModuleManager->sensor_tray_loder_module.states);

    CheckProcessModel checkProcessModel;
    engine.rootContext()->setContextProperty("checkProcessModel",&checkProcessModel);

    //QImage Provider
    engine.addImageProvider(QLatin1String("uplookCameraImage"), highSprrow.baseModuleManager->pylonUplookCamera);
    engine.addImageProvider(QLatin1String("downlookCameraImage"), highSprrow.baseModuleManager->pylonDownlookCamera);
    engine.addImageProvider(QLatin1String("pickarmCameraImage"), highSprrow.baseModuleManager->pylonPickarmCamera);
    engine.addImageProvider(QLatin1String("pickarmULCameraImage"), highSprrow.baseModuleManager->pylonPickarmULCamera);
    engine.addImageProvider(QLatin1String("pickarmBarcodeCameraImage"), highSprrow.baseModuleManager->pylonBarcodeCamera);
    engine.addImageProvider(QLatin1String("aa2DownlookCameraImage"), highSprrow.baseModuleManager->pylonAA2DownlookCamera);
    engine.addImageProvider(QLatin1String("sensorPickarmCameraImage"), highSprrow.baseModuleManager->pylonSensorPickarmCamera);
    engine.addImageProvider(QLatin1String("preview1"), highSprrow.baseModuleManager->visionModule);
    engine.addImageProvider(QLatin1String("imageGrabberLiveImage"), highSprrow.baseModuleManager->imageGrabberThread->m_pImgProvider);
    engine.addImageProvider(QLatin1String("ocImage1"), highSprrow.baseModuleManager->aaCoreNew.ocImageProvider_1);
    engine.addImageProvider(QLatin1String("sfrImage"), highSprrow.baseModuleManager->aaCoreNew.sfrImageProvider);
    engine.addImageProvider(QLatin1String("dispenseImage"), highSprrow.baseModuleManager->aaCoreNew.dispenseImageProvider);
    engine.addImageProvider(QLatin1String("aaDebugImage"), highSprrow.baseModuleManager->visionModule->aaDebugImageProvider);
    //camera sequence -- Uplook Camera, AA1 Downlook Camera, Lens Pickarm Camera,
    //AA2 Downlook Camera, Sensor Pickarm Camera, Sensor Uplook Camera, Barcode Camera
    engine.addImageProvider(QLatin1String("uplookCameraResultImage"), &highSprrow.baseModuleManager->visionModule->visionModuleImageProviders[0]);
    engine.addImageProvider(QLatin1String("aa1DownlookCameraResultImage"), &highSprrow.baseModuleManager->visionModule->visionModuleImageProviders[1]);
    engine.addImageProvider(QLatin1String("lensPickarmCameraResultImage"), &highSprrow.baseModuleManager->visionModule->visionModuleImageProviders[2]);
    engine.addImageProvider(QLatin1String("aa2DownlookCameraResultImage"), &highSprrow.baseModuleManager->visionModule->visionModuleImageProviders[3]);
    engine.addImageProvider(QLatin1String("sensorPickarmCameraResultImage"), &highSprrow.baseModuleManager->visionModule->visionModuleImageProviders[4]);
    engine.addImageProvider(QLatin1String("sensorUplookCameraResultImage"), &highSprrow.baseModuleManager->visionModule->visionModuleImageProviders[5]);
    engine.addImageProvider(QLatin1String("barcodeCameraResultImage"), &highSprrow.baseModuleManager->visionModule->visionModuleImageProviders[6]);

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    basicConfig.languageConfig()->publishLanguage();

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

    int res = app.exec();
    logManager.disposeLogSystem();
    return res;
}
