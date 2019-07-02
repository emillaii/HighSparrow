#include "motorstatesgeter.h"
#include "commonutils.h"

MotorStatesGeter::MotorStatesGeter(QObject *parent) : QObject(parent)
{

}

MotorStatesGeter::motorState MotorStatesGeter::getMotorState(QString motor_name)
{
    QJsonObject temp_json;
    temp_json["cmd"] = "quareMotorPos";
    temp_json["motorName"] = motor_name;
    QString result = emit sendGetMotorState(getStringFromJsonObject(temp_json));
    QJsonObject result_json = getJsonObjectFromString(result);
    motorState state;

    if((!result_json.contains("motorName"))||result_json["motorName"].toString() != motor_name)
        return state;

    if(!result_json.contains("motorPosition"))
        return state;
    state.current_position = result_json["motorPosition"].toDouble();

    if(!result_json.contains("motorTargetPosition"))
        return state;
    state.target_position = result_json["motorTargetPosition"].toDouble();

    if((!result_json.contains("error"))||result_json["error"].toString() != "")
        return state;
    state.result = true;
    return state;
}
