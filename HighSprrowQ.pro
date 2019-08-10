QT += quick
QT += serialport
QT += webengine
QT += widgets
QT += charts qml quick
QT += websockets
CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Refer to the documentation for the
# deprecated API to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += QT_MESSAGELOGCONTEXT

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


RESOURCES += qml.qrc \
    icons.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

LIBS += -L$$PWD/../libs/pylon/lib/x64/ -lPylonBase_MD_VC120_v5_0
LIBS += -L$$PWD/../libs/pylon/Runtime/x64
INCLUDEPATH += $$PWD/../libs/pylon/lib/x64/
INCLUDEPATH += $$PWD/../libs/pylon/include
DEPENDPATH += $$PWD/../libs/pylon/lib/x64/
DEPENDPATH += $$PWD/../libs/pylon/Runtime/x64

LIBS += -L$$PWD/../libs/AdaptiveVision/lib/x64/ -lAVL
INCLUDEPATH += $$PWD/../libs/AdaptiveVision/include
DEPENDPATH += $$PWD/../libs/AdaptiveVision/include

LIBS += -L$$PWD/../libs/opencv/x64/vc14/lib/ -lopencv_world310
LIBS += -L$$PWD/../libs/opencv/x64/vc14/bin
INCLUDEPATH += $$PWD/../libs/opencv/include
DEPENDPATH += $$PWD/../libs/opencv/x64/vc14/bin

LIBS += -L$$PWD/../libs/sparrow_core/sparrow_core/lib/ -lSparrowCore
INCLUDEPATH += $$PWD/../libs/sparrow_core/sparrow_core/include
DEPENDPATH += $$PWD/../libs/sparrow_core/sparrow_core/include

LIBS += -L$$PWD/../libs/motion_x64/ -lMotionControlDll
LIBS += -L$$PWD/../libs/motion_x64/ -lMotionControlExtendDll
LIBS += -L$$PWD/../libs/motion_x64/ -lvoice_motor_dll

INCLUDEPATH += $$PWD/../libs/motion_x64
DEPENDPATH += $$PWD/../libs/motion_x64
INCLUDEPATH += $$PWD/../libs/eigen/eigen-eigen-5a0156e40feb

LIBS += -L$$PWD/../libs/DTCCM2_SDK/X64_Lib/ -ldtccm2
INCLUDEPATH += $$PWD/../libs/DTCCM2_SDK
DEPENDPATH += $$PWD/../libs/DTCCM2_SDK/X64_Lib



DISTFILES += \
    icons/index.theme

HEADERS += \
    Dispense/dispense_module.h \
    Dispense/dispense_parameter.h \
    Dispense/dispenser.h \
    Dispense/dispenser_parameter.h \
    DualHead/lensloadermodule.h \
    DualHead/lensloaderparameter.h \
    DualHead/lenspickarm.h \
    DualHead/lenspickarmparameter.h \
    DualHead/lut_module.h \
    DualHead/lut_parameter.h \
    DualHead/lutclient.h \
    DualHead/material_picker.h \
    DualHead/sensorclip.h \
    DualHead/sensorclipparameter.h \
    DualHead/sensorloadermodule.h \
    DualHead/sensorloaderparameter.h \
    DualHead/sensorpickarm.h \
    DualHead/sensorpickarmparameter.h \
    DualHead/sensortrayloadermodule.h \
    DualHead/sensortrayloaderparameter.h \
    DualHead/sut_module.h \
    DualHead/sut_parameter.h \
    DualHead/sutclient.h \
    DualHead/trayclip.h \
    DualHead/trayclipstandardsparameter.h \
    DualHead/trayloadermodule.h \
    DualHead/trayloaderparameter.h \
    LontryLight.h \
    Matrix/Matrix.h \
    SingleHead/singlehead_lsut_module.h \
    SingleHead/singleheadlsutparameter.h \
    SingleHead/singleheadmachinematerialloadermodule.h \
    SingleHead/singleheadmachinematerialloadermoduleparameter.h \
    SingleHead/singleheadmachinematerialpickarm.h \
    SingleHead/singleheadmachinematerialpickarmparameter.h \
    Utils/commonutils.h \
    Utils/config.h \
    Utils/errorcode.h \
    Utils/filecontent.h \
    Utils/iniparser.h \
    Utils/logger.h \
    Utils/position_define.h \
    Utils/propertybase.h \
    Utils/unitlog.h \
    Vision/baslerpyloncamera.h \
    Vision/calibration.h \
    Vision/calibration_parameter.h \
    Vision/pixel2mech.h \
    Vision/vision_location.h \
    Vision/vision_location_parameter.h \
    Vision/visionmodule.h \
    Vision/wordoplight.h \
    Vision/chart_calibration.h \
    XtMotion/XtCylinder.h \
    XtMotion/XtGeneralInput.h \
    XtMotion/XtGeneralOutput.h \
    XtMotion/XtVacuum.h \
    XtMotion/XtVcMotor.h \
    XtMotion/iolimitparameter.h \
    XtMotion/parallellimitparameter.h \
    XtMotion/verticallimitparameter.h \
    XtMotion/xtadcmodule.h \
    XtMotion/xtcylinderparameter.h \
    XtMotion/xtmotor.h \
    XtMotion/xtmotorparameter.h \
    XtMotion/xtvacuumparameter.h \
    XtMotion/xtvcmotorparameter.h \
    AACore/aacorenew.h \
    AACore/aacoreparameters.h \
    AACore/aadata.h \
    AAHeadModule/aaheadmodule.h \
    AAHeadModule/aaheadparameters.h \
    basemodulemanager.h \
    checkprocessitem.h \
    checkprocessmodel.h \
    ImageGrabber/dothinkey.h \
    ImageGrabber/imagegrabbingworkerthread.h \
    highsprrow.h \
    imageprovider.h \
    logicmanager.h \
    machinestatemonitor.h \
    machinestatemonitorcontroller.h \
    material_carrier.h \
    material_carrier_parameter.h \
    materialtray.h \
    modulemanagerparameter.h \
    motorspositionmodel.h \
    sendmessagetool.h \
    AACore/sfrworker.h \
    sparrowqclient.h \
    sparrowqserver.h \
    thread_worker_base.h \
    trayparameter.h \
    traystandardsparameter.h \
    workers_manager.h \
    TrayMap/traymapmodel.h

SOURCES += \
    Dispense/dispense_module.cpp \
    Dispense/dispenser.cpp \
    DualHead/lensloadermodule.cpp \
    DualHead/lenspickarm.cpp \
    DualHead/lut_module.cpp \
    DualHead/lutclient.cpp \
    DualHead/sensorclip.cpp \
    DualHead/sensorloadermodule.cpp \
    DualHead/sensorpickarm.cpp \
    DualHead/sensortrayloadermodule.cpp \
    DualHead/sut_module.cpp \
    DualHead/sutclient.cpp \
    DualHead/trayclip.cpp \
    DualHead/trayloadermodule.cpp \
    LontryLight.cpp \
    Matrix/Matrix.cpp \
    SingleHead/singlehead_lsut_module.cpp \
    SingleHead/singleheadmachinematerialloadermodule.cpp \
    SingleHead/singleheadmachinematerialpickarm.cpp \
    Utils/commonutils.cpp \
    Utils/errorcode.cpp \
    Utils/filecontent.cpp \
    Utils/iniparser.cpp \
    Utils/logger.cpp \
    Utils/propertybase.cpp \
    Utils/unitlog.cpp \
    Vision/baslerpyloncamera.cpp \
    Vision/calibration.cpp \
    Vision/pixel2mech.cpp \
    Vision/vision_location.cpp \
    Vision/visionmodule.cpp \
    Vision/wordoplight.cpp \
    Vision/chart_calibration.cpp \
    XtMotion/XtCylinder.cpp \
    XtMotion/XtGeneralInput.cpp \
    XtMotion/XtGeneralOutput.cpp \
    XtMotion/XtVacuum.cpp \
    XtMotion/XtVcMotor.cpp \
    XtMotion/xtadcmodule.cpp \
    XtMotion/xtmotor.cpp \
    AACore/aacorenew.cpp \
    AACore/aadata.cpp \
    AACore/sfrworker.cpp \
    AAHeadModule/aaheadmodule.cpp \
    AAHeadModule/aaheadparameters.cpp \
    basemodulemanager.cpp \
    checkprocessitem.cpp \
    checkprocessmodel.cpp \
    ImageGrabber/dothinkey.cpp \
    ImageGrabber/imagegrabbingworkerthread.cpp \
    highsprrow.cpp \
    imageprovider.cpp \
    logicmanager.cpp \
    machinestatemonitor.cpp \
    machinestatemonitorcontroller.cpp \
    main.cpp \
    material_carrier.cpp \
    materialtray.cpp \
    motorspositionmodel.cpp \
    sparrowqclient.cpp \
    sparrowqserver.cpp \
    thread_worker_base.cpp \
    workers_manager.cpp \
    TrayMap/traymapmodel.cpp








