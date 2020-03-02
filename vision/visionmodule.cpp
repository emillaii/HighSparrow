#include "vision/visionmodule.h"

#include "AVL.h"
#include "STD.h"
#include "ATL/Error.h"
#include "ATL/Dummy.h"
#include "ATL/Optional.h"
#include "ATL/AtlTypes.h"
#include "ThirdPartySdk.h"
#include "Serialization/Serialization.h"
#include "ExpressionsRuntime/ExpressionsRuntime.h"
#include <QTime>
#include <QDir>
#include <config.h>
#include <utils/commonutils.h>
#include "vision/baslerpyloncamera.h"
#include <QElapsedTimer>
#include "utils/uiHelper/uioperation.h"
#include "utils/singletoninstances.h"
#include "visionserver.h"

VisionModule::VisionModule():QQuickImageProvider(QQuickImageProvider::Image){}
VisionModule::VisionModule(BaslerPylonCamera *downlookCamera, BaslerPylonCamera * uplookCamera,
                           BaslerPylonCamera* pickarmCamera, BaslerPylonCamera * aa2DownlookCamera,
                           BaslerPylonCamera* sensorPickarmCamera, BaslerPylonCamera* sensorUplookCamera,
                           BaslerPylonCamera* barcodeCamera, QString name, int serverMode)
    :ThreadWorkerBase (name), QQuickImageProvider(QQuickImageProvider::Image)
{
    this->downlookCamera = downlookCamera;
    this->uplookCamera = uplookCamera;
    this->pickarmCamera = pickarmCamera;
    this->aa2DownlookCamera = aa2DownlookCamera;
    this->sensorPickarmCamera = sensorPickarmCamera;
    this->sensorUplookCamera = sensorUplookCamera;
    this->barcodeCamera = barcodeCamera;
    aaDebugImageProvider = new ImageProvider();
    if (serverMode == 0) {
        qInfo("Vision Server is opened");
        this->server = new VisionServer(this->downlookCamera, this->uplookCamera, this->pickarmCamera,
                                        this->aa2DownlookCamera, this->sensorPickarmCamera, this->sensorUplookCamera,
                                        this->barcodeCamera);
        host.setHostUrl(QUrl("tcp://192.168.1.2:9999"));
        host.enableRemoting(server);
        qInfo("Vision Server is opened");
    } else {
        node.connectToNode(QUrl("tcp://192.168.1.2:9999"));
        auto visionRep = node.acquire<SilicoolVisionReplica>();
        this->visionRep = visionRep;
    }
}
QVector<QPoint> VisionModule::Read_Dispense_Path()
{
    atl::String g_constData1 = DISPENSE_PATH_2D_POINT_FILE;
    atl::String g_constData2 = L"PathArray";
    QVector<QPoint> results = QVector<QPoint>();

    atl::Array< avl::Path > pathArray1;
    atl::Array< avl::Point2D > point2DArray1;

    avs::LoadObject< atl::Array< avl::Path > >( g_constData1, avl::StreamMode::Binary, g_constData2, pathArray1 );

    for( int i = 0; i < pathArray1.Size(); ++i )
    {
        avs::AvsFilter_AccessPath( pathArray1[i], point2DArray1, atl::Dummy<bool>().Get() );
        for ( int j = 0; j < point2DArray1.Size(); ++j)
        {
            float x = point2DArray1[j].x;
            float y = point2DArray1[j].y;
            qDebug(" %d : x: %f y: %f", j, x, y);
            results.push_back(QPoint(x, y));
        }
    }
    return results;
}

bool VisionModule::grabImageFromCamera(QString cameraName, avl::Image &image)
{
    QMutexLocker locker(&mutex);
    if (this->Name() == VISION_MODULE_2) { //If vision module 2, need to ask vision module 1 to get image
        QRemoteObjectPendingReply<QByteArray> reply = visionRep->getImageEx(cameraName);
        if (reply.waitForFinished()){
            QByteArray byteArray = reply.returnValue();
            QImage img((uchar *)byteArray.data(), visionRep->width(), visionRep->height(), (QImage::Format)visionRep->imgFormat());
            QPixmap p = QPixmap::fromImage(img);
            QImage q2 = p.toImage();
            q2 = q2.convertToFormat(QImage::Format_RGB888);
            avl::Image image2(q2.width(), q2.height(), q2.bytesPerLine(), avl::PlainType::Type::UInt8, q2.depth() / 8, q2.bits());
            image = image2;
        } else {
            qFatal("grabImageFromCamera fail");
            return false;
        }
    } else {
        BaslerPylonCamera *camera = Q_NULLPTR;
        if (cameraName.contains(DOWNLOOK_VISION_CAMERA)) { camera = downlookCamera; }
        else if (cameraName.contains(UPLOOK_VISION_CAMERA)) { camera = uplookCamera; }
        else if (cameraName.contains(PICKARM_VISION_CAMERA)) { camera = pickarmCamera; }
        else if (cameraName.contains(CAMERA_AA2_DL)) { camera = aa2DownlookCamera; }
        else if (cameraName.contains(CAMERA_SPA_DL)) { camera = sensorPickarmCamera; }
        else if (cameraName.contains(CAMERA_LPA_UL)) { camera = sensorUplookCamera; }
        else if (cameraName.contains(CAMERA_LPA_BARCODE)) { camera = barcodeCamera; }
        if (camera == Q_NULLPTR) {
            qWarning("Cannot find camera %s", cameraName.toStdString().c_str());
            return false;
        }
        if (!camera->isCameraGrabbing())
        {
            qWarning("camera grabbing fail %s", cameraName.toStdString().c_str());
            return false;
        }
        QPixmap p = QPixmap::fromImage(camera->getNewImage());
        QImage q2 = p.toImage();
        q2 = q2.convertToFormat(QImage::Format_RGB888);
        avl::Image image2(q2.width(), q2.height(), q2.bytesPerLine(), avl::PlainType::Type::UInt8, q2.depth() / 8, q2.bits());
        image = image2;
    }
    return true;
}

bool VisionModule::saveImageAndCheck(avl::Image image1, QString imageName)
{
    try {
        avl::SaveImageToJpeg( image1 , imageName.toStdString().c_str(), atl::NIL, false );
    } catch(const atl::Error& error) {
        qInfo("saveImageAndCheck: %s", error.Message());
        qWarning(error.Message());
        return false;
    }
    return true;
}

void VisionModule::startWork(int run_mode)
{
    qDebug("startWork");
}

void VisionModule::stopWork(bool wait_finish)
{
    qDebug("stopWork");
}

void VisionModule::resetLogic()
{
    qDebug("resetLogic");
}

void VisionModule::performHandlingOperation(int cmd, QVariant params)
{
    qDebug("performHandlingOperation");
}

PropertyBase *VisionModule::getModuleState()
{
    return Q_NULLPTR;
}

void VisionModule::RegionJudge( RegionJudgeState& state, const avl::Image& inSubtractImage, const avl::Image&, bool& outRegionOk, atl::Conditional< avl::Region >& outRegion, avl::Region& outRegion1, avl::Region& outRegion2, atl::Conditional< avl::Region >& outRegion3 )
{
    atl::Conditional< bool > bool1;

    avl::ThresholdToRegion_Dynamic( inSubtractImage, atl::NIL, atl::NIL, 5, 5, 5.0f, atl::NIL, 0.0f, state.region1, atl::Dummy<avl::Image>().Get() );
    avl::SplitRegionIntoBlobs( state.region1, avl::RegionConnectivity::EightDirections, 100, atl::NIL, true, state.regionArray1, atl::Dummy< atl::Array< int > >().Get() );

    state.regionArray2.Resize(state.regionArray1.Size());

    for( int i = 0; i < state.regionArray1.Size(); ++i )
    {
        avl::DilateRegion( state.regionArray1[i], avl::KernelShape::Box, 2, atl::NIL, state.regionArray2[i] );
    }

    avl::RegionUnion_OfArray( state.regionArray2, outRegion1 );
    avl::FillRegionHoles( outRegion1, avl::RegionConnectivity::EightDirections, 0, GlueInnerFrameMinArea, outRegion2 );
    avl::SplitRegionIntoBlobs( outRegion2, avl::RegionConnectivity::EightDirections, GlueLineMinArea, atl::NIL, true, state.regionArray3, atl::Dummy< atl::Array< int > >().Get() );
    avl::GetMaximumRegion_OrNil( state.regionArray3, avl::RegionFeature::Area, outRegion, atl::Dummy< atl::Conditional< float > >().Get(), atl::Dummy< atl::Conditional< int > >().Get() );

    if (outRegion != atl::NIL)
    {
        int integer1;

        bool1.AssignNonNil();

        avl::RegionNumberOfHoles( outRegion.Get(), avl::RegionConnectivity::EightDirections, GlueInnerFrameMinArea, atl::NIL, integer1 );
        bool1.Get() = integer1 == 1;
    }
    else
    {
        bool1 = atl::NIL;
    }

    avl::MergeDefault< bool >( bool1, false, outRegionOk );

    outRegion3 = outRegion;
}

void VisionModule::WidthJudge( WidthJudgeState& state, bool inRegionOk, atl::Conditional< const avl::Region& > inRegion, float inResolution, float inMinWidth, float inMaxWidth, const avl::Image& inAfterImage, float inMaxAveWidth, bool& outResultOK, avl::Image& outResultImage, atl::Conditional< float >& outMaxWidth, atl::Conditional< float >& outMinWidth, atl::Conditional< float >& outAveWidth )
{
    atl::Array<atl::Conditional<atl::String>> g_constData1;
    atl::Array<atl::Conditional<avl::Location>> g_constData2;
    atl::Array<atl::Conditional<atl::String>> g_constData3;
    atl::String g_constData4;
    atl::String g_constData5;
    atl::String g_constData6;
    atl::String g_constData7;
    atl::Array<atl::Conditional<avl::Location>> g_constData8;

    outResultOK = false;
    g_constData1.Reset(1);
    g_constData1[0] = "胶线破损严重";

    g_constData2.Reset(1);
    g_constData2[0] = avl::Location(323, 66);

    g_constData3.Reset(1);
    g_constData3[0] = "胶线宽度问题";

    g_constData4 = "Result:";

    g_constData5 = " Max:";

    g_constData6 = " Min:";

    g_constData7 = " Avg:";

    g_constData8.Reset(1);
    g_constData8[0] = avl::Location(313, 23);

    QString imageName;
    imageName.append(getDispensePrLogDir())
            .append(getCurrentTimeString())
            .append(".jpg");

    if (inResolution < 0.0f)
        throw atl::DomainError("Argument \"inResolution\" of function \"WidthJudge\" is out of the range of valid values.");

    if (inRegionOk == false)
    {
        avs::DrawStrings_SingleColor( inAfterImage, g_constData1, g_constData2, atl::NIL, avl::Anchor2D::MiddleCenter, avl::Pixel(255.0f, 0.0f, 0.0f, 0.0f), avl::DrawingStyle(avl::DrawingMode::HighQuality, 1.0f, 1.0f, false, atl::NIL, 1.0f), 32.0f, 0.0f, true, atl::NIL, state.image1 );

        if (inRegion != atl::NIL)
        {
            state.pathArray1.AssignNonNil();

            avl::RegionContours( inRegion.Get(), avl::RegionContourMode::PixelCenters, avl::RegionConnectivity::EightDirections, state.pathArray2 );
            avl::SelectClosedPaths( state.pathArray2, state.pathArray3 );

            state.pathArray1.Get().Resize(state.pathArray3.Size());

            for( int i = 0; i < state.pathArray3.Size(); ++i )
            {
                state.pathArray1.Get()[i].AssignNonNil();
                state.pathArray1.Get()[i].Get() = state.pathArray3[i];
            }
        }
        else
        {
            state.pathArray1 = atl::NIL;
        }

        avs::DrawPaths_SingleColor( state.image1, state.pathArray1, atl::NIL, avl::Pixel(255.0f, 0.0f, 0.0f, 0.0f), avl::DrawingStyle(avl::DrawingMode::HighQuality, 1.0f, 1.0f, false, atl::NIL, 1.0f), true, outResultImage );
        avl::SaveImageToJpeg(outResultImage , imageName.toStdString().c_str(), atl::NIL, false);
        outMaxWidth = atl::NIL;
        outMinWidth = atl::NIL;
        outAveWidth = atl::NIL;
    }
    else if (inRegionOk == true)
    {
        atl::Conditional< bool > bool1;

        if (inRegion != atl::NIL)
        {
            avl::RegionContours( inRegion.Get(), avl::RegionContourMode::PixelCenters, avl::RegionConnectivity::EightDirections, state.pathArray4 );
            avl::SelectClosedPaths( state.pathArray4, state.pathArray5 );

            state.pathArray6.Resize(state.pathArray5.Size());

            for( int i = 0; i < state.pathArray5.Size(); ++i )
            {
                avl::SmoothPath_Gauss( state.pathArray5[i], 0.6f, 3.0f, state.pathArray6[i] );
            }

            avl::GetMinimumPath_OrNil( state.pathArray6, avl::PathFeature::Length, state.path1, atl::Dummy< atl::Conditional< float > >().Get(), atl::Dummy< atl::Conditional< int > >().Get() );
            avl::GetMaximumPath_OrNil( state.pathArray6, avl::PathFeature::Length, state.path2, atl::Dummy< atl::Conditional< float > >().Get(), atl::Dummy< atl::Conditional< int > >().Get() );

            if (state.path1 != atl::NIL)
            {
                state.path3.AssignNonNil();

                avl::ConvertToEquidistantPath( state.path1.Get(), 3.0f, avl::EquidistanceType::OutputPathEquidistance, state.path3.Get() );

                // Function AvsFilter_AccessPath is intended for generated code only. Consider use of Path accessors instead.
                avs::AvsFilter_AccessPath( state.path3.Get(), state.point2DArray1, atl::Dummy<bool>().Get() );
            }
            else
            {
                state.path3 = atl::NIL;
            }

            if (state.path2 != atl::NIL)
            {
                state.path4.AssignNonNil();

                avl::ConvertToEquidistantPath( state.path2.Get(), 3.0f, avl::EquidistanceType::OutputPathEquidistance, state.path4.Get() );

                if (state.path3 != atl::NIL)
                {
                    atl::Conditional< bool > bool2;

                    bool1.AssignNonNil();
                    state.segment2DArray1.AssignNonNil();

                    avl::PathToPathDistanceProfile( state.path3.Get(), state.path4.Get(), avl::PathDistanceMode::PointToSegment, inResolution, state.profile1, state.realArray1, state.segment2DArray2 );
                    avl::MaximumReal_OfArray_OrNil( state.realArray1, outMaxWidth, atl::NIL );
                    avl::MinimumReal_OfArray_OrNil( state.realArray1, outMinWidth, atl::NIL );
                    avl::AverageReals_OfArray_OrNil( state.realArray1, outAveWidth );

                    if (outAveWidth != atl::NIL)
                    {
                        bool2.AssignNonNil();
                        state.string1.AssignNonNil();

                        avl::TestRealLessOrEqual( outAveWidth.Get(), inMaxAveWidth, bool2.Get() );
                        avl::RealToString( outAveWidth.Get(), state.string1.Get() );
                    }
                    else
                    {
                        bool2 = atl::NIL;
                        state.string1 = atl::NIL;
                    }

                    state.boolArray1.Resize(state.realArray1.Size());
                    state.boolArray2.Resize(state.realArray1.Size());

                    for( int i = 0; i < state.realArray1.Size(); ++i )
                    {
                        avl::TestRealGreaterOrEqual( state.realArray1[i], inMinWidth, state.boolArray1[i] );

                        // AvsFilter_MergeAnd is intended for generated code only. In regular programs builtin operator&& should be used.
                        avs::AvsFilter_MergeAnd( state.boolArray1[i], bool2, true, true, true, true, true, true, false, state.boolArray2[i] );
                    }

                    // AvsFilter_And_OfArray is intended for generated code only. In regular programs builtin operator&& should be used.
                    avs::AvsFilter_And_OfArray( state.boolArray2, bool1.Get() );
                    avl::ClassifyByPredicate< avl::Segment2D >( state.segment2DArray2, state.boolArray1, state.segment2DArray3, state.segment2DArray4 );

                    state.segment2DArray1.Get().Resize(state.segment2DArray4.Size());

                    for( int i = 0; i < state.segment2DArray4.Size(); ++i )
                    {
                        state.segment2DArray1.Get()[i].AssignNonNil();
                        state.segment2DArray1.Get()[i].Get() = state.segment2DArray4[i];
                    }

                    if (outMinWidth != atl::NIL)
                    {
                        state.string2.AssignNonNil();

                        avl::RealToString( outMinWidth.Get(), state.string2.Get() );
                    }
                    else
                    {
                        state.string2 = atl::NIL;
                    }
                }
                else
                {
                    bool1 = atl::NIL;
                    state.segment2DArray1 = atl::NIL;
                    outMaxWidth = atl::NIL;
                    outMinWidth = atl::NIL;
                    outAveWidth = atl::NIL;
                    state.string1 = atl::NIL;
                    state.string2 = atl::NIL;
                }
            }
            else
            {
                state.path4 = atl::NIL;
                outMaxWidth = atl::NIL;
                outMinWidth = atl::NIL;
                outAveWidth = atl::NIL;
                state.string1 = atl::NIL;
                bool1 = atl::NIL;
                state.segment2DArray1 = atl::NIL;
                state.string2 = atl::NIL;
            }
        }
        else
        {
            state.path3 = atl::NIL;
            state.path4 = atl::NIL;
            outMaxWidth = atl::NIL;
            outMinWidth = atl::NIL;
            outAveWidth = atl::NIL;
            state.string1 = atl::NIL;
            bool1 = atl::NIL;
            state.segment2DArray1 = atl::NIL;
            state.string2 = atl::NIL;
        }

        avl::MergeDefault< bool >( bool1, false, outResultOK );
        state.pathArray7.Resize(1);
        state.pathArray7[0] = state.path3;
        avs::DrawPaths_SingleColor( inAfterImage, state.pathArray7, atl::NIL, avl::Pixel(0.0f, 255.0f, 0.0f, 0.0f), avl::DrawingStyle(avl::DrawingMode::HighQuality, 1.0f, 1.0f, false, atl::NIL, 1.0f), true, state.image2 );
        state.pathArray8.Resize(1);
        state.pathArray8[0] = state.path4;
        avs::DrawPaths_SingleColor( state.image2, state.pathArray8, atl::NIL, avl::Pixel(0.0f, 0.0f, 255.0f, 0.0f), avl::DrawingStyle(avl::DrawingMode::HighQuality, 1.0f, 1.0f, false, atl::NIL, 1.0f), true, state.image3 );
        avs::DrawSegments_SingleColor( state.image3, state.segment2DArray1, atl::NIL, avl::Pixel(255.0f, 0.0f, 0.0f, 0.0f), avl::DrawingStyle(avl::DrawingMode::HighQuality, 1.0f, 5.0f, false, atl::NIL, 1.0f), avl::MarkerType::None, 5.0f, true, state.image4 );
        state.stringArray1 = atl::NIL;
        state.stringArray2 = g_constData3;
        state.stringArray3 = outResultOK ? state.stringArray1 : state.stringArray2;
        avs::DrawStrings_TwoColors( state.image4, state.stringArray3, g_constData2, atl::NIL, avl::Anchor2D::MiddleCenter, avl::Pixel(0.0f, 200.0f, 0.0f, 0.0f), avl::Pixel(200.0f, 0.0f, 0.0f, 0.0f), atl::ToArray< atl::Conditional< bool > >(outResultOK), avl::DrawingStyle(avl::DrawingMode::HighQuality, 1.0f, 1.0f, false, atl::NIL, 1.0f), 32.0f, 0.0f, true, atl::NIL, state.image5 );

        if (outMaxWidth != atl::NIL && state.string2 != atl::NIL && state.string1 != atl::NIL)
        {
            if (outMinWidth.Get() < inMinWidth || outMaxWidth.Get() > inMaxWidth || outAveWidth.Get() > inMaxAveWidth) outResultOK = false;
            else outResultOK = true;
            state.string3.AssignNonNil();

            avl::BoolToString( outResultOK, state.string4 );
            avl::RealToString( outMaxWidth.Get(), state.string5 );

            // AvsFilter_ConcatenateStrings is intended for generated code only. In regular programs  String::operator+() or String:Append() member function should be used.
            avs::AvsFilter_ConcatenateStrings( g_constData4, state.string4, g_constData6, state.string2.Get(), g_constData5, state.string5, g_constData7, state.string1.Get(), state.string3.Get() );
        }
        else
        {
            state.string3 = atl::NIL;
        }

        state.stringArray4.Resize(1);
        state.stringArray4[0] = state.string3;
        if (outResultOK)
            avs::DrawStrings_SingleColor( state.image5, state.stringArray4, g_constData8, atl::NIL, avl::Anchor2D::MiddleCenter, avl::Pixel(0.0f, 0.0f, 255.0f, 0.0f), avl::DrawingStyle(avl::DrawingMode::HighQuality, 1.0f, 1.0f, false, atl::NIL, 1.0f), 22.0f, 0.0f, true, atl::NIL, outResultImage );
        else {
            avs::DrawStrings_SingleColor( state.image5, state.stringArray4, g_constData8, atl::NIL, avl::Anchor2D::MiddleCenter, avl::Pixel(255.0f, 0.0f, 0.0f, 0.0f), avl::DrawingStyle(avl::DrawingMode::HighQuality, 1.0f, 1.0f, false, atl::NIL, 1.0f), 22.0f, 0.0f, true, atl::NIL, outResultImage );
        }
        //avl::SaveImageToJpeg(outResultImage , imageName.toStdString().c_str(), atl::NIL, false);
    }
    else
    {
        throw atl::RuntimeError("Unsupported fork value encountered in variantable macro function \"WidthJudge\".");
    }
}

void VisionModule::diffenenceImage(QImage image1, QImage image2)
{
    QString imageName;
    imageName.append(getVisionLogDir())
            .append(getCurrentTimeString())
            .append(".jpg");
    avl::Image output;
    avl::Image in1(image1.width(), image1.height(), image1.bytesPerLine(), avl::PlainType::Type::UInt8, image1.depth() / 8, image1.bits());
    avl::Image in2(image2.width(), image2.height(), image2.bytesPerLine(), avl::PlainType::Type::UInt8, image2.depth() / 8, image2.bits());
    avl::DifferenceImage(in1, in2, atl::NIL, output);
    avl::SaveImageToJpeg( output , imageName.toStdString().c_str(), atl::NIL, false );
}

void VisionModule::testVision()
{
    QString imageName;
    QElapsedTimer timer; timer.start();
    double outMinGlueWidth = 0, outMaxGlueWidth = 0, outMaxAvgGlueWidth = 0;
    this->Glue_Inspection(1, 1, 1, 1, "", "", &imageName, &outMinGlueWidth, &outMaxGlueWidth, &outMaxAvgGlueWidth);

    qInfo("Glue inspection reuslt: %s time: %d", imageName.toStdString().c_str(), timer.elapsed());
    //PRResultStruct prResult;
    //this->PR_Generic_NCC_Template_Matching(DOWNLOOK_VISION_CAMERA, "prConfig\\downlook.avdata", prResult);
    //this->PR_Edge_Template_Matching(DOWNLOOK_VISION_CAMERA, "prConfig\\downlook_edgeModel.avdata", prResult);
    //this->PR_Prism_Only_Matching(DOWNLOOK_VISION_CAMERA, prResult);
    //this->PR_Prism_SUT_Matching(DOWNLOOK_VISION_CAMERA, prResult);
    //this->PR_Prism_SUT_Two_Circle_Matching(DOWNLOOK_VISION_CAMERA, prResult);
    //qInfo("%f %f %f %f %f", prResult.x, prResult.y, prResult.theta, prResult.width, prResult.height);
}

bool VisionModule::saveImage(QString cameraName, QString imageName)
{
    avl::Image image1;
    bool ret = grabImageFromCamera(cameraName, image1);
    if (!ret) {
        qInfo("Cannot save image due to camera is not running");
        return ret;
    }

    if (!image1.Empty())
        avl::SaveImageToJpeg(image1 , imageName.toStdString().c_str(), atl::NIL, false);
    return ret;
}

ErrorCodeStruct VisionModule::PR_Prism_SUT_Matching(QString camera_name, PRResultStruct &prResult)
{
    ErrorCodeStruct error_code = { OK, "" };
    QString imageName;
    imageName.append(getVisionLogDir())
            .append(getCurrentTimeString())
            .append(".jpg");
    QString rawImageName;
    rawImageName.append(getVisionLogDir())
            .append(getCurrentTimeString())
            .append("_raw.jpg");

    static atl::String g_constData1;
    static avl::GrayModel g_constData2;
    static atl::String g_constData3;
    static atl::String g_emptyString;
    static atl::Array< atl::Conditional< avl::Location > > g_constData4;

    g_constData3 = L"Angle: ";

    g_emptyString = L"";

    g_constData4.Reset(1);
    g_constData4[0] = avl::Location(114, 71);
    try {
        avl::Image image1;
        avs::FitSegmentToEdgesState fitSegmentToEdgesState1;
        avs::FitSegmentToEdgesState fitSegmentToEdgesState2;
        atl::Conditional< atl::String > string1;
        atl::Conditional< atl::Array< atl::Conditional< avl::Line2D > > > line2DArray1;
        atl::Array< avl::Line2D > line2DArray2;
        atl::String string2;
        avl::Image image2;
        avl::Image image3;
        atl::Array< atl::Conditional< atl::String > > stringArray1;
        avl::Image image4;
        atl::Conditional< avl::Object2D > object2D1;
        atl::Conditional< avl::Point2D > point2D1;
        g_constData1 = L"C:\\Users\\emil\\Downloads\\grabber+log\\grabber log\\15-34-27-400.jpg";
        avs::ReadDataFromFile( L"config\\prConfig\\sutPrismDetection.70f9b147.avdata", L"GrayModel", g_constData2 );

        this->grabImageFromCamera(camera_name, image1);
        avl::SaveImageToJpeg( image1 , rawImageName.toStdString().c_str(), atl::NIL, false );
        //        avl::LoadImage( g_constData1, false, image1 );
        avl::LocateSingleObject_NCC( image1, atl::NIL, g_constData2, 0, 3, false, 0.6f, object2D1, atl::NIL, atl::Dummy< atl::Array< avl::Image > >().Get(), atl::Dummy< atl::Array< avl::Image > >().Get(), atl::Dummy< atl::Conditional< atl::Array< float > > >().Get() );

        if (object2D1 != atl::NIL)
        {
            avl::CoordinateSystem2D coordinateSystem2D1;
            atl::Conditional< avl::Segment2D > segment2D1;
            atl::Conditional< avl::Segment2D > segment2D2;

            coordinateSystem2D1 = object2D1.Get().Alignment();

            // Function AvsFilter_FitSegmentToEdges is intended for generated code only. Consider use of CreateFittingMap and FitSegmentToEdges functions in regular programs.
            avs::AvsFilter_FitSegmentToEdges( fitSegmentToEdgesState1, image1, avl::SegmentFittingField(avl::Segment2D(avl::Point2D(246.308151f, 178.574448f), avl::Point2D(340.674469f, 277.443329f)), 18.52667f), coordinateSystem2D1, 20, 5, avl::InterpolationMethod::Bilinear, avl::EdgeScanParams(avl::ProfileInterpolationMethod::Quadratic4, 1.0f, 5.0f, avl::EdgeTransition::BrightToDark), avl::Selection::Best, atl::NIL, 0.8f, avl::MEstimator::Tukey, segment2D1, atl::NIL, atl::NIL, atl::NIL, atl::Dummy< atl::Array< avl::Segment2D > >().Get(), atl::Dummy< atl::Array< avl::Rectangle2D > >().Get(), atl::Dummy< atl::Array< avl::Profile > >().Get(), atl::Dummy< atl::Array< avl::Profile > >().Get() );

            // Function AvsFilter_FitSegmentToEdges is intended for generated code only. Consider use of CreateFittingMap and FitSegmentToEdges functions in regular programs.
            avs::AvsFilter_FitSegmentToEdges( fitSegmentToEdgesState2, image1, avl::SegmentFittingField(avl::Segment2D(avl::Point2D(345.701355f, 268.6625f), avl::Point2D(420.345764f, 195.70195f)), 19.0827923f), coordinateSystem2D1, 20, 5, avl::InterpolationMethod::Bilinear, avl::EdgeScanParams(avl::ProfileInterpolationMethod::Quadratic4, 1.0f, 5.0f, avl::EdgeTransition::BrightToDark), avl::Selection::Best, atl::NIL, 0.8f, avl::MEstimator::Tukey, segment2D2, atl::NIL, atl::NIL, atl::NIL, atl::Dummy< atl::Array< avl::Segment2D > >().Get(), atl::Dummy< atl::Array< avl::Rectangle2D > >().Get(), atl::Dummy< atl::Array< avl::Profile > >().Get(), atl::Dummy< atl::Array< avl::Profile > >().Get() );

            if (segment2D1 != atl::NIL)
            {
                avl::Line2D line2D1;
                avl::Line2D line2D2;
                float real1;

                string1.AssignNonNil();

                // Function AvsFilter_MakeLine is intended for generated code only. Consider use of proper Line2D constructor instead.
                avs::AvsFilter_MakeLine( 0.0f, 1.0f, 0.0f, line2D1 );
                avl::SegmentLine( segment2D1.Get(), line2D2 );
                avl::AngleBetweenLines( line2D1, line2D2, real1, atl::NIL );
                prResult.theta = real1;
                if (segment2D2 != atl::NIL)
                {
                    avl::Line2D line2D3;

                    line2DArray1.AssignNonNil();

                    avl::SegmentLine( segment2D2.Get(), line2D3 );
                    avl::LineLineIntersection( line2D2, line2D3, point2D1 );
                    prResult.x = point2D1.Get().X();
                    prResult.y = point2D1.Get().Y();
                    prResult.ori_x = point2D1.Get().X();
                    prResult.ori_y = point2D1.Get().Y();

                    // AvsFilter_CreateArray is intended for use in generated code only. Consider use of proper constructor or Array::Clear() and Array::Reserve function in hand-written programs.
                    avs::AvsFilter_CreateArray< avl::Line2D >( line2D2, line2D3, atl::NIL, atl::NIL, atl::NIL, atl::NIL, atl::NIL, atl::NIL, line2DArray2 );

                    line2DArray1.Get().Resize(line2DArray2.Size());

                    for( int i = 0; i < line2DArray2.Size(); ++i )
                    {
                        line2DArray1.Get()[i].AssignNonNil();
                        line2DArray1.Get()[i].Get() = line2DArray2[i];
                    }
                }
                else
                {
                    line2DArray1 = atl::NIL;
                    point2D1 = atl::NIL;

                    qWarning("Cannot find line edge 2");
                    error_code.code = ErrorCode::PR_OBJECT_NOT_FOUND;
                    return error_code;
                }

                avl::RealToString( real1, string2 );

                // AvsFilter_ConcatenateStrings is intended for generated code only. In regular programs  String::operator+() or String:Append() member function should be used.
                avs::AvsFilter_ConcatenateStrings( g_constData3, string2, g_emptyString, g_emptyString, g_emptyString, g_emptyString, g_emptyString, g_emptyString, string1.Get() );
            }
            else
            {
                string1 = atl::NIL;
                point2D1 = atl::NIL;
                line2DArray1 = atl::NIL;
                qWarning("Cannot find line edge 1");
                error_code.code = ErrorCode::PR_OBJECT_NOT_FOUND;
                return error_code;
            }
        }
        else
        {
            point2D1 = atl::NIL;
            line2DArray1 = atl::NIL;
            string1 = atl::NIL;
            qWarning("Cannot find prism object");
            error_code.code = ErrorCode::PR_OBJECT_NOT_FOUND;
            return error_code;
        }

        avs::DrawLines_Palette( image1, line2DArray1, atl::NIL, atl::NIL, avl::DrawingStyle(avl::DrawingMode::HighQuality, 1.0f, 4.0f, false, atl::NIL, 1.0f), true, image2 );
        avs::DrawPoints_SingleColor( image2, atl::ToArray< atl::Conditional< avl::Point2D > >(point2D1), atl::NIL, avl::Pixel(0.0f, 255.0f, 0.0f, 0.0f), avl::DrawingStyle(avl::DrawingMode::HighQuality, 1.0f, 4.0f, false, avl::PointShape::Cross, 50.0f), true, image3 );
        stringArray1.Resize(1);
        stringArray1[0] = string1;
        avs::DrawStrings_SingleColor( image3, stringArray1, g_constData4, atl::NIL, avl::Anchor2D::MiddleCenter, avl::Pixel(0.0f, 255.0f, 0.0f, 0.0f), avl::DrawingStyle(avl::DrawingMode::HighQuality, 1.0f, 1.0f, false, atl::NIL, 30.0f), 30.0f, 0.0f, true, atl::NIL, image4 );
        avl::SaveImageToJpeg( image4 , imageName.toStdString().c_str(), atl::NIL, false );
    } catch(const atl::Error& error) {
        qInfo("PR Error: %s", error.Message());
        qWarning(error.Message());
        error_code.code = ErrorCode::PR_OBJECT_NOT_FOUND;
        return error_code;
    }
    return error_code;

}

ErrorCodeStruct VisionModule::PR_Prism_Only_Matching(QString camera_name, PRResultStruct &prResult)
{
    ErrorCodeStruct error_code = { OK, "" };
    atl::String g_constData1;
    avl::Path g_constData2;
    avl::Path g_constData3;
    avl::Path g_constData4;
    avl::Path g_constData5;
    atl::String g_constData6;
    atl::String g_emptyString;
    atl::Array< atl::Conditional< avl::Location > > g_constData7;
    QString imageName;
    imageName.append(getVisionLogDir())
            .append(getCurrentTimeString())
            .append(".jpg");
    QString rawImageName;
    rawImageName.append(getVisionLogDir())
            .append(getCurrentTimeString())
            .append("_raw.jpg");
    try {
        g_constData1 = L"C:\\Users\\emil\\Documents\\WeChat Files\\milklai1987\\FileStorage\\File\\2019-12\\20191223\\09-36-27-238.jpg";
        avs::ReadDataFromFile( L"config\\prConfig\\PrismOnly.9ec2dbe0.avdata", L"Path", g_constData2 );
        avs::ReadDataFromFile( L"config\\prConfig\\PrismOnly.5aab78b7.avdata", L"Path", g_constData3 );
        avs::ReadDataFromFile( L"config\\prConfig\\PrismOnly.685b0ee7.avdata", L"Path", g_constData4 );
        avs::ReadDataFromFile( L"config\\prConfig\\PrismOnly.1da1935a.avdata", L"Path", g_constData5 );
        g_constData6 = L"Angle: ";
        g_emptyString = L"";
        g_constData7.Reset(1);
        g_constData7[0] = avl::Location(143, 55);
        avl::Image image1;
        avl::Region region1;
        avl::Region region2;
        avl::Image image2;
        avs::ScanSingleEdgeState scanSingleEdgeState1;
        atl::Conditional< avl::Edge1D > edge1D1;
        avs::ScanSingleEdgeState scanSingleEdgeState2;
        atl::Conditional< avl::Edge1D > edge1D2;
        avs::ScanSingleEdgeState scanSingleEdgeState3;
        atl::Conditional< avl::Edge1D > edge1D3;
        avs::ScanSingleEdgeState scanSingleEdgeState4;
        atl::Conditional< avl::Edge1D > edge1D4;
        atl::Conditional< atl::Array< atl::Conditional< avl::Point2D > > > point2DArray1;
        atl::Conditional< atl::Array< atl::Conditional< avl::Point2D > > > point2DArray2;
        atl::Conditional< atl::Array< atl::Conditional< avl::Point2D > > > point2DArray3;
        avs::FitSegmentToEdgesState fitSegmentToEdgesState1;
        atl::Array< atl::Conditional< avl::Edge1D > > edge1DArray1;
        avs::FitSegmentToEdgesState fitSegmentToEdgesState2;
        atl::Array< atl::Conditional< avl::Edge1D > > edge1DArray2;
        avs::FitSegmentToEdgesState fitSegmentToEdgesState3;
        atl::Array< atl::Conditional< avl::Edge1D > > edge1DArray3;
        atl::Conditional< atl::String > string1;
        atl::String string2;
        atl::Conditional< atl::Array< atl::Conditional< avl::Line2D > > > line2DArray1;
        atl::Array< avl::Point2D > point2DArray4;
        atl::Conditional< atl::Array< atl::Conditional< avl::Point2D > > > point2DArray5;
        atl::Array< avl::Point2D > point2DArray6;
        atl::Array< avl::Line2D > line2DArray2;
        avl::Image image3;
        avl::Image image4;
        avl::Image image5;
        avl::Image image6;
        avl::Image image7;
        atl::Array< atl::Conditional< atl::String > > stringArray1;
        avl::Image image8;

        this->grabImageFromCamera(camera_name, image1);
        avl::SaveImageToJpeg( image1 , rawImageName.toStdString().c_str(), atl::NIL, false );
        //avl::LoadImage( g_constData1, false, image1 );
        avl::ThresholdToRegion( image1, atl::NIL, 200.0f, 255.0f, 0.0f, region1 );
        avl::RemoveRegionBlobs( region1, avl::RegionConnectivity::EightDirections, avl::RegionFeature::Area, 100.0f, atl::NIL, true, region2 );
        avl::RegionToImage( region2, image2 );

        // Function AvsFilter_ScanSingleEdge is intended for generated code only, consider use of CreateScanMap and ScanSingleEdge functions in regular programs.
        avs::AvsFilter_ScanSingleEdge( scanSingleEdgeState1, image2, g_constData2, atl::NIL, 5, avl::InterpolationMethod::Bilinear, avl::EdgeScanParams(avl::ProfileInterpolationMethod::Quadratic4, 0.6f, 5.0f, avl::EdgeTransition::DarkToBright), avl::Selection::First, atl::NIL, edge1D1, atl::NIL, atl::Dummy<avl::Profile>().Get(), atl::Dummy<avl::Profile>().Get(), atl::Dummy< atl::Array< avl::Path > >().Get() );

        // Function AvsFilter_ScanSingleEdge is intended for generated code only, consider use of CreateScanMap and ScanSingleEdge functions in regular programs.
        avs::AvsFilter_ScanSingleEdge( scanSingleEdgeState2, image2, g_constData3, atl::NIL, 5, avl::InterpolationMethod::Bilinear, avl::EdgeScanParams(avl::ProfileInterpolationMethod::Quadratic4, 0.6f, 5.0f, avl::EdgeTransition::DarkToBright), avl::Selection::First, atl::NIL, edge1D2, atl::NIL, atl::Dummy<avl::Profile>().Get(), atl::Dummy<avl::Profile>().Get(), atl::Dummy< atl::Array< avl::Path > >().Get() );

        // Function AvsFilter_ScanSingleEdge is intended for generated code only, consider use of CreateScanMap and ScanSingleEdge functions in regular programs.
        avs::AvsFilter_ScanSingleEdge( scanSingleEdgeState3, image2, g_constData4, atl::NIL, 5, avl::InterpolationMethod::Bilinear, avl::EdgeScanParams(avl::ProfileInterpolationMethod::Quadratic4, 0.6f, 5.0f, avl::EdgeTransition::DarkToBright), avl::Selection::First, atl::NIL, edge1D3, atl::NIL, atl::Dummy<avl::Profile>().Get(), atl::Dummy<avl::Profile>().Get(), atl::Dummy< atl::Array< avl::Path > >().Get() );

        // Function AvsFilter_ScanSingleEdge is intended for generated code only, consider use of CreateScanMap and ScanSingleEdge functions in regular programs.
        avs::AvsFilter_ScanSingleEdge( scanSingleEdgeState4, image2, g_constData5, atl::NIL, 5, avl::InterpolationMethod::Bilinear, avl::EdgeScanParams(avl::ProfileInterpolationMethod::Quadratic4, 0.6f, 5.0f, avl::EdgeTransition::DarkToBright), avl::Selection::First, atl::NIL, edge1D4, atl::NIL, atl::Dummy<avl::Profile>().Get(), atl::Dummy<avl::Profile>().Get(), atl::Dummy< atl::Array< avl::Path > >().Get() );

        if (edge1D3 != atl::NIL && edge1D4 != atl::NIL && edge1D1 != atl::NIL && edge1D2 != atl::NIL)
        {
            avl::Point2D point2D1;
            avl::Point2D point2D2;
            avl::Line2D line2D1;
            avl::Point2D point2D3;
            avl::Point2D point2D4;
            avl::Line2D line2D2;
            atl::Conditional< avl::Point2D > point2D5;

            point2D1 = edge1D3.Get().Point();
            point2D2 = edge1D4.Get().Point();
            avl::LineThroughPoints( point2D1, point2D2, line2D1 );
            point2D3 = edge1D1.Get().Point();
            point2D4 = edge1D2.Get().Point();
            avl::LineThroughPoints( point2D3, point2D4, line2D2 );
            avl::LineLineIntersection( line2D2, line2D1, point2D5 );

            if (point2D5 != atl::NIL)
            {
                avl::Line2D line2D3;
                avl::CoordinateSystem2D coordinateSystem2D1;
                atl::Conditional< avl::Segment2D > segment2D1;
                atl::Conditional< avl::Segment2D > segment2D2;
                atl::Conditional< avl::Segment2D > segment2D3;
                atl::Conditional< avl::Line2D > line2D4;
                atl::Conditional< avl::Line2D > line2D5;
                atl::Conditional< avl::Point2D > point2D6;

                point2DArray1.AssignNonNil();
                point2DArray2.AssignNonNil();
                point2DArray3.AssignNonNil();

                // Function AvsFilter_MakeLine is intended for generated code only. Consider use of proper Line2D constructor instead.
                avs::AvsFilter_MakeLine( 0.0f, 1.0f, 0.0f, line2D3 );
                avl::CreateCoordinateSystemFromPoint( point2D5.Get(), 0.0f, 1.0f, 1.0f, coordinateSystem2D1 );

                // Function AvsFilter_FitSegmentToEdges is intended for generated code only. Consider use of CreateFittingMap and FitSegmentToEdges functions in regular programs.
                avs::AvsFilter_FitSegmentToEdges( fitSegmentToEdgesState1, image2, avl::SegmentFittingField(avl::Segment2D(avl::Point2D(160.3417f, -153.329147f), avl::Point2D(50.339447f, -47.8167725f)), 82.9198f), coordinateSystem2D1, 10, 5, avl::InterpolationMethod::Bilinear, avl::EdgeScanParams(avl::ProfileInterpolationMethod::Quadratic4, 1.0f, 5.0f, avl::EdgeTransition::Any), avl::Selection::Best, atl::NIL, 0.1f, atl::NIL, segment2D1, edge1DArray1, atl::NIL, atl::NIL, atl::Dummy< atl::Array< avl::Segment2D > >().Get(), atl::Dummy< atl::Array< avl::Rectangle2D > >().Get(), atl::Dummy< atl::Array< avl::Profile > >().Get(), atl::Dummy< atl::Array< avl::Profile > >().Get() );

                // Function AvsFilter_FitSegmentToEdges is intended for generated code only. Consider use of CreateFittingMap and FitSegmentToEdges functions in regular programs.
                avs::AvsFilter_FitSegmentToEdges( fitSegmentToEdgesState2, image2, avl::SegmentFittingField(avl::Segment2D(avl::Point2D(137.892273f, 143.5647f), avl::Point2D(54.2680969f, 52.64447f)), 87.22975f), coordinateSystem2D1, 10, 5, avl::InterpolationMethod::Bilinear, avl::EdgeScanParams(avl::ProfileInterpolationMethod::Quadratic4, 1.0f, 5.0f, avl::EdgeTransition::Any), avl::Selection::Best, atl::NIL, 0.1f, atl::NIL, segment2D2, edge1DArray2, atl::NIL, atl::NIL, atl::Dummy< atl::Array< avl::Segment2D > >().Get(), atl::Dummy< atl::Array< avl::Rectangle2D > >().Get(), atl::Dummy< atl::Array< avl::Profile > >().Get(), atl::Dummy< atl::Array< avl::Profile > >().Get() );

                // Function AvsFilter_FitSegmentToEdges is intended for generated code only. Consider use of CreateFittingMap and FitSegmentToEdges functions in regular programs.
                avs::AvsFilter_FitSegmentToEdges( fitSegmentToEdgesState3, image2, avl::SegmentFittingField(avl::Segment2D(avl::Point2D(212.536682f, -106.185333f), avl::Point2D(211.786133f, 67.11984f)), 87.22975f), coordinateSystem2D1, 10, 5, avl::InterpolationMethod::Bilinear, avl::EdgeScanParams(avl::ProfileInterpolationMethod::Quadratic4, 1.0f, 5.0f, avl::EdgeTransition::Any), avl::Selection::Best, atl::NIL, 0.1f, atl::NIL, segment2D3, edge1DArray3, atl::NIL, atl::NIL, atl::Dummy< atl::Array< avl::Segment2D > >().Get(), atl::Dummy< atl::Array< avl::Rectangle2D > >().Get(), atl::Dummy< atl::Array< avl::Profile > >().Get(), atl::Dummy< atl::Array< avl::Profile > >().Get() );

                if (segment2D2 != atl::NIL)
                {
                    line2D4.AssignNonNil();

                    avl::SegmentLine( segment2D2.Get(), line2D4.Get() );
                    avl::AngleBetweenLines( line2D3, line2D4.Get(), atl::NIL, atl::NIL );
                }
                else
                {
                    line2D4 = atl::NIL;
                }

                if (segment2D1 != atl::NIL)
                {
                    float real1;

                    line2D5.AssignNonNil();
                    string1.AssignNonNil();

                    avl::SegmentLine( segment2D1.Get(), line2D5.Get() );

                    if (line2D4 != atl::NIL)
                    {
                        avl::LineLineIntersection( line2D5.Get(), line2D4.Get(), point2D6 );
                    }
                    else
                    {
                        point2D6 = atl::NIL;
                    }

                    avl::AngleBetweenLines( line2D3, line2D5.Get(), real1, atl::NIL );
                    avl::RealToString( real1, string2 );
                    prResult.theta = real1;
                    // AvsFilter_ConcatenateStrings is intended for generated code only. In regular programs  String::operator+() or String:Append() member function should be used.
                    avs::AvsFilter_ConcatenateStrings( g_constData6, string2, g_emptyString, g_emptyString, g_emptyString, g_emptyString, g_emptyString, g_emptyString, string1.Get() );
                }
                else
                {
                    line2D5 = atl::NIL;
                    string1 = atl::NIL;
                    point2D6 = atl::NIL;
                }

                point2DArray1.Get().Resize(edge1DArray2.Size());

                for( int i = 0; i < edge1DArray2.Size(); ++i )
                {
                    if (edge1DArray2[i] != atl::NIL)
                    {
                        point2DArray1.Get()[i].AssignNonNil();

                        point2DArray1.Get()[i].Get() = edge1DArray2[i].Get().Point();
                    }
                    else
                    {
                        point2DArray1.Get()[i] = atl::NIL;
                    }
                }

                point2DArray2.Get().Resize(edge1DArray1.Size());

                for( int i = 0; i < edge1DArray1.Size(); ++i )
                {
                    if (edge1DArray1[i] != atl::NIL)
                    {
                        point2DArray2.Get()[i].AssignNonNil();

                        point2DArray2.Get()[i].Get() = edge1DArray1[i].Get().Point();
                    }
                    else
                    {
                        point2DArray2.Get()[i] = atl::NIL;
                    }
                }

                point2DArray3.Get().Resize(edge1DArray3.Size());

                for( int i = 0; i < edge1DArray3.Size(); ++i )
                {
                    if (edge1DArray3[i] != atl::NIL)
                    {
                        point2DArray3.Get()[i].AssignNonNil();

                        point2DArray3.Get()[i].Get() = edge1DArray3[i].Get().Point();
                    }
                    else
                    {
                        point2DArray3.Get()[i] = atl::NIL;
                    }
                }

                if (segment2D3 != atl::NIL && line2D4 != atl::NIL && line2D5 != atl::NIL)
                {
                    avl::Line2D line2D6;
                    atl::Conditional< avl::Point2D > point2D7;
                    atl::Conditional< avl::Point2D > point2D8;

                    line2DArray1.AssignNonNil();

                    avl::SegmentLine( segment2D3.Get(), line2D6 );
                    avl::LineLineIntersection( line2D4.Get(), line2D6, point2D7 );
                    avl::LineLineIntersection( line2D5.Get(), line2D6, point2D8 );

                    if (point2D6 != atl::NIL && point2D8 != atl::NIL && point2D7 != atl::NIL)
                    {
                        atl::Conditional< avl::Point2D > point2D9;

                        // AvsFilter_CreateArray is intended for use in generated code only. Consider use of proper constructor or Array::Clear() and Array::Reserve function in hand-written programs.
                        avs::AvsFilter_CreateArray< avl::Point2D >( point2D6.Get(), point2D8.Get(), point2D7.Get(), atl::NIL, atl::NIL, atl::NIL, atl::NIL, atl::NIL, point2DArray4 );
                        avl::PointsMassCenter_OrNil( point2DArray4, point2D9 );
                        prResult.x = point2D9.Get().X();
                        prResult.y = point2D9.Get().Y();
                        prResult.ori_x = point2D9.Get().X();
                        prResult.ori_y = point2D9.Get().Y();

                        if (point2D9 != atl::NIL)
                        {
                            point2DArray5.AssignNonNil();

                            // AvsFilter_CreateArray is intended for use in generated code only. Consider use of proper constructor or Array::Clear() and Array::Reserve function in hand-written programs.
                            avs::AvsFilter_CreateArray< avl::Point2D >( point2D6.Get(), point2D8.Get(), point2D7.Get(), point2D9.Get(), atl::NIL, atl::NIL, atl::NIL, atl::NIL, point2DArray6 );

                            point2DArray5.Get().Resize(point2DArray6.Size());

                            for( int i = 0; i < point2DArray6.Size(); ++i )
                            {
                                point2DArray5.Get()[i].AssignNonNil();
                                point2DArray5.Get()[i].Get() = point2DArray6[i];
                            }
                        }
                        else
                        {
                            point2DArray5 = atl::NIL;
                        }
                    }
                    else
                    {
                        point2DArray5 = atl::NIL;
                    }

                    // AvsFilter_CreateArray is intended for use in generated code only. Consider use of proper constructor or Array::Clear() and Array::Reserve function in hand-written programs.
                    avs::AvsFilter_CreateArray< avl::Line2D >( line2D5.Get(), line2D4.Get(), line2D6, atl::NIL, atl::NIL, atl::NIL, atl::NIL, atl::NIL, line2DArray2 );

                    line2DArray1.Get().Resize(line2DArray2.Size());

                    for( int i = 0; i < line2DArray2.Size(); ++i )
                    {
                        line2DArray1.Get()[i].AssignNonNil();
                        line2DArray1.Get()[i].Get() = line2DArray2[i];
                    }
                }
                else
                {
                    line2DArray1 = atl::NIL;
                    point2DArray5 = atl::NIL;
                    qWarning("PR Error: Cannot find prism object");
                    error_code.code = ErrorCode::PR_OBJECT_NOT_FOUND;
                    return error_code;
                }
            }
            else
            {
                point2DArray1 = atl::NIL;
                point2DArray2 = atl::NIL;
                point2DArray3 = atl::NIL;
                string1 = atl::NIL;
                point2DArray5 = atl::NIL;
                line2DArray1 = atl::NIL;
                qWarning("PR Error: Cannot find prism object");
                error_code.code = ErrorCode::PR_OBJECT_NOT_FOUND;
                return error_code;
            }
        }
        else
        {
            string1 = atl::NIL;
            point2DArray1 = atl::NIL;
            point2DArray2 = atl::NIL;
            point2DArray3 = atl::NIL;
            point2DArray5 = atl::NIL;
            line2DArray1 = atl::NIL;
            qWarning("PR Error: Cannot find prism object");
            error_code.code = ErrorCode::PR_OBJECT_NOT_FOUND;
            return error_code;
        }

        avs::DrawPoints_SingleColor( image1, point2DArray1, atl::NIL, avl::Pixel(255.0f, 1.0f, 255.0f, 0.0f), avl::DrawingStyle(avl::DrawingMode::HighQuality, 1.0f, 5.0f, false, avl::PointShape::Cross, 25.0f), true, image3 );
        avs::DrawPoints_SingleColor( image3, point2DArray2, atl::NIL, avl::Pixel(255.0f, 1.0f, 255.0f, 0.0f), avl::DrawingStyle(avl::DrawingMode::HighQuality, 1.0f, 5.0f, false, avl::PointShape::Cross, 25.0f), true, image4 );
        avs::DrawPoints_SingleColor( image4, point2DArray5, atl::NIL, avl::Pixel(0.0f, 0.0f, 255.0f, 0.0f), avl::DrawingStyle(avl::DrawingMode::HighQuality, 1.0f, 5.0f, false, avl::PointShape::Cross, 25.0f), true, image5 );
        avs::DrawPoints_SingleColor( image5, point2DArray3, atl::NIL, avl::Pixel(255.0f, 1.0f, 255.0f, 0.0f), avl::DrawingStyle(avl::DrawingMode::HighQuality, 1.0f, 5.0f, false, avl::PointShape::Cross, 25.0f), true, image6 );
        avs::DrawLines_Palette( image6, line2DArray1, atl::NIL, atl::NIL, avl::DrawingStyle(avl::DrawingMode::HighQuality, 1.0f, 4.0f, false, atl::NIL, 1.0f), true, image7 );
        stringArray1.Resize(1);
        stringArray1[0] = string1;
        avs::DrawStrings_SingleColor( image7, stringArray1, g_constData7, atl::NIL, avl::Anchor2D::MiddleCenter, avl::Pixel(0.0f, 255.0f, 0.0f, 0.0f), avl::DrawingStyle(avl::DrawingMode::HighQuality, 1.0f, 1.0f, false, atl::NIL, 1.0f), 28.0f, 0.0f, true, atl::NIL, image8 );
        avl::SaveImageToJpeg( image8 , imageName.toStdString().c_str(), atl::NIL, false );
    } catch(const atl::Error& error) {
        qWarning("PR Error: %s", error.Message());
        qWarning(error.Message());
        error_code.code = ErrorCode::PR_OBJECT_NOT_FOUND;
        return error_code;
    }

    return error_code;
}

ErrorCodeStruct VisionModule::PR_Prism_SUT_Two_Circle_Matching(QString camera_name, PRResultStruct &prResult)
{
    qInfo("PR_Prism_SUT_Two_Circle_Matching");
    ErrorCodeStruct error_code = { OK, "" };
    atl::String g_constData1;
    avl::Region g_constData2;
    atl::String g_constData3;
    atl::String g_emptyString;
    atl::Array< atl::Conditional< avl::Location > > g_constData4;

    QString imageName;
    imageName.append(getVisionLogDir())
            .append(getCurrentTimeString())
            .append(".jpg");
    QString rawImageName;
    rawImageName.append(getVisionLogDir())
            .append(getCurrentTimeString())
            .append("_raw.jpg");
    g_constData1 = "C:\\Users\\emil\\Documents\\WeChat Files\\milklai1987\\FileStorage\\File\\2019-12\\20191217\\13-54-02-283.jpg";
    g_constData3 = L"Angle:";

    g_emptyString = L"";

    g_constData4.Reset(1);
    g_constData4[0] = avl::Location(156, 53);
    try {
        avs::ReadDataFromFile("config\\prConfig\\twoCircles.db0fe743.avdata", "Region", g_constData2 );
        avl::Image image1;
        atl::String file1;
        atl::String string1;
        atl::Array< avl::HoughCircle > houghCircleArray1;
        atl::Array< avl::Circle2D > circle2DArray1;
        atl::Array< bool > boolArray1;
        atl::Array< avl::Circle2D > circle2DArray2;
        atl::Array< avl::Circle2D > circle2DArray3;
        atl::Array< atl::Conditional< avl::Point2D > > point2DArray1;
        avs::FitCircleToEdgesState fitCircleToEdgesState1;
        avs::FitCircleToEdgesState fitCircleToEdgesState2;
        avs::FitCircleToRidgesState fitCircleToRidgesState1;
        avl::Image image2;
        atl::Conditional< atl::String > string2;
        atl::String string3;
        avl::Image image3;
        avl::Image image4;
        atl::Array< atl::Conditional< atl::String > > stringArray1;
        avl::Image image5;

        atl::Conditional< avl::Point2D > point2D1;
        atl::Conditional< avl::Point2D > point2D2;
        atl::Conditional< avl::Line2D > line2D1;
        atl::Conditional< avl::Point2D > point2D3;

        this->grabImageFromCamera(camera_name, image1);
        //avl::LoadImage( g_constData1, false, image1 );
        avl::SaveImageToJpeg( image1 , rawImageName.toStdString().c_str(), atl::NIL, false );

        //ToDo: Open the parameter for the circle radius detection ( 11.0 )
        avl::DetectMultipleCircles( image1, g_constData2, 11.0f, 0.1f, 15.0f, 10.0f, houghCircleArray1, atl::Dummy<avl::Image>().Get(), atl::Dummy<avl::Image>().Get() );

        circle2DArray1.Resize(houghCircleArray1.Size());
        boolArray1.Resize(houghCircleArray1.Size());

        for( int i = 0; i < houghCircleArray1.Size(); ++i )
        {
            avl::ShapeRegion shapeRegion1;

            circle2DArray1[i] = houghCircleArray1[i].Circle();
            avl::Circle2DToShapeRegion( circle2DArray1[i], shapeRegion1 );
            avl::CheckPresence_Intensity( image1, shapeRegion1, atl::NIL, 240.0f, atl::NIL, 0.0f, atl::NIL, boolArray1[i], atl::NIL, atl::NIL, atl::NIL );
        }

        avl::ClassifyByPredicate< avl::Circle2D >( circle2DArray1, boolArray1, circle2DArray2, circle2DArray3 );

        point2DArray1.Resize(circle2DArray2.Size());

        for( int i = 0; i < circle2DArray2.Size(); ++i )
        {
            atl::Conditional< avl::Circle2D > circle2D1;

            avl::CircleFittingField circleFittingField1 = avl::CircleFittingField(circle2DArray2[i], 10.0f);

            // Function AvsFilter_FitCircleToEdges is intended for generated code only. Consider use of CreateFittingMap and FitCircleToEdges functions in regular programs.
            avs::AvsFilter_FitCircleToEdges( fitCircleToEdgesState1, image1, circleFittingField1, atl::NIL, 10, 5, avl::InterpolationMethod::Bilinear, avl::EdgeScanParams(avl::ProfileInterpolationMethod::Quadratic4, 1.0f, 5.0f, avl::EdgeTransition::BrightToDark), avl::Selection::Best, atl::NIL, 0.1f, avl::CircleFittingMethod::AlgebraicTaubin, avl::MEstimator::Huber, circle2D1, atl::NIL, atl::NIL, atl::NIL, atl::Dummy< atl::Array< avl::Segment2D > >().Get(), atl::Dummy< atl::Array< avl::Rectangle2D > >().Get(), atl::Dummy< atl::Array< avl::Profile > >().Get(), atl::Dummy< atl::Array< avl::Profile > >().Get() );

            if (circle2D1 != atl::NIL)
            {
                point2DArray1[i].AssignNonNil();

                point2DArray1[i].Get() = circle2D1.Get().Center();
            }
            else
            {
                point2DArray1[i] = atl::NIL;
            }
        }

        avs::DrawPoints_SingleColor( image1, point2DArray1, atl::NIL, avl::Pixel(255.0f, 128.0f, 0.0f, 0.0f), avl::DrawingStyle(avl::DrawingMode::HighQuality, 1.0f, 4.0f, false, avl::PointShape::Cross, 30.0f), true, image2 );

        // AvsFilter_GetArrayElement_OrNil is intended for generated code only, consider use of built-in language features in regular programs.
        avs::AvsFilter_GetArrayElement_OrNil< atl::Conditional< avl::Point2D > >( point2DArray1, 0, false, point2D1 );

        // AvsFilter_GetArrayElement_OrNil is intended for generated code only, consider use of built-in language features in regular programs.
        avs::AvsFilter_GetArrayElement_OrNil< atl::Conditional< avl::Point2D > >( point2DArray1, 1, false, point2D2 );

        if (point2D1 != atl::NIL && point2D2 != atl::NIL)
        {
            avl::Line2D line2D2;
            float real1;

            line2D1.AssignNonNil();
            point2D3.AssignNonNil();
            string2.AssignNonNil();

            // Function AvsFilter_MakeLine is intended for generated code only. Consider use of proper Line2D constructor instead.
            avs::AvsFilter_MakeLine( 0.0f, 1.0f, 0.0f, line2D2 );
            avl::LineThroughPoints( point2D1.Get(), point2D2.Get(), line2D1.Get() );
            avl::AveragePoint( point2D1.Get(), point2D2.Get(), point2D3.Get() );
            avl::AngleBetweenLines( line2D1.Get(), line2D2, real1, atl::NIL );
            avl::RealToString( real1, string3 );
            prResult.x = point2D3.Get().X();
            prResult.y = point2D3.Get().Y();
            prResult.ori_x = point2D3.Get().X();
            prResult.ori_y = point2D3.Get().Y();
            prResult.theta = real1;
            // AvsFilter_ConcatenateStrings is intended for generated code only. In regular programs  String::operator+() or String:Append() member function should be used.
            avs::AvsFilter_ConcatenateStrings( g_constData3, string3, g_emptyString, g_emptyString, g_emptyString, g_emptyString, g_emptyString, g_emptyString, string2.Get() );
        }
        else
        {
            line2D1 = atl::NIL;
            point2D3 = atl::NIL;
            string2 = atl::NIL;
            qWarning("PR Error: Cannot find circles");
            error_code.code = ErrorCode::PR_OBJECT_NOT_FOUND;
            return error_code;
        }

        avs::DrawLines_SingleColor( image2, atl::ToArray< atl::Conditional< avl::Line2D > >(line2D1), atl::NIL, avl::Pixel(0.0f, 0.0f, 255.0f, 0.0f), avl::DrawingStyle(avl::DrawingMode::HighQuality, 1.0f, 4.0f, false, atl::NIL, 20.0f), true, image3 );
        avs::DrawPoints_SingleColor( image3, atl::ToArray< atl::Conditional< avl::Point2D > >(point2D3), atl::NIL, avl::Pixel(0.0f, 255.0f, 0.0f, 0.0f), avl::DrawingStyle(avl::DrawingMode::HighQuality, 1.0f, 4.0f, false, avl::PointShape::Cross, 50.0f), true, image4 );
        stringArray1.Resize(1);
        stringArray1[0] = string2;
        avs::DrawStrings_SingleColor( image4, stringArray1, g_constData4, atl::NIL, avl::Anchor2D::MiddleCenter, avl::Pixel(0.0f, 255.0f, 0.0f, 0.0f), avl::DrawingStyle(avl::DrawingMode::HighQuality, 1.0f, 1.0f, false, atl::NIL, 28.0f), 40.0f, 0.0f, true, atl::NIL, image5 );

        avl::SaveImageToJpeg( image5 , imageName.toStdString().c_str(), atl::NIL, false );
    }catch(const atl::Error& error) {
        qWarning("PR Error: %s", error.Message());
        qWarning(error.Message());
        error_code.code = ErrorCode::PR_OBJECT_NOT_FOUND;
        return error_code;
    }
    return error_code;
}

ErrorCodeStruct VisionModule::PR_Generic_NCC_Template_Matching(QString camera_name, QString pr_name, PRResultStruct &prResult, double object_score)
{
    //if(is_debug)return ErrorCodeStruct{ OK, "" };
    if (pr_name.contains("_edgeModel")) {
        return PR_Edge_Template_Matching(camera_name, pr_name, prResult);
    }
    qInfo("%s perform %s with object_score: %f",camera_name.toStdString().c_str(),pr_name.toStdString().c_str(), object_score);
    pr_name.replace("file:///", "");

    QFileInfo fileInfo(pr_name);
    if(!fileInfo.isFile())
    {
        qInfo("pr file name not exist");
        return ErrorCodeStruct{ GENERIC_ERROR, "pr file name not exist" };
    }

    QString pr_offset_name = pr_name;
    QString pr_region_name = pr_name;
    pr_offset_name.replace(".avdata", "_offset.avdata");
    pr_region_name.replace(".avdata", "_searchRegion.avdata");
    ErrorCodeStruct error_code = { OK, "" };
    try {
        atl::String g_constData1;
        atl::String g_constData2;
        atl::String g_constData3;
        atl::String g_constData4;
        atl::String g_constData5;
        atl::String g_constData6;
        atl::String g_emptyString;
        atl::Array< atl::Conditional< avl::Location > > g_constData7;
        atl::String g_constData8;
        atl::String	g_constData9;
        atl::String g_constData10;
        QString imageName;
        imageName.append(getVisionLogDir())
                .append(getCurrentTimeString())
                .append(".jpg");
        QString rawImageName;
        rawImageName.append(getVisionLogDir())
                .append(getCurrentTimeString())
                .append("_raw.jpg");
        g_constData1 = L"test.jpg";
        g_constData2 = pr_offset_name.toStdString().c_str();
        g_constData3 = L"Vector2D";
        g_constData4 = pr_name.toStdString().c_str();
        g_constData5 = L"GrayModel";
        g_constData6 = L"Angle:";
        g_constData8 = pr_region_name.toStdString().c_str();
        g_constData9 = L"Region";
        g_constData10 = L" Object:";
        g_emptyString = L"";

        g_constData7.Reset(1);
        g_constData7[0] = avl::Location(253, 50);
        avl::Image image1;
        avl::Image image2;
        avl::Region region1;
        avl::Vector2D vector2D1;
        avl::GrayModel grayModel1;
        atl::Conditional< avl::Object2D > object2D1;
        atl::Conditional< avl::Point2D > point2D1;
        atl::Conditional< avl::Point2D > point2D2;
        atl::Conditional< atl::String > string1;
        atl::Conditional< avl::Rectangle2D > rectangle2D1;
        atl::String string2;
        atl::String string3;
        atl::Array< atl::Conditional< atl::String > > stringArray1;
        avl::Image image3;
        avl::Image image4;
        avl::Image image5;
        avl::Image image6;
        avl::Image image7;
        atl::Array< atl::Conditional< avl::Region > > regionArray1;
        //avl::LoadImage( "test.jpg", false, image1 );
        this->grabImageFromCamera(camera_name, image1);
        avl::SaveImageToJpeg( image1 , rawImageName.toStdString().c_str(), atl::NIL, false );
        prResult.rawImageName = rawImageName;
        //        bool isSearchRegionFound = true;
        //Testing use
        //avl::RotateImage( image1, 4.0f, avl::RotationSizeMode::Fit, avl::InterpolationMethod::Bilinear, false, image2 );
        avs::LoadObject< avl::Vector2D >( g_constData2, avl::StreamMode::Binary, g_constData3, vector2D1 );
        avs::LoadObject< avl::GrayModel >( g_constData4, avl::StreamMode::Binary, g_constData5, grayModel1 );
        //        try {
        //            avs::LoadObject< avl::Region >( g_constData8, avl::StreamMode::Binary, g_constData9, region1 );
        //        } catch(const atl::Error& error) {
        //            isSearchRegionFound = false;
        //            qInfo("Missing search region file, set this to nil for backward compatible: %s", error.Message());
        //        }
        //        if (isSearchRegionFound) {
        //            avl::LocateSingleObject_NCC( image1, region1, grayModel1, 0, 3, false, 0.5, object2D1, atl::NIL, atl::Dummy< atl::Array< avl::Image > >().Get(), atl::Dummy< atl::Array< avl::Image > >().Get(), atl::Dummy< atl::Conditional< atl::Array< float > > >().Get() );
        //        }
        //        else {
        avl::LocateSingleObject_NCC( image1, atl::NIL, grayModel1, 0, 3, false, 0.5, object2D1, atl::NIL, atl::Dummy< atl::Array< avl::Image > >().Get(), atl::Dummy< atl::Array< avl::Image > >().Get(), atl::Dummy< atl::Conditional< atl::Array< float > > >().Get() );
        //        }
        bool is_object_score_pass = true;
        if (object2D1 != atl::NIL)
        {
            float real1;
            avl::Point2D point2D3;
            float real2;
            float real3;
            float real4;

            point2D1.AssignNonNil();
            point2D2.AssignNonNil();
            string1.AssignNonNil();
            rectangle2D1.AssignNonNil();

            point2D1.Get() = object2D1.Get().Point();
            avl::TranslatePoint( point2D1.Get(), vector2D1, false, point2D2.Get() );
            real1 = object2D1.Get().Angle();
            avl::RealToString( real1, string2 );

            // AvsFilter_ConcatenateStrings is intended for generated code only. In regular programs  String::operator+() or String:Append() member function should be used.
            point2D3 = object2D1.Get().Match().Origin();
            real2 = object2D1.Get().Match().Width();
            real3 = object2D1.Get().Match().Height();
            real4 = object2D1.Get().Score();

            if (real4 < object_score) {
                is_object_score_pass = false;
                qWarning("PR oject score is too low: %f < object_socr: %f", object2D1.Get().Score(), object_score);
                QThread::msleep(500);
                return PR_Generic_NCC_Template_Matching_Retry(camera_name, pr_name,prResult,object_score);
            }
            avl::RealToString( real4, string3 );
            avs::AvsFilter_ConcatenateStrings( g_constData6, string2, g_constData10, string3, g_emptyString, g_emptyString, g_emptyString, g_emptyString, string1.Get() );

            // Function AvsFilter_MakeRectangle is intended for generated code only. Consider use of proper Rectangle2D constructor instead.
            avs::AvsFilter_MakeRectangle( point2D3, real1, real2, real3, rectangle2D1.Get() );
            prResult.ori_x = point2D1.Get().x;
            prResult.ori_y = point2D1.Get().y;
            prResult.x = point2D2.Get().x;
            prResult.y = point2D2.Get().y;
            prResult.theta = real1;
            prResult.width = object2D1.Get().Match().Width();
            prResult.height = object2D1.Get().Match().Height();
            prResult.imageName = imageName;
            prResult.rawImageName = rawImageName;
            qInfo("Object score: %f", object2D1.Get().score);
        }
        else
        {
            point2D1 = atl::NIL;
            point2D2 = atl::NIL;
            string1 = atl::NIL;
            rectangle2D1 = atl::NIL;
            error_code.code = ErrorCode::PR_OBJECT_NOT_FOUND;
            error_code.errorMessage = "PR Object Not Found";
            qWarning("PR Error! Object Not Found");
            QThread::msleep(500);
            return PR_Generic_NCC_Template_Matching_Retry(camera_name, pr_name,prResult,object_score);
        }

        stringArray1.Resize(1);
        stringArray1[0] = string1;
        if (is_object_score_pass)
            avs::DrawStrings_SingleColor( image1, stringArray1, g_constData7, atl::NIL, avl::Anchor2D::MiddleCenter, avl::Pixel(0.0f, 255.0f, 0.0f, 0.0f), avl::DrawingStyle(avl::DrawingMode::HighQuality, 1.0f, 1.0f, false, atl::NIL, 40.0f), 38.0f, 0.0f, true, atl::NIL, image3 );
        else {
            avs::DrawStrings_SingleColor( image1, stringArray1, g_constData7, atl::NIL, avl::Anchor2D::MiddleCenter, avl::Pixel(255.0f, 0.0f, 0.0f, 0.0f), avl::DrawingStyle(avl::DrawingMode::HighQuality, 1.0f, 1.0f, false, atl::NIL, 40.0f), 38.0f, 0.0f, true, atl::NIL, image3 );
        }
        avs::DrawPoints_SingleColor( image3, atl::ToArray< atl::Conditional< avl::Point2D > >(point2D2), atl::NIL, avl::Pixel(255.0f, 115.0f, 251.0f, 0.0f), avl::DrawingStyle(avl::DrawingMode::HighQuality, 1.0f, 4.0f, false, avl::PointShape::Cross, 40.0f), true, image4 );
        avs::DrawPoints_SingleColor( image4, atl::ToArray< atl::Conditional< avl::Point2D > >(point2D1), atl::NIL, avl::Pixel(0.0f, 255.0f, 0.0f, 0.0f), avl::DrawingStyle(avl::DrawingMode::HighQuality, 1.0f, 4.0f, false, avl::PointShape::Cross, 40.0f), true, image5 );
        avs::DrawRectangles_SingleColor( image5, atl::ToArray< atl::Conditional< avl::Rectangle2D > >(rectangle2D1), atl::NIL, avl::Pixel(255.0f, 255.0f, 0.0f, 0.0f), avl::DrawingStyle(avl::DrawingMode::HighQuality, 1.0f, 2.0f, false, atl::NIL, 1.0f), true, image6 );
        regionArray1.Resize(1);
        regionArray1[0].AssignNonNil();
        regionArray1[0].Get() = region1;
        avs::DrawRegions_SingleColor( image6, regionArray1, atl::NIL, avl::Pixel(192.0f, 255.0f, 192.0f, 0.0f), 0.3f, true, image7 );
        avl::SaveImageToJpeg( image7 , imageName.toStdString().c_str(), atl::NIL, false );
        if(!is_object_score_pass) {
            error_code.code = ErrorCode::PR_OBJECT_SCORE_FAIL;
            return error_code;
        }
        //displayPRResult(camera_name, prResult);
    } catch(const atl::Error& error) {
        qWarning("PR Error: %s", error.Message());
        qWarning(error.Message());
        error_code.code = ErrorCode::PR_OBJECT_NOT_FOUND;
        return error_code;
    }
    return error_code;
}

ErrorCodeStruct VisionModule::PR_Generic_NCC_Template_Matching_Retry(QString camera_name, QString pr_name, PRResultStruct &prResult, double object_score)
{
    //if(is_debug)return ErrorCodeStruct{ OK, "" };
    if (pr_name.contains("_edgeModel")) {
        return PR_Edge_Template_Matching(camera_name, pr_name, prResult);
    }
    qInfo("%s perform %s with object_score: %f",camera_name.toStdString().c_str(),pr_name.toStdString().c_str(), object_score);
    pr_name.replace("file:///", "");

    QFileInfo fileInfo(pr_name);
    if(!fileInfo.isFile())
    {
        qWarning("pr file name not exist");
        return ErrorCodeStruct{ GENERIC_ERROR, "pr file name not exist" };
    }

    QString pr_offset_name = pr_name;
    QString pr_region_name = pr_name;
    pr_offset_name.replace(".avdata", "_offset.avdata");
    pr_region_name.replace(".avdata", "_searchRegion.avdata");
    ErrorCodeStruct error_code = { OK, "" };
    try {
        atl::String g_constData1;
        atl::String g_constData2;
        atl::String g_constData3;
        atl::String g_constData4;
        atl::String g_constData5;
        atl::String g_constData6;
        atl::String g_emptyString;
        atl::Array< atl::Conditional< avl::Location > > g_constData7;
        atl::String g_constData8;
        atl::String	g_constData9;
        atl::String g_constData10;
        QString imageName;
        imageName.append(getVisionLogDir())
                .append(getCurrentTimeString())
                .append("_retry.jpg");
        QString rawImageName;
        rawImageName.append(getVisionLogDir())
                .append(getCurrentTimeString())
                .append("_retry_raw.jpg");
        g_constData1 = L"test.jpg";
        g_constData2 = pr_offset_name.toStdString().c_str();
        g_constData3 = L"Vector2D";
        g_constData4 = pr_name.toStdString().c_str();
        g_constData5 = L"GrayModel";
        g_constData6 = L"Angle:";
        g_constData8 = pr_region_name.toStdString().c_str();
        g_constData9 = L"Region";
        g_constData10 = L" Object:";
        g_emptyString = L"";

        g_constData7.Reset(1);
        g_constData7[0] = avl::Location(253, 50);
        avl::Image image1;
        avl::Image image2;
        avl::Region region1;
        avl::Vector2D vector2D1;
        avl::GrayModel grayModel1;
        atl::Conditional< avl::Object2D > object2D1;
        atl::Conditional< avl::Point2D > point2D1;
        atl::Conditional< avl::Point2D > point2D2;
        atl::Conditional< atl::String > string1;
        atl::Conditional< avl::Rectangle2D > rectangle2D1;
        atl::String string2;
        atl::String string3;
        atl::Array< atl::Conditional< atl::String > > stringArray1;
        avl::Image image3;
        avl::Image image4;
        avl::Image image5;
        avl::Image image6;
        avl::Image image7;
        atl::Array< atl::Conditional< avl::Region > > regionArray1;
        //avl::LoadImage( "pr//19-39-23-431_raw.jpg", false, image1 );
        this->grabImageFromCamera(camera_name, image1);
        avl::SaveImageToJpeg( image1 , rawImageName.toStdString().c_str(), atl::NIL, false );
        //        bool isSearchRegionFound = true;
        //Testing use
        //avl::RotateImage( image1, 4.0f, avl::RotationSizeMode::Fit, avl::InterpolationMethod::Bilinear, false, image2 );
        avs::LoadObject< avl::Vector2D >( g_constData2, avl::StreamMode::Binary, g_constData3, vector2D1 );
        avs::LoadObject< avl::GrayModel >( g_constData4, avl::StreamMode::Binary, g_constData5, grayModel1 );
        //        try {
        //            avs::LoadObject< avl::Region >( g_constData8, avl::StreamMode::Binary, g_constData9, region1 );
        //        } catch(const atl::Error& error) {
        //            isSearchRegionFound = false;
        //            qInfo("Missing search region file, set this to nil for backward compatible: %s", error.Message());
        //        }
        //        if (isSearchRegionFound) {
        //            avl::LocateSingleObject_NCC( image1, region1, grayModel1, 0, 3, false, 0.5, object2D1, atl::NIL, atl::Dummy< atl::Array< avl::Image > >().Get(), atl::Dummy< atl::Array< avl::Image > >().Get(), atl::Dummy< atl::Conditional< atl::Array< float > > >().Get() );
        //        }
        //        else {
        avl::LocateSingleObject_NCC( image1, atl::NIL, grayModel1, 0, 3, false, 0.5, object2D1, atl::NIL, atl::Dummy< atl::Array< avl::Image > >().Get(), atl::Dummy< atl::Array< avl::Image > >().Get(), atl::Dummy< atl::Conditional< atl::Array< float > > >().Get() );
        //        }
        bool is_object_score_pass = true;
        if (object2D1 != atl::NIL)
        {
            float real1;
            avl::Point2D point2D3;
            float real2;
            float real3;
            float real4;

            point2D1.AssignNonNil();
            point2D2.AssignNonNil();
            string1.AssignNonNil();
            rectangle2D1.AssignNonNil();

            point2D1.Get() = object2D1.Get().Point();
            avl::TranslatePoint( point2D1.Get(), vector2D1, false, point2D2.Get() );
            real1 = object2D1.Get().Angle();
            avl::RealToString( real1, string2 );

            // AvsFilter_ConcatenateStrings is intended for generated code only. In regular programs  String::operator+() or String:Append() member function should be used.
            point2D3 = object2D1.Get().Match().Origin();
            real2 = object2D1.Get().Match().Width();
            real3 = object2D1.Get().Match().Height();
            real4 = object2D1.Get().Score();

            if (real4 < object_score) {
                is_object_score_pass = false;
                qInfo("PR oject score is too low: %f < object_socr: %f", object2D1.Get().Score(), object_score);
            }
            avl::RealToString( real4, string3 );
            avs::AvsFilter_ConcatenateStrings( g_constData6, string2, g_constData10, string3, g_emptyString, g_emptyString, g_emptyString, g_emptyString, string1.Get() );

            // Function AvsFilter_MakeRectangle is intended for generated code only. Consider use of proper Rectangle2D constructor instead.
            avs::AvsFilter_MakeRectangle( point2D3, real1, real2, real3, rectangle2D1.Get() );
            prResult.ori_x = point2D1.Get().x;
            prResult.ori_y = point2D1.Get().y;
            prResult.x = point2D2.Get().x;
            prResult.y = point2D2.Get().y;
            prResult.theta = real1;
            prResult.width = object2D1.Get().Match().Width();
            prResult.height = object2D1.Get().Match().Height();
            prResult.imageName = imageName;
            prResult.rawImageName = rawImageName;
            qWarning("Object score: %f", object2D1.Get().score);
        }
        else
        {
            point2D1 = atl::NIL;
            point2D2 = atl::NIL;
            string1 = atl::NIL;
            rectangle2D1 = atl::NIL;
            error_code.code = ErrorCode::PR_OBJECT_NOT_FOUND;
            error_code.errorMessage = "PR Object Not Found";
            qWarning("PR Error! Object Not Found");
            return error_code;
        }

        stringArray1.Resize(1);
        stringArray1[0] = string1;
        if (is_object_score_pass)
            avs::DrawStrings_SingleColor( image1, stringArray1, g_constData7, atl::NIL, avl::Anchor2D::MiddleCenter, avl::Pixel(0.0f, 255.0f, 0.0f, 0.0f), avl::DrawingStyle(avl::DrawingMode::HighQuality, 1.0f, 1.0f, false, atl::NIL, 40.0f), 38.0f, 0.0f, true, atl::NIL, image3 );
        else {
            avs::DrawStrings_SingleColor( image1, stringArray1, g_constData7, atl::NIL, avl::Anchor2D::MiddleCenter, avl::Pixel(255.0f, 0.0f, 0.0f, 0.0f), avl::DrawingStyle(avl::DrawingMode::HighQuality, 1.0f, 1.0f, false, atl::NIL, 40.0f), 38.0f, 0.0f, true, atl::NIL, image3 );
        }
        avs::DrawPoints_SingleColor( image3, atl::ToArray< atl::Conditional< avl::Point2D > >(point2D2), atl::NIL, avl::Pixel(255.0f, 115.0f, 251.0f, 0.0f), avl::DrawingStyle(avl::DrawingMode::HighQuality, 1.0f, 4.0f, false, avl::PointShape::Cross, 40.0f), true, image4 );
        avs::DrawPoints_SingleColor( image4, atl::ToArray< atl::Conditional< avl::Point2D > >(point2D1), atl::NIL, avl::Pixel(0.0f, 255.0f, 0.0f, 0.0f), avl::DrawingStyle(avl::DrawingMode::HighQuality, 1.0f, 4.0f, false, avl::PointShape::Cross, 40.0f), true, image5 );
        avs::DrawRectangles_SingleColor( image5, atl::ToArray< atl::Conditional< avl::Rectangle2D > >(rectangle2D1), atl::NIL, avl::Pixel(255.0f, 255.0f, 0.0f, 0.0f), avl::DrawingStyle(avl::DrawingMode::HighQuality, 1.0f, 2.0f, false, atl::NIL, 1.0f), true, image6 );
        regionArray1.Resize(1);
        regionArray1[0].AssignNonNil();
        regionArray1[0].Get() = region1;
        avs::DrawRegions_SingleColor( image6, regionArray1, atl::NIL, avl::Pixel(192.0f, 255.0f, 192.0f, 0.0f), 0.3f, true, image7 );
        avl::SaveImageToJpeg( image7 , imageName.toStdString().c_str(), atl::NIL, false );
        if(!is_object_score_pass) {
            error_code.code = ErrorCode::PR_OBJECT_SCORE_FAIL;
            return error_code;
        }
        //displayPRResult(camera_name, prResult);
    } catch(const atl::Error& error) {
        qWarning("PR Error: %s", error.Message());
        qWarning(error.Message());
        error_code.code = ErrorCode::PR_OBJECT_NOT_FOUND;
        return error_code;
    }
    return error_code;
}

ErrorCodeStruct VisionModule::PR_Edge_Template_Matching(QString camera_name, QString pr_name, PRResultStruct &prResult)
{
    qInfo("%s perform edge templage matching pr %s",camera_name.toStdString().c_str(),pr_name.toStdString().c_str());
    pr_name.replace("file:///", "");
    ErrorCodeStruct error_code = { OK, "" };
    static atl::String g_constData1;
    static atl::String g_constData2;
    static atl::String g_constData3;
    static avl::Region g_constData4;
    static atl::String g_constData5;
    static atl::String g_emptyString;
    static atl::String g_constData6;
    static atl::String g_constData7;
    static atl::Array< atl::Conditional< avl::Location > > g_constData8;
    static atl::Array< atl::Conditional< avl::Location > > g_constData9;
    g_constData1 = L"C:\\Users\\emil\\Desktop\\Test\\calibrationPhotot\\sut_updownlook_up.jpg";

    //g_constData2 = L"C:\\Users\\emil\\Desktop\\Test\\EdgeFinder_New\\config\\prConfig\\spa_up_edgeModel.avdata";
    g_constData2 = pr_name.toStdString().c_str();

    g_constData3 = L"EdgeModel?";

    g_constData5 = L"Angle:";

    g_emptyString = L"";

    g_constData6 = L"W:";

    g_constData7 = L" H:";

    g_constData8.Reset(1);
    g_constData8[0] = avl::Location(125, 19);

    g_constData9.Reset(1);
    g_constData9[0] = avl::Location(178, 55);

    avl::Image image1;
    atl::Conditional< avl::EdgeModel > edgeModel1;
    atl::Conditional< atl::Array< avl::Path > > pathArray1;
    atl::Conditional< avl::Point2D > point2D1;
    atl::Array< avl::Path > pathArray2;
    atl::Conditional< avl::Path > path1;
    atl::Conditional< atl::String > string1;
    atl::Conditional< atl::String > string2;
    atl::String string3;
    atl::String string4;
    atl::String string5;
    avl::Image image2;
    atl::Array< atl::Conditional< atl::String > > stringArray1;
    avl::Image image3;
    atl::Array< atl::Conditional< atl::String > > stringArray2;
    avl::Image image4;
    avl::Image image5;
    atl::Conditional< avl::Rectangle2D > rectangle2D1;

    try {
        QString imageName;
        imageName.append(getVisionLogDir())
                .append(getCurrentTimeString())
                .append(".jpg");
        //avl::LoadImage( g_constData1, false, image1 );
        this->grabImageFromCamera(camera_name, image1);
        avs::LoadObject< atl::Conditional< avl::EdgeModel > >( g_constData2, avl::StreamMode::Binary, g_constData3, edgeModel1 );
        if (edgeModel1 != atl::NIL)
        {
            avl::LocateSingleObject_Edges( image1, atl::NIL, edgeModel1.Get(), 0, 3, 10.0f, false, false, 0.6f, atl::Dummy< atl::Conditional< avl::Object2D > >().Get(), pathArray1, atl::NIL, atl::Dummy< atl::Array< avl::Image > >().Get(), atl::Dummy< atl::Array< avl::Image > >().Get(), atl::Dummy< atl::Conditional< atl::Array< float > > >().Get() );

            if (pathArray1 != atl::NIL)
            {
                avl::JoinAdjacentPaths( pathArray1.Get(), 10.0f, 180.0f, 1.0f, atl::NIL, 100.0f, pathArray2 );
                avl::GetMaximumPath_OrNil( pathArray2, avl::PathFeature::Length, path1, atl::Dummy< atl::Conditional< float > >().Get(), atl::Dummy< atl::Conditional< int > >().Get() );

                if (path1 != atl::NIL)
                {
                    float real1;
                    float real2;
                    float real3;

                    rectangle2D1.AssignNonNil();
                    string1.AssignNonNil();
                    string2.AssignNonNil();
                    point2D1.AssignNonNil();

                    avl::PathBoundingRectangle( path1.Get(), avl::BoundingRectangleFeature::MinimalArea, 90.0f, avl::RectangleOrientation::Horizontal, rectangle2D1.Get(), point2D1.Get(), atl::NIL, atl::NIL );
                    real1 = rectangle2D1.Get().Angle();
                    avl::RealToString( real1, string3 );

                    // AvsFilter_ConcatenateStrings is intended for generated code only. In regular programs  String::operator+() or String:Append() member function should be used.
                    avs::AvsFilter_ConcatenateStrings( g_constData5, string3, g_emptyString, g_emptyString, g_emptyString, g_emptyString, g_emptyString, g_emptyString, string1.Get() );
                    real2 = rectangle2D1.Get().Width();
                    real3 = rectangle2D1.Get().Height();
                    avl::RealToString( real3, string4 );
                    avl::RealToString( real2, string5 );

                    // AvsFilter_ConcatenateStrings is intended for generated code only. In regular programs  String::operator+() or String:Append() member function should be used.
                    avs::AvsFilter_ConcatenateStrings( g_constData6, string5, g_constData7, string4, g_emptyString, g_emptyString, g_emptyString, g_emptyString, string2.Get() );
                }
                else
                {
                    rectangle2D1 = atl::NIL;
                    string1 = atl::NIL;
                    string2 = atl::NIL;
                    error_code.code = ErrorCode::PR_OBJECT_NOT_FOUND;
                    return error_code;
                }
            }
            else
            {
                rectangle2D1 = atl::NIL;
                string1 = atl::NIL;
                string2 = atl::NIL;
                error_code.code = ErrorCode::PR_OBJECT_NOT_FOUND;
                return error_code;
            }
        }
        else
        {
            rectangle2D1 = atl::NIL;
            string1 = atl::NIL;
            string2 = atl::NIL;
            error_code.code = ErrorCode::PR_OBJECT_NOT_FOUND;
            return error_code;
        }
        prResult.x = point2D1.Get().X();
        prResult.y = point2D1.Get().Y();
        prResult.theta = rectangle2D1.Get().Angle();
        prResult.width = rectangle2D1.Get().Width();
        prResult.height = rectangle2D1.Get().Height();
        avs::DrawRectangles_SingleColor( image1, atl::ToArray< atl::Conditional< avl::Rectangle2D > >(rectangle2D1), atl::NIL, avl::Pixel(255.0f, 128.0f, 0.0f, 0.0f), avl::DrawingStyle(avl::DrawingMode::HighQuality, 1.0f, 4.0f, false, atl::NIL, 1.0f), true, image2 );
        stringArray1.Resize(1);
        stringArray1[0] = string1;
        avs::DrawStrings_SingleColor( image2, stringArray1, g_constData8, atl::NIL, avl::Anchor2D::MiddleCenter, avl::Pixel(0.0f, 255.0f, 0.0f, 0.0f), avl::DrawingStyle(avl::DrawingMode::HighQuality, 1.0f, 1.0f, false, atl::NIL, 1.0f), 32.0f, 0.0f, true, atl::NIL, image3 );
        stringArray2.Resize(1);
        stringArray2[0] = string2;
        avs::DrawStrings_SingleColor( image3, stringArray2, g_constData9, atl::NIL, avl::Anchor2D::MiddleCenter, avl::Pixel(0.0f, 255.0f, 0.0f, 0.0f), avl::DrawingStyle(avl::DrawingMode::HighQuality, 1.0f, 1.0f, false, atl::NIL, 1.0f), 32.0f, 0.0f, true, atl::NIL, image4 );
        avs::DrawPoints_SingleColor( image4, atl::ToArray< atl::Conditional< avl::Point2D > >(point2D1), atl::NIL, avl::Pixel(0.0f, 255.0f, 0.0f, 0.0f), avl::DrawingStyle(avl::DrawingMode::HighQuality, 1.0f, 4.0f, false, avl::PointShape::Cross, 40.0f), true, image5 );
        avl::SaveImageToJpeg( image5 , imageName.toStdString().c_str(), atl::NIL, false );
    } catch(const atl::Error& error) {
        error_code.code = ErrorCode::PR_OBJECT_NOT_FOUND;
        qWarning(error.Message());
        return error_code;
    }
    return error_code;
}

void EnableControl( bool inObject )
{
    avl::CopyObject< bool >( inObject, atl::Dummy<bool>().Get() );
}

ErrorCodeStruct VisionModule::Glue_Inspection(double resolution, double minWidth, double maxWidth, double maxAvgWidth,
                                              QString beforeImage, QString afterImage, QString *glueInspectionImageName,
                                              double *outMinGlueWidth, double *outMaxGlueWidth, double *outMaxAvgGlueWidth)
{
    ErrorCodeStruct error_code = { OK, "" };
    atl::String file1;
    atl::String file2;
    atl::String string1;
    atl::String string2;
    atl::String string3;
    avl::Image image1;
    avl::Image image2;
    RegionJudgeState regionJudgeState1;
    atl::Conditional< avl::Region > region1;
    avl::Region region2;
    avl::Region region3;
    atl::Conditional< avl::Region > region4;
    atl::Conditional< avl::Region > region5;
    WidthJudgeState widthJudgeState1;
    avl::Image image3;
    bool bool1;

    QString imageNameBeforeDispense;
    imageNameBeforeDispense.append(getDispensePrLogDir())
            .append(getCurrentTimeString())
            .append("_before_dispense.jpg");
    QString outResultImageName;
    outResultImageName.append(getDispensePrLogDir())
            .append(getCurrentTimeString())
            .append("_glue_inspection_result.jpg");
    QString substractImage;
    substractImage.append(getDispensePrLogDir())
            .append(getCurrentTimeString())
            .append("_substract_result.jpg");

    //file1 = L"C:\\Users\\emil\\Documents\\WeChat Files\\milklai1987\\FileStorage\\File\\2019-11\\2.3\\2.3\\Image\\before\\0.jpg";
    //file2 = L"C:\\Users\\emil\\Documents\\WeChat Files\\milklai1987\\FileStorage\\File\\2019-11\\2.3\\2.3\\Image\\after\\0_d.jpg";
    file1 = beforeImage.toStdString().c_str();
    file2 = afterImage.toStdString().c_str();
    qDebug("Going to do the glueInspection. Before Dispense Image: %s After dispense image: %s", beforeImage.toStdString().c_str(), afterImage.toStdString().c_str());
    try {
        avl::LoadImage( file1, false, image1 );
        avl::LoadImage( file2, false, image2 );
        avl::SaveImageToJpeg( image1 , imageNameBeforeDispense.toStdString().c_str(), atl::NIL, false );
        avl::SubtractImages( image1, image2, atl::NIL, 2.0f, image1 );
        avl::SaveImageToJpeg( image1 , substractImage.toStdString().c_str(), atl::NIL, false );
        RegionJudge( regionJudgeState1, image1, image2, bool1, region1, region2, region3, region4 );

        if (region1 != atl::NIL)
        {
            region5.AssignNonNil();

            avl::OpenRegion( region1.Get(), avl::KernelShape::Box, 1, atl::NIL, region5.Get() );
        }
        else
        {
            region5 = atl::NIL;
        }
        bool outResultOK = true;
        atl::Conditional<float> outMaxWidth = 0; atl::Conditional<float> outMinWidth = 0; atl::Conditional<float> outAveWidth = 0;

        WidthJudge( widthJudgeState1, bool1, region5, resolution, minWidth, maxWidth, image2, maxAvgWidth, outResultOK, image3, outMaxWidth, outMinWidth, outAveWidth );

        if (outMaxWidth != atl::NIL && outMaxWidth.HasValue()) *outMaxGlueWidth = outMaxWidth.Get();
        if (outMinWidth != atl::NIL && outMinWidth.HasValue()) *outMinGlueWidth = outMinWidth.Get();
        if (outAveWidth != atl::NIL && outAveWidth.HasValue()) *outMaxAvgGlueWidth = outAveWidth.Get();
        qDebug("Glue Inspection result: %d outMaxWidth: %f outMinWidth: %f outAvgWidth: %f", outResultOK, *outMaxGlueWidth,  *outMinGlueWidth, *outMaxAvgGlueWidth);
        avl::SaveImageToJpeg( image3, outResultImageName.toStdString().c_str(), atl::NIL, false );
        *glueInspectionImageName = outResultImageName;
        if (outResultOK) {
            return error_code;
        } else {
            error_code.code = ErrorCode::GLUE_INSPECTION_FAIL;
            return error_code;
        }
    }catch(const atl::Error& error) {
        error_code.code = ErrorCode::PR_OBJECT_NOT_FOUND;
        qWarning(error.Message());
        return error_code;
    }
    return error_code;
}

void VisionModule::aaDebugImage(QString input_filename, int threshold, int min_area, int max_area)
{
    qInfo("aaDebugImage is called: %s intensity_threshold: %d min_area: %d max_area: %d", input_filename.toStdString().c_str(), threshold, min_area, max_area);
    static atl::String g_emptyString;
    avl::Image image1;
    atl::String file1;
    atl::String string1;
    avl::Region region1;
    avl::Image image2;
    avl::Image image3;
    atl::Array< avl::Region > regionArray1;
    atl::Array< atl::Conditional< avl::Point2D > > point2DArray1;
    atl::Array< atl::Conditional< atl::String > > stringArray1;
    atl::Array< atl::Conditional< avl::Location > > locationArray1;
    atl::String string2;
    atl::String string3;
    avl::Image image4;
    avl::Image image5;
    avl::Image image6;
    avl::Image image7;
    atl::Array< avl::Image > imageArray1;
    atl::Array< avl::Image > imageArray2;
    avl::Image image8;
    avl::Image image9;
    avl::Image image10;

    int integer1;
    int integer2;
    float real1;
    float real2;
    avl::Line2D line2D1;
    avl::Line2D line2D2;
    try  {
        avl::LoadImage( input_filename.toStdString().c_str(), false, image1 );
        avl::ThresholdToRegion( image1, atl::NIL, 0.0f, threshold, 0.0f, region1 );
        avl::RegionToImage( region1, image2 );
        avl::ConvertToMultichannel( image2, atl::NIL, 3, image3 );
        avl::SplitRegionIntoBlobs( region1, avl::RegionConnectivity::EightDirections, min_area, max_area, true, regionArray1, atl::Dummy< atl::Array< int > >().Get() );
        point2DArray1.Resize(regionArray1.Size());
        stringArray1.Resize(regionArray1.Size());
        locationArray1.Resize(regionArray1.Size());
        for( int i = 0; i < regionArray1.Size(); ++i )
        {
            int integer3;

            avl::RegionMassCenter_OrNil( regionArray1[i], point2DArray1[i] );
            avl::RegionArea( regionArray1[i], integer3 );
            avl::IntegerToString( integer3, string2 );

            // AvsFilter_ConcatenateStrings is intended for generated code only. In regular programs  String::operator+() or String:Append() member function should be used.
            avs::AvsFilter_ConcatenateStrings( string2, g_emptyString, g_emptyString, g_emptyString, g_emptyString, g_emptyString, g_emptyString, g_emptyString, string3 );
            stringArray1[i].AssignNonNil();
            stringArray1[i].Get() = string3;

            if (point2DArray1[i] != atl::NIL)
            {
                locationArray1[i].AssignNonNil();

                avl::Point2DToLocation( point2DArray1[i].Get(), locationArray1[i].Get() );
            }
            else
            {
                locationArray1[i] = atl::NIL;
            }
        }

        avs::DrawPoints_SingleColor( image1, point2DArray1, atl::NIL, avl::Pixel(255.0f, 1.0f, 255.0f, 0.0f), avl::DrawingStyle(avl::DrawingMode::HighQuality, 1.0f, 9.0f, false, avl::PointShape::Cross, 52.0f), true, image4 );
        integer1 = image1.Width();
        integer2 = image1.Height();
        real1 = _avfml_st_real(_avfml_ld_real(static_cast<float>(integer1)) / 2.0);
        real2 = _avfml_st_real(_avfml_ld_real(static_cast<float>(integer2)) / 2.0);

        // Function AvsFilter_MakeLine is intended for generated code only. Consider use of proper Line2D constructor instead.
        avs::AvsFilter_MakeLine( -1.0f, 0.0f, real1, line2D1 );

        // Function AvsFilter_MakeLine is intended for generated code only. Consider use of proper Line2D constructor instead.
        avs::AvsFilter_MakeLine( 0.0f, -1.0f, real2, line2D2 );
        avs::DrawLines_SingleColor( image4, atl::ToArray< atl::Conditional< avl::Line2D > >(line2D1), atl::NIL, avl::Pixel(255.0f, 192.0f, 128.0f, 0.0f), avl::DrawingStyle(avl::DrawingMode::HighQuality, 1.0f, 10.0f, false, atl::NIL, 4.0f), true, image5 );
        avs::DrawLines_SingleColor( image5, atl::ToArray< atl::Conditional< avl::Line2D > >(line2D2), atl::NIL, avl::Pixel(255.0f, 192.0f, 128.0f, 0.0f), avl::DrawingStyle(avl::DrawingMode::HighQuality, 1.0f, 10.0f, false, atl::NIL, 1.0f), true, image6 );
        avs::DrawStrings_SingleColor( image6, stringArray1, locationArray1, atl::NIL, avl::Anchor2D::MiddleCenter, avl::Pixel(255.0f, 128.0f, 0.0f, 0.0f), avl::DrawingStyle(avl::DrawingMode::HighQuality, 1.0f, 1.0f, false, atl::NIL, 100.0f), 150.0f, 0.0f, true, atl::NIL, image7 );

        // AvsFilter_CreateArray is intended for use in generated code only. Consider use of proper constructor or Array::Clear() and Array::Reserve function in hand-written programs.
        avs::AvsFilter_CreateArray< avl::Image >( image1, image3, atl::NIL, atl::NIL, atl::NIL, atl::NIL, atl::NIL, atl::NIL, imageArray1 );
        // AvsFilter_CreateArray is intended for use in generated code only. Consider use of proper constructor or Array::Clear() and Array::Reserve function in hand-written programs.
        avs::AvsFilter_CreateArray< avl::Image >( image6, image7, atl::NIL, atl::NIL, atl::NIL, atl::NIL, atl::NIL, atl::NIL, imageArray2 );
        avl::JoinImages_OfArray( imageArray1, avl::JoinDirection::Horizontal, image8 );
        avl::JoinImages_OfArray( imageArray2, avl::JoinDirection::Horizontal, image9 );
        avl::JoinImages( image8, image9, avl::JoinDirection::Vertical, image10 );
        avl::SaveImageToJpeg(image10 , "AA_debug.jpg", atl::NIL, false);
        QImage image("AA_debug.jpg");
        aaDebugImageProvider->setImage(image);
        emit callQmlRefeshImg(4);
    }catch(const atl::Error& error) {
        qWarning(error.Message());
        return;
    }
    return;
}

void VisionModule::displayPRResult(const QString camera_name, const PRResultStruct prResult)
{
    if (camera_name.contains(DOWNLOOK_VISION_CAMERA)) {
        last_downlook_pr_result = prResult.imageName;
    }
    else if (camera_name.contains(UPLOOK_VISION_CAMERA)) {
        last_uplook_pr_result = prResult.imageName;
    }
    else if (camera_name.contains(PICKARM_VISION_CAMERA)) {
        last_pickarm_pr_result = prResult.imageName;
    }
    emit callQmlRefeshImg(0);
}

QImage VisionModule::requestImage(const QString &id, QSize *size, const QSize &requestedSize)
{
    if (id.contains(DOWNLOOK_VISION_CAMERA)) {
        qInfo(QString("Fetch " + last_downlook_pr_result).toStdString().c_str());
        return QImage(last_downlook_pr_result);
    }
    else if (id.contains(PREVIEW_2)) {
        return QImage(last_uplook_pr_result);
    }
    else if (id.contains(PREVIEW_3)) {
        return QImage(last_pickarm_pr_result);
    }
    return QImage();
}

void VisionModule::receivceModuleMessage(QVariantMap message)
{
    if (message["TargetModule"].toString() == this->Name() && message["Message"].toString() == "GrabImage"){
        qInfo("target module: %s", message["TargetModule"].toString().toStdString().c_str());
        QString cameraName = message["cameraName"].toString();
        QString imageName = message["imageName"].toString();
        avl::Image image1;
        this->grabImageFromCamera(cameraName, image1);
        if (!image1.Empty()) {
            avl::SaveImageToJpeg( image1 , imageName.toStdString().c_str(), atl::NIL, false );
            QJsonObject params;
            params.insert("cameraName", cameraName);
            params.insert("filename", imageName);
            emit sendMessageToModule(message["OriginModule"].toString(), "Response", params);
        }
    }
}

void VisionModule::saveImage(int channel)
{
    avl::Image image1; bool ret;
    if (channel == HandleCameraChannel::CAMERA_CHANNEL_UPLOOK_VISION_CAMERA)
        ret = this->grabImageFromCamera(UPLOOK_VISION_CAMERA, image1);
    else if (channel == HandleCameraChannel::CAMERA_CHANNEL_DOWNLOOK_VISION_CAMERA)
        ret = this->grabImageFromCamera(DOWNLOOK_VISION_CAMERA, image1);
    else if (channel == HandleCameraChannel::CAMERA_CHANNEL_PICKARM_VISION_CAMERA)
        ret = this->grabImageFromCamera(PICKARM_VISION_CAMERA, image1);
    else if (channel == HandleCameraChannel::CAMERA_CHANNEL_CAMERA_AA2_DL)
        ret = this->grabImageFromCamera(CAMERA_AA2_DL, image1);
    else if (channel == HandleCameraChannel::CAMERA_CHANNEL_CAMERA_SPA_DL)
        ret = this->grabImageFromCamera(CAMERA_SPA_DL, image1);
    else if (channel == HandleCameraChannel::CAMERA_CHANNEL_CAMERA_LPA_UL)
        ret = this->grabImageFromCamera(CAMERA_LPA_UL, image1);
    else if (channel == HandleCameraChannel::CAMERA_CHANNEL_CAMERA_LPA_BARCODE)
        ret = this->grabImageFromCamera(CAMERA_LPA_BARCODE, image1);
    else return;
    if (!ret) {
        qWarning("Cannot save image due to camera is not running");
        return;
    }
    QString imageName;
    imageName.append(getVisionLogDir())
            .append(getCurrentTimeString())
            .append(".jpg");
    if (!image1.Empty())
    {
        avl::SaveImageToJpeg( image1 , imageName.toStdString().c_str(), atl::NIL, false );
        QImage image(imageName);
        this->visionModuleImageProviders[channel].setImage(image);
        SI::ui.showMessage("VisionModule", QString("Save Image Success! ").append(imageName).append(" is saved."), MsgBoxIcon::Information, "OK");
        emit callQmlRefeshImg(5+channel);
    }else {
        SI::ui.showMessage("VisionModule", QString("Save Image Fail!"), MsgBoxIcon::Information, "OK");
    }
}

QMap<QString, PropertyBase *> VisionModule::getModuleParameter()
{
    QMap<QString, PropertyBase *> map;
    return map;
}

void VisionModule::setModuleParameter(QMap<QString, PropertyBase *> params)
{

}
