#include "Vision/visionmodule.h"

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
#include "Utils/config.h"
#include "Utils/commonutils.h"
#include "Vision/baslerpyloncamera.h"
#include "Vision/hikcamera.h"

VisionModule:: VisionModule(BaslerPylonCamera *downlookCamera, BaslerPylonCamera * uplookCamera, BaslerPylonCamera* pickarmCamera, HIKCamera * hikCamera)
    :QQuickImageProvider(QQuickImageProvider::Image)
{
    this->downlookCamera = downlookCamera;
    this->uplookCamera = uplookCamera;
    this->pickarmCamera = pickarmCamera;
    this->hikCamera = hikCamera;
    aaDebugImageProvider = new ImageProvider();
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
            qInfo(" %d : x: %f y: %f", j, x, y);
            results.push_back(QPoint(x, y));
        }
    }
    return results;
}

bool VisionModule::grabImageFromCamera(QString cameraName, avl::Image &image)
{
    BaslerPylonCamera *camera = Q_NULLPTR;
    if (cameraName.contains(CAMERA_SH_AA_DL)) {camera = downlookCamera;}
    else if (cameraName.contains(CAMERA_SH_UT_UL)) {camera = uplookCamera;}
    else if (cameraName.contains(CAMERA_SH_PA_DL)) {camera = pickarmCamera;}
    else if (cameraName.contains(CAMERA_SH_HIK_CAM)) {
        if (hikCamera != Q_NULLPTR) {
            QPixmap p = QPixmap::fromImage(hikCamera->getImage());
            QImage q2 = p.toImage();
            q2 = q2.convertToFormat(QImage::Format_RGB888);
            avl::Image image2(q2.width(), q2.height(), q2.bytesPerLine(), avl::PlainType::Type::UInt8, q2.depth() / 8, q2.bits());
            image = image2;
            return true;
        } else {
            qWarning("HikCamera is null pointer");
            return false;
        }
    }
    if (camera == Q_NULLPTR) {
        qInfo("Cannot find camera %s", cameraName.toStdString().c_str());
        return false;
    }
    if (!camera->isCameraGrabbing())
    {
        qInfo("camera grabbing fail %s", cameraName.toStdString().c_str());
        return false;
    }
    //QPixmap p = QPixmap::fromImage(camera->getImage());
    QPixmap p = QPixmap::fromImage(camera->getNewImage());
    QImage q2 = p.toImage();
    q2 = q2.convertToFormat(QImage::Format_RGB888);
    avl::Image image2(q2.width(), q2.height(), q2.bytesPerLine(), avl::PlainType::Type::UInt8, q2.depth() / 8, q2.bits());
    image = image2;
    return true;
}


void VisionModule::saveImage(int channel, QString filename)
{
    avl::Image image1; bool ret;
    if (channel == 0)
        ret = this->grabImageFromCamera(CAMERA_SH_UT_UL, image1);
    else if (channel == 1)
        ret = this->grabImageFromCamera(CAMERA_SH_AA_DL, image1);
    else if (channel == 2)
        ret = this->grabImageFromCamera(CAMERA_SH_PA_DL, image1);
    else if (channel == 3)
        ret = this->grabImageFromCamera(CAMERA_SH_HIK_CAM, image1);
    else return;
    if (!ret) {
        qInfo("Cannot save image due to camera is not running");
        return;
    }
    if (!image1.Empty())
        avl::SaveImageToJpeg( image1 , filename.toStdString().c_str(), atl::NIL, false );
}

void VisionModule::reconnect(int channel)
{
    if (channel == 0) {
        this->uplookCamera->close();
        if(this->uplookCamera->Init())
        {
            qInfo("uplookCamera init ok");
            this->uplookCamera->start();
        }
    }
    else if (channel == 1) {
        this->downlookCamera->close();
        if (this->downlookCamera->Init())
        {
            qInfo("downlookCamera init ok");
            this->downlookCamera->start();
        }
    }
    else if (channel == 2)
    {
        this->pickarmCamera->close();
        if (this->pickarmCamera->Init())
        {
            qInfo("pickarmCamera init ok");
            this->pickarmCamera->start();
        }
    }
    else if (channel == 3)
    {
        hikCamera->SCCameraStop();
        //hikCamera->SCCameraInit("192.168.1.8");
        hikCamera->SCCameraInit("192.168.0.15");
    }
}

void VisionModule::saveImage(int channel)
{
    avl::Image image1; bool ret;
    if (channel == 0)
        ret = this->grabImageFromCamera(CAMERA_SH_UT_UL, image1);
    else if (channel == 1)
        ret = this->grabImageFromCamera(CAMERA_SH_AA_DL, image1);
    else if (channel == 2)
        ret = this->grabImageFromCamera(CAMERA_SH_PA_DL, image1);
    else if (channel == 3)
        ret = this->grabImageFromCamera(CAMERA_SH_HIK_CAM, image1);
    else return;
    if (!ret) {
        qInfo("Cannot save image due to camera is not running");
        return;
    }
    QString imageName;
    imageName.append(getVisionLogDir())
            .append(getCurrentTimeString())
            .append(".jpg");
    if (!image1.Empty())
        avl::SaveImageToJpeg( image1 , imageName.toStdString().c_str(), atl::NIL, false );
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

ErrorCodeStruct VisionModule::PR_Generic_NCC_Template_Matching(QString camera_name, QString pr_name, PRResultStruct &prResult)
{
    //if(is_debug)return ErrorCodeStruct{ OK, "" };
    if (pr_name.contains("_edgeModel")) {      
        return PR_Edge_Fitting(camera_name, pr_name, prResult, 0.5, false);
//        return PR_Edge_Template_Matching(camera_name, pr_name, prResult);
    }
    qInfo("%s perform %s",camera_name.toStdString().c_str(),pr_name.toStdString().c_str());
    pr_name.replace("file:///", "");
    QString pr_offset_name = pr_name;
    pr_offset_name.replace(".avdata", "_offset.avdata");
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
        QString imageName;
        imageName.append(getVisionLogDir())
                .append(getCurrentTimeString())
                .append(".jpg");
        QString rawImageName;
        rawImageName.append(getVisionLogDir())
                .append(getCurrentTimeString())
                .append("_raw.jpg");
        g_constData1 = L"C:\\Users\\emil\\Documents\\Projects\\HighSparrow_pure.git\\branches\\build-HighSprrowQ-Desktop_Qt_5_13_0_MSVC2017_64bit-Release\\config\\prConfig\\sensor_tray_sensor_pr.jpg";
        g_constData2 = pr_offset_name.toStdString().c_str();
        g_constData3 = L"Vector2D";
        g_constData4 = pr_name.toStdString().c_str();
        g_constData5 = L"GrayModel";
        g_constData6 = L"Angle:";
        g_emptyString = L"";

        g_constData7.Reset(1);
        g_constData7[0] = avl::Location(152, 50);
        avl::Image image1;
        avl::Image image2;
        avl::Vector2D vector2D1;
        avl::GrayModel grayModel1;
        atl::Conditional< avl::Object2D > object2D1;
        atl::Conditional< avl::Point2D > point2D1;
        atl::Conditional< avl::Point2D > point2D2;
        atl::Conditional< atl::String > string1;
        atl::Conditional< avl::Rectangle2D > rectangle2D1;
        atl::String string2;
        atl::Array< atl::Conditional< atl::String > > stringArray1;
        avl::Image image3;
        avl::Image image4;
        avl::Image image5;
        avl::Image image6;
        //avl::LoadImage( g_constData1, false, image1 );
        this->grabImageFromCamera(camera_name, image1);
        avl::SaveImageToJpeg( image1 , rawImageName.toStdString().c_str(), atl::NIL, false );

        //Testing use
        //avl::RotateImage( image1, 4.0f, avl::RotationSizeMode::Fit, avl::InterpolationMethod::Bilinear, false, image2 );
        avs::LoadObject< avl::Vector2D >( g_constData2, avl::StreamMode::Binary, g_constData3, vector2D1 );
        avs::LoadObject< avl::GrayModel >( g_constData4, avl::StreamMode::Binary, g_constData5, grayModel1 );

        //
        avl::LocateSingleObject_NCC( image1, atl::NIL, grayModel1, 0, 3, false, 0.5f, object2D1, atl::NIL, atl::Dummy< atl::Array< avl::Image > >().Get(), atl::Dummy< atl::Array< avl::Image > >().Get(), atl::Dummy< atl::Conditional< atl::Array< float > > >().Get() );

        if (object2D1 != atl::NIL)
        {
            float real1;
            avl::Point2D point2D3;
            float real2;
            float real3;
            float real4; // PR Score
            point2D1.AssignNonNil();
            point2D2.AssignNonNil();
            string1.AssignNonNil();
            rectangle2D1.AssignNonNil();

            point2D1.Get() = object2D1.Get().Point();
            avl::TranslatePoint( point2D1.Get(), vector2D1, false, point2D2.Get() );
            real1 = object2D1.Get().Angle();
            avl::RealToString( real1, string2 );

            // AvsFilter_ConcatenateStrings is intended for generated code only. In regular programs  String::operator+() or String:Append() member function should be used.
            avs::AvsFilter_ConcatenateStrings( g_constData6, string2, g_emptyString, g_emptyString, g_emptyString, g_emptyString, g_emptyString, g_emptyString, string1.Get() );
            point2D3 = object2D1.Get().Match().Origin();
            real2 = object2D1.Get().Match().Width();
            real3 = object2D1.Get().Match().Height();
            real4 = object2D1.Get().Score();

            qInfo("PR Score for pr_name: %s = %f", pr_name.toStdString().c_str(), real4);
            //ToDo: Add the score limit in json instead of hardcode.
            if (real4 < 0.53 ) {
                error_code.code = ErrorCode::PR_OBJECT_NOT_FOUND;
                error_code.errorMessage = "PR Object Not Found. Score < 0.53";
                qInfo("PR Error! Object Not Found. PR Score < 0.53");
                return error_code;
            }
            // Function AvsFilter_MakeRectangle is intended for generated code only. Consider use of proper Rectangle2D constructor instead.
            avs::AvsFilter_MakeRectangle( point2D3, real1, real2, real3, rectangle2D1.Get() );
            prResult.roi_x = point2D1.Get().x;
            prResult.roi_y = point2D1.Get().y;
            prResult.x = point2D2.Get().x;
            prResult.y = point2D2.Get().y;
            prResult.theta = real1;
            prResult.width = object2D1.Get().Match().Width();
            prResult.height = object2D1.Get().Match().Height();
            prResult.imageName = imageName;
        }
        else
        {
            point2D1 = atl::NIL;
            point2D2 = atl::NIL;
            string1 = atl::NIL;
            rectangle2D1 = atl::NIL;
            error_code.code = ErrorCode::PR_OBJECT_NOT_FOUND;
            error_code.errorMessage = "PR Object Not Found";
            qInfo("PR Error! Object Not Found");
            return error_code;
        }

        stringArray1.Resize(1);
        stringArray1[0] = string1;
        avs::DrawStrings_SingleColor( image1, stringArray1, g_constData7, atl::NIL, avl::Anchor2D::MiddleCenter, avl::Pixel(0.0f, 255.0f, 0.0f, 0.0f), avl::DrawingStyle(avl::DrawingMode::HighQuality, 1.0f, 1.0f, false, atl::NIL, 40.0f), 38.0f, 0.0f, true, atl::NIL, image3 );
        avs::DrawPoints_SingleColor( image3, atl::ToArray< atl::Conditional< avl::Point2D > >(point2D2), atl::NIL, avl::Pixel(255.0f, 115.0f, 251.0f, 0.0f), avl::DrawingStyle(avl::DrawingMode::HighQuality, 1.0f, 4.0f, false, avl::PointShape::Cross, 40.0f), true, image4 );
        avs::DrawPoints_SingleColor( image4, atl::ToArray< atl::Conditional< avl::Point2D > >(point2D1), atl::NIL, avl::Pixel(0.0f, 255.0f, 0.0f, 0.0f), avl::DrawingStyle(avl::DrawingMode::HighQuality, 1.0f, 4.0f, false, avl::PointShape::Cross, 40.0f), true, image5 );
        avs::DrawRectangles_SingleColor( image5, atl::ToArray< atl::Conditional< avl::Rectangle2D > >(rectangle2D1), atl::NIL, avl::Pixel(255.0f, 255.0f, 0.0f, 0.0f), avl::DrawingStyle(avl::DrawingMode::HighQuality, 1.0f, 2.0f, false, atl::NIL, 1.0f), true, image6 );
        avl::SaveImageToJpeg( image6 , imageName.toStdString().c_str(), atl::NIL, false );
        //displayPRResult(camera_name, prResult);
    } catch(const atl::Error& error) {
        qInfo("PR Error: %s", error.Message());
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
        prResult.roi_x = point2D1.Get().x;
        prResult.roi_y = point2D1.Get().y;
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

QImage VisionModule::requestImage(const QString &id, QSize *size, const QSize &requestedSize)
{
    if (id.contains(CAMERA_SH_AA_DL)) {
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

//Huawei TOF use
void VisionModule::Y1Line(float inX, int inY, atl::Array<avl::Point2D> &outArray)
{
    avl::Point2D point2D1;
    avl::Point2D point2D2;

    point2D1 = avl::Point2D(inX, 0.0f);
    point2D2 = avl::Point2D(inX, static_cast<float>(inY));

    // AvsFilter_CreateArray is intended for use in generated code only. Consider use of proper constructor or Array::Clear() and Array::Reserve function in hand-written programs.
    avs::AvsFilter_CreateArray< avl::Point2D >( point2D1, point2D2, atl::NIL, atl::NIL, atl::NIL, atl::NIL, atl::NIL, atl::NIL, outArray );
}

void VisionModule::CenterXLine( float inY, int inX, atl::Array< avl::Point2D >& outArray )
{
    avl::Point2D point2D1;
    avl::Point2D point2D2;

    point2D1 = avl::Point2D(0.0f, inY);
    point2D2 = avl::Point2D(static_cast<float>(inX), inY);

    // AvsFilter_CreateArray is intended for use in generated code only. Consider use of proper constructor or Array::Clear() and Array::Reserve function in hand-written programs.
    avs::AvsFilter_CreateArray< avl::Point2D >( point2D1, point2D2, atl::NIL, atl::NIL, atl::NIL, atl::NIL, atl::NIL, atl::NIL, outArray );
}

void VisionModule::CenterYLine( float inX, int inY, atl::Array< avl::Point2D >& outArray )
{
    avl::Point2D point2D1;
    avl::Point2D point2D2;

    point2D1 = avl::Point2D(inX, 0.0f);
    point2D2 = avl::Point2D(inX, static_cast<float>(inY));

    // AvsFilter_CreateArray is intended for use in generated code only. Consider use of proper constructor or Array::Clear() and Array::Reserve function in hand-written programs.
    avs::AvsFilter_CreateArray< avl::Point2D >( point2D1, point2D2, atl::NIL, atl::NIL, atl::NIL, atl::NIL, atl::NIL, atl::NIL, outArray );
}

void VisionModule::ProfileCalc( ProfileCalcState& state, const avl::Image& inImage, const avl::Path& inScanPath, float& outCenterY1, float& outValue1, float& outValue2, float& outHalfHeightWidth, avl::Profile& outProfile, double intensity_percentage )
{
    float real1;
    float real2;

    avl::ImageProfileAlongPath( state.imageProfileAlongPathState1, inImage, inScanPath, atl::NIL, 5, avl::InterpolationMethod::Bilinear, 0.6f, state.profile1, state.path1, atl::NIL, atl::Dummy< atl::Array< avl::Path > >().Get() );
    avl::SmoothProfile_Mean( state.profile1, 3, false, outProfile );
    avl::ProfileMaximum( outProfile, avl::ProfileInterpolationMethod::Quadratic4, atl::Dummy<float>().Get(), atl::NIL, real1 );
    real2 = _avfml_st_real(_avfml_ld_real(real1) * intensity_percentage);
    avl::SubtractFromProfile( outProfile, real2, state.profile2 );
    avl::SmoothProfile_Mean( state.profile2, 30, false, state.profile3 );
    avl::ProfileZeroCrossings( state.profile3, state.realArray1 );

    // AvsFilter_GetArrayElement is intended for use in generated programs. One should use indexing operator[] to access array' elements.
    avs::AvsFilter_GetArrayElement< float >( state.realArray1, 0, false, outValue1 );

    // AvsFilter_GetArrayElement is intended for use in generated programs. One should use indexing operator[] to access array' elements.
    avs::AvsFilter_GetArrayElement< float >( state.realArray1, 1, false, outValue2 );
    outHalfHeightWidth = _avfml_st_real(_avfml_ld_real(outValue2) - _avfml_ld_real(outValue1));
    outCenterY1 = _avfml_st_real(_avfml_ld_real(outValue1) + _avfml_ld_real(outHalfHeightWidth) / 2.0);
}

TOFResult VisionModule::imageProcessing1(QString filename, double y1, double y2, double y4, double y5, double intensity_percentage, double alpha, bool grabFromCamera)
{
    TOFResult tofResult;
    static atl::String g_constData1;
    //g_constData1 = L"C:\\Users\\emil\\Desktop\\huawei\\2.jpg";
    g_constData1 = filename.toStdString().c_str();
    avl::Image image1;
    avl::Region region1;
    atl::Array< avl::Region > regionArray1;
    avl::Region region2;
    avl::Point2D point2D1;
    int integer1;
    int integer2;
    float real1;
    float real2;
    float real3;
    float real4;
    atl::Array< avl::Point2D > point2DArray1;
    atl::Array< avl::Point2D > point2DArray2;
    atl::Array< avl::Point2D > point2DArray3;
    atl::Array< avl::Point2D > point2DArray4;
    float real5;
    atl::Array< avl::Point2D > point2DArray5;
    float real6;
    atl::Array< avl::Point2D > point2DArray6;
    avl::Path path1;
    avl::Path path2;
    avl::Path path3;
    avl::Path path4;
    avl::Path path5;
    avl::Path path6;
    ProfileCalcState profileCalcState1;
    float real7;
    avl::Profile profile1;
    ProfileCalcState profileCalcState2;
    float real8;
    avl::Profile profile2;
    ProfileCalcState profileCalcState3;
    float real9;
    avl::Profile profile3;
    ProfileCalcState profileCalcState4;
    float real10;
    avl::Profile profile4;
    ProfileCalcState profileCalcState5;
    float real11;
    avl::Profile profile5;
    ProfileCalcState profileCalcState6;
    float real12;
    avl::Profile profile6;
    avl::Point2D point2D2;
    avl::Point2D point2D3;
    avl::Point2D point2D4;
    avl::Point2D point2D5;
    avl::Point2D point2D6;
    avl::Point2D point2D7;
    atl::Array< avl::Point2D > point2DArray7;
    atl::Array< atl::Conditional< avl::Point2D > > point2DArray8;
    avl::Image image2;
    avl::SpatialMap LinkParameter_1;
    bool enableRemap = true;
    try {
        if (enableRemap) {
            avl::Image inputImage, averageImage, imageRemap, image3;
            if (!grabFromCamera)
                avl::LoadImage( g_constData1, false, inputImage );
            else {
                grabImageFromCamera(CAMERA_SH_HIK_CAM, inputImage);
                QString imageName;
                imageName.append(getVisionLogDir())
                        .append(getCurrentTimeString())
                        .append("_raw.jpg");
                qInfo("ImageName: %s", imageName.toStdString().c_str());
                if (!inputImage.Empty())
                    avl::SaveImageToJpeg( inputImage , imageName.toStdString().c_str(), atl::NIL, false );
            }
            avs::ReadDataFromFile( L"409.a361848a.SpatialMap.avdata", L"SpatialMap", LinkParameter_1 );
            avl::RemapImage(inputImage, LinkParameter_1, atl::NIL, imageRemap);
            avl::Matrix matrix1;
            avl::AverageChannels( imageRemap, atl::NIL, averageImage);
            avl::ImageToMatrix( averageImage, matrix1);

            for (int i = 0; i < averageImage.Height(); i++) {
                for (int j = 0; j < averageImage.Width(); j++) {
                    float i_ori = 0, i_new = 0;
                    float x = sqrt( pow(j-1296, 2) + pow(i-1024, 2) );
                    float y = 0.00000000000000086806*pow(x,5) - 0.00000000000270049926*pow(x,4) +
                              0.00000000275257614039*pow(x,3) - 0.00000124593360004144*pow(x,2) +
                              0.00012844171616510600*x + 0.99393844417034000000;
                    avl::GetMatrixElement( matrix1, i, j, i_ori);
                    i_new = i_ori/y;
                    avl::SetMatrixElement( matrix1, i, j, i_new);
                }
            }
            avl::MatrixToImage( matrix1, image3 );
            avl::ConvertPixelType( image3, atl::NIL, avl::PlainType::UInt8, 0, image1 );
            {
                QString imageName;
                imageName.append(getVisionLogDir())
                        .append(getCurrentTimeString())
                        .append("_remapped.jpg");
                if (!image1.Empty())
                    avl::SaveImageToJpeg( image1 , imageName.toStdString().c_str(), atl::NIL, false );
            }
        } else {
            avl::LoadImage( g_constData1, false, image1 );
        }
        avl::ThresholdToRegion( image1, atl::NIL, 30.0f, 255.0f, 0.0f, region1 );
        avl::SplitRegionIntoBlobs( region1, avl::RegionConnectivity::EightDirections, 1, atl::NIL, false, regionArray1, atl::Dummy< atl::Array< int > >().Get() );
        avl::GetMaximumRegion( regionArray1, avl::RegionFeature::Area, region2, atl::NIL, atl::NIL );
        avl::RegionMassCenter( region2, point2D1 );
        integer1 = image1.Width();
        integer2 = image1.Height();
        real1 = _avfml_st_real(static_cast<double>(integer1) * y1);
        real2 = _avfml_st_real(static_cast<double>(integer1) * y2);
        real3 = _avfml_st_real(static_cast<double>(integer1) * y4);
        real4 = _avfml_st_real(static_cast<double>(integer1) * y5);

        // Y1Line
        Y1Line( real1, integer2, point2DArray1 );

        // Y2Line
        Y1Line( real2, integer2, point2DArray2 );

        // Y3Line
        Y1Line( real3, integer2, point2DArray3 );

        // Y4Line
        Y1Line( real4, integer2, point2DArray4 );
        real5 = point2D1.Y();
        CenterXLine( real5, integer1, point2DArray5 );
        real6 = point2D1.X();
        CenterYLine( real6, integer2, point2DArray6 );

        // Center X Line Scan Path
        // Function AvsFilter_MakePath is intended for generated code only. Consider use of proper Path constructor instead.
        avs::AvsFilter_MakePath( point2DArray5, false, path1 );

        // Center Y Line Scan Path
        // Function AvsFilter_MakePath is intended for generated code only. Consider use of proper Path constructor instead.
        avs::AvsFilter_MakePath( point2DArray6, false, path2 );

        // Y1 Line Scan Path
        // Function AvsFilter_MakePath is intended for generated code only. Consider use of proper Path constructor instead.
        avs::AvsFilter_MakePath( point2DArray1, false, path3 );

        // Y2 Line Scan Path
        // Function AvsFilter_MakePath is intended for generated code only. Consider use of proper Path constructor instead.
        avs::AvsFilter_MakePath( point2DArray2, false, path4 );

        // Y3 Line Scan Path
        // Function AvsFilter_MakePath is intended for generated code only. Consider use of proper Path constructor instead.
        avs::AvsFilter_MakePath( point2DArray3, false, path5 );

        // Y4 Line Scan Path
        // Function AvsFilter_MakePath is intended for generated code only. Consider use of proper Path constructor instead.
        avs::AvsFilter_MakePath( point2DArray4, false, path6 );

        // Center X Profile Calculation
        float b;
        ProfileCalc( profileCalcState1, image1, path1, real7, atl::Dummy<float>().Get(), atl::Dummy<float>().Get(), b, profile1, intensity_percentage );

        // Center Y3 Profile Calulation
        float j;
        ProfileCalc( profileCalcState2, image1, path2, real8, atl::Dummy<float>().Get(), atl::Dummy<float>().Get(), j, profile2, intensity_percentage );

        // Center Y1 Profile Calulation
        float h;
        ProfileCalc( profileCalcState3, image1, path3, real9, atl::Dummy<float>().Get(), atl::Dummy<float>().Get(), h, profile3, intensity_percentage );

        // Center Y2 Profile Calulation
        float i;
        ProfileCalc( profileCalcState4, image1, path4, real10, atl::Dummy<float>().Get(), atl::Dummy<float>().Get(), i, profile4 );

        // Center Y4 Profile Calulation
        float k;
        ProfileCalc( profileCalcState5, image1, path5, real11, atl::Dummy<float>().Get(), atl::Dummy<float>().Get(), k, profile5 );

        // Center Y5 Profile Calulation
        float l;
        ProfileCalc( profileCalcState6, image1, path6, real12, atl::Dummy<float>().Get(), atl::Dummy<float>().Get(), l, profile6 );

        point2D2 = avl::Point2D(real7, real5);  //X
        point2D3 = avl::Point2D(real6, real8);  //Y3
        point2D4 = avl::Point2D(real1, real9);  //Y1
        point2D5 = avl::Point2D(real2, real10); //Y2
        point2D6 = avl::Point2D(real3, real11); //Y4
        point2D7 = avl::Point2D(real4, real12); //Y5

        tofResult.a = variableFormat(real7).toDouble();   //X Center
        tofResult.b = variableFormat(b).toDouble();       //X Half width

        tofResult.c = variableFormat(real9).toDouble();  //Y1 center
        tofResult.d = variableFormat(real10).toDouble();  //Y2 center
        tofResult.e = variableFormat(real8).toDouble();  //Y3 center
        tofResult.f = variableFormat(real11).toDouble();  //Y4 center
        tofResult.g = variableFormat(real12).toDouble();  //Y5 center

        tofResult.h = variableFormat(h).toDouble(); //Y1 Half width
        tofResult.i = variableFormat(i).toDouble(); //Y2 Half width
        tofResult.j = variableFormat(j).toDouble(); //Y3 Half width
        tofResult.k = variableFormat(k).toDouble(); //Y4 Half width
        tofResult.l = variableFormat(l).toDouble(); //Y5 Half width

        // AvsFilter_CreateArray is intended for use in generated code only. Consider use of proper constructor or Array::Clear() and Array::Reserve function in hand-written programs.
        avs::AvsFilter_CreateArray< avl::Point2D >( point2D2, point2D3, point2D4, point2D5, point2D6, point2D7, atl::NIL, atl::NIL, point2DArray7 );

        point2DArray8.Resize(point2DArray7.Size());

        for( int i = 0; i < point2DArray7.Size(); ++i )
        {
            point2DArray8[i].AssignNonNil();
            point2DArray8[i].Get() = point2DArray7[i];
        }

        avs::DrawPoints_Palette( image1, point2DArray8, atl::NIL, atl::NIL, avl::DrawingStyle(avl::DrawingMode::HighQuality, 1.0f, 5.0f, false, avl::PointShape::Cross, 200.0f), true, image2 );

        QString imageName;
        imageName.append(getVisionLogDir())
                .append(getCurrentTimeString())
                .append(".jpg");
        if (!image2.Empty())
            avl::SaveImageToJpeg( image2 , imageName.toStdString().c_str(), atl::NIL, false );
        tofResult.imageName = imageName;
        tofResult.ret = true;

    } catch(const atl::Error& error) {
        tofResult.ret = false;
    }

    return tofResult;
}

void VisionModule::StepMacro_1( StepMacro_1State& state, float inX, int inY, float inX2, float inX3, const avl::Image& inImage, float& outCenterY1, float& outValueY1_1, float& outValueY1_2, float& outHalfHeightWidth, float& outCenterY2, float& outValueY2_1, float& outValueY2_2, float& outCenterY3, float& outValueY3_1, float& outValueY3_2, float& outHalfHeightWidth2, float& outHalfHeightWidth3, avl::Profile& outProfile, avl::Profile& outProfile1, avl::Profile& outProfile2, double intensity_profile)
{
    // Y1Line
    Y1Line( inX, inY, state.point2DArray1 );

    // Y2Line
    Y1Line( inX2, inY, state.point2DArray2 );

    // Y3Line
    Y1Line( inX3, inY, state.point2DArray3 );

    // Y1 Line Scan Path
    // Function AvsFilter_MakePath is intended for generated code only. Consider use of proper Path constructor instead.
    avs::AvsFilter_MakePath( state.point2DArray1, false, state.path1 );

    // Y2 Line Scan Path
    // Function AvsFilter_MakePath is intended for generated code only. Consider use of proper Path constructor instead.
    avs::AvsFilter_MakePath( state.point2DArray2, false, state.path2 );

    // Y3 Line Scan Path
    // Function AvsFilter_MakePath is intended for generated code only. Consider use of proper Path constructor instead.
    avs::AvsFilter_MakePath( state.point2DArray3, false, state.path3 );

    // Y1 Profile Calculation
    ProfileCalc( state.profileCalcState1, inImage, state.path1, outCenterY1, outValueY1_1, outValueY1_2, outHalfHeightWidth, outProfile, intensity_profile );

    // Y1 Profile Calculation
    ProfileCalc( state.profileCalcState2, inImage, state.path2, outCenterY2, outValueY2_1, outValueY2_2, outHalfHeightWidth2, outProfile1, intensity_profile );

    // Y1 Profile Calculation
    ProfileCalc( state.profileCalcState3, inImage, state.path3, outCenterY3, outValueY3_1, outValueY3_2, outHalfHeightWidth3, outProfile2, intensity_profile );
}

TOFResult VisionModule::imageProcessing2(QString filename, double y1, double y2, double y3, double intensity_percentage, double alpha, bool grabFromCamera)
{
    TOFResult tofResult;
    atl::String g_constData1;
    //g_constData1 = L"C:\\Users\\emil\\Desktop\\huawei\\x_5-2.JPG";
    g_constData1 = filename.toStdString().c_str();
    try {
        avl::Image image1;
        int integer1;
        int integer2;
        float real1;
        float real2;
        avl::Box box1;
        avl::Box box2;
        avl::Image image2;
        avl::Image image3;
        float real3;
        float real4;
        float real5;
        int integer3;
        StepMacro_1State stepMacro_1State1;
        float real6;
        float real7;
        float real8;
        avl::Profile profile1;
        avl::Profile profile2;
        avl::Profile profile3;
        avl::Point2D point2D1;
        avl::Point2D point2D2;
        avl::Point2D point2D3;
        int integer4;
        StepMacro_1State stepMacro_1State2;
        float real9;
        float real10;
        float real11;
        avl::Profile profile4;
        avl::Profile profile5;
        avl::Profile profile6;
        float real12;
        float real13;
        float real14;
        avl::Point2D point2D4;
        avl::Point2D point2D5;
        avl::Point2D point2D6;
        atl::Array< avl::Point2D > point2DArray1;
        atl::Array< atl::Conditional< avl::Point2D > > point2DArray2;
        avl::Image image4;
        avl::SpatialMap LinkParameter_1;
        bool enableRemap = true;
        if (enableRemap) {
            avl::Image inputImage, averageImage, imageRemap, image3;
            if (!grabFromCamera) {
                avl::LoadImage( g_constData1, false, inputImage );
            } else {
                grabImageFromCamera(CAMERA_SH_HIK_CAM, inputImage);
                QString imageName;
                imageName.append(getVisionLogDir())
                        .append(getCurrentTimeString())
                        .append("_raw.jpg");
                if (!inputImage.Empty())
                    avl::SaveImageToJpeg( inputImage , imageName.toStdString().c_str(), atl::NIL, false );
            }
            avs::ReadDataFromFile( L"409.a361848a.SpatialMap.avdata", L"SpatialMap", LinkParameter_1 );
            avl::RemapImage(inputImage, LinkParameter_1, atl::NIL, imageRemap);
            avl::Matrix matrix1;
            avl::AverageChannels( imageRemap, atl::NIL, averageImage);
            avl::ImageToMatrix( averageImage, matrix1);
            float L = sqrt(pow(inputImage.Width()/2, 2) + pow(inputImage.Height()/2, 2));

            for (int i = 0; i < averageImage.Height(); i++) {
                for (int j = 0; j < averageImage.Width(); j++) {
                    float i_ori = 0, i_new = 0;
                    float x = sqrt( pow(j-1296, 2) + pow(i-1024, 2) );
                    float y = 0.00000000000000086806*pow(x,5) - 0.00000000000270049926*pow(x,4) +
                              0.00000000275257614039*pow(x,3) - 0.00000124593360004144*pow(x,2) +
                              0.00012844171616510600*x + 0.99393844417034000000;
                    avl::GetMatrixElement( matrix1, i, j, i_ori);
                    i_new = i_ori/y;
                    avl::SetMatrixElement( matrix1, i, j, i_new);
                }
            }
            avl::MatrixToImage( matrix1, image3 );
            avl::ConvertPixelType( image3, atl::NIL, avl::PlainType::UInt8, 0, image1 );
            {
                QString imageName;
                imageName.append(getVisionLogDir())
                        .append(getCurrentTimeString())
                        .append("_remapped.jpg");
                if (!image1.Empty())
                    avl::SaveImageToJpeg( image1 , imageName.toStdString().c_str(), atl::NIL, false );
            }
        } else {
            avl::LoadImage( g_constData1, false, image1 );
        }
        integer1 = image1.Width();
        integer2 = image1.Height();
        real1 = _avfml_st_real(static_cast<double>(integer1) / 2.0);
        real2 = _avfml_st_real(static_cast<double>(integer2) / 2.0);
        box1 = avl::Box(0, 0, integer1, static_cast<int>(real2));
        box2 = avl::Box(0, static_cast<int>(real2), integer1, static_cast<int>(real2));

        // Upper Box Image
        avl::CropImage( image1, box1, image2 );

        // Lower Box Image
        avl::CropImage( image1, box2, image3 );
        real3 = _avfml_st_real(static_cast<double>(integer1) * y1);
        real4 = _avfml_st_real(static_cast<double>(integer1) * y2);
        real5 = _avfml_st_real(static_cast<double>(integer1) * y3);
        integer3 = image2.Height();

        // Upper Box Calculation
        float g, h, i;
        StepMacro_1( stepMacro_1State1, real3, integer3, real4, real5, image2, real6, atl::Dummy<float>().Get(), atl::Dummy<float>().Get(), g, real7, atl::Dummy<float>().Get(), atl::Dummy<float>().Get(), real8, atl::Dummy<float>().Get(), atl::Dummy<float>().Get(), h, i, profile1, profile2, profile3, intensity_percentage );
        point2D1 = avl::Point2D(real3, real6);
        point2D2 = avl::Point2D(real4, real7);
        point2D3 = avl::Point2D(real5, real8);
        integer4 = image3.Height();

        // Lower Box Calculation
        float j,k,l;
        StepMacro_1( stepMacro_1State2, real3, integer4, real4, real5, image3, real9, atl::Dummy<float>().Get(), atl::Dummy<float>().Get(), j, real10, atl::Dummy<float>().Get(), atl::Dummy<float>().Get(), real11, atl::Dummy<float>().Get(), atl::Dummy<float>().Get(), k, l, profile4, profile5, profile6, intensity_percentage );
        real12 = _avfml_st_real(_avfml_ld_real(real2) + _avfml_ld_real(real9));
        real13 = _avfml_st_real(_avfml_ld_real(real2) + _avfml_ld_real(real10));
        real14 = _avfml_st_real(_avfml_ld_real(real2) + _avfml_ld_real(real11));
        point2D4 = avl::Point2D(real3, real12);
        point2D5 = avl::Point2D(real4, real13);
        point2D6 = avl::Point2D(real5, real14);

        // AvsFilter_CreateArray is intended for use in generated code only. Consider use of proper constructor or Array::Clear() and Array::Reserve function in hand-written programs.
        avs::AvsFilter_CreateArray< avl::Point2D >( point2D1, point2D2, point2D3, point2D4, point2D5, point2D6, atl::NIL, atl::NIL, point2DArray1 );

        point2DArray2.Resize(point2DArray1.Size());

        for( int i = 0; i < point2DArray1.Size(); ++i )
        {
            point2DArray2[i].AssignNonNil();
            point2DArray2[i].Get() = point2DArray1[i];
        }

        avs::DrawPoints_Palette( image1, point2DArray2, atl::NIL, atl::NIL, avl::DrawingStyle(avl::DrawingMode::HighQuality, 1.0f, 4.0f, false, avl::PointShape::Cross, 10.0f), true, image4 );

        tofResult.a = real6;
        tofResult.b = real7;
        tofResult.c = real8;

        tofResult.d = real12;
        tofResult.e = real13;
        tofResult.f = real14;

        tofResult.g = g;
        tofResult.h = h;
        tofResult.i = i;
        tofResult.j = j;
        tofResult.k = k;
        tofResult.l = l;

        QString imageName;
        imageName.append(getVisionLogDir())
                .append(getCurrentTimeString())
                .append(".jpg");
        if (!image4.Empty())
            avl::SaveImageToJpeg( image4 , imageName.toStdString().c_str(), atl::NIL, false );
        tofResult.imageName = imageName;
        tofResult.ret = true;

    }catch(const atl::Error& error) {
        tofResult.ret = false;
    }
    return tofResult;
}

ErrorCodeStruct VisionModule::PR_Edge_Fitting(QString camera_name, QString pr_name, PRResultStruct &prResult, double object_score, bool detect_small_hole)
{
    ErrorCodeStruct error_code = { OK, "" };
    pr_name.replace("file:///", "");
    qInfo("PR Edge Fitting is called. Camera name: %s pr name: %s", camera_name.toStdString().c_str(), pr_name.toStdString().c_str());
    atl::String g_constData1;
    atl::String g_constData2;
    atl::String g_emptyString;
    atl::String g_constData3;
    atl::String g_constData4;
    atl::String g_constData5;
    atl::String g_constData6;
    atl::String g_constData7;
    atl::String g_constData8;
    atl::String g_constData9;
    atl::String g_constData10;
    atl::String g_constData11;
    atl::String g_constData12;
    atl::String g_constData13;
    atl::String g_constData14;
    atl::String g_constData15;
    atl::String g_constData16;
    atl::Array< atl::Conditional< avl::Location > > g_constData17;

    QString edgeFittingField1Filename = pr_name;
    QString edgeFittingField2Filename = pr_name;
    QString edgeFittingField3Filename = pr_name;
    QString edgeFittingField4Filename = pr_name;
    QString offsetFilename = pr_name;
    QString searchHoleFilename = pr_name;

    edgeFittingField1Filename.replace("_edgeModel", "_edgeFittingField1");
    edgeFittingField2Filename.replace("_edgeModel", "_edgeFittingField2");
    edgeFittingField3Filename.replace("_edgeModel", "_edgeFittingField3");
    edgeFittingField4Filename.replace("_edgeModel", "_edgeFittingField4");
    offsetFilename.replace("_edgeModel", "_offset");
    searchHoleFilename.replace("_edgeModel", "_searchHole");

    qInfo("Edge Fitting pr filename: %s %s %s %s %s %s", edgeFittingField1Filename.toStdString().c_str(),
          edgeFittingField2Filename.toStdString().c_str(),
          edgeFittingField3Filename.toStdString().c_str(),
          edgeFittingField4Filename.toStdString().c_str(),
          offsetFilename.toStdString().c_str(),
          searchHoleFilename.toStdString().c_str());

    g_constData1 = L"C:\\Users\\emil\\Desktop\\AAProject\\huawei\\5-50\\5-50\\10-42-02-187.jpg";
    g_emptyString = L"";
    g_constData11 = L"SegmentFittingField?";
    g_constData12 = L"GrayModel?";
    g_constData13 = L"Vector2D?";
    g_constData14 = L"CircleFittingField?";
    g_constData15 = L"Angle: ";
    g_constData16 = L" Score:";
    g_constData17.Reset(1);
    g_constData17[0] = avl::Location(200, 60);

    avl::Image image1;
    atl::String string1;
    atl::String string2 = edgeFittingField1Filename.toStdString().c_str();
    atl::String string3 = edgeFittingField2Filename.toStdString().c_str();
    atl::String string4 = edgeFittingField3Filename.toStdString().c_str();
    atl::String string5 = edgeFittingField4Filename.toStdString().c_str();
    atl::String string6 = pr_name.toStdString().c_str();
    atl::String string7 = offsetFilename.toStdString().c_str();
    atl::String string8 = searchHoleFilename.toStdString().c_str();

    atl::Conditional< avl::SegmentFittingField > segmentFittingField1;
    atl::Conditional< avl::SegmentFittingField > segmentFittingField2;
    atl::Conditional< avl::SegmentFittingField > segmentFittingField3;
    atl::Conditional< avl::SegmentFittingField > segmentFittingField4;
    atl::Conditional< avl::GrayModel > grayModel1;
    atl::Conditional< avl::Vector2D > vector2D1;
    atl::Conditional< avl::CircleFittingField > circleFittingField1;
    avs::FitSegmentToEdgesState fitSegmentToEdgesState1;
    avs::FitSegmentToEdgesState fitSegmentToEdgesState2;
    avs::FitSegmentToEdgesState fitSegmentToEdgesState3;
    avs::FitSegmentToEdgesState fitSegmentToEdgesState4;
    atl::Conditional< atl::String > string9;
    atl::String string10;
    atl::Array< avl::Point2D > point2DArray1;
    avs::FitCircleToEdgesState fitCircleToEdgesState1;
    atl::String string11;
    atl::Conditional< avl::Rectangle2D > rectangle2D1;
    atl::Conditional< avl::Point2D > point2D1;
    atl::Conditional< avl::Point2D > point2D2;
    atl::Conditional< avl::Circle2D > circle2D1;
    avl::Image image2;
    avl::Image image3;
    avl::Image image4;
    avl::Image image5;
    atl::Array< atl::Conditional< atl::String > > stringArray1;
    avl::Image image6;
    avl::SaveImageState saveImage_AsynchronousState1;
    avl::SaveImageState saveImage_AsynchronousState2;
    QString imageName;
    imageName.append(getVisionLogDir())
            .append(getCurrentTimeString())
            .append(".jpg");
    QString rawImageName;
    rawImageName.append(getVisionLogDir())
            .append(getCurrentTimeString())
            .append("_raw.jpg");
    prResult.imageName = rawImageName;
    try {
        this->grabImageFromCamera(camera_name, image1);
        //avl::LoadImage( g_constData1, false, image1 );
        avl::SaveImage_Asynchronous( saveImage_AsynchronousState1, image1, atl::NIL, rawImageName.toStdString().c_str());
        avs::LoadObject< atl::Conditional< avl::SegmentFittingField > >( string2, avl::StreamMode::Binary, g_constData11, segmentFittingField1 );
        avs::LoadObject< atl::Conditional< avl::SegmentFittingField > >( string3, avl::StreamMode::Binary, g_constData11, segmentFittingField2 );
        avs::LoadObject< atl::Conditional< avl::SegmentFittingField > >( string4, avl::StreamMode::Binary, g_constData11, segmentFittingField3 );
        avs::LoadObject< atl::Conditional< avl::SegmentFittingField > >( string5, avl::StreamMode::Binary, g_constData11, segmentFittingField4 );
        avs::LoadObject< atl::Conditional< avl::GrayModel > >( string6, avl::StreamMode::Binary, g_constData12, grayModel1 );
        avs::LoadObject< atl::Conditional< avl::Vector2D > >( string7, avl::StreamMode::Binary, g_constData13, vector2D1 );
        bool circleFittingFieldFileExist = false;
        avl::TestFileExists( string8, circleFittingFieldFileExist );
        if (circleFittingFieldFileExist) {
            avs::LoadObject< atl::Conditional< avl::CircleFittingField > >( string8, avl::StreamMode::Binary, g_constData14, circleFittingField1 );
        }
        if (grayModel1 != atl::NIL)
        {
            atl::Conditional< avl::Object2D > object2D1;

            avl::LocateSingleObject_NCC( image1, atl::NIL, grayModel1.Get(), 3, 3, false, 0.3f, object2D1, atl::NIL, atl::Dummy< atl::Array< avl::Image > >().Get(), atl::Dummy< atl::Array< avl::Image > >().Get(), atl::Dummy< atl::Conditional< atl::Array< float > > >().Get() );

            if (object2D1 != atl::NIL)
            {
                if (object2D1.Get().Score() < object_score) {
                    error_code.code = ErrorCode::PR_OBJECT_NOT_FOUND;
                    qWarning("PR Score does not pass: %f < %f", object2D1.Get().Score(), object_score);
                    return error_code;
                }
                float alignmentAngle;

                avl::CoordinateSystem2D coordinateSystem2D1;
                atl::Conditional< avl::Line2D > line2D1;
                atl::Conditional< avl::Line2D > line2D2;
                atl::Conditional< avl::Point2D > point2D3;
                atl::Conditional< avl::Line2D > line2D3;
                atl::Conditional< avl::Point2D > point2D4;

                alignmentAngle = object2D1.Get().Angle();
                avl::CreateCoordinateSystemFromPoint( object2D1.Get().Point(), alignmentAngle, 1.0f, 1.0f, coordinateSystem2D1 );

                if (segmentFittingField2 != atl::NIL)
                {
                    atl::Conditional< avl::Segment2D > segment2D1;

                    avs::AvsFilter_FitSegmentToEdges( fitSegmentToEdgesState1, image1, segmentFittingField2.Get(), coordinateSystem2D1, 20, 3, avl::InterpolationMethod::Bilinear, avl::EdgeScanParams(avl::ProfileInterpolationMethod::Quadratic4, 1.0f, 1.0f, avl::EdgeTransition::Any), avl::Selection::Best, atl::NIL, 0.1f, atl::NIL, segment2D1, atl::NIL, atl::NIL, atl::NIL, atl::Dummy< atl::Array< avl::Segment2D > >().Get(), atl::Dummy< atl::Array< avl::Rectangle2D > >().Get(), atl::Dummy< atl::Array< avl::Profile > >().Get(), atl::Dummy< atl::Array< avl::Profile > >().Get() );

                    if (segment2D1 != atl::NIL)
                    {
                        line2D1.AssignNonNil();

                        avl::Segment2DToLine2D( segment2D1.Get(), line2D1.Get() );
                    }
                    else
                    {
                        line2D1 = atl::NIL;
                    }
                }
                else
                {
                    line2D1 = atl::NIL;
                }

                if (segmentFittingField1 != atl::NIL)
                {
                    atl::Conditional< avl::Segment2D > segment2D1;

                    // Function AvsFilter_FitSegmentToEdges is intended for generated code only. Consider use of CreateFittingMap and FitSegmentToEdges functions in regular programs.
                    avs::AvsFilter_FitSegmentToEdges( fitSegmentToEdgesState2, image1, segmentFittingField1.Get(), coordinateSystem2D1, 20, 3, avl::InterpolationMethod::Bilinear, avl::EdgeScanParams(avl::ProfileInterpolationMethod::Quadratic4, 1.0f, 1.0f, avl::EdgeTransition::Any), avl::Selection::Best, atl::NIL, 0.1f, atl::NIL, segment2D1, atl::NIL, atl::NIL, atl::NIL, atl::Dummy< atl::Array< avl::Segment2D > >().Get(), atl::Dummy< atl::Array< avl::Rectangle2D > >().Get(), atl::Dummy< atl::Array< avl::Profile > >().Get(), atl::Dummy< atl::Array< avl::Profile > >().Get() );

                    if (segment2D1 != atl::NIL)
                    {
                        line2D2.AssignNonNil();

                        avl::Segment2DToLine2D( segment2D1.Get(), line2D2.Get() );

                        if (line2D1 != atl::NIL)
                        {
                            avl::LineLineIntersection( line2D2.Get(), line2D1.Get(), point2D3 );
                        }
                        else
                        {
                            point2D3 = atl::NIL;
                        }
                    }
                    else
                    {
                        line2D2 = atl::NIL;
                        point2D3 = atl::NIL;
                    }
                }
                else
                {
                    line2D2 = atl::NIL;
                    point2D3 = atl::NIL;
                }

                if (segmentFittingField3 != atl::NIL)
                {
                    atl::Conditional< avl::Segment2D > segment2D1;

                    // Function AvsFilter_FitSegmentToEdges is intended for generated code only. Consider use of CreateFittingMap and FitSegmentToEdges functions in regular programs.
                    avs::AvsFilter_FitSegmentToEdges( fitSegmentToEdgesState3, image1, segmentFittingField3.Get(), coordinateSystem2D1, 20, 3, avl::InterpolationMethod::Bilinear, avl::EdgeScanParams(avl::ProfileInterpolationMethod::Quadratic4, 1.0f, 1.0f, avl::EdgeTransition::Any), avl::Selection::Best, atl::NIL, 0.1f, atl::NIL, segment2D1, atl::NIL, atl::NIL, atl::NIL, atl::Dummy< atl::Array< avl::Segment2D > >().Get(), atl::Dummy< atl::Array< avl::Rectangle2D > >().Get(), atl::Dummy< atl::Array< avl::Profile > >().Get(), atl::Dummy< atl::Array< avl::Profile > >().Get() );

                    if (segment2D1 != atl::NIL)
                    {
                        line2D3.AssignNonNil();

                        avl::Segment2DToLine2D( segment2D1.Get(), line2D3.Get() );

                        if (line2D1 != atl::NIL)
                        {
                            avl::LineLineIntersection( line2D1.Get(), line2D3.Get(), point2D4 );
                        }
                        else
                        {
                            point2D4 = atl::NIL;
                        }
                    }
                    else
                    {
                        line2D3 = atl::NIL;
                        point2D4 = atl::NIL;
                    }
                }
                else
                {
                    line2D3 = atl::NIL;
                    point2D4 = atl::NIL;
                }

                if (segmentFittingField4 != atl::NIL)
                {
                    atl::Conditional< avl::Segment2D > segment2D1;

                    // Function AvsFilter_FitSegmentToEdges is intended for generated code only. Consider use of CreateFittingMap and FitSegmentToEdges functions in regular programs.
                    avs::AvsFilter_FitSegmentToEdges( fitSegmentToEdgesState4, image1, segmentFittingField4.Get(), coordinateSystem2D1, 20, 3, avl::InterpolationMethod::Bilinear, avl::EdgeScanParams(avl::ProfileInterpolationMethod::Quadratic4, 1.0f, 1.0f, avl::EdgeTransition::Any), avl::Selection::Best, atl::NIL, 0.1f, atl::NIL, segment2D1, atl::NIL, atl::NIL, atl::NIL, atl::Dummy< atl::Array< avl::Segment2D > >().Get(), atl::Dummy< atl::Array< avl::Rectangle2D > >().Get(), atl::Dummy< atl::Array< avl::Profile > >().Get(), atl::Dummy< atl::Array< avl::Profile > >().Get() );

                    if (segment2D1 != atl::NIL && line2D2 != atl::NIL && line2D3 != atl::NIL)
                    {
                        avl::Line2D line2D4;
                        atl::Conditional< avl::Point2D > point2D5;
                        atl::Conditional< avl::Point2D > point2D6;

                        avl::Segment2DToLine2D( segment2D1.Get(), line2D4 );
                        avl::LineLineIntersection( line2D2.Get(), line2D4, point2D5 );
                        avl::LineLineIntersection( line2D3.Get(), line2D4, point2D6 );

                        if (point2D3 != atl::NIL && point2D4 != atl::NIL && point2D6 != atl::NIL && point2D5 != atl::NIL)
                        {
                            float real1;
                            float real2;

                            rectangle2D1.AssignNonNil();
                            point2D1.AssignNonNil();
                            string9.AssignNonNil();

                            real1 = object2D1.Get().Score();
                            avl::RealToString( real1, string10 );

                            // AvsFilter_CreateArray is intended for use in generated code only. Consider use of proper constructor or Array::Clear() and Array::Reserve function in hand-written programs.
                            avs::AvsFilter_CreateArray< avl::Point2D >( point2D3.Get(), point2D4.Get(), point2D6.Get(), point2D5.Get(), atl::NIL, atl::NIL, atl::NIL, atl::NIL, point2DArray1 );
                            avl::PointsBoundingRectangle( point2DArray1, avl::BoundingRectangleFeature::MinimalArea, 0.0f, avl::RectangleOrientation::Vertical, rectangle2D1.Get(), point2D1.Get(), atl::NIL, atl::NIL );

                            if (vector2D1 != atl::NIL)
                            {
                                point2D2.AssignNonNil();

                                avl::TranslatePoint( point2D1.Get(), vector2D1.Get(), false, point2D2.Get() );
                            }
                            else
                            {
                                point2D2 = atl::NIL;
                            }

                            avl::Rectangle2D rectangle2D2;
                            float real3;
                            avl::CoordinateSystem2D coordinateSystem2D2;

                            avl::NormalizeRectangleOrientation( rectangle2D1.Get(), 0.0f, avl::RectangleOrientation::Horizontal, rectangle2D2 );
                            real3 = rectangle2D2.Angle();
                            avl::RotatePoint(point2D2.Get(), point2D1.Get(), real3, false, point2D2.Get());
                            prResult.theta = real3;
                            avl::RealToString( real3, string11 );


                            if (circleFittingField1 != atl::NIL)
                            {
                                coordinateSystem2D2 = avl::CoordinateSystem2D(point2D1.Get(), real3, 1.0f);
                                avs::AvsFilter_FitCircleToEdges( fitCircleToEdgesState1, image1, circleFittingField1.Get(), coordinateSystem2D2, 10, 5, avl::InterpolationMethod::Bilinear, avl::EdgeScanParams(avl::ProfileInterpolationMethod::Quadratic4, 1.0f, 5.0f, avl::EdgeTransition::Any), avl::Selection::Best, atl::NIL, 0.1f, avl::CircleFittingMethod::AlgebraicTaubin, atl::NIL, circle2D1, atl::NIL, atl::NIL, atl::NIL, atl::Dummy< atl::Array< avl::Segment2D > >().Get(), atl::Dummy< atl::Array< avl::Rectangle2D > >().Get(), atl::Dummy< atl::Array< avl::Profile > >().Get(), atl::Dummy< atl::Array< avl::Profile > >().Get() );
                            }
                            else
                            {
                                circle2D1 = atl::NIL;
                            }

                            real2 = rectangle2D1.Get().Angle();

                            avs::AvsFilter_ConcatenateStrings( g_constData15, string11, g_constData16, string10, g_emptyString, g_emptyString, g_emptyString, g_emptyString, string9.Get() );
                        }
                        else
                        {
                            rectangle2D1 = atl::NIL;
                            point2D1 = atl::NIL;
                            string9 = atl::NIL;
                            point2D2 = atl::NIL;
                            circle2D1 = atl::NIL;
                        }
                    }
                    else
                    {
                        rectangle2D1 = atl::NIL;
                        point2D1 = atl::NIL;
                        point2D2 = atl::NIL;
                        circle2D1 = atl::NIL;
                        string9 = atl::NIL;
                    }
                }
                else
                {
                    rectangle2D1 = atl::NIL;
                    point2D1 = atl::NIL;
                    point2D2 = atl::NIL;
                    circle2D1 = atl::NIL;
                    string9 = atl::NIL;
                }
            }
            else
            {
                rectangle2D1 = atl::NIL;
                point2D1 = atl::NIL;
                point2D2 = atl::NIL;
                circle2D1 = atl::NIL;
                string9 = atl::NIL;
            }
        }
        else
        {
            rectangle2D1 = atl::NIL;
            point2D1 = atl::NIL;
            point2D2 = atl::NIL;
            circle2D1 = atl::NIL;
            string9 = atl::NIL;
        }

        if (point2D1 != atl::NIL) {
            prResult.roi_x = point2D1.Get().X();
            prResult.roi_y = point2D1.Get().Y();
        } else {
            error_code.code = ErrorCode::PR_OBJECT_NOT_FOUND;
            error_code.errorMessage = "Cannot find target point 1";
            qWarning("Cannot find target point 1");
            return error_code;
        }

        if (point2D2 != atl::NIL) {
            prResult.x = point2D2.Get().X();
            prResult.y = point2D2.Get().Y();
        } else {
            error_code.code = ErrorCode::PR_OBJECT_NOT_FOUND;
            error_code.errorMessage = "Cannot find target point 2";
            qWarning("Cannot find target point 2");
            return error_code;
        }

        if (circle2D1 != atl::NIL) {
            qInfo("Detected small hole at x: %f y: %f", circle2D1.Get().Center().X(), circle2D1.Get().Center().Y());
        } else {
            if (detect_small_hole) {
                error_code.code = ErrorCode::PR_OBJECT_NOT_FOUND;
                error_code.errorMessage = "Cannot detect small hole";
                qWarning("Cannot find the small hole");
            }
        }

        avs::DrawRectangles_SingleColor( image1, atl::ToArray< atl::Conditional< avl::Rectangle2D > >(rectangle2D1), atl::NIL, avl::Pixel(255.0f, 1.0f, 255.0f, 0.0f), avl::DrawingStyle(avl::DrawingMode::HighQuality, 1.0f, 3.0f, false, atl::NIL, 1.0f), true, image2 );
        avs::DrawCircles_SingleColor( image2, atl::ToArray< atl::Conditional< avl::Circle2D > >(circle2D1), atl::NIL, avl::Pixel(255.0f, 0.0f, 0.0f, 0.0f), avl::DrawingStyle(avl::DrawingMode::HighQuality, 1.0f, 5.0f, false, atl::NIL, 1.0f), true, image3 );
        avs::DrawPoints_SingleColor( image3, atl::ToArray< atl::Conditional< avl::Point2D > >(point2D1), atl::NIL, avl::Pixel(0.0f, 255.0f, 0.0f, 0.0f), avl::DrawingStyle(avl::DrawingMode::HighQuality, 1.0f, 4.0f, false, avl::PointShape::Cross, 30.0f), true, image4 );
        avs::DrawPoints_SingleColor( image4, atl::ToArray< atl::Conditional< avl::Point2D > >(point2D2), atl::NIL, avl::Pixel(255.0f, 128.0f, 0.0f, 0.0f), avl::DrawingStyle(avl::DrawingMode::HighQuality, 1.0f, 4.0f, false, avl::PointShape::Cross, 30.0f), true, image5 );
        stringArray1.Resize(1);
        stringArray1[0] = string9;
        avs::DrawStrings_SingleColor( image5, stringArray1, g_constData17, atl::NIL, avl::Anchor2D::MiddleCenter, avl::Pixel(0.0f, 255.0f, 0.0f, 0.0f), avl::DrawingStyle(avl::DrawingMode::HighQuality, 1.0f, 1.0f, false, atl::NIL, 20.0f), 25.0f, 0.0f, true, atl::NIL, image6 );
        avl::SaveImage_Asynchronous( saveImage_AsynchronousState1, image6, atl::NIL, imageName.toStdString().c_str());
    } catch (const atl::Error& error) {
        error_code.code = ErrorCode::PR_OBJECT_NOT_FOUND;
        qWarning(error.Message());
        return error_code;
    }
    return error_code;
}
