#include "sensorclip.h"

SensorClip::SensorClip(QObject *parent) : QObject(parent),ErrorBase()
{

}

void SensorClip::Init(QString name,SensorClipStandParameter *stand_parameter)
{
    this->stand_parameter = stand_parameter;
    setName(name);
}

int SensorClip::getCurrentIndex()
{
    return parameters.currentIndex();
}

void SensorClip::caculateStandParameter()
{
    stand_parameter->setDelta((parameters.endPosition()-parameters.startPosition())/(stand_parameter->count()-1));
}

void SensorClip::setCurrentIndex(int index)
{
    if(index<stand_parameter->count())
        parameters.setCurrentIndex(index);
    else
        parameters.setCurrentIndex(stand_parameter->count() - 1);
}

double SensorClip::getCurrentPosition()
{
    if(parameters.invertOrder())
        return parameters.endPosition() - stand_parameter->delta()*parameters.currentIndex();
    else
        return parameters.startPosition() + stand_parameter->delta()*parameters.currentIndex();
}

double SensorClip::getNextPosition()
{
    if(parameters.needChange())
    {
        AppendError(u8"盘已经使用完");
        return 0;
    }
    if(parameters.finishCurrent())
    {
        parameters.setCurrentIndex(parameters.currentIndex() + 1);
        if(parameters.currentIndex() >= stand_parameter->count() - 1)
            parameters.setNeedChange(true);
    }
    return getCurrentPosition();
}

bool SensorClip::getChangeState()
{
    return parameters.needChange();
}

void SensorClip::setChangeState(bool needChange)
{
    parameters.setNeedChange(needChange);
}

void SensorClip::finishCurrentPosition()
{
    parameters.setFinishCurrent(true);
}

void SensorClip::resetClip()
{
    parameters.setNeedChange(false);
    parameters.setFinishCurrent(false);
    parameters.setCurrentIndex(0);
}
