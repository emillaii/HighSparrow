#include "uphhelper.h"

UPHHelper::UPHHelper():PropertyBase()
{

}

void UPHHelper::addCurrentNumber(double accumulated_hour)
{
    this->accumulated_hour = accumulated_hour;
    setCurrentNumber(currentNumber() + 1);
    updateYeild();
    updateUPH();
}

void UPHHelper::addCurrentReslutNumber(double accumulated_hour)
{
    this->accumulated_hour = accumulated_hour;
    setCurrentResultNumber(currentResultNumber() + 1);
    updateYeild();
    updateUPH();
}

void UPHHelper::updateYeild(int decimal_digit)
{
    double  temp_yeild = 0;
    if(addOK())
        temp_yeild = currentResultNumber();
    else
        temp_yeild = currentNumber() - currentResultNumber();
    temp_yeild /= currentNumber();
    double scale = pow(10,decimal_digit);
    temp_yeild = round(temp_yeild * scale)/scale;
    setAccumulatedYeild(temp_yeild);
}

void UPHHelper::updateUPH()
{
    if(accumulated_hour > 0)
    {
        setAccumulatedUPH(qRound(currentNumber()/accumulated_hour));
        if(addOK())
            setAccumulatedOkUPH(qRound(currentResultNumber()/accumulated_hour));
        else
            setAccumulatedOkUPH(qRound((currentNumber() - currentResultNumber())/accumulated_hour));
    }
}

void UPHHelper::clearNumber()
{
    setCurrentNumber(0);
    setCurrentResultNumber(0);
    setAccumulatedYeild(0);
    setAccumulatedUPH(0);
    setAccumulatedOkUPH(0);
}
