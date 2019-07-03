#include "devicestatesgeter.h"
#include "commonutils.h"

DeviceStatesGeter::DeviceStatesGeter(QObject *parent) : QObject(parent)
{

}

DeviceStatesGeter::motorState DeviceStatesGeter::getMotorState(QString motor_name)
{
    QJsonObject temp_json;
    temp_json["cmd"] = "inquiryMotorPos";
    temp_json["motorName"] = motor_name;
    QString result = emit sendGetDeviceState(getStringFromJsonObject(temp_json));
    QJsonObject result_json = getJsonObjectFromString(result);
    motorState state;

    if((!result_json.contains("motorName"))||result_json["motorName"].toString() != motor_name)
    {
        qInfo("has no motorName key");
        return state;
    }
    if(!result_json.contains("motorPosition"))
    {
        qInfo("has no motorPosition key");
        return state;
    }
    state.current_position = result_json["motorPosition"].toDouble();

    if(!result_json.contains("motorTargetPosition"))
    {
        qInfo("has no motorTargetPosition key");
        return state;
    }
    state.target_position = result_json["motorTargetPosition"].toDouble();

    if((!result_json.contains("error"))||result_json["error"].toString() != "")
    {
        qInfo("has no error key or has error");
        return state;
    }
    state.result = true;
    return state;
}

DeviceStatesGeter::IoState DeviceStatesGeter::getInputIoState(QString input_io_name)
{
    QJsonObject temp_json;
    temp_json["cmd"] = "inquiryInputIoState";
    temp_json["inputIoName"] = input_io_name;
    QString result = emit sendGetDeviceState(getStringFromJsonObject(temp_json));
    QJsonObject result_json = getJsonObjectFromString(result);
    IoState state;

    if((!result_json.contains("inputIoName"))||result_json["inputIoName"].toString() != input_io_name)
        return state;

    if(!result_json.contains("IoValue"))
        return state;
    state.current_state = result_json["IoValue"].toBool();

    if((!result_json.contains("error"))||result_json["error"].toString() != "")
        return state;
    state.result = true;
    return state;
}

DeviceStatesGeter::IoState DeviceStatesGeter::getOutputIoState(QString output_io_name)
{
    QJsonObject temp_json;
    temp_json["cmd"] = "inquiryOutputIoState";
    temp_json["outputIoName"] = output_io_name;
    QString result = emit sendGetDeviceState(getStringFromJsonObject(temp_json));
    QJsonObject result_json = getJsonObjectFromString(result);
    IoState state;

    if((!result_json.contains("outputIoName"))||result_json["outputIoName"].toString() != output_io_name)
        return state;

    if(!result_json.contains("IoValue"))
        return state;
    state.current_state = result_json["IoValue"].toBool();

    if((!result_json.contains("error"))||result_json["error"].toString() != "")
        return state;
    state.result = true;
    return state;
}
