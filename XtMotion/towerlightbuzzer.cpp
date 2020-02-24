#include "towerlightbuzzer.h"

TowerLightBuzzer::TowerLightBuzzer()
{

}

void TowerLightBuzzer::Init(XtGeneralOutput *buzzerOut,
                            XtGeneralOutput *redOut,
                            XtGeneralOutput *greenOut,
                            XtGeneralOutput *yellowOut
//                            int buzzerBlinkInterval,
//                            int buzzerBlinkDuration,
//                            int towerLightBlinkInterval,
//                            int towerLightBlinkDuration
                            )
{
    this->buzzerOut = buzzerOut;
    this->redOut = redOut;
    this->greenOut = greenOut;
    this->yellowOut = yellowOut;
}

bool TowerLightBuzzer::Set(bool new_state)
{
    buzzerOut->Set(new_state);
//    if((redOut != nullptr)&&(!new_state))
//    {
//        redOut->Set(true);
//    }
    return true;
//}
}

void TowerLightBuzzer::openBuzzer(bool new_state)
{
    buzzerOut->Set(new_state);
}

void TowerLightBuzzer::closeBuzzer(bool new_state)
{
    buzzerOut->Set(new_state);
}

//void TowerLightBuzzer::switchColor(TowerLightBuzzer::TowerLightColor color)
//{

//}
