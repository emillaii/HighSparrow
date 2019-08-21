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
    QPointF getCurrentOffset();

    PrOffset getCurrentResult();
    PrOffset getCurrentResult(bool use_origin);
    QPointF getCurrentResultOffset();
    PRResultStruct getCurrentPixelResult();

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
    PrOffset current_result;
    PRResultStruct current_pixel_result;
};

#endif // VISION_LOCATION_H