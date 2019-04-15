#ifndef VISION_LOCATION_H
#define VISION_LOCATION_H

#include "pixel2mech.h"
#include "vision_location_parameter.h"
#include "visionmodule.h"
#include "wordoplight.h"

class VisionLocation
{
public:
    VisionLocation();
    void Init(VisionModule* vison,Pixel2Mech* mapping,WordopLight* lighting);
    void loadParam();
    void saveParam();
    bool performPR(PrOffset &offset);
    bool performPR(PRResultStruct &pr_result);
    void OpenLight();
    void CloseLight();

public:
    VisionLocationParameter parameters;
private:
    VisionModule* vison;
    Pixel2Mech* mapping;
    WordopLight* lighting;
};

#endif // VISION_LOCATION_H