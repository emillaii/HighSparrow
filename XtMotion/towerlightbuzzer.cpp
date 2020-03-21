#include "towerlightbuzzer.h"
#include <QDebug>

TowerLightBuzzer::TowerLightBuzzer()
{

}

void TowerLightBuzzer::Init(XtGeneralOutput *buzzerOut,
                            XtGeneralOutput *redOut,
                            XtGeneralOutput *greenOut,
                            XtGeneralOutput *yellowOut)
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

void TowerLightBuzzer::blinkBuzzer()  //todo
{
    int buzzerBlinkDuration = 3;
    isBuzzerOn = true;
    if(buzzerOut == nullptr ){
        qCritical("Buzzer io is null, please check!");
        return;
    }
    while (isBuzzerOn) {
        buzzerBlinkDuration--;
        Sleep(500);
        buzzerOut->Set(true);
        Sleep(500);
        buzzerOut->Set(false);
        if(!isBuzzerOn||buzzerBlinkDuration <=0) break;
    }
}
void TowerLightBuzzer::closeBuzzer()
{
    isBuzzerOn = false;
    buzzerOut->Set(false);
}

void TowerLightBuzzer::switchColor(TowerLightBuzzer::TowerLightColor color) //todo
{
    if(color == TowerLightColor::Red)
    {
        redOut->Set(true);
        yellowOut->Set(false);
        greenOut->Set(false);
        this->blinkBuzzer();
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
    int towerLightBlinkDuration = 20;
    if(color == TowerLightColor::Red)
    {
        yellowOut->Set(false);
        greenOut->Set(false);
        for (int  i = 0; i < towerLightBlinkDuration; i++) {
            Sleep(500);
            redOut->Set(true);
            Sleep(500);
            redOut->Set(false);
        }
    }
}



