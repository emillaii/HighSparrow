#include "visionmodule.h"

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
#include <commonutils.h>
#include "baslerpyloncamera.h"

VisionModule::VisionModule()
             :QQuickImageProvider(QQuickImageProvider::Image)
{

}

bool VisionModule::grabImageFromCamera(QString cameraName, avl::Image &image)
{
    BaslerPylonCamera *camera = Q_NULLPTR;
    if (cameraName.contains(CAMERA_AA1_DL)) {  }
    else if (cameraName.contains(UPLOOK_VISION_CAMERA)) { }
    else if (cameraName.contains(PICKARM_VISION_CAMERA)) {  }
    QPixmap p = QPixmap::fromImage(camera->getImage());
    QImage q2 = p.toImage();
    q2 = q2.convertToFormat(QImage::Format_RGB888);
    avl::Image image2(q2.width(), q2.height(), q2.bytesPerLine(), avl::PlainType::Type::UInt8, q2.depth() / 8, q2.bits());
    image = image2;
    return true;
}

ErrorCodeStruct VisionModule::PR_Generic_NCC_Template_Matching(QString camera_name, QString pr_name, PRResultStruct &prResult)
{
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
        g_constData1 = L"C:\\Users\\emil\\Desktop\\Test\\04blens_lighting100-220 (1)\\04blens_lighting100-220\\04blighting150.jpg";
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
        //Testing use
        //avl::RotateImage( image1, 4.0f, avl::RotationSizeMode::Fit, avl::InterpolationMethod::Bilinear, false, image2 );
        avs::LoadObject< avl::Vector2D >( g_constData2, avl::StreamMode::Binary, g_constData3, vector2D1 );
        avs::LoadObject< avl::GrayModel >( g_constData4, avl::StreamMode::Binary, g_constData5, grayModel1 );
        avl::LocateSingleObject_NCC( image1, atl::NIL, grayModel1, 0, 3, false, 0.7f, object2D1, atl::NIL, atl::Dummy< atl::Array< avl::Image > >().Get(), atl::Dummy< atl::Array< avl::Image > >().Get(), atl::Dummy< atl::Conditional< atl::Array< float > > >().Get() );

        if (object2D1 != atl::NIL)
        {
            float real1;
            avl::Point2D point2D3;
            float real2;
            float real3;

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

            // Function AvsFilter_MakeRectangle is intended for generated code only. Consider use of proper Rectangle2D constructor instead.
            avs::AvsFilter_MakeRectangle( point2D3, real1, real2, real3, rectangle2D1.Get() );
            prResult.x = point2D2.Get().x;
            prResult.y = point2D2.Get().y;
            prResult.theta = real1;
            prResult.imageName = imageName;
        }
        else
        {
            point2D1 = atl::NIL;
            point2D2 = atl::NIL;
            string1 = atl::NIL;
            rectangle2D1 = atl::NIL;
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
        qWarning(error.Message());
        return error_code;
    }
    return error_code;
}

bool grabImageFromCamera(QString cameraName, avl::Image &image)
{
    BaslerPylonCamera *camera = Q_NULLPTR;
    if (cameraName.contains(DOWNLOOK_VISION_CAMERA)) {  }
    else if (cameraName.contains(UPLOOK_VISION_CAMERA)) {  }
    else if (cameraName.contains(PICKARM_VISION_CAMERA)) {  }
    QPixmap p = QPixmap::fromImage(camera->getImage());
    QImage q2 = p.toImage();
    q2 = q2.convertToFormat(QImage::Format_RGB888);
    avl::Image image2(q2.width(), q2.height(), q2.bytesPerLine(), avl::PlainType::Type::UInt8, q2.depth() / 8, q2.bits());
    image = image2;
    return true;
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
    //emit callQmlRefeshImg();
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
