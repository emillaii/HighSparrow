#include "XtVcMotor.h"
#include "basemodulemanager.h"
#include "xtvcmotorparameter.h"

#include <QMessageBox>
#include <qcoreapplication.h>
#include <qdir.h>
#include <qfileinfo.h>
#include <qjsonarray.h>
#include <qjsondocument.h>
#include "utils/commonutils.h"

wchar_t BaseModuleManager::ip[] =  L"192.168.8.251";
wchar_t BaseModuleManager::profile_path1[] = L".\\config\\";
wchar_t BaseModuleManager::profile_path2[] = L"..\\config\\xt_motion_config.csv";

BaseModuleManager::BaseModuleManager(QObject *parent)
    : PropertyBase (parent),
    ErrorBase ("BaseModuleManager")
{
    qDebug("main thread id :%d",QThread::currentThreadId());
    work_thread.setObjectName("device manager");
    this->moveToThread(&work_thread);
    this->work_thread.start();
    QMap<QString,PropertyBase*> temp_map;
    temp_map.insert("BASE_MODULE_PARAMS", this);
    PropertyBase::loadJsonConfig(BASE_MODULE_JSON,temp_map);
    qDebug("Server Mode: %d", ServerMode());
    setInitState(false);
    profile_loaded = false;
    if(!QDir(".//notopencamera").exists())
    {
        if (ServerMode() == 0) {
            pylonUplookCamera = new BaslerPylonCamera(UPLOOK_VISION_CAMERA);
            pylonDownlookCamera = new BaslerPylonCamera(DOWNLOOK_VISION_CAMERA);
            pylonPickarmCamera = new BaslerPylonCamera(PICKARM_VISION_CAMERA);
            pylonAA2DownlookCamera = new BaslerPylonCamera(CAMERA_AA2_DL);
            pylonSensorPickarmCamera = new BaslerPylonCamera(CAMERA_SPA_DL);
            if (MachineVersion() == 1) {
                pylonPickarmULCamera = new BaslerPylonCamera(CAMERA_LPA_UL);
                pylonBarcodeCamera = new BaslerPylonCamera(CAMERA_LPA_BARCODE);
            }
        }
    }
    if (this->ServerMode() == 0) {
        lightingModule = new WordopLight(ServerMode(), LIGHTING_CONTROLLER_1);
        visionModule = new VisionModule(pylonDownlookCamera, pylonUplookCamera,
                                        pylonPickarmCamera, pylonAA2DownlookCamera,
                                        pylonSensorPickarmCamera, pylonPickarmULCamera,
                                        pylonBarcodeCamera, VISION_MODULE_1);
    }
    else {
        lightingModule = new WordopLight(ServerMode(), LIGHTING_CONTROLLER_2);
        visionModule = new VisionModule(pylonDownlookCamera, pylonUplookCamera,
                                        pylonPickarmCamera, pylonAA2DownlookCamera,
                                        pylonSensorPickarmCamera, pylonPickarmULCamera,
                                        pylonBarcodeCamera, VISION_MODULE_2);
    }
    dothinkey = new Dothinkey();
    imageGrabberThread = new ImageGrabbingWorkerThread(dothinkey);
    if (ServerMode() == 0) {
        this->lut_module.openServer(19998);
        connect(&lut_module,&LutModule::sendLoadLensRequst,&lens_loader_module,&LensLoaderModule::receiveLoadLensRequst,Qt::DirectConnection);
        connect(&lens_loader_module,&LensLoaderModule::sendLoadLensFinish,&lut_module,&LutModule::receiveLoadLensRequstFinish,Qt::DirectConnection);
    }

    connect(&sut_module,&SutModule::sendLoadSensorFinish,&aa_head_module,&AAHeadModule::receiveSensorFromSut,Qt::DirectConnection);

    connect(&aa_head_module,&AAHeadModule::sendSensrRequestToSut,&sut_module,&SutModule::receiveLoadSensorRequst,Qt::DirectConnection);

    connect(&aaCoreNew, &AACoreNew::needUpdateParameterInTcpModule, this, &BaseModuleManager::receiveNeedUpdateTcpParameter);
    connect(&aaCoreNew, &AACoreNew::pushDataToUnit, &unitlog, &Unitlog::pushDataToUnit);
    connect(&aaCoreNew, &AACoreNew::clearHeaders, &unitlog, &Unitlog::clearHeaders,Qt::DirectConnection);
    connect(&aaCoreNew, &AACoreNew::postDataToELK, &unitlog, &Unitlog::postDataToELK);
    connect(&aaCoreNew, &AACoreNew::postSfrDataToELK, &unitlog, &Unitlog::postSfrDataToELK);
    connect(&lut_module, &LutModule::postCSVDataToUnit, &unitlog, &Unitlog::pushCSVDataToUnit);
    connect(&lens_loader_module, &LensLoaderModule::postCSVDataToUnit, &unitlog, &Unitlog::pushCSVDataToUnit);
    connect(&lens_loader_module, &LensLoaderModule::saveUnitDataToCSV, &unitlog, &Unitlog::saveUnitDataToCSV);
    if(!QDir(".//notopencamera").exists() && ServerMode() == 0)
    {
        if(pylonUplookCamera) pylonUplookCamera->start();
        if(pylonDownlookCamera) pylonDownlookCamera->start();
        if(pylonPickarmCamera) pylonPickarmCamera->start();
        if(pylonAA2DownlookCamera) pylonAA2DownlookCamera->start();
        if(pylonSensorPickarmCamera) pylonSensorPickarmCamera->start();
        if(pylonPickarmULCamera) pylonPickarmULCamera->start();
        if(pylonBarcodeCamera) pylonBarcodeCamera->start();
    }

    material_tray.standards_parameters.setTrayCount(5);
    unitlog.setServerAddress(configs.dataServerURL());
    setHomeState(false);
    connect(this,&BaseModuleManager::sendMsgSignal,this,&BaseModuleManager::sendMessageTest,Qt::BlockingQueuedConnection);
    connect(&timer, &QTimer::timeout, this, &BaseModuleManager::alarmChecking);
    connect(this,&BaseModuleManager::sendHandlingOperation,this,&BaseModuleManager::performHandlingOperation);
    connect(&state_geter,&DeviceStatesGeter::sendGetDeviceState,this,&BaseModuleManager::deviceResp,Qt::DirectConnection);

    //Remote module initialization
    tcp_sut.carrier = &tcp_sut_carrier;
    tcp_lutModule.carrier = &tcp_lutCarrier;
    tcp_aaCoreNew.aa_head = &tcp_aaHeadModule;
    tcp_dispenseModule.dispenser = &tcp_dispenser;
    tcp_aaCoreNew.dispense = &tcp_dispenseModule;
    tcp_lensTrayLoaderModule.tray_clip_in = &tcp_lensTrayClipIn;
    tcp_lensTrayLoaderModule.tray_clip_out = &tcp_lensTrayClipOut;

    tcp_lens_tray.standards_parameters.setTrayCount(5);
    tcp_lensLoaderModule.tray = &tcp_lens_tray;
    //Machine Map Initialization
    //machineMap = new GraphWidget;
    //machineMap->show();
    timer.start(3000);
}

BaseModuleManager::~BaseModuleManager()
{
    this->work_thread.quit();
    this->work_thread.wait();
}

void BaseModuleManager::tcpResp(QString message)
{
    QJsonObject message_object = TcpMessager::getJsonObjectFromString(message);
    if(message_object.contains("cmd"))
    {
        QString cmd = message_object["cmd"].toString();
        if(cmd == "inquiryMotorPos")
        {
            QString motor_name = message_object["motorName"].toString();
            qDebug()<<"qure motor pos :"<< motor_name<<"thread id:"<<QThread::currentThreadId();
            QJsonObject result;
            result["motorName"] = motor_name;
            XtMotor* temp_motor = GetMotorByName(motor_name);
            if(temp_motor == nullptr)
                result["error"] =QString("can not find ").append(motor_name);
            else {
                result["motorPosition"] = temp_motor->GetFeedbackPos();
                result["motorTargetPosition"] = temp_motor->GetCurrentTragetPos();
                result["motorHomeState"] = temp_motor->states.seekedOrigin();
                result["motorEnableState"] = temp_motor->states.isEnabled();
                result["error"] = "";
            }
            receive_messagers[message_object["sender_name"].toString()]->sendMessage(TcpMessager::getStringFromJsonObject(result));
        }
        else if(cmd == "inquiryModuleState")
        {
            QString module_name = message_object["moduleName"].toString();
            qDebug()<<"inquiry module state :"<< module_name<<"thread id:"<<QThread::currentThreadId();
            if(module_name.contains("LightController") || module_name.contains("VisionModule")) return;
            if(workers.contains(module_name))
            {
                QJsonObject result;
                result["resp"] = "moduleState";
                result["moduleName"] = module_name;
                QJsonObject module_state;
                workers[module_name]->getModuleState()->write(module_state);
                result["moduleState"] = module_state;
                receive_messagers[message_object["sender_name"].toString()]->sendMessage(TcpMessager::getStringFromJsonObject(result));
            }
        }
        else if(cmd == "inquiryModuleParameter")
        {
            QString module_name = message_object["moduleName"].toString();
            qDebug()<<"inquiry module parameter :"<< module_name<<"thread id:"<<QThread::currentThreadId();
            if(module_name.contains("LightController") || module_name.contains("VisionModule")) return;
            if(workers.contains(module_name))
            {
                QJsonObject result;
                result["resp"] = "moduleParameter";
                result["moduleName"] = module_name;
                QMap<QString, PropertyBase *> map = workers[module_name]->getModuleParameter();
                if (map.size() == 0) { return; }
                foreach (QString param_name, map.keys()) {
                    QJsonObject module_parameter;
                    map[param_name]->write(module_parameter);
                    result[param_name] = module_parameter;
                }
                receive_messagers[message_object["sender_name"].toString()]->sendMessage(TcpMessager::getStringFromJsonObject(result));
            }
        }
        else if(cmd == "inquiryModuleNames")
        {
            QJsonObject result;
            result["resp"] = "moduleNames";
            result["moduleNames"] = QJsonArray::fromStringList(workers.keys());
            receive_messagers[message_object["sender_name"].toString()]->sendMessage(TcpMessager::getStringFromJsonObject(result));
        }
        else if(cmd == "inquiryVisionLocations")
        {
            QJsonObject result;
            result["resp"] = "visionLocations";
            for(QString key: vision_locations.keys()){
                QJsonObject vision_location_parameter;
                vision_locations[key]->parameters.write(vision_location_parameter);
                result[key] = vision_location_parameter;
            }
            qInfo("vision location: %s", getStringFromJsonObject(result).toStdString().c_str());
            receive_messagers[message_object["sender_name"].toString()]->sendMessage(TcpMessager::getStringFromJsonObject(result));
        }
        else if (cmd == "inquiryAllMotorNames")
        {
            QJsonObject result;
            result["resp"] = "motorNames";
            QJsonArray array;
            for (QString name : motors.keys()){
                array.push_back(name);
            }
            result["motorNames"] = array;
            qInfo("motor names: %s", getStringFromJsonObject(result).toStdString().c_str());
            receive_messagers[message_object["sender_name"].toString()]->sendMessage(TcpMessager::getStringFromJsonObject(result));
        }
        else if (cmd == "setModuleParameter")
        {
            QString module_name = message_object["moduleName"].toString();
            qDebug()<<"Set module parameter :"<< module_name;
            if(workers.contains(module_name))
            {
                QJsonObject result;
                result["resp"] = "moduleParameter";
                result["moduleName"] = module_name;
                QMap<QString, PropertyBase *> parameters = workers[module_name]->getModuleParameter();
                for (QMap<QString, PropertyBase*>::const_iterator i = parameters.constBegin(); i != parameters.constEnd(); ++i) {
                    if(message_object.contains(i.key()))
                    {
                       i.value()->read(message_object.value(i.key()).toObject());
                    }
                }
                saveParameters();
            }
        }
        else if (cmd == "needUpdateParameter")
        {
            qInfo("Receive need update parameter");
            inquiryTcpModuleParameter(tcp_aaCoreNew.Name());
            inquiryTcpModuleParameter(tcp_sut.Name());
            inquiryTcpModuleParameter(tcp_lensTrayLoaderModule.Name());
            inquiryTcpModuleParameter(tcp_lutModule.Name());
            inquiryTcpModuleParameter(tcp_lensLoaderModule.Name());
        }
        else if (cmd == "inquiryOutputIoState")
        {
            //qInfo("inquiryOutputIoState: %s", message.toStdString().c_str());
            QString outputIo_name = message_object["outputIoName"].toString();
            XtGeneralOutput* temp_io = GetOutputIoByName(outputIo_name);
            QJsonObject result;
            result["outputIoName"] = outputIo_name;
            if(temp_io == nullptr)
                result["error"] =QString("can not find ").append(outputIo_name);
            else {
                result["IoValue"] = temp_io->Value();
                result["error"] = "";
            }
            receive_messagers[message_object["sender_name"].toString()]->sendMessage(TcpMessager::getStringFromJsonObject(result));
        }
        else if (cmd == "inquiryInputIoState")
        {
            //qInfo("inquiryInputIoState: %s", message.toStdString().c_str());
            QString inputIo_name = message_object["inputIoName"].toString();
            XtGeneralInput* temp_io = GetInputIoByName(inputIo_name);
            QJsonObject result;
            result["inputIoName"] = inputIo_name;
            if(temp_io == nullptr)
                result["error"] =QString("can not find ").append(inputIo_name);
            else {
                result["IoValue"] = temp_io->Value();
                result["error"] = "";
            }
            receive_messagers[message_object["sender_name"].toString()]->sendMessage(TcpMessager::getStringFromJsonObject(result));
        }
        else if (cmd == "toggleOutputIoState")
        {
            QString outputIo_name = message_object["outputIoName"].toString();
            int inputState = message_object["inputState"].toInt(-1);
            XtGeneralOutput* temp_io = GetOutputIoByName(outputIo_name);
            QJsonObject result;
            result["outputIoName"] = outputIo_name;
            if(temp_io == nullptr)
                result["error"] =QString("can not find ").append(outputIo_name);
            else {
                if (inputState == -1) //toggle state
                    temp_io->Set(!temp_io->Value());
                else {
                    temp_io->Set(inputState);
                }
                result["IoValue"] = temp_io->Value();
                result["error"] = "";
            }
            receive_messagers[message_object["sender_name"].toString()]->sendMessage(TcpMessager::getStringFromJsonObject(result));
        }
        else if (cmd == "stepMove")
        {
            QString motor_name = message_object["motorName"].toString();
            double step = message_object["step"].toDouble(0);
            bool isPositive = message_object["isPositive"].toBool(false);
            XtMotor* temp_motor = GetMotorByName(motor_name);
            if(temp_motor == nullptr){
               qWarning("Tcp step move command fail, cannot find motor: %s", motor_name.toStdString().c_str());
            } else {
               qInfo("Tcp step move command : %s step: %f direction: %d", motor_name.toStdString().c_str(), step, isPositive);
               stepMove(motor_name, step, isPositive);
            }
        }
        else if (cmd == "aaHeadInterMove")
        {
            int channel = message_object["channel"].toInt(0);
            double step = message_object["step"].toDouble(0);
            qInfo("Receive aa head inter move request. channel: %d step: %f", channel, step);
            if (channel == 0) {
                aa_head_module.stepInterpolation_AB_Sync(step, 0);
            } else {
                aa_head_module.stepInterpolation_AB_Sync(0, step);
            }
        }
        else if (cmd == "enableMotor")
        {
            QString motorName = message_object["motorName"].toString();
            bool enable = message_object["enable"].toBool(false);
            if (motors.contains(motorName)) {
               if (InitState()) {
                   if (enable) {
                       GetMotorByName(motorName)->Enable();
                   }
                   else {
                       GetMotorByName(motorName)->Disable();
                   }
               }
            }
        }
        else if (cmd == "homeMotor")
        {
            QString motorName = message_object["motorName"].toString();
            if (motors.contains(motorName)) {
               if (InitState()) {
                    GetMotorByName(motorName)->SeekOrigin();
                    GetMotorByName(motorName)->WaitSeekDone();
               }
            }
        }
    }
    else if(message_object.contains("resp"))
    {
        QString resp = message_object["resp"].toString();
        if(resp == "moduleState")
        {
            QString module_name = message_object["moduleName"].toString();
            qDebug()<<"resp moduleState :"<< module_name<<"thread id:"<<QThread::currentThreadId();
            if(tcp_workers.contains(module_name))
            {
                QJsonObject module_state = TcpMessager::getJsonObjectFromString(message_object["moduleState"].toString());
                tcp_workers[module_name]->getModuleState()->read(module_state);
            }
        }
        else if(resp == "moduleParameter")
        {
            QString module_name = message_object["moduleName"].toString();
            if(tcp_workers.contains(module_name))
            {
                qDebug()<<"resp moduleParameter :"<< module_name ;
                QMap<QString, PropertyBase *> map = tcp_workers[module_name]->getModuleParameter();
                foreach (QString param_name, map.keys()) {
                    QJsonObject parameter_json = message_object[param_name].toObject();
                    QString jsonString = getStringFromJsonObject(parameter_json);
                    map[param_name]->read(parameter_json);
                }
            }
            if (module_name.contains("LensLoaderModule")){
                QPointF point = tcp_lensLoaderModule.tray->getStartPosition(0);
                qInfo("point 1: %f %f ", point.x(), point.y());
            }
        }
        else if(resp == "moduleNames")
        {
            QJsonArray module_names = message_object["moduleNames"].toArray();
            qDebug()<<"resp moduleState :"<< module_names<<"thread id:"<<QThread::currentThreadId();
            foreach (QJsonValue temp_name, module_names) {
                //todo 构建相应的模块对象//仅仅包含参数和状态、可以暂时用原来的类
                qDebug("inquiryModuleNames moduleName: %s", temp_name.toString().toStdString().c_str());
                if(temp_name.toString().contains("AA"))
                {
                    tcp_aaCoreNew.setName(temp_name.toString());
                    tcp_workers.insert(temp_name.toString(), &tcp_aaCoreNew);
                }
                else if(temp_name.toString().contains("Sut"))
                {
                    tcp_sut.setName(temp_name.toString());
                    tcp_workers.insert(temp_name.toString(), &tcp_sut);
                }
                else if (temp_name.toString().contains("LUTModule"))
                {
                    tcp_workers.insert(temp_name.toString(), &tcp_lutModule);
                }
                else if (temp_name.toString().contains("LensTrayLoaderModule"))
                {
                    tcp_lensTrayLoaderModule.setName(temp_name.toString());
                    tcp_workers.insert(temp_name.toString(), &tcp_lensTrayLoaderModule);
                }
                else if (temp_name.toString().contains("LensLoaderModule"))
                {
                    tcp_lensLoaderModule.setName(temp_name.toString());
                    tcp_workers.insert(temp_name.toString(), &tcp_lensLoaderModule);
                }
                inquiryTcpModuleState(temp_name.toString());
                inquiryTcpModuleParameter(temp_name.toString());
            }
        }
        else if (resp == "visionLocations") {
            qInfo("receive visionLocations resp");
            QJsonArray vision_locations_names = message_object["visionLocations"].toArray();
            QString vision_location_message = getStringFromJsonObject(message_object);
            qInfo("receive visionLocations resp: %s", vision_location_message.toStdString().c_str());
        }
        else if (resp == "motorNames") {
            qInfo("receive motor names resp");
            QJsonArray motor_names = message_object["motorNames"].toArray();
            for (int i = 0; i < motor_names.size(); i++) {
                qInfo("Add motor : %s into tcp list", motor_names[i].toString().toStdString().c_str());
                tcpMotorNames.push_back(motor_names[i].toString());
                qInfo("tcpMotorNames size: %d", tcpMotorNames.size());
            }
            QString motor_name_message = getStringFromJsonObject(message_object);
            qInfo("receive motor names resp: %s", motor_name_message.toStdString().c_str());
        }
    }
}

QString BaseModuleManager::deviceResp(QString message)
{
    QJsonObject message_object = getJsonObjectFromString(message);
    QJsonObject result;
    if(message_object.contains("cmd"))
    {
        QString cmd = message_object["cmd"].toString();
        if(cmd == "inquiryMotorPos")
        {
            QString motor_name = message_object["motorName"].toString();
            qDebug()<<"inquiry motor pos :"<< motor_name<<"thread id:"<<QThread::currentThreadId();

            if(motor_name == "")
            {
                result["error"] = "motor name is null";
                return getStringFromJsonObject(result);
            }
            result["motorName"] = motor_name;
            XtMotor* temp_motor = GetMotorByName(motor_name);
            if(temp_motor == nullptr)
            {
                result["error"] = QString("can not find motor ").append(motor_name);
                bool geted = false;
                foreach (QString messger_name, sender_messagers.keys())
                {
                  QString tcp_result =  sender_messagers[messger_name]->inquiryMessage(message);
                  QJsonObject result_json = getJsonObjectFromString(tcp_result);
                  if(result_json.contains("error"))
                  {
                      if(result_json["error"].toString() == "")
                      {
                            geted = true;
                            result["motorPosition"] = result_json["motorPosition"];
                            result["motorTargetPosition"] = result_json["motorTargetPosition"];
                            result["motorHomeState"] = result_json["motorHomeState"];
                            result["motorEnableState"] = result_json["motorEnableState"];
                            result["error"] = "";
                            break;
                      }
                      else
                      {
                          qCritical("tcp fail and auto retray:%s",result_json["error"].toString().toStdString().c_str());
                          tcp_result =  sender_messagers[messger_name]->inquiryMessage(message);
                          result_json = getJsonObjectFromString(tcp_result);
                          if(result_json.contains("error"))
                          {
                              if(result_json["error"].toString() == "")
                              {
                                    geted = true;
                                    result["motorPosition"] = result_json["motorPosition"];
                                    result["motorTargetPosition"] = result_json["motorTargetPosition"];
                                    result["motorHomeState"] = result_json["motorHomeState"];
                                    result["motorEnableState"] = result_json["motorEnableState"];
                                    result["error"] = "";
                                    break;
                              }
                              else
                              {
                                  qCritical("tcp fail:%s",result_json["error"].toString().toStdString().c_str());

                              }
                          }
                          else
                          {
                              qCritical("messge fail");
                          }
                      }
                  }
                  else
                  {
                      qCritical("messge fail");
                  }
                }
            }
            else {
                result["motorPosition"] = temp_motor->GetFeedbackPos();
                result["motorTargetPosition"] = temp_motor->GetCurrentTragetPos();
                result["motorHomeState"] = temp_motor->states.seekedOrigin();
                result["motorEnableState"] = temp_motor->states.isEnabled();
                result["error"] = "";
            }
            return getStringFromJsonObject(result);
        }
        else if(cmd == "inquiryInputIoState")
        {
            QString temp_name = message_object["inputIoName"].toString();
            qDebug()<<"inquiry input io state :"<< temp_name<<"thread id:"<<QThread::currentThreadId();
            QJsonObject result;
            result["inputIoName"] = temp_name;
            XtGeneralInput* temp_io = GetInputIoByName(temp_name);
            if(temp_io == nullptr)
            {
                result["error"] = QString("can not find input io ").append(temp_name);
                bool geted = false;
                foreach (QString messger_name, sender_messagers.keys())
                {
                  //QString tcp_result =  receive_messagers[messger_name]->inquiryMessage(message);
                  QString tcp_result =  sender_messagers[messger_name]->inquiryMessage(message);

                  QJsonObject result_json = getJsonObjectFromString(tcp_result);
                  if(result_json.contains("error"))
                  {
                      if(result_json["error"].toString() == "")
                      {
                            geted = true;
                            result["IoValue"] = result_json["IoValue"];
                            result["error"] = "";
                            break;
                      }
                  }
                }
                if(!geted)
                    result["error"] = QString("tcp cannot find input io ").append(temp_name);
            }
            else {
                result["IoValue"] = temp_io->Value();
                result["error"] = "";

            }
            return getStringFromJsonObject(result);
        }
        else if(cmd == "inquiryOutputIoState")
        {
            QString temp_name = message_object["outputIoName"].toString();
            qDebug()<<"inquiry output io state :"<< temp_name<<"thread id:"<<QThread::currentThreadId();
            QJsonObject result;
            result["outputIoName"] = temp_name;
            XtGeneralOutput* temp_io = GetOutputIoByName(temp_name);
            if(temp_io == nullptr)
            {
                result["error"] = QString("can not find input io ").append(temp_name);
                bool geted = false;
                foreach (QString messger_name, sender_messagers.keys())
                {
                  //QString tcp_result =  receive_messagers[messger_name]->inquiryMessage(message);
                  QString tcp_result =  sender_messagers[messger_name]->inquiryMessage(message);

                  QJsonObject result_json = getJsonObjectFromString(tcp_result);
                  if(result_json.contains("error"))
                  {
                      if(result_json["error"].toString() == "")
                      {
                            geted = true;
                            result["IoValue"] = result_json["IoValue"];
                            result["error"] = "";
                            break;
                      }
                  }
                }
                if(!geted)
                    result["error"] = QString("tcp cannot find input io ").append(temp_name);
            }
            else {
                result["IoValue"] = temp_io->Value();
                result["error"] = "";

            }
            return getStringFromJsonObject(result);
        }
        else if(cmd == "toggleOutputIoState")
        {
            QString temp_name = message_object["outputIoName"].toString();
            int inputState = message_object["inputState"].toInt(-1);
            qDebug()<<"inquiry output io state :"<< temp_name<<"thread id:"<<QThread::currentThreadId();
            QJsonObject result;
            result["outputIoName"] = temp_name;
            XtGeneralOutput* temp_io = GetOutputIoByName(temp_name);
            if(temp_io == nullptr)
            {
                result["error"] = QString("can not find input io ").append(temp_name);
                bool geted = false;
                foreach (QString messger_name, sender_messagers.keys())
                {
                  QString tcp_result =  sender_messagers[messger_name]->inquiryMessage(message);

                  QJsonObject result_json = getJsonObjectFromString(tcp_result);
                  if(result_json.contains("error"))
                  {
                      if(result_json["error"].toString() == "")
                      {
                            geted = true;
                            result["IoValue"] = result_json["IoValue"];
                            result["error"] = "";
                            break;
                      }
                  }
                }
                if(!geted)
                    result["error"] = QString("tcp cannot find input io ").append(temp_name);
            }
            else {
                if (inputState == -1)
                    temp_io->Set(!temp_io->Value());
                else
                    temp_io->Set(inputState);
                result["IoValue"] = temp_io->Value();
                result["error"] = "";
            }
            return getStringFromJsonObject(result);
        }
    }
    result["error"] = "cmd error";
    return getStringFromJsonObject(result);
}

void BaseModuleManager::alarmChecking()
{
    bool checked_alarm = false;
//    foreach (XtMotor* temp_motor, motors)
//    {
//        if(temp_motor->states.seekedOrigin()&&temp_motor->checkAlarm())
//        {
//            checked_alarm = true;
//            AppendError(temp_motor->GetCurrentError());
//            temp_motor->states.setSeekedOrigin(false);
//            states.setSeekedOrigin(false);
//            setHomeState(false);
//        }
//    }
//    if(checked_alarm)
//        emit sendAlarm(0,ErrorLevel::ErrorMustStop,GetCurrentError());
    if (ServerMode() == 1 && this->m_InitState) //Testing code (Incomplete)
    {
        QJsonArray array;
        array.push_back(tcp_lutModule.parameters.vacuum1Name());
        array.push_back(tcp_lutModule.parameters.vacuum2Name());
        array.push_back(tcp_lensTrayLoaderModule.parameters.cylinderTrayName());
        QJsonObject data;
        data.insert("IoNames", array);
        QString jsonString = getStringFromJsonObject(data);
        tcp_lutModule.states.setTcpVaccum1State(state_geter.getOutputIoState(tcp_lutModule.parameters.vacuum1Name()).current_state);
        tcp_lutModule.states.setTcpVaccum2State(state_geter.getOutputIoState(tcp_lutModule.parameters.vacuum2Name()).current_state);
        //ToDo: Add the disable button for different config. Standard AA - Lens AA
        //tcp_aaCoreNew.states.setTcpAAGripperState(state_geter.getOutputIoState(tcp_aaHeadModule.parameters.gripperName()).current_state);
        tcp_lensLoaderModule.states.setTcpVaccumState(state_geter.getInputIoState(tcp_lensLoaderModule.parameters.pickarmVaccumSensorName()).current_state);
        tcp_lensTrayLoaderModule.states.setTcpCylinderClipSensorState(state_geter.getInputIoState(tcp_lensTrayLoaderModule.parameters.tcpCylinderClipSensorName()).current_state);
        tcp_lensTrayLoaderModule.states.setTcpCylinderLTK1SensorState(state_geter.getInputIoState(tcp_lensTrayLoaderModule.parameters.tcpCylinderLTKX1SensorName()).current_state);
        tcp_lensTrayLoaderModule.states.setTcpCylinderLTK2SensorState(state_geter.getInputIoState(tcp_lensTrayLoaderModule.parameters.tcpCylinderLTKX2SensorName()).current_state);
        tcp_lensTrayLoaderModule.states.setTcpCylinderLTLSensorState(state_geter.getInputIoState(tcp_lensTrayLoaderModule.parameters.tcpCylinderLTLSensorName()).current_state);
    }
}

bool BaseModuleManager::sendMessageTest(QString title, QString content)
{
    QMessageBox::StandardButton rb = QMessageBox::information(nullptr,title,content,QMessageBox::Yes|QMessageBox::No);
    if(rb==QMessageBox::Yes){
        return true;
    }else{
        return false;
    }
}

void BaseModuleManager::receiveMessageFromWorkerManger(QVariantMap message)
{
    qInfo("receiveMessageFromWorkerManger: %s",TcpMessager::getStringFromQvariantMap(message).toStdString().c_str());

    QString message_content = message["Message"].toString();
    if(message_content == "inquirRunParameters")
    {
        QVariantMap temp_param;
        if(getServerMode() == 1)
        {
            temp_param.insert("RunMode",parameters.runMode());
            temp_param.insert("HandlyChangeSensor",sensor_loader_module.parameters.handlyChangeSensor());
            temp_param.insert("HandlyChangeSensorTray",sensor_tray_loder_module.parameters.handlyChangeSensorTray());
        }
        else
        {
            temp_param.insert("HandlyChangeLens",lens_loader_module.parameters.handlyChangeLens());
            temp_param.insert("HandlyChangeLensTray",tray_loader_module.parameters.handlyChangeLensTray());
        }
        temp_param.insert("StationNumber",QString::number(getServerMode()));
        if(aaCoreNew.parameters.taskMode() == 1)
            temp_param.insert("StationTask",aaCoreNew.parameters.taskNumberFSJ());
        else if(aaCoreNew.parameters.taskMode() == 2)
            temp_param.insert("StationTask",aaCoreNew.parameters.taskNumberSSJ());
        else if(aaCoreNew.parameters.taskMode() == 3)
            temp_param.insert("StationTask",aaCoreNew.parameters.taskNumber());
        else
            temp_param.insert("StationTask",0);
        temp_param.insert("DisableStation",parameters.disableStation());

        temp_param.insert("OriginModule",message["OriginModule"]);
        temp_param.insert("TargetModule","WorksManager");
        temp_param.insert("Message","runParametersResp");
        emit sendMessageToWorkerManger(temp_param);
    }
}

bool BaseModuleManager::loadParameters()
{
//    configs.loadJsonConfig(QString(SYSTERM_PARAM_DIR).append(SYSTERM_CONGIF_FILE),"systermConfig");
    if(!this->parameters.loadJsonConfig(QString(CONFIG_DIR).append(SYSTERM_PARAM_FILE),SYSTERM_PARAMETER))
        return false;

    tcp_manager.loadJsonConfig(getSystermParameterDir().append(TCP_CONFIG_FILE));
    material_tray.loadJsonConfig(getCurrentParameterDir().append(MATERIAL_TRAY_FILE));
    aa_head_module.loadJsonConfig(getCurrentParameterDir().append(AA_HEAD_FILE));
    sut_module.loadParams(getCurrentParameterDir().append(SUT_FILE));
    dothinkey->loadParams(getCurrentParameterDir().append(DOTHINGKEY_FILE));
    dispenser.parameters.loadJsonConfig(getCurrentParameterDir().append(DISPENSER_FILE),DISPENSER_PARAMETER);
    dispense_module.parameters.loadJsonConfig(getCurrentParameterDir().append(DISPENSE_MODULE_FILE),DISPENSER_MODULE_PARAMETER);
    sut_carrier.parameters.loadJsonConfig(getCurrentParameterDir().append(SUT_CARRIER_FILE),"sut");
    if(ServerMode())
    {
        sensor_loader_module.loadJsonConfig(getCurrentParameterDir().append(SENSOR_LOADER_FILE));
        sensor_pickarm.parameters.loadJsonConfig(getCurrentParameterDir().append(SENSOR_PICKARM_FILE),SENSOR_PICKARM_PARAMETER);

        QMap<QString,PropertyBase*> temp_map;
        temp_map.insert("sensor_tray_loader", &sensor_tray_loder_module.parameters);
        temp_map.insert("sensor_clip_stand", &sensor_clip_stand);
        temp_map.insert("entance_clip", &entrance_clip.parameters);
        temp_map.insert("exit_clip", &exit_clip.parameters);
        PropertyBase::loadJsonConfig(getCurrentParameterDir().append(SENSOR_TRAY_LOADER_FILE), temp_map);
    }
    else
    {
        lut_module.loadJsonConfig(getCurrentParameterDir().append(LUT_FILE));
        lut_carrier.parameters.loadJsonConfig(getCurrentParameterDir().append(LUT_CARRIER_FILE),LUT_CARRIER_PARAMETER);
        lens_loader_module.loadJsonConfig(getCurrentParameterDir().append(LENS_LOADER_MODULE_FILE));
        lens_pick_arm.parameters.loadJsonConfig(getCurrentParameterDir().append(LENS_PICKARM_FILE),LENS_PICKARM_PARAMETER);
        tray_loader_module.parameters.loadJsonConfig(getCurrentParameterDir().append(TRAY_LOADER_FILE),TRAY_LOADER_PARAMETER);
        trayClipIn.standards_parameters.loadJsonConfig(getCurrentParameterDir().append(TRAY_CLIPIN_FILE),TRAY_CLIPIN_PARAMETER);
        trayClipOut.standards_parameters.loadJsonConfig(getCurrentParameterDir().append(TRAY_CLIPOUT_FILE),TRAY_CLIPOUT_PARAMETER);
     }
    aaCoreNew.loadJsonConfig(getCurrentParameterDir().append(AA_CORE_MODULE_FILE));
    loadVcmFile(getCurrentParameterDir().append(VCM_PARAMETER_FILE));
    loadMotorFile(getCurrentParameterDir().append(MOTOR_PARAMETER_FILE));
    loadCylinderFiles(getCurrentParameterDir().append(CYLINDER_PARAMETER_FILE));
    loadVacuumFiles(getCurrentParameterDir().append(VACUUM_PARAMETER_FILE));
    loadCalibrationFiles(getCurrentParameterDir().append(CALIBRATION_PARAMETER_FILE));
    loadVisionLoactionFiles(getCurrentParameterDir().append(VISION_LOCATION_PARAMETER_FILE));
    loadMotorLimitFiles(getCurrentParameterDir().append(LIMIT_PARAMETER_FILE));
    return true;
}

bool BaseModuleManager::loadconfig()
{
    loadMotorLimitFiles(getCurrentParameterDir().append(LIMIT_PARAMETER_FILE));
    loadMotorFile(getCurrentParameterDir().append(MOTOR_PARAMETER_FILE));
    loadCylinderFiles(getCurrentParameterDir().append(CYLINDER_PARAMETER_FILE));
    loadVacuumFiles(getCurrentParameterDir().append(VACUUM_PARAMETER_FILE));
    loadVisionLoactionFiles(getCurrentParameterDir().append(VISION_LOCATION_PARAMETER_FILE));
    tcp_manager.loadJsonConfig(getSystermParameterDir().append(TCP_CONFIG_FILE));
    return true;
}

bool BaseModuleManager::saveParameters()
{
    //pr文件拷贝
    this->parameters.saveJsonConfig(QString(CONFIG_DIR).append(SYSTERM_PARAM_FILE),SYSTERM_PARAMETER);
    material_tray.saveJsonConfig(getCurrentParameterDir().append(MATERIAL_TRAY_FILE));
    aa_head_module.saveJsonConfig(getCurrentParameterDir().append(AA_HEAD_FILE));
    sut_module.saveJsonConfig(getCurrentParameterDir().append(SUT_FILE));
    dothinkey->saveJsonConfig(getCurrentParameterDir().append(DOTHINGKEY_FILE));
    dispenser.parameters.saveJsonConfig(getCurrentParameterDir().append(DISPENSER_FILE),DISPENSER_PARAMETER);
    dispense_module.parameters.saveJsonConfig(getCurrentParameterDir().append(DISPENSE_MODULE_FILE),DISPENSER_MODULE_PARAMETER);
    sut_carrier.parameters.saveJsonConfig(getCurrentParameterDir().append(SUT_CARRIER_FILE),"sut");
    if(ServerMode())
    {
        sensor_loader_module.saveJsonConfig(getCurrentParameterDir().append(SENSOR_LOADER_FILE));
        sensor_pickarm.parameters.saveJsonConfig(getCurrentParameterDir().append(SENSOR_PICKARM_FILE),SENSOR_PICKARM_PARAMETER);

        QMap<QString,PropertyBase*> temp_map;
        temp_map.insert("sensor_tray_loader", &sensor_tray_loder_module.parameters);
        temp_map.insert("sensor_clip_stand", &sensor_clip_stand);
        temp_map.insert("entance_clip", &entrance_clip.parameters);
        temp_map.insert("exit_clip", &exit_clip.parameters);
        PropertyBase::saveJsonConfig(getCurrentParameterDir().append(SENSOR_TRAY_LOADER_FILE), temp_map);
    }
    else
    {
        lut_module.saveJsonConfig(getCurrentParameterDir().append(LUT_FILE));
        lut_carrier.parameters.saveJsonConfig(getCurrentParameterDir().append(LUT_CARRIER_FILE),LUT_CARRIER_PARAMETER);
        lens_loader_module.saveJsonConfig(getCurrentParameterDir().append(LENS_LOADER_MODULE_FILE));
        lens_pick_arm.parameters.saveJsonConfig(getCurrentParameterDir().append(LENS_PICKARM_FILE),LENS_PICKARM_PARAMETER);
        tray_loader_module.parameters.saveJsonConfig(getCurrentParameterDir().append(TRAY_LOADER_FILE),TRAY_LOADER_PARAMETER);
        trayClipIn.standards_parameters.saveJsonConfig(getCurrentParameterDir().append(TRAY_CLIPIN_FILE),TRAY_CLIPIN_PARAMETER);
        trayClipOut.standards_parameters.saveJsonConfig(getCurrentParameterDir().append(TRAY_CLIPOUT_FILE),TRAY_CLIPOUT_PARAMETER);
    }
    aaCoreNew.saveJsonConfig(getCurrentParameterDir().append(AA_CORE_MODULE_FILE));
    saveMotorFile(getCurrentParameterDir().append(MOTOR_PARAMETER_FILE));
    saveCylinderFiles(getCurrentParameterDir().append(CYLINDER_PARAMETER_FILE));
    saveCalibrationFiles(getCurrentParameterDir().append(CALIBRATION_PARAMETER_FILE));
    saveVisionLoactionFiles(getCurrentParameterDir().append(VISION_LOCATION_PARAMETER_FILE));
    saveVacuumFiles(getCurrentParameterDir().append(VACUUM_PARAMETER_FILE));
    return true;
}

bool BaseModuleManager::saveStates()
{
    if(ServerMode())
    {
        sensor_loader_module.states.saveJsonConfig(getCurrentStatesDir().append(SENSOR_LOADER_FILE),"States");
        sensor_tray_loder_module.states.saveJsonConfig(getCurrentStatesDir().append(SENSOR_TRAY_LOADER_FILE),"States");
    }
    else
    {
        lut_module.states.saveJsonConfig(getCurrentStatesDir().append(LUT_FILE),"States");
        lens_loader_module.states.saveJsonConfig(getCurrentStatesDir().append(LENS_LOADER_MODULE_FILE),"States");
        tray_loader_module.states.saveJsonConfig(getCurrentStatesDir().append(TRAY_LOADER_FILE),"States");
    }
    sut_module.states.saveJsonConfig(getCurrentStatesDir().append(SUT_FILE),"States");
    return true;
}

bool BaseModuleManager::loadDispenseParam()
{
    dispenser.parameters.loadJsonConfig(getCurrentParameterDir().append(DISPENSER_FILE),DISPENSER_PARAMETER);
    return true;
}
void BaseModuleManager::showSettingDialog()
{
    XtVcMotor::showSettingDialog();
}

void BaseModuleManager::showMachineMap()
{
//    if (!machineMap->isHidden()) {
//        this->machineMap->close();
//    } else {
//        this->machineMap->show();
//    }
}

void BaseModuleManager::loadModuleParameter(QString module_name)
{
    if(module_name == sut_module.parameters.moduleName())
          sut_module.loadParams(getCurrentParameterDir().append(SUT_FILE));
}

bool BaseModuleManager::registerWorkers(WorkersManager *manager)
{
    bool result = true;
    if (this->ServerMode() == 0)
    {
        result &= manager->registerWorker(&lens_loader_module);
        workers.insert(lens_loader_module.Name(),&lens_loader_module);
        result &= manager->registerWorker(&lut_module);
        workers.insert(lut_module.Name(),&lut_module);
        result &= manager->registerWorker(&tray_loader_module);
        workers.insert(tray_loader_module.Name(),&tray_loader_module);
    }
    else {
        result &= manager->registerWorker(&sensor_loader_module);
        workers.insert(sensor_loader_module.Name(),&sensor_loader_module);
        result &= manager->registerWorker(&sensor_tray_loder_module);
        workers.insert(sensor_tray_loder_module.Name(),&sensor_tray_loder_module);
    }
    result &= manager->registerWorker(&sut_module);
    workers.insert(sut_module.Name(),&sut_module);
    result &= manager->registerWorker(&aaCoreNew);
    workers.insert(aaCoreNew.Name(),&aaCoreNew);
    result &= manager->registerWorker(visionModule);
    workers.insert(visionModule->Name(),visionModule);
    result &= manager->registerWorker(lightingModule);
    workers.insert(lightingModule->Name(),lightingModule);
    return result;
}

void BaseModuleManager::performHandling(int cmd)
{
    emit sendHandlingOperation(cmd);
}

void BaseModuleManager::loadAACoreParameter()
{
    aaCoreNew.loadJsonConfig(getCurrentParameterDir().append(AA_CORE_MODULE_FILE));
}

void BaseModuleManager::performHandlingOperation(int cmd)
{
    qInfo("performHandlingOperation cmd: ", cmd);
}

bool BaseModuleManager::loadProfile()
{
    if(profile_loaded)
        return true;
    profile_loaded = false;
    if(loadStructConfig(SYSTERM_PARAM_DIR))
        if(loadParameters())
        {
            InitStruct();
            profile_loaded = true;
        }
    return false;
}

bool BaseModuleManager::loadStructConfig(QString file_dir)
{
   bool result = loadMachineConfig(QString(file_dir).append(MACHINE_PARAM_FILE));
   if(!result)return false;
   //todo将和打料的型号无关的参数移入该函数
//   configs.loadJsonConfig()
    return true;
}

bool BaseModuleManager::loadMachineConfig(QString file_path)
{
    if(QCoreApplication::applicationDirPath()!=QDir::currentPath())
        file_path = QString("..//").append(file_path);
    int res = XT_Controler_Extend::Profile_Load((LPWSTR)file_path.toStdWString().c_str());
    if(res!=0)
    {
        QString temp_name;
        int input_count = XT_Controler_Extend::Profile_Get_IoIn_Count();
        XtGeneralInput::count = input_count;
        for (int i = 0; i < input_count; ++i) {
            temp_name = QString::fromStdWString(XT_Controler_Extend::Profile_Get_IoIn_Name(i));
            if(temp_name == ""||input_ios.contains(temp_name))
            {
                qDebug("this already has an input io : %s",temp_name.toStdString().c_str());
                continue;
            }
            XtGeneralInput* input_io = new XtGeneralInput(temp_name,i);
            input_ios.insert(temp_name,input_io);
        }
        int output_count = XT_Controler_Extend::Profile_Get_IoOut_Count();
        XtGeneralOutput::count = output_count;
        for (int i = 0; i < output_count; ++i) {
            temp_name = QString::fromStdWString(XT_Controler_Extend::Profile_Get_IoOut_Name(i));
            if(temp_name == ""||output_ios.contains(temp_name))
            {
                qDebug("this already has an output io : %s",temp_name.toStdString().c_str());
                continue;
            }
            XtGeneralOutput* output_io = new XtGeneralOutput(temp_name,i);
            output_ios.insert(temp_name,output_io);
        }
        int motor_count = XT_Controler_Extend::Profile_Get_Axis_Count();
        XtMotor::axis_id_resource = motor_count;
        for (int i = 0; i < motor_count; ++i) {
            temp_name = QString::fromStdWString(XT_Controler_Extend::Profile_Get_Axis_Name(i));
            if(temp_name == ""||motors.contains(temp_name))
            {
                qDebug("this already has an motor io : %s",temp_name.toStdString().c_str());
                continue;
            }
            XtMotor* motor = new XtMotor();
            motor->Init(temp_name);

            motors.insert(temp_name,motor);
        }
        return true;
    }
}

bool BaseModuleManager::loadVcmFile(QString file_name)
{
    QJsonArray array;
    if(!loadJsonArray(file_name,array))return false;
    for (int i = 0; i < array.count(); i++)
    {
        XtVcMotor* temp_motor = new XtVcMotor();
        temp_motor->parameters.read(array.at(i).toObject());
        temp_motor->Init();
        QJsonObject temp_object;
        temp_motor->parameters.write(temp_object);
        if(!motors.contains(temp_motor->parameters.motorName()))
            motors.insert(temp_motor->parameters.motorName(),temp_motor);
        else
        {
            qDebug("vcm motor param name(%s)repeat!",temp_motor->parameters.motorName().toStdString().c_str());
            delete temp_motor;
        }
    }
    return true;
}

bool BaseModuleManager::saveVcmfile(QString file_name)
{
    QJsonArray array;
    foreach (QString motor_name, motors.keys()) {
        XtVcMotor* temp_motor = GetVcMotorByName(motor_name);
        if(temp_motor != nullptr)
        {
            QJsonObject object;
            temp_motor->parameters.write(object);
            array.append(object);
        }
    }
    if(array.size() > 0)
        return  saveJsonArray(file_name,array);
    return  false;
}

bool BaseModuleManager::loadMotorFile(QString file_name)
{
    QJsonObject param_object;
    if(!loadJsonObject(file_name,param_object))return false;
    foreach (XtMotor* temp_motor, motors)
        if(param_object.contains(temp_motor->Name()))
                temp_motor->parameters.read(param_object[temp_motor->Name()].toObject());
    return true;
}

bool BaseModuleManager::saveMotorFile(QString file_name)
{
    QJsonObject param_object;
    foreach (XtMotor* temp_motor, motors)
    {
        QJsonObject temp_object;
        temp_motor->parameters.write(temp_object);
        param_object[temp_motor->Name()] = temp_object;
    }
    return saveJsonObject(file_name,param_object);
}

bool BaseModuleManager::loadVacuumFiles(QString file_name)
{
    QJsonArray array;
    if(!loadJsonArray(file_name,array))
    {
//        saveVacuumFiles(file_name);
        return false;
    }
    for (int i = 0; i < array.count(); i++)
    {
        XtVacuum* temp_vacuum = new XtVacuum();
        temp_vacuum->parameters.read(array.at(i).toObject());
        QJsonObject temp_object;
        temp_vacuum->parameters.write(temp_object);
        if(!vacuums.contains(temp_vacuum->parameters.vacuumName()))
            vacuums.insert(temp_vacuum->parameters.vacuumName(),temp_vacuum);
        else
        {
            vacuums[temp_vacuum->parameters.vacuumName()]->parameters.read(temp_object);
//            qInfo("vcm motor param name(%s)repeat!",temp_vacuum->parameters.vacuumName().toStdString().c_str());
            delete temp_vacuum;
        }
    }
    return true;
}

bool BaseModuleManager::saveVacuumFiles(QString file_name)
{
    QJsonArray array;
    foreach (QString temp_name, vacuums.keys()) {
        XtVacuum* temp_vacuum = GetVacuumByName(temp_name);
        if(temp_vacuum != nullptr)
        {
            QJsonObject object;
            temp_vacuum->parameters.write(object);
            array.append(object);
        }
    }
    if(array.size() > 0)
        return  saveJsonArray(file_name,array);
//    else
//    {
//        XtVacuumParameter temp_param;
//        QString vcauum_name = temp_param.vacuumName();
//        QJsonArray json;
//        for (int i = 0; i < 6; ++i) {
//            QJsonObject temp_object;
//            QString temp_name = vcauum_name;
//            temp_name.append(QString::number(i));
//            temp_param.setVacuumName(temp_name);
//            temp_param.write(temp_object);
//            json.append(temp_object);
//        }
//        return  saveJsonArray(file_name,json);
//    }
    return  false;
}

bool BaseModuleManager::loadCylinderFiles(QString file_name)
{
    QJsonArray array;
    if(!loadJsonArray(file_name,array))
    {
//        saveCylinderFiles(file_name);
        return false;
    }
    for (int i = 0; i < array.count(); i++)
    {
        XtCylinder* temp_cylinder = new XtCylinder();
        temp_cylinder->parameters.read(array.at(i).toObject());
        QJsonObject temp_object;
        temp_cylinder->parameters.write(temp_object);
        if(!cylinder.contains(temp_cylinder->parameters.cylinderName()))
            cylinder.insert(temp_cylinder->parameters.cylinderName(),temp_cylinder);
        else
        {
            cylinder[temp_cylinder->parameters.cylinderName()]->parameters.read(temp_object);
//            qInfo("vcm motor param name(%s)repeat!",temp_cylinder->parameters.cylinderName().toStdString().c_str());
            delete temp_cylinder;
        }
    }
    return true;
}

bool BaseModuleManager::saveCylinderFiles(QString file_name)
{
    QJsonArray array;
    foreach (QString temp_name, cylinder.keys()) {
        XtCylinder* temp_cylinder = GetCylinderByName(temp_name);
        if(temp_cylinder != nullptr)
        {
            QJsonObject object;
            temp_cylinder->parameters.write(object);
            array.append(object);
        }
    }
    if(array.size() > 0)
        return  saveJsonArray(file_name,array);
    return  false;
}

bool BaseModuleManager::loadVisionLoactionFiles(QString file_name)
{
    QJsonArray array;
    if(!loadJsonArray(file_name,array))
    {
        return false;
    }
    for (int i = 0; i < array.count(); i++)
    {
        VisionLocation* temp_location = new VisionLocation();
        temp_location->parameters.read(array.at(i).toObject());
        QJsonObject temp_object;
        temp_location->parameters.write(temp_object);
        if(!vision_locations.contains(temp_location->parameters.locationName()))
            vision_locations.insert(temp_location->parameters.locationName(),temp_location);
        else
        {
            vision_locations[temp_location->parameters.locationName()]->parameters.read(temp_object);
            delete temp_location;
        }
    }

    return true;
}

bool BaseModuleManager::saveVisionLoactionFiles(QString file_name)
{
    QJsonArray array;
    foreach (QString temp_name, vision_locations.keys()) {
        VisionLocation* temp_location = GetVisionLocationByName(temp_name);
        if(temp_location != nullptr)
        {
            QJsonObject object;
            temp_location->parameters.write(object);
            array.append(object);
        }
    }
    if(array.size() > 0)
        return  saveJsonArray(file_name,array);
//    else
//    {
//        VisionLocationParameter temp_param;
//        QString location_name = temp_param.locationName();
//        QJsonArray json;
//        for (int i = 0; i < 6; ++i) {
//            QJsonObject temp_object;
//            QString temp_name = location_name;
//            temp_name.append(QString::number(i));
//            temp_param.setLocationName(temp_name);
//            temp_param.write(temp_object);
//            json.append(temp_object);
//        }
//        return  saveJsonArray(file_name,json);
//    }
    return  false;
}

bool BaseModuleManager::loadCalibrationFiles(QString file_name)
{
    QJsonArray array;
    if(!loadJsonArray(file_name,array))
    {
//        saveCalibrationFiles(file_name);
        return false;
    }
    for (int i = 0; i < array.count(); i++)
    {
        QJsonObject data_object =array.at(i).toObject();
        Calibration* temp_calibration;
        if(data_object["calibrationName"].toString().contains("chart_calibration"))
        {
            temp_calibration = chart_calibration = new ChartCalibration(dothinkey, aaCoreNew.parameters.MaxIntensity(), aaCoreNew.parameters.MinArea(),  aaCoreNew.parameters.MaxArea(), CHART_CALIBRATION, CALIBRATION_RESULT_PATH);
        }
        else
            temp_calibration = new Calibration();
        temp_calibration->parameters.read(data_object);
        QJsonObject temp_object;
        temp_calibration->parameters.write(temp_object);
        if(!calibrations.contains(temp_calibration->parameters.calibrationName()))
            calibrations.insert(temp_calibration->parameters.calibrationName(),temp_calibration);
        else
        {
            qDebug("vcm motor param name(%s)repeat!",temp_calibration->parameters.calibrationName().toStdString().c_str());
            delete temp_calibration;
        }
    }
    return true;
}

bool BaseModuleManager::saveCalibrationFiles(QString file_name)
{
    QJsonArray array;
    foreach (QString temp_name, calibrations.keys()) {
        Calibration* temp_calibration = GetCalibrationByName(temp_name);
        if(temp_calibration != nullptr)
        {
            QJsonObject object;
            temp_calibration->parameters.write(object);
            array.append(object);
        }
    }
    if(array.size() > 0)
        return  saveJsonArray(file_name,array);
//    else
//    {
//        CalibrationParameter temp_param;
//        QString caibration_name = temp_param.calibrationName();
//        QJsonArray json;
//        for (int i = 0; i < 6; ++i) {
//            QJsonObject temp_object;
//            QString temp_name = caibration_name;
//            temp_name.append(QString::number(i));
//            temp_param.setCalibrationName(temp_name);
//            temp_param.write(temp_object);
//            json.append(temp_object);
//        }
//        return  saveJsonArray(file_name,json);
//    }
    return  false;
}

bool BaseModuleManager::loadMotorLimitFiles(QString file_name)
{
    QJsonObject param_object;
    if(!loadJsonObject(file_name,param_object))
        return false;
    foreach (XtMotor* temp_motor, motors)
    {
        temp_motor->vertical_limit_parameters.clear();
        temp_motor->parallel_limit_parameters.clear();
        temp_motor->io_limit_parameters.clear();
        if(param_object.contains(temp_motor->Name()))
        {
           QJsonObject temp_object = param_object[temp_motor->Name()].toObject();
           for (int i = 0; i < temp_object.size(); ++i)
           {
               QString temp_name = temp_object.keys()[i];
               if(temp_name.contains("VerticalMotorLimit"))
               {
                   VerticalLimitParameter* temp_param = new VerticalLimitParameter();
                   temp_param->read(temp_object[temp_name].toObject());
                   temp_motor-> vertical_limit_parameters.append(temp_param);
                   continue;
               }
               if(temp_name.contains("ParallelMotorLimit"))
               {
                   ParallelLimitParameter* temp_param = new ParallelLimitParameter();
                   temp_param->read(temp_object[temp_name].toObject());
                   temp_motor->parallel_limit_parameters.append(temp_param);
                   continue;
               }
               if(temp_name.contains("IOLimit"))
               {
                   IOLimitParameter* temp_param = new IOLimitParameter();
                   temp_param->read(temp_object[temp_name].toObject());
                   temp_motor->io_limit_parameters.append(temp_param);
                   continue;
               }
           }
        }
        qInfo("%s loadmotorlimit motors limit %d %d %d",temp_motor->Name().toStdString().c_str(),temp_motor->vertical_limit_parameters.count(),temp_motor->parallel_limit_parameters.count(),temp_motor->io_limit_parameters.count());
    }
    return true;
}

bool BaseModuleManager::saveMotorLimitFiles(QString file_name)
{
    QJsonObject parameters_object;
    foreach (QString temp_name, motors.keys())
    {
        XtMotor* temp_motor = GetMotorByName(temp_name);
        if(temp_motor != nullptr)
        {
            QJsonObject temp_object;
            int i = 0;
            foreach(VerticalLimitParameter* limit,temp_motor->vertical_limit_parameters)
            {
                i++;
                QJsonObject object;
                limit->write(object);
                temp_object[QString("VerticalMotorLimit").append(QString::number(i))] = object;
            }
            i = 0;
            foreach(ParallelLimitParameter* limit,temp_motor->parallel_limit_parameters)
            {
                i++;
                QJsonObject object;
                limit->write(object);
                temp_object[QString("ParallelMotorLimit").append(QString::number(i))] = object;
            }
            i = 0;
            foreach(IOLimitParameter* io_limit,temp_motor->io_limit_parameters)
            {
                i++;
                QJsonObject object;
                io_limit->write(object);
                temp_object[QString("IOLimit").append(QString::number(i))] = object;
            }
            if(temp_object.count()>0)
                parameters_object[temp_name] = temp_object;
        }
    }
   return saveJsonObject(file_name,parameters_object);
}

bool BaseModuleManager::loadJsonArray(QString file_name,QJsonArray &array)
{
    QString val;
    QFile file;
    file.setFileName(file_name);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        AppendError(QString(u8"加载参数文件失败，文件名 %1").arg(file_name));
        qWarning("load parameters to %s failed, Couldn't open save file.",file_name.toStdString().data());
        return false;
    }
    val = file.readAll();
    file.close();

    QJsonParseError error;
    QJsonDocument doucment = QJsonDocument::fromJson(val.toUtf8(), &error);
    if (!doucment.isNull() && (error.error == QJsonParseError::NoError)) { //解析否出现错误
        if (doucment.isArray()) { // 数组判断
            array = doucment.array(); // 转数组
        }
    }
    if(array.size()>0)
        return true;
    else
    {
        AppendError(QString(u8"解析参数文件失败，文件名 %1").arg(file_name));
        qInfo("load parameters to %s failed, Couldn't open save file.",file_name.toStdString().data());
        return false;
    }
}

bool BaseModuleManager::saveJsonArray(QString file_name,QJsonArray &array)
{
    QFile file;
    file.setFileName(file_name);
    if(!file.open(QFile::WriteOnly)){
        AppendError(QString(u8"保存参数文件失败，文件名 %1").arg(file_name));
        qWarning("save parameters to %s failed, Couldn't open save file.",file_name.toStdString().data());
        return false;
    }
    QJsonDocument document;
    document.setArray(array);
    file.write(document.toJson());
    file.close();
    return true;
}

bool BaseModuleManager::loadJsonObject(QString file_name, QJsonObject &object)
{
    QString val;
    QFile file;
    file.setFileName(file_name);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qWarning("load parameters to %s failed, Couldn't open save file.",file_name.toStdString().data());
        return false;
    }
    val = file.readAll();
    file.close();

    QJsonParseError error;
    QJsonDocument doucment = QJsonDocument::fromJson(val.toUtf8(), &error);
    if (!doucment.isNull() && (error.error == QJsonParseError::NoError)) { //解析否出现错误
        if (doucment.isObject()) { // 数组判断
            object = doucment.object(); // 转数组
            return true;
        }
    }
    qInfo("load parameters to %s failed, load file error:%s.",file_name.toStdString().data(),error.errorString().toStdString().c_str());
    return false;
}

bool BaseModuleManager::saveJsonObject(QString file_name, QJsonObject &object)
{
    QFile file;
    file.setFileName(file_name);
    if(!file.open(QFile::WriteOnly)){
        qWarning("save parameters to %s failed, Couldn't open save file.",file_name.toStdString().data());
        return false;
    }
    QJsonDocument document;
    document.setObject(object);
    file.write(document.toJson());
    file.close();
    return true;
}

QString BaseModuleManager::getCurrentParameterDir()
{
    QString dir = QString(CONFIG_DIR).append(parameters.materialType()).append("//");
    if(!QDir(dir).exists())
        QDir().mkdir(dir);
    return dir;
}

QString BaseModuleManager::getCurrentStatesDir()
{
    QString dir = QString(STATES_DIR).append(getCurrentDateString()).append("//");
    if(!QDir(dir).exists())
        QDir().mkdir(dir);
    return dir;

}

QString BaseModuleManager::getSystermParameterDir()
{
    QString dir = QString(SYSTERM_PARAM_DIR);
    return dir;
}

bool BaseModuleManager::InitStruct()
{
    qInfo("InitStruct");
    tcp_manager.Init();
    foreach (QVariant messager_name, parameters.respMessagerNames()) {
        TcpMessager* temp_messager = tcp_manager.GetAllTcpMessager(messager_name.toString());
        if(temp_messager!=nullptr)
        {
            receive_messagers.insert(messager_name.toString(),temp_messager);
            connect(temp_messager,&TcpMessager::receiveTextMessage,this,&BaseModuleManager::tcpResp);
        }
    }
    foreach (QVariant messager_name, parameters.cmsMessageerNames()) {
        TcpMessager* temp_messager = tcp_manager.GetAllTcpMessager(messager_name.toString());
        if(temp_messager!=nullptr)
        {
            sender_messagers.insert(messager_name.toString(),temp_messager);
            connect(temp_messager,&TcpMessager::receiveTextMessage,this,&BaseModuleManager::tcpResp);
        }
    }
    bool result = true;
    foreach (XtMotor* temp_motor, motors)
        temp_motor->Init(&state_geter);

    foreach (XtVacuum* temp_vacuum, vacuums.values()) {
        temp_vacuum->Init(GetOutputIoByName(temp_vacuum->parameters.outIoName()),
                          GetInputIoByName(temp_vacuum->parameters.inIoName()),
                          GetOutputIoByName(temp_vacuum->parameters.breakIoName()));
    }

    foreach (XtCylinder* temp_cylinder, cylinder.values()) {
        temp_cylinder->Init(GetOutputIoByName(temp_cylinder->parameters.oneOutName()),
                            GetInputIoByName(temp_cylinder->parameters.oneInName()),
                            GetInputIoByName(temp_cylinder->parameters.zeroInName()),
                            GetOutputIoByName(temp_cylinder->parameters.zeroOutName()));
    }
    sut_carrier.Init("sut_carrier",GetMotorByName(sut_module.parameters.motorXName()),
                     GetMotorByName(sut_module.parameters.motorYName()),
                     GetVcMotorByName(sut_module.parameters.motorZName()),
                     GetVacuumByName(sut_module.parameters.vacuumName()));
    aa_head_module.Init("AAHead",GetMotorByName(aa_head_module.parameters.motorXName()),
                        GetMotorByName(aa_head_module.parameters.motorYName()),
                        GetMotorByName(aa_head_module.parameters.motorZName()),
                        GetMotorByName(aa_head_module.parameters.motorAName()),
                        GetMotorByName(aa_head_module.parameters.motorBName()),
                        GetMotorByName(aa_head_module.parameters.motorCName()),
                        GetOutputIoByName(aa_head_module.parameters.gripperName()),
                        GetOutputIoByName(aa_head_module.parameters.uv1Name()),
                        GetOutputIoByName(aa_head_module.parameters.uv2Name()),
                        GetOutputIoByName(aa_head_module.parameters.uv3Name()),
                        GetOutputIoByName(aa_head_module.parameters.uv4Name()),
                        XtMotor::GetThreadResource(),
                        &sut_carrier);

    foreach (Calibration* temp_calibraion, calibrations) {
        temp_calibraion->Init(GetMotorByName(temp_calibraion->parameters.motorXName()),
                              GetMotorByName(temp_calibraion->parameters.motorYName()),
                              GetVisionLocationByName(temp_calibraion->parameters.locationName()));
    }
    if (chart_calibration) {
        chart_calibration->Init(GetMotorByName(chart_calibration->parameters.motorXName()),
                                GetMotorByName(chart_calibration->parameters.motorYName()),
                                nullptr);
        connect(chart_calibration, &Calibration::updata_aaCore_sensor_parameters_signal, &aaCoreNew, &AACoreNew::updateAACoreSensorParameters);
    }
    foreach (VisionLocation* temp_vision, vision_locations.values()) {
        temp_vision->Init(visionModule,GetPixel2MechByName(temp_vision->parameters.calibrationName()),lightingModule);
    }
//    sut_clitent->Init(GetVacuumByName(sut_module.parameters.vacuumName()));
    sut_module.Init(&sut_carrier,
                    GetVisionLocationByName(sut_module.parameters.downlookLocationName()),
                    GetVisionLocationByName(sut_module.parameters.updownlookDownLocationName()),
                    GetVisionLocationByName(sut_module.parameters.updownlookUpLocationName()),
                    GetVacuumByName(sut_module.parameters.vacuumName()),
                    GetCylinderByName(sut_module.parameters.cylinderName()),
                    GetOutputIoByName(sut_module.parameters.downlookFlyIoName()),
                    XtMotor::GetThreadResource());
    QVector<XtMotor *> executive_motors;
    executive_motors.push_back(GetMotorByName(sut_module.parameters.motorXName()));
    executive_motors.push_back(GetMotorByName(sut_module.parameters.motorYName()));
    executive_motors.push_back(GetMotorByName(sut_module.parameters.motorZName()));
    dispenser.Init(XtMotor::GetCurveResource(),XtMotor::GetThreadResource(),XtMotor::GetThreadResource(),executive_motors,
                   GetOutputIoByName(dispenser.parameters.dispenseIo()));
    dispense_module.Init(DISPENSER_PARAMETER_PATH,"dispense_module",
                         GetCalibrationByName(GetVisionLocationByName(sut_module.parameters.downlookLocationName())->parameters.calibrationName()),
                         &dispenser,visionModule,&sut_carrier,
                         GetOutputIoByName(dispenser.parameters.dispenseIo()));
    dispense_module.setMapPosition(sut_module.downlook_position.X(),sut_module.downlook_position.Y());

    qInfo("CP1 fuck");
    if(ServerMode())
    {
        sensor_picker1.Init(GetVcMotorByName(sensor_pickarm.parameters.motorZName()),
                            GetMotorByName(sensor_pickarm.parameters.motorTName()),
                            GetVacuumByName(sensor_pickarm.parameters.vacuumName()));
        sensor_picker2.Init(GetVcMotorByName(sensor_pickarm.parameters.motorZ2Name()),
                            GetMotorByName(sensor_pickarm.parameters.motorT2Name()),
                            GetVacuumByName(sensor_pickarm.parameters.vacuum2Name()));
        sensor_pickarm.Init(GetMotorByName(sensor_pickarm.parameters.motorXName()),
                            GetMotorByName(sensor_pickarm.parameters.motorYName()),
                            &sensor_picker1,&sensor_picker2);
        sensor_loader_module.Init(&sensor_pickarm,&material_tray,
                                  GetVisionLocationByName(sensor_loader_module.parameters.sensorLocationName()),
                                  GetVisionLocationByName(sensor_loader_module.parameters.vacancyLocationName()),
                                  GetVisionLocationByName(sensor_loader_module.parameters.sutLocationName()),
                                  GetVisionLocationByName(sensor_loader_module.parameters.sutSensorLocationName()),
                                  GetVisionLocationByName(sensor_loader_module.parameters.sutProductLocationName()),
                                  GetVisionLocationByName(sensor_loader_module.parameters.calibrationGlassLocationName()),
                                  XtMotor::GetThreadResource());
        //connect(&sensor_loader_module,&SensorLoaderModule::sendMsgSignal,this,&BaseModuleManager::sendMessageTest);
    }
    else
    {
        lut_carrier.Init("lut_carrier",GetMotorByName(lut_module.parameters.motorXName()),
                         GetMotorByName(lut_module.parameters.motorYName()),
                         GetVcMotorByName(lut_module.parameters.motorZName()),
                         GetVacuumByName(lut_module.parameters.vacuum1Name()));
        lut_module.Init(&lut_carrier,GetVisionLocationByName(lut_module.parameters.uplookLocationName()),
                        GetVisionLocationByName(lut_module.parameters.loadlookLocationName()),
                        GetVisionLocationByName(lut_module.parameters.mushroomLocationName()),
                        GetVacuumByName(lut_module.parameters.vacuum1Name()),
                        GetVacuumByName(lut_module.parameters.vacuum2Name()),
                        GetOutputIoByName(aa_head_module.parameters.gripperName()), &sut_module,
                        XtMotor::GetThreadResource());
        lens_picker.Init(GetVcMotorByName(lens_pick_arm.parameters.motorZName()),
                         GetMotorByName(lens_pick_arm.parameters.motorTName()),
                         GetVacuumByName(lens_pick_arm.parameters.vacuumName()));
        lens_pick_arm.Init(GetMotorByName(lens_pick_arm.parameters.motorTrayName()),
                           GetMotorByName(lens_pick_arm.parameters.motorXName()),
                           GetMotorByName(lens_pick_arm.parameters.motorYName()),
                           &lens_picker);
        lens_loader_module.Init(&lens_pick_arm,&material_tray,&lut_carrier,
                                GetVacuumByName(lut_module.parameters.vacuum1Name()),
                                GetVacuumByName(lut_module.parameters.vacuum2Name()),
                                GetVisionLocationByName(lens_loader_module.parameters.lensLocationName()),
                                GetVisionLocationByName(lens_loader_module.parameters.vacancyLocationName()),
                                GetVisionLocationByName(lens_loader_module.parameters.lutLocationName()),
                                GetVisionLocationByName(lens_loader_module.parameters.lutLensLocationName()),
                                GetVisionLocationByName(lens_loader_module.parameters.lpaUplookPickerLocationName()),
                                GetVisionLocationByName(lens_loader_module.parameters.lpaUpdownlookUpLocationName()),
                                GetVisionLocationByName(lens_loader_module.parameters.lpaUpdownlookDownLocationName()),
                                GetVisionLocationByName(lens_loader_module.parameters.lutNgSlotLocationName()));

    }
    tray_loader_module.Init(GetMotorByName(tray_loader_module.parameters.motorLTIEName()),
                            GetMotorByName(tray_loader_module.parameters.motorLTKX1Name()),
                            GetMotorByName(tray_loader_module.parameters.motorLTLXName()),
                            GetMotorByName(tray_loader_module.parameters.motorLTKX2Name()),
                            GetMotorByName(tray_loader_module.parameters.motorLTOEName()),
                            GetCylinderByName(tray_loader_module.parameters.cylinderClipName()),
                            GetCylinderByName(tray_loader_module.parameters.cylinderLTK1Name()),
                            GetCylinderByName(tray_loader_module.parameters.cylinderLTK2Name()),
                            GetCylinderByName(tray_loader_module.parameters.cylinderTrayName()),
                            &trayClipIn,&trayClipOut,
                            GetInputIoByName(tray_loader_module.parameters.workTrayCheckIoName()),
                            GetInputIoByName(tray_loader_module.parameters.entranceTrayCheckIoName()),
                            GetInputIoByName(tray_loader_module.parameters.exitTrayCheckIoName()),
                            GetInputIoByName(tray_loader_module.parameters.entranceClipCheckIoName()),
                            GetInputIoByName(tray_loader_module.parameters.exitClipCheckIoName()),
                            GetInputIoByName(tray_loader_module.parameters.readyTrayCheckIoName()));

    sfrWorkerController = new SfrWorkerController(&aaCoreNew);
    aaCoreNew.setSfrWorkerController(sfrWorkerController);
    aaCoreNew.Init(&aa_head_module, &sut_module, dothinkey, chart_calibration, &dispense_module, imageGrabberThread, &unitlog, ServerMode());
    entrance_clip.Init(u8"Sensor进料盘弹夹",&sensor_clip_stand);
    exit_clip.Init(u8"Sensor出料盘弹夹",&sensor_clip_stand);
    sensor_tray_loder_module.Init(GetMotorByName(sensor_tray_loder_module.parameters.motorTrayName()),
                                  GetMotorByName(sensor_tray_loder_module.parameters.motorSTKName()),
                                  GetMotorByName(sensor_tray_loder_module.parameters.motorSTIEName()),
                                  GetMotorByName(sensor_tray_loder_module.parameters.motorSTOEName()),
                                  GetMotorByName(sensor_tray_loder_module.parameters.motorSPOName()),
                                  GetCylinderByName(sensor_tray_loder_module.parameters.cylinderSTK1Name()),
                                  GetCylinderByName(sensor_tray_loder_module.parameters.cylinderSTK2Name()),
                                  GetCylinderByName(sensor_tray_loder_module.parameters.cylinderHoldTrayName()),
                                  GetCylinderByName(sensor_tray_loder_module.parameters.cylinderVacancyTrayName()),
                                  GetCylinderByName(sensor_tray_loder_module.parameters.cylinderEntanceClipPushName()),
                                  GetCylinderByName(sensor_tray_loder_module.parameters.cylinderExitClipPushName()),
                                  GetCylinderByName(sensor_tray_loder_module.parameters.cylinderGripperName()),
                                  &entrance_clip,&exit_clip,
                                  GetInputIoByName(sensor_tray_loder_module.parameters.entranceClipCheckoName()),
                                  GetInputIoByName(sensor_tray_loder_module.parameters.exitClipCheckIoName()),
                                  GetInputIoByName(sensor_tray_loder_module.parameters.readyTrayCheckIoName()),
                                  GetInputIoByName(sensor_tray_loder_module.parameters.kickTrayCheckIoName()),
                                  GetInputIoByName(sensor_tray_loder_module.parameters.sensorTrayCheckIoName()),
                                  GetInputIoByName(sensor_tray_loder_module.parameters.vacancyTrayCheckIoName()),
                                  GetInputIoByName(sensor_tray_loder_module.parameters.entanceTrayChcekIoName()),
                                  GetInputIoByName(sensor_tray_loder_module.parameters.exitTrayCheckIoName()));
    //todo
    material_tray.resetTrayState(0);
    material_tray.resetTrayState(1);
    return result;
}

void BaseModuleManager::inquiryTcpAllMotors()
{
    QJsonObject message;
    message["cmd"] = "inquiryAllMotorNames";
    foreach (TcpMessager* temp_messager, sender_messagers) {
        temp_messager->sendMessage(TcpMessager::getStringFromJsonObject(message));
    }
}

void BaseModuleManager::inquiryTcpModule()
{
    QJsonObject message;
    message["cmd"] = "inquiryModuleNames";
    foreach (TcpMessager* temp_messager, sender_messagers) {
        temp_messager->sendMessage(TcpMessager::getStringFromJsonObject(message));
    }
}

void BaseModuleManager::inquiryTcpVisionLocations()
{
    QJsonObject message;
    message["cmd"] = "inquiryVisionLocations";
    foreach (TcpMessager* temp_messager, sender_messagers) {
        temp_messager->sendMessage(TcpMessager::getStringFromJsonObject(message));
    }
}

void BaseModuleManager::inquiryTcpModuleState(QString moduleName)
{
    QJsonObject message;
    message["cmd"] = "inquiryModuleState";
    message["moduleName"] = moduleName;
    foreach (TcpMessager* temp_messager, sender_messagers) {
        temp_messager->sendMessage(TcpMessager::getStringFromJsonObject(message));
    }
}

void BaseModuleManager::inquiryTcpModuleParameter(QString moduleName)
{
    QJsonObject message;
    message["cmd"] = "inquiryModuleParameter";
    message["moduleName"] = moduleName;
    foreach (TcpMessager* temp_messager, sender_messagers) {
        temp_messager->sendMessage(TcpMessager::getStringFromJsonObject(message));
    }
}

void BaseModuleManager::sendTcpUpdateParameterRequest()
{
    QJsonObject message;
    message["cmd"] = "needUpdateParameter";
    foreach (TcpMessager* temp_messager, sender_messagers) {
        temp_messager->sendMessage(TcpMessager::getStringFromJsonObject(message));
    }
}

void BaseModuleManager::setTcpModuleParameter(QString moduleName)
{
    QJsonObject message;
    message["cmd"] = "setModuleParameter";
    message["moduleName"] = moduleName;

    QMap<QString, PropertyBase *> map;
    if (moduleName.contains("Sut")){
        map = tcp_sut.getModuleParameter();
    }else if (moduleName.contains("LensLoaderModule")){
        map = tcp_lensLoaderModule.getModuleParameter();
    }else if (moduleName.contains("LensTrayLoaderModule")){
        map = tcp_lensTrayLoaderModule.getModuleParameter();
    }else if (moduleName.contains("AA")){
        map = tcp_aaCoreNew.getModuleParameter();
    }else if (moduleName.contains("LUTModule")){
        map = tcp_lutModule.getModuleParameter();
    }
    foreach (QString param_name, map.keys()) {
        QJsonObject module_parameter;
        map[param_name]->write(module_parameter);
        message[param_name] = module_parameter;
    }
    foreach (TcpMessager* temp_messager, sender_messagers) {
        temp_messager->sendMessage(TcpMessager::getStringFromJsonObject(message));
    }
}

bool BaseModuleManager::initialDevice()
{
    if(InitState())
        return true;
    if(!profile_loaded)
        return false;
    qInfo("Init module manager");
    LPWSTR pTarget = ip;
    XT_Controler::InitDevice_PC_Local_Controler(0);
    int res = XT_Controler::beCurConnectServerAndInterfaceBoard();
    if (1 != res) {
        res = XT_Controler::ConnectControlServer(pTarget, 0, 0);
        if (1 != res)
        {
            qInfo("Motion control server cannot connect");
            return false;
        }
    }

    XT_Controler::ReBuildSystem();

    QTime tic;
    tic.start();
    while(tic.elapsed()<300);

    res = XT_Controler::ConnectControlServer(pTarget, 0, 0);

    if (1 != res)
    {
        qInfo("Motion control server cannot connect");
        return false;
    }

    res = XT_Controler_Extend::Profile_Init_Controller(1);

    if (1 != res)
    {
        qInfo("Motion control server cannot connect");
        return false;
    }
    XT_Controler_Extend::Stop_Buffer_Sync();

    XtVcMotor::InitAllVCM();
    XtVcMotor* temp_motor;
    foreach (QString mator_name, motors.keys()) {
        temp_motor = GetVcMotorByName(mator_name);
        if(temp_motor!= nullptr)
            temp_motor->ConfigVCM();
    }
    XT_Controler_Extend::Start_Buffer_Sync(-1);

    setInitState(true);
    //this must after "setInitState(true);!!!"
    foreach (XtMotor *m, motors.values()) {
        m->GetMasterAxisID();
    }
    enableMotors();

    if (ServerMode() == 1)
    {
        setOutput(u8"三色报警指示灯_绿", true);
        inquiryTcpModule();
        inquiryTcpVisionLocations();
        inquiryTcpAllMotors();
    }
    return true;
}

bool BaseModuleManager::generateConfigFiles()
{
    foreach (XtMotor* temp_motor, motors) {
        temp_motor->parallel_limit_parameters.append(new ParallelLimitParameter());
        temp_motor->vertical_limit_parameters.append(new VerticalLimitParameter());
        QVariantList temp_space;temp_space.append(0);temp_space.append(1);
        temp_motor->vertical_limit_parameters[0]->setMoveSpance(temp_space);
        temp_motor->vertical_limit_parameters[0]->setLimitSpance(temp_space);
        temp_motor->io_limit_parameters.append(new IOLimitParameter());
        temp_motor->io_limit_parameters[0]->setMoveSpance(temp_space);
    }
    bool result = saveMotorLimitFiles(LIMIT_PARAMETER_MODE_FILENAME);
    tcp_manager.generateConfig(getSystermParameterDir().append(TCP_CONFIG_FILE));
    return result;
}

void BaseModuleManager::enableMotor(QString motorName)
{
    if (motors.contains(motorName)) {
       if (InitState())
           GetMotorByName(motorName)->Enable();
    } else {
        qInfo("sendTcpEnableMotor");
        sendTcpEnableMotor(motorName, true);
    }
}

void BaseModuleManager::enableMotors()
{
    if(InitState()) {
        foreach (XtMotor *m, motors.values()) {
            m->Enable();
        }
    }
}

void BaseModuleManager::disableMotor(QString motorName)
{
    if (motors.contains(motorName)) {
       if (InitState())
           GetMotorByName(motorName)->Disable();
    } else {
        qInfo("sendTcpEnableMotor");
        sendTcpEnableMotor(motorName, false);
    }
}

void BaseModuleManager::disableAllMotors()
{
    if(InitState()) {
        foreach (XtMotor *m, motors.values()) {
            m->Disable();
        }
    }
}

bool BaseModuleManager::allMotorsSeekOrigin()
{
    if (ServerMode() == 0) {
        return allMotorsSeekOriginal1();
    } else {
        return allMotorsSeekOriginal2();
    }
}

bool BaseModuleManager::allMotorsSeekOriginal1()
{
    qInfo("allMotorsSeekOriginal1 Start");
    bool result;
    GetVcMotorByName(this->lut_module.parameters.motorZName())->resetSoftLanding();
    if(!GetCylinderByName(this->sut_module.parameters.cylinderName())->Set(true))
        return false;
    if(!GetCylinderByName(this->tray_loader_module.parameters.cylinderLTK2Name())->Set(1))
        return false;
    GetMotorByName(this->lut_module.parameters.motorZName())->SeekOrigin();//LUT_Z
    GetVcMotorByName(this->sut_module.parameters.motorZName())->SeekOrigin();//SUT_Z
    GetVcMotorByName(this->lens_pick_arm.parameters.motorZName())->SeekOrigin();//LPA_Z

    result = GetMotorByName(this->lut_module.parameters.motorZName())->WaitSeekDone();
    if(!result)return false;
    GetMotorByName(this->lut_module.parameters.motorYName())->SeekOrigin();//LUT_Y

    result = GetVcMotorByName(sut_module.parameters.motorZName())->WaitSeekDone();
    if (!result) return false;
    sut_module.moveZToSafety();
    GetMotorByName(this->sut_module.parameters.motorXName())->SeekOrigin();//SUT_X
    GetMotorByName(this->sut_module.parameters.motorYName())->SeekOrigin();//SUT_Y

    result = GetVcMotorByName(this->lens_pick_arm.parameters.motorZName())->WaitSeekDone();
    if(!result)return false;

    GetVcMotorByName(this->lens_pick_arm.parameters.motorXName())->SeekOrigin();//LPA_X
    GetMotorByName(this->lens_pick_arm.parameters.motorYName())->SeekOrigin();//LPA_Y
    GetMotorByName(this->lens_pick_arm.parameters.motorTName())->SeekOrigin();//LPA_R

    GetMotorByName(this->aa_head_module.parameters.motorAName())->SeekOrigin();//AA_A
    GetMotorByName(this->aa_head_module.parameters.motorBName())->SeekOrigin();//AA_B
    GetMotorByName(this->aa_head_module.parameters.motorCName())->SeekOrigin();//AA_C

    result = GetMotorByName(this->lut_module.parameters.motorYName())->WaitSeekDone();
    if(!result)return false;
    GetMotorByName(this->lut_module.parameters.motorXName())->SeekOrigin();//LUT_X

    //缩回气缸
    GetCylinderByName(this->tray_loader_module.parameters.cylinderClipName())->Set(0);

    GetMotorByName(this->tray_loader_module.parameters.motorLTIEName())->SeekOrigin();//LTIE

    GetMotorByName(this->tray_loader_module.parameters.motorLTOEName())->SeekOrigin();//LTOE



    result = GetMotorByName(this->lens_pick_arm.parameters.motorYName())->WaitSeekDone();
    if(!result)return false;
    //升起气缸,降下托盘
    //
    GetMotorByName(this->lens_pick_arm.parameters.motorTrayName())->SeekOrigin();//LTL


    GetCylinderByName(this->tray_loader_module.parameters.cylinderLTK1Name())->Set(0);
    GetCylinderByName(this->tray_loader_module.parameters.cylinderLTK2Name())->Set(0);
    GetMotorByName(this->tray_loader_module.parameters.motorLTKX1Name())->SeekOrigin();//LTK1
    GetMotorByName(this->tray_loader_module.parameters.motorLTKX2Name())->SeekOrigin();//LTK2


    result &= GetMotorByName(this->sut_module.parameters.motorXName())->WaitSeekDone();
    result &= GetMotorByName(this->sut_module.parameters.motorYName())->WaitSeekDone();

    result &= GetVcMotorByName(this->lens_pick_arm.parameters.motorXName())->WaitSeekDone();
    result &= GetMotorByName(this->lens_pick_arm.parameters.motorTName())->WaitSeekDone();

    result &= GetMotorByName(this->aa_head_module.parameters.motorAName())->WaitSeekDone();
    result &= GetMotorByName(this->aa_head_module.parameters.motorBName())->WaitSeekDone();
    result &= GetMotorByName(this->aa_head_module.parameters.motorCName())->WaitSeekDone();

    result &= GetMotorByName(this->lut_module.parameters.motorXName())->WaitSeekDone();

    result &= GetMotorByName(this->lens_pick_arm.parameters.motorTrayName())->WaitSeekDone();

    result &= GetMotorByName(this->tray_loader_module.parameters.motorLTKX1Name())->WaitSeekDone();
    result &= GetMotorByName(this->tray_loader_module.parameters.motorLTKX2Name())->WaitSeekDone();
    result &= GetMotorByName(this->tray_loader_module.parameters.motorLTIEName())->WaitSeekDone();
    result &= GetMotorByName(this->tray_loader_module.parameters.motorLTOEName())->WaitSeekDone();
    if(result)
    {
        qInfo("all motors seeked origin success!");
        setHomeState(true);
        this->aa_head_module.moveToMushroomPosition(true);
        return true;
    }
    return false;
}

bool BaseModuleManager::allMotorsSeekOriginal2()
{
    qInfo("allMotorsSeekOriginal2 Start");

    XtGeneralOutput *gripperIo = GetOutputIoByName(u8"夹爪稳压阀");
    if (gripperIo)
        gripperIo->Set(1);
    //推料氣缸復位
    bool result;
    if(!GetCylinderByName(this->sut_module.parameters.cylinderName())->Set(true))
        return false;
    GetMotorByName(sut_module.parameters.motorZName())->SeekOrigin();
    GetMotorByName(sensor_pickarm.parameters.motorZName())->SeekOrigin();
    GetMotorByName(sensor_pickarm.parameters.motorZ2Name())->SeekOrigin();
    result = GetMotorByName(sut_module.parameters.motorZName())->WaitSeekDone();
    if(!result) return false;
    sut_module.moveZToSafety();
    if(!result) return false;
    GetMotorByName(sut_module.parameters.motorYName())->SeekOrigin();

    result = GetMotorByName(sensor_pickarm.parameters.motorZName())->WaitSeekDone();
    result &= GetMotorByName(sensor_pickarm.parameters.motorZ2Name())->WaitSeekDone();
    if(!result) return false;
    //KicK復位
    GetMotorByName(this->sensor_pickarm.parameters.motorXName())->SeekOrigin();
    GetMotorByName(this->sensor_pickarm.parameters.motorTName())->SeekOrigin();
    GetMotorByName(this->sensor_pickarm.parameters.motorT2Name())->SeekOrigin();

    result &= GetMotorByName(sut_module.parameters.motorYName())->WaitSeekDone();
    if(!result)return false;

    GetMotorByName(this->aa_head_module.parameters.motorAName())->SeekOrigin();
    GetMotorByName(this->aa_head_module.parameters.motorBName())->SeekOrigin();
    GetMotorByName(this->aa_head_module.parameters.motorCName())->SeekOrigin();
    GetMotorByName(this->sut_module.parameters.motorXName())->SeekOrigin();
    if(!result)return false;
    GetMotorByName(this->sensor_pickarm.parameters.motorYName())->SeekOrigin();
    GetMotorByName(this->sensor_tray_loder_module.parameters.motorSPOName())->SeekOrigin();
    result &= GetMotorByName(this->sensor_tray_loder_module.parameters.motorSPOName())->WaitSeekDone();
    if(!result)return false;
    GetMotorByName(this->sensor_tray_loder_module.parameters.motorSTIEName())->SeekOrigin();
    GetMotorByName(this->sensor_tray_loder_module.parameters.motorSTOEName())->SeekOrigin();

    result &= GetMotorByName(this->sensor_pickarm.parameters.motorYName())->WaitSeekDone();
    if(!result)return false;
    //升起氣缸
    GetMotorByName(this->sensor_tray_loder_module.parameters.motorTrayName())->SeekOrigin();
    GetMotorByName(this->sensor_tray_loder_module.parameters.motorSTKName())->SeekOrigin();

    result &= GetMotorByName(this->aa_head_module.parameters.motorAName())->WaitSeekDone();
    result &= GetMotorByName(this->aa_head_module.parameters.motorBName())->WaitSeekDone();
    result &= GetMotorByName(this->aa_head_module.parameters.motorCName())->WaitSeekDone();
    result &= GetMotorByName(this->sut_module.parameters.motorXName())->WaitSeekDone();
    result &= GetMotorByName(this->sensor_pickarm.parameters.motorXName())->WaitSeekDone();
    result &= GetMotorByName(this->sensor_pickarm.parameters.motorTName())->WaitSeekDone();
    result &= GetMotorByName(this->sensor_pickarm.parameters.motorT2Name())->WaitSeekDone();
    result &= GetMotorByName(this->sensor_tray_loder_module.parameters.motorSTIEName())->WaitSeekDone();
    result &= GetMotorByName(this->sensor_tray_loder_module.parameters.motorSTOEName())->WaitSeekDone();
    result &= GetMotorByName(this->sensor_tray_loder_module.parameters.motorTrayName())->WaitSeekDone();
    result &= GetMotorByName(this->sensor_tray_loder_module.parameters.motorSTKName())->WaitSeekDone();

    if(result)
    {
        qInfo("all motor seeked origin successed!");
        setHomeState(true);
        this->aa_head_module.moveToMushroomPosition(true);
        if(ServerMode()!=0)
            sensor_loader_module.performHandling(SensorLoaderModule::HandleCameraPosition::SPA_STANDBY_POS);
        return  true;
    }
    return false;
}

void BaseModuleManager::stopSeeking()
{
    if(InitState())
        foreach (XtMotor *m, motors.values()) {
            m->StopSeeking();
            m->Disable();
        }
}

int BaseModuleManager::getNumberOfMotors()
{
    return motors.size();
}

QString BaseModuleManager::getMotorsName(int index)
{
    return motors.keys()[index];
}

void BaseModuleManager::updateParams()
{
    QMap<QString,PropertyBase*> temp_map;
    temp_map.insert("BASE_MODULE_PARAMS", this);
    PropertyBase::saveJsonConfig(BASE_MODULE_JSON,temp_map);
    saveParameters();
    if (this->m_InitState && ServerMode() == 1)  //If this is AA2, set the remote tcp parameter to AA1 parameter
    {
        setTcpModuleParameter(tcp_aaCoreNew.Name());
        setTcpModuleParameter(tcp_sut.Name());
        setTcpModuleParameter(tcp_lensTrayLoaderModule.Name());
        setTcpModuleParameter(tcp_lutModule.Name());
        setTcpModuleParameter(tcp_lensLoaderModule.Name());
    } else if (this->m_InitState && ServerMode() == 0){  // If this is AA1, send the need update parameter request to AA2, and then AA2 will inquiry tcp parameter
        sendTcpUpdateParameterRequest();
    }
}

void BaseModuleManager::loadFlowchart(QString json, QString filename)
{
    if (!filename.isEmpty()) {
        qInfo("Flowchart filename: %s", filename.toStdString().c_str());
        this->setFlowchartFilename(filename);
        updateParams();
    }
    aaCoreNew.setFlowchartDocument(json);
}

void BaseModuleManager::loadSensorTrayLoaderMuduleParameter()
{
    QMap<QString,PropertyBase*> temp_map;
    temp_map.insert("sensor_tray_loader", &sensor_tray_loder_module.parameters);
    temp_map.insert("sensor_clip_stand", &sensor_clip_stand);
    temp_map.insert("entance_clip", &entrance_clip.parameters);
    temp_map.insert("exit_clip", &exit_clip.parameters);
    PropertyBase::loadJsonConfig(getCurrentParameterDir().append(SENSOR_TRAY_LOADER_FILE), temp_map);
}

XtMotor *BaseModuleManager::GetMotorByName(QString name)
{
    if(name == "")return nullptr;
    if(motors.contains(name))
        return motors[name];
    else
        qDebug("can not find motor io %s",name.toStdString().c_str());
    return nullptr;
}

XtVcMotor *BaseModuleManager::GetVcMotorByName(QString name)
{
    if(name == "")return nullptr;
    if(motors.contains(name))
        return  dynamic_cast<XtVcMotor*>(motors[name]);
    else
        qDebug("can not find vcm motor io %s",name.toStdString().c_str());
    return nullptr;
}

XtGeneralOutput *BaseModuleManager::GetOutputIoByName(QString name)
{
    if(name == "")return nullptr;
    if(output_ios.contains(name))
        return output_ios[name];
    else
        qDebug("can not find output io %s",name.toStdString().c_str());
    return nullptr;
}

XtGeneralInput *BaseModuleManager::GetInputIoByName(QString name)
{
    if(name == "")return nullptr;
    if(input_ios.contains(name))
        return input_ios[name];
    else
        qDebug("can not find input io %s",name.toStdString().c_str());
    return nullptr;
}

XtVacuum *BaseModuleManager::GetVacuumByName(QString name)
{
    if(name == "")return nullptr;
    if(vacuums.contains(name))
        return vacuums[name];
    else
        qDebug("can not find vacuum io %s",name.toStdString().c_str());
    return nullptr;
}

XtCylinder *BaseModuleManager::GetCylinderByName(QString name)
{
    if(name == "")return nullptr;
    if(cylinder.contains(name))
        return cylinder[name];
    else
        qDebug("can not find cylinder io %s",name.toStdString().c_str());
    return nullptr;
}

VisionLocation *BaseModuleManager::GetVisionLocationByName(QString name)
{
    if(name == "")return nullptr;
    if(vision_locations.contains(name))
        return vision_locations[name];
    else
        qDebug("can not find vision location: %s",name.toStdString().c_str());
    return nullptr;
}

Pixel2Mech *BaseModuleManager::GetPixel2MechByName(QString name)
{
    if(name == "")return nullptr;
    if(calibrations.contains(name))
        return calibrations[name]->getCaliMapping();
    else
        qWarning("can not find calibration pixel2mech io %s",name.toStdString().c_str());
    return nullptr;
}

Calibration *BaseModuleManager::GetCalibrationByName(QString name)
{
    if(name == "")return nullptr;
    if(calibrations.contains(name))
        return calibrations[name];
    else
        qWarning("can not find calibration io %s",name.toStdString().c_str());
    return nullptr;
}

QList<TcpMessager *> BaseModuleManager::GetTcpMessagersByName(QVariantList messager_names)
{
    QList<TcpMessager *> temp_list;
    foreach (QVariant temp_name, messager_names) {
       TcpMessager* temp_messager = tcp_manager.GetAllTcpMessager(temp_name.toString());
       if(temp_messager != nullptr)
           temp_list.append(temp_messager);
       else {
           qInfo("can not find tcpmessager %s",temp_name.toString().toStdString().c_str());
       }
    }
    qInfo("TcpMessagers size %d",temp_list.size());
    return temp_list;
}

void BaseModuleManager::sendTcpStepMoveRequest(QString name, double step, bool isPositive)
{
    QJsonObject message;
    message["cmd"] = "stepMove";
    message["motorName"] = name;
    message["step"] = step;
    message["isPositive"] = isPositive;
    foreach (TcpMessager* temp_messager, sender_messagers) {
        temp_messager->sendMessage(TcpMessager::getStringFromJsonObject(message));
    }
}

void BaseModuleManager::sendTcpEnableMotor(QString motorName, bool on)
{
    QJsonObject message;
    message["cmd"] = "enableMotor";
    message["motorName"] = motorName;
    message["enable"] = on;
    foreach (TcpMessager* temp_messager, sender_messagers) {
        temp_messager->sendMessage(TcpMessager::getStringFromJsonObject(message));
    }
}

void BaseModuleManager::sendTcpHomeMotor(QString motorName)
{
    QJsonObject message;
    message["cmd"] = "homeMotor";
    message["motorName"] = motorName;
    foreach (TcpMessager* temp_messager, sender_messagers) {
        temp_messager->sendMessage(TcpMessager::getStringFromJsonObject(message));
    }
}

void BaseModuleManager::sendAAHeadInterMoveRequest(int channel, double step)
{
    QJsonObject message;
    message["cmd"] = "aaHeadInterMove";
    message["channel"] = channel;
    message["step"] = step;
    foreach (TcpMessager* temp_messager, sender_messagers) {
        temp_messager->sendMessage(TcpMessager::getStringFromJsonObject(message));
    }
}

bool BaseModuleManager::stepMove(QString name, double step, bool isPositive)
{
    if (!motors.contains(name)) {
        if(tcpMotorNames.contains(name)) {
            qInfo("Send step move in tcp motor name: %s step: %f isPositive: %d", name.toStdString().c_str(), step, isPositive);
            sendTcpStepMoveRequest(name, step, isPositive);
            return true;
        } else {
            qWarning("Selected motor does not exist", name.toStdString().c_str());
            return false;
        }
    }
    XtMotor* temp_motor = motors[name];
    qInfo("Step move: %s %f %d %f", temp_motor->Name().toStdString().c_str(), step, isPositive, temp_motor->GetFeedbackPos());
    if (isPositive)
        temp_motor->StepMove(step);
    else {
        temp_motor->StepMove(-step);
    }
    return true;
}


bool BaseModuleManager::stepMove(int index, double step, bool isPositive)
{
    XtMotor* temp_motor = motors.values()[index];
    qInfo("Step move: %s %f %d %f", temp_motor->Name().toStdString().c_str(), step, isPositive, temp_motor->GetFeedbackPos());
    if (isPositive)
        temp_motor->StepMove(step);
    else {
        temp_motor->StepMove(-step);
    }
    return true;
}

void BaseModuleManager::setMotorParamByName(QString name, double vel, double acc, double jert)
{
    qInfo("setMotorParamByName %f, %f, %f",vel,acc,jert);
  XtMotor* temp_motor = GetMotorByName(name);
  if(temp_motor != nullptr)
  {
      temp_motor->SetVel(vel);
      temp_motor->SetAcc(acc);
      temp_motor->SetJerk(jert);
  }
}

bool BaseModuleManager::performCalibration(QString calibration_name)
{
    qInfo("perform %s",calibration_name.toStdString().c_str());
    if(calibration_name.contains("chart_calibration"))
        return  chart_calibration->performCalibration();
    Calibration* temp_caliration = GetCalibrationByName(calibration_name);
    if(temp_caliration == nullptr)
    {
        qInfo("not calibration %s",calibration_name.toStdString().c_str());
        return  false;
    }
    return  temp_caliration->performCalibration();
}

bool BaseModuleManager::performUpDnLookCalibration()
{
    qInfo("performUpDnLookCalibration");
    //ToDo: Move the lut movement in LUT Client.
//    lutClient->sendLUTMoveToPos(0); //Unload Pos
    PrOffset offset1, offset2;
    sut_module.moveToToolDownlookPos(true);
    if (!sut_module.toolDownlookPR(offset1,true,false)) {
        qInfo("SUT Cannot do the tool downlook PR");
        return false;
    }
    qInfo("UpDnlook Down PR: %f %f %f", offset1.X, offset1.Y, offset1.Theta);
    sut_module.moveToToolUplookPos(true);
    if (ServerMode() == 0) {
//        lutClient->sendLUTMoveToPos(1); //AA 1 Pos
//        lutClient->requestToolUpPRResult(offset2); //Do uplook PR
    } else {
//        lutClient->sendLUTMoveToPos(2); //AA 2 Pos
    }
    double offsetT = offset1.Theta - offset2.Theta;
    qInfo("UpDnlook Up PR: %f %f %f", offset2.X, offset2.Y, offset2.Theta);
    qInfo("UpDnlook Camera Offset downlook - uplook = %f", offsetT);
    sut_module.up_downlook_offset.setX(sut_module.tool_uplook_positon.X() - offset2.X - sut_module.tool_downlook_position.X() + offset1.X);
    sut_module.up_downlook_offset.setY(sut_module.tool_uplook_positon.Y() - offset2.Y - sut_module.tool_downlook_position.Y() + offset1.Y);
    sut_module.up_downlook_offset.setTheta(offset1.Theta - offset2.Theta);
    qInfo("updownlook offset %f,%f,%f",sut_module.up_downlook_offset.X(),sut_module.up_downlook_offset.Y(),sut_module.up_downlook_offset.Theta());
    sut_module.parameters.setCameraTheta(offsetT);
//    lutClient->sendLUTMoveToPos(0); //AA 1 Pos
    return true;
}
bool BaseModuleManager::performLensUpDnLookCalibration()
{
    //ToDo: This is the calibration glass width and height in mm
    double realWidth = 7, realHeight = 7;
    PrOffset offset1, offset2;
    this->lens_loader_module.performUpdowlookUpPR(offset1);
    offset1.X /= offset1.W/realWidth;
    offset1.Y /= offset1.H/realHeight;
    qInfo("Lens loader UpDnlook up PR: %f %f %f %f %f", offset1.X, offset1.Y, offset1.Theta, offset1.W, offset1.H);
    this->lens_loader_module.performUpDownlookDownPR(offset2);
    offset2.X /= offset2.W/realWidth;
    offset2.Y /= offset2.H/realHeight;
    qInfo("Lens loader UpDnlook down PR: %f %f %f %f %f", offset2.X, offset2.Y, offset2.Theta, offset2.W, offset2.H);
    double offsetX = offset1.X - offset2.X;
    double offsetY = offset1.Y - offset2.Y;
    this->lens_loader_module.camera_to_picker_offset.setX(offsetX);
    this->lens_loader_module.camera_to_picker_offset.setY(offsetY);
    this->lut_module.lpa_camera_to_picker_offset.setX(-offsetX);
    this->lut_module.lpa_camera_to_picker_offset.setY(-offsetY);

    qInfo("Lens UpDnlook Calibration result offsetX : %f offsetY: %f", offsetX,offsetY);
    return true;
}

bool BaseModuleManager::performLocation(QString location_name,bool use_origin)
{
//    if(!emit sendMsgSignal("title","content")){
//        return true;
//    }
    qInfo("perform %s",location_name.toStdString().c_str());
    VisionLocation* temp_location = GetVisionLocationByName(location_name);
    if(temp_location == nullptr)
    {
        qInfo("location(%s) is null",location_name.toStdString().c_str());
        return false;
    }
    Calibration* temp_caliration = GetCalibrationByName(temp_location->parameters.calibrationName());
    if(temp_caliration == nullptr)
    {
        qInfo("calibraion(%s)is null",temp_location->parameters.calibrationName().toStdString().c_str());
        return  false;
    }
    PrOffset offset;
    if(temp_location->parameters.calibrationName().contains("chart_calibration"))
    {
        double x,y;
        if(!temp_caliration->GetPixelPoint(x,y))
            return false;
        QPointF result_point;
        temp_caliration->getDeltaDistanceFromCenter(QPointF(x,y),result_point);
        offset.X = result_point.x(); offset.Y = result_point.y();
    }
    else
    {
//        temp_location->OpenLight();
//        QThread::msleep(200);
        if(!temp_location->performPR())
            return false;
        offset = temp_location->getCurrentResult(use_origin);
    }
    if(temp_location->parameters.canMotion())
    {
//        if(!emit sendMsgSignal(tr(u8"提示"),tr(u8"x: %1,y:%2,theta:%3,是否移动").arg(offset.X).arg(offset.Y).arg(offset.Theta))){
//            return true;
//        }
        temp_caliration->performPRResult(offset);
    }
    return true;
}

QString BaseModuleManager::getCalibrationParam(QString calibration_name)
{
    Calibration* temp_caliration = GetCalibrationByName(calibration_name);
    if(temp_caliration == nullptr)return  "no calibration";
    QString temp_value = "(";
    QPointF temp_point = temp_caliration->getOnePxielDistance();
    temp_value.append(QString::number(round(temp_point.x()*10000)/10000));
    temp_value.append(",");
    temp_value.append(QString::number(round(temp_point.y()*10000)/10000));
    temp_value.append(",");
    temp_value.append(QString::number(round(temp_caliration->getRotationAngle()*100)/100));
    temp_value.append(")");
    return temp_value;
}

void BaseModuleManager::setOutput(QString name, bool on)
{
    if(name=="SUT1吸真空")
        on = !on;
    if (this->output_ios.contains(name)){
        qDebug("set output : %s %d", name.toStdString().c_str(), on);
        output_ios[name]->Set(on);
    }
    else
    {
        qDebug(u8"Cannot find output IO with name: %s", name.toStdString().c_str());
    }
}

bool BaseModuleManager::getCylinderOutput(QString name)
{
    if(GetCylinderByName(name)!=nullptr)
        return  GetCylinderByName(name)->Value();
    return false;
}

void BaseModuleManager::setCylinderOutput(QString name, bool on)
{
    if(GetCylinderByName(name)!=nullptr)
        GetCylinderByName(name)->Set(on);
}

bool BaseModuleManager::getOutput(QString name)
{
    if (this->output_ios.contains(name)){
        if(name=="SUT1吸真空")
            return !output_ios[name]->Value();
        return  output_ios[name]->Value();
    }
    return  false;
}

bool BaseModuleManager::getInput(QString name)
{
    if (this->input_ios.contains(name)){
        return  input_ios[name]->Value();
    }
    return  false;
}

void BaseModuleManager::motorSeekOrigin(QString name)
{
    if (motors.contains(name)) {
        qInfo("Start motorSeekOrigin: %s", name.toStdString().c_str());
        motors[name]->SeekOrigin();
    } else {
        sendTcpHomeMotor(name);
    }
}

double BaseModuleManager::getPROffsetX(QString location_name)
{
    return  GetVisionLocationByName(location_name)->getCurrentOffset().x();
}

double BaseModuleManager::getPROffsetY(QString location_name)
{
    return  GetVisionLocationByName(location_name)->getCurrentOffset().y();
}

bool BaseModuleManager::getMotorEnableState(QString name)
{
    if (motors.contains(name)) {
        return motors[name]->states.isEnabled();
    }else {
        DeviceStatesGeter::motorState motor_state = state_geter.getMotorState(name);
        return motor_state.isEnabled;
    }
    return false;
}

double BaseModuleManager::getMotorFeedbackPos(QString name)
{
    if (motors.contains(name)) {
        return motors[name]->GetFeedbackPos();
    } else {
        DeviceStatesGeter::motorState motor_state = state_geter.getMotorState(name);
        return motor_state.current_position;
    }
}

bool BaseModuleManager::getMotorHomeState(QString name)
{
    if (motors.contains(name)) {
        return motors[name]->states.seekedOrigin();
    } else {
        DeviceStatesGeter::motorState motor_state = state_geter.getMotorState(name);
        return motor_state.isHome;
    }
    return false;
}

double BaseModuleManager::getMotorFeedbackPos(int index)
{
    return motors.values()[index]->GetFeedbackPos();
}

void BaseModuleManager::setLightingBrightness(QString location_name)
{
    GetVisionLocationByName(location_name)->OpenLight();
}

void BaseModuleManager::sendLoadLens(bool has_ng)
{
    aa_head_module.moveToPickLensPosition();
    if(has_ng)
        emit lut_module.sendLoadLensRequst(true,0,0);
    else
        emit lut_module.sendLoadLensRequst(true,-1,-1);
}

void BaseModuleManager::sendLoadSensor(bool has_product, bool has_ng)
{
    if(has_product)
    {
        emit  aa_head_module.sendSensrRequestToSut(SUT_STATE::HAS_PRODUCT);
        qInfo("sendSensrRequestToSut 2 in %d",QThread::currentThreadId());
    }
    else if(has_ng)
    {
        emit  aa_head_module.sendSensrRequestToSut(SUT_STATE::HAS_NG_SENSOR);
        qInfo("sendSensrRequestToSut 1 in %d",QThread::currentThreadId());
    }
    else
    {
        emit  aa_head_module.sendSensrRequestToSut(SUT_STATE::NO_MATERIAL);
        qInfo("sendSensrRequestToSut 0 in %d",QThread::currentThreadId());
    }
}

void BaseModuleManager::toogleIoState(QString io_name, int inputState)
{
    state_geter.toggleOutputIoState(io_name, inputState);
}

//void BaseModuleManager::sendChangeSensorTray()
//{
//   emit sensor_loader_module.sendChangeTrayRequst();
//}

bool BaseModuleManager::initSensor()
{
    if (dothinkey->DothinkeyIsGrabbing()) {
        qDebug("Dothinkey is already init and grabbing image.");
        return true;
    }
    const int channel = 0;
    bool res = dothinkey->DothinkeyEnum();
    if (!res) { qCritical("Cannot find dothinkey"); return false; }
    dothinkey->DothinkeyOpen();
    if (!res) { qCritical("Cannot open dothinkey"); return false; }
    dothinkey->DothinkeyLoadIniFile(channel);
    if (!res) { qCritical("Cannot load dothinkey ini file"); return false; }
    dothinkey->DothinkeyStartCamera(channel);
    if (!res) { qCritical("Cannot start camera"); return false; }
    imageGrabberThread->start();
    return true;
}

bool BaseModuleManager::closeSensor()
{
    imageGrabberThread->stop();
    Sleep(100);
    imageGrabberThread->exit();
    return dothinkey->DothinkeyClose();
}

void BaseModuleManager::loadSensorLoaderParameter()
{
    material_tray.loadJsonConfig(getCurrentParameterDir().append(MATERIAL_TRAY_FILE));
    sensor_loader_module.loadJsonConfig(getCurrentParameterDir().append(SENSOR_LOADER_FILE));
}

void BaseModuleManager::loadLensTrayLoaderParameter()
{
    material_tray.loadJsonConfig(getCurrentParameterDir().append(MATERIAL_TRAY_FILE));
    tray_loader_module.parameters.loadJsonConfig(getCurrentParameterDir().append(TRAY_LOADER_FILE),TRAY_LOADER_PARAMETER);
    trayClipIn.standards_parameters.loadJsonConfig(getCurrentParameterDir().append(TRAY_CLIPIN_FILE),TRAY_CLIPIN_PARAMETER);
    trayClipOut.standards_parameters.loadJsonConfig(getCurrentParameterDir().append(TRAY_CLIPOUT_FILE),TRAY_CLIPOUT_PARAMETER);
}

double BaseModuleManager::showChartCalibrationRotation()
{
    return chart_calibration->getRotationAngle();
}

void BaseModuleManager::testPRTest()
{
    for (int i = 0; i < 10; ++i) {

        for (int j = 0; j < 10; ++j) {
            QString uuid = QString("left_").append(QString::number(i)).append("_").append(QString::number(j));
            lens_loader_module.recordTrayLensPr(uuid);
            lens_loader_module.recordLutVacancyPr(uuid);
            lens_loader_module.recordLutLensPr(uuid);
            lut_module.recordAALensPr(uuid);
            lens_loader_module.recordNgLensPr(uuid);
            lens_loader_module.recordTrayVacancyPr(uuid);

            uuid = QString("right_").append(QString::number(i)).append("_").append(QString::number(j));
            lens_loader_module.recordTrayLensPr(uuid);
            lens_loader_module.recordLutVacancyPr(uuid);
            lens_loader_module.recordLutLensPr(uuid);
            lut_module.recordAALensPr(uuid);
            lens_loader_module.recordNgLensPr(uuid);
            lens_loader_module.recordTrayVacancyPr(uuid);
        }
    }
}
