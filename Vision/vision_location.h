#ifndef VISION_LOCATION_H
#define VISION_LOCATION_H

#include "Vision/pixel2mech.h"
#include "vision_location_parameter.h"
#include "Vision/visionmodule.h"
#include "Vision/wordoplight.h"

class VisionLocation:public ErrorBase
{
public:
    VisionLocation();
    void Init(VisionModule* vison,Pixel2Mech* mapping,WordopLight* lighting);
    void loadParam();
    void saveParam();
    bool performPR(PrOffset &offset, bool need_conversion = true);
    bool saveImage(QString filename);
    bool performPR(PRResultStruct &pr_result);
    Q_INVOKABLE QPointF getCurrentOffset();
    void OpenLight();
    void CloseLight();
//    QString getLastImageName();

    Pixel2Mech* mapping;

public:
    VisionLocationParameter parameters;
private:
    QString last_image_name = "";
    VisionModule* vison;
    WordopLight* lighting;
    PrOffset current_offset;
};

#endif // VISION_LOCATION_H
