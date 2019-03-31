#include "visionmodule.h"
#include <AVL.h>
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

VisionModule::VisionModule()
             :QQuickImageProvider(QQuickImageProvider::Image)
{

}

ErrorCodeStruct VisionModule::PR_Generic_NCC_Template_Matching(QString camera_name, PRResultStruct &prResult)
{
    ErrorCodeStruct error_code = { OK, ""};
    try {
        static atl::String g_constData1;
        static avl::GrayModel g_constData2;
        g_constData1 = L"C:\\Users\\emil\\Desktop\\Test\\lut_lens\\1470924728.jpg";
        avs::ReadDataFromFile( L"GenericNCCavproj.b19eee0c.avdata", L"GrayModel", g_constData2 );
        avl::EnumerateFilesState enumerateImagesState1;
        avl::Image image1;
        atl::String file1;
        atl::String string1;
        avl::Image image2;
        atl::Conditional< avl::Object2D > object2D1;
        atl::Conditional< avl::Point2D > point2D1;
        QString imageName;
        imageName.append(getVisionLogDir())
                 .append(getCurrentTimeString())
                 .append(".jpg");
        avl::LoadImage( g_constData1, false, image1 ); // Offline testing
//        if (!grabImageFromCamera(camera_name, image1)) {
//            return error_code;
//        }
        avl::LocateSingleObject_NCC( image1, atl::NIL, g_constData2, 0, 3, false, 0.7f, object2D1, atl::NIL, atl::Dummy< atl::Array< avl::Image > >().Get(), atl::Dummy< atl::Array< avl::Image > >().Get(), atl::Dummy< atl::Conditional< atl::Array< float > > >().Get() );
        if (object2D1 != atl::NIL)
        {
            point2D1.AssignNonNil();
            point2D1.Get() = object2D1.Get().Point();
        }
        else
        {
            point2D1 = atl::NIL;
            return error_code;
        }
        avs::DrawPoints_SingleColor( image1, atl::ToArray< atl::Conditional< avl::Point2D > >(point2D1), atl::NIL, avl::Pixel(0.0f, 255.0f, 0.0f, 0.0f), avl::DrawingStyle(avl::DrawingMode::HighQuality, 1.0f, 4.0f, false, avl::PointShape::Cross, 40.0f), true, image2 );
        prResult.x = point2D1.Get().X();
        prResult.y = point2D1.Get().Y();
        prResult.imageName = imageName;
        prResult.ret = true;
        avl::SaveImageToJpeg( image2 , imageName.toStdString().c_str(), atl::NIL, false );
        displayPRResult(camera_name, prResult);
    } catch(const atl::Error& error) {
        qWarning(error.Message());
        return error_code;
    }
    return error_code;
}

void VisionModule::displayPRResult(const QString camera_name, const PRResultStruct prResult)
{
    if (camera_name.contains(DOWNLOOK_VISION_CAMERA)) {
        last_downlook_pr_result = prResult;
    }
    else if (camera_name.contains(UPLOOK_VISION_CAMERA)) {
        last_uplook_pr_result = prResult;
    }
    else if (camera_name.contains(PICKARM_VISION_CAMERA)) {
        last_pickarm_pr_result = prResult;
    }
    emit callQmlRefeshImg();
}

QImage VisionModule::requestImage(const QString &id, QSize *size, const QSize &requestedSize)
{
    if (id.contains(DOWNLOOK_VISION_CAMERA)) {
        return QImage(last_downlook_pr_result.imageName);
    }
    else if (id.contains(UPLOOK_VISION_CAMERA)) {
        return QImage(last_uplook_pr_result.imageName);
    }
    else if (id.contains(PICKARM_VISION_CAMERA)) {
        return QImage(last_pickarm_pr_result.imageName);
    }
    return QImage();
}
