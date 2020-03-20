#include "towerlightbuzzer.h"
#include <QDebug>

TowerLightBuzzer::TowerLightBuzzer()
{

}

void TowerLightBuzzer::Init(XtGeneralOutput *buzzerOut,
                            XtGeneralOutput *redOut,
                            XtGeneralOutput *greenOut,
                            XtGeneralOutput *yellowOut
                            )
{
    this->buzzerOut = buzzerOut;
    this->redOut = redOut;
    this->greenOut = greenOut;
    this->yellowOut = yellowOut;
}

void TowerLightBuzzer::loadJsonConfig(QString file_name)
{
    QMap<QString, PropertyBase*> temp_map;
    temp_map.insert("tower_light_buzzer",&parameters);
    PropertyBase::loadJsonConfig(file_name,temp_map);
}

void TowerLightBuzzer::saveJsonConfig(QString file_name)
{
    QMap<QString, PropertyBase*> temp_map;
    temp_map.insert("tower_light_buzzer",&parameters);
    PropertyBase::saveJsonConfig(file_name,temp_map);
}

void TowerLightBuzzer::openBuzzer()
{
    buzzerOut->Set(true);
}

void TowerLightBuzzer::blinkBuzzer()
{
    isBuzzerOn= true;
    if (isBuzzerOn) {
        buzzerOut->Set(true);
    }
}
void TowerLightBuzzer::closeBuzzer()
{
    isBuzzerOn = false;
    buzzerOut->Set(false);
}

void TowerLightBuzzer::switchColor(TowerLightBuzzer::TowerLightColor color)
{
    if(color == TowerLightColor::Red)
    {
        redOut->Set(true);
        yellowOut->Set(false);
        greenOut->Set(false);
    }
    if(color == TowerLightColor::Yellow)
    {
        redOut->Set(false);
        yellowOut->Set(true);
        greenOut->Set(false);
    }
    if(color == TowerLightColor::Green)
    {
        redOut->Set(false);
        yellowOut->Set(false);
        greenOut->Set(true);
    }
}

void TowerLightBuzzer::blinkColor(TowerLightBuzzer::TowerLightColor color)
{

}

