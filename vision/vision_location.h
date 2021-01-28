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
    bool performNoMaterialPR();
    bool performPR(PRResultStruct &pr_result);
    bool performGlueInspection(QString beforeDispenseImageName, QString afterDispenseImageName, QString *glueInspectionImageName,
                               double min_glue_width, double max_glue_width, double avg_glue_width,
                               double &outMinGlueWidth, double &outMaxGlueWidth, double &outMaxAvgGlueWidth);
    void resetResult();
    QPointF getCurrentOffset();

    PrOffset getCurrentResult();
    void setCurrentResult(PrOffset pr_offset);
    PrOffset getCurrentResult(bool use_origin);
    QPointF getCurrentResultOffset();
    QPointF getOffsetResult(QPoint offset);
    PRResultStruct getCurrentPixelResult();

    void OpenLight();
    void CloseLight();
    void OpenLight(int channel, uint8_t brightness);
    void CloseLight(int channel);
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
