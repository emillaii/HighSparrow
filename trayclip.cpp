#include "trayclip.h"


TrayClip::TrayClip()
{

}

void TrayClip::Init()
{

}

void TrayClip::calculateDelta()
{
    double startPos = standards_parameters.firstTrayPos();
    double endPos = standards_parameters.lastTrayPos();
    int count = standards_parameters.columnCount();
    standards_parameters.setColumnDelta((endPos-startPos)/(count-1));
}

double TrayClip::getPosition(int column_index)
{
    return column_index*standards_parameters.columnDelta() + standards_parameters.firstTrayPos();
}

int TrayClip::getCurrentIndex()
{
    return standards_parameters.currentIndex();
}

double TrayClip::getCurrentPosition()
{
    return standards_parameters.firstTrayPos() + standards_parameters.currentIndex()*standards_parameters.columnDelta();
}

bool TrayClip::updateCurrentIndex()
{
    standards_parameters.setCurrentIndex(standards_parameters.currentIndex() + 1);
    if(standards_parameters.currentIndex() >= standards_parameters.columnCount())
    {
        standards_parameters.setCurrentIndex(0);
        return false;
    }
}

void TrayClip::reset()
{
    standards_parameters.setCurrentIndex(0);
}
