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
        qInfo("save image from hikcamera");
        if (hikCamera != Q_NULLPTR) {
            qInfo("save image from hikcamera 12");
            QPixmap p = QPixmap::fromImage(hikCamera->getImage());
            QImage q2 = p.toImage();
            q2 = q2.convertToFormat(QImage::Format_RGB888);
            avl::Image image2(q2.width(), q2.height(), q2.bytesPerLine(), avl::PlainType::Type::UInt8, q2.depth() / 8, q2.bits());
            image = image2;
        }
        return true;
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
        return PR_Edge_Template_Matching(camera_name, pr_name, prResult);
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
