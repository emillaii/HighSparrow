#ifndef VISION_LOCATION_H
#define VISION_LOCATION_H

#include "utils/pixel2mech.h"
#include "vision/vision_location_parameter.h"
#include "vision/visionmodule.h"
#include "vision/wordoplight.h"

class VisionLocation:public ErrorBase
{
public:
    VisionLocation();
    void Init(VisionModule* vison,Pixel2Mech* mapping,WordopLight* lighting);
    void loadParam();
    void saveParam();
    bool performPR(PrOffset &offset, bool need_conversion = true);
    bool performPR();
    bool performPR(PRResultStruct &pr_result);
    Q_INVOKABLE QPointF getCurrentOffset();
    void OpenLight();
    void CloseLight();
    QString getLastImageName();
    bool saveImage(QString imageName);
public:
    VisionLocationParameter parameters;
private:
    QString last_image_name = "";
    VisionModule* vison;
    Pixel2Mech* mapping;
    WordopLight* lighting;
    PrOffset current_offset;
};

#endif // VISION_LOCATION_H
