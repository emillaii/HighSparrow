#include "towerlightbuzzer.h"

TowerLightBuzzer::TowerLightBuzzer()
{

}

void TowerLightBuzzer::init(XtGeneralOutput *buzzerOut,
                            XtGeneralOutput *redOut,
                            XtGeneralOutput *greenOut,
                            XtGeneralOutput *yellowOut,
                            int buzzerBlinkInterval,
                            int buzzerBlinkDuration,
                            int towerLightBlinkInterval,
                            int towerLightBlinkDuration)
{
    this->buzzerOut = buzzerOut;
    this->redOut = redOut;
    this->greenOut = greenOut;
    this->yellowOut = yellowOut;
}
