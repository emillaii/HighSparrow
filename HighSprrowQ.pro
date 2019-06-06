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

SOURCES += \
    main.cpp \
    baslerpyloncamera.cpp \
    highsprrow.cpp \
    errorcode.cpp \
    aaheadmodule.cpp \
    visionmodule.cpp \
    commonutils.cpp \
    logger.cpp \
    motorspositionmodel.cpp \
    propertybase.cpp \
    basemodule.cpp \
    xtadcmodule.cpp \
    XtGeneralOutput.cpp \
    XtGeneralInput.cpp \
    sfrworker.cpp \
    wordoplight.cpp \
    logicmanager.cpp \
    filecontent.cpp \
    basemodulemanager.cpp \
    LontryLight.cpp \
    XtCylinder.cpp \
    XtMotor.cpp \
    XtVcMotor.cpp \
    XtVacuum.cpp \
    lut_module.cpp \
    sut_module.cpp \
    aadata.cpp \
    material_carrier.cpp \
    pixel2mech.cpp \
    Matrix/Matrix.cpp \
    calibration.cpp \
    chart_calibration.cpp \
    imagegrabbingworkerthread.cpp\
	dothinkey.cpp \
    iniparser.cpp \
    imageprovider.cpp \
    dispenser.cpp \
    dispense_module.cpp \
    vision_location.cpp \
    unitlog.cpp \
    workers_manager.cpp \
    thread_worker_base.cpp \
    materialtray.cpp \
    unitlog.cpp \
    sparrowqserver.cpp \
    sparrowqclient.cpp \
    lenspickarm.cpp \
    lensloadermodule.cpp \
    trayloadermodule.cpp \
    trayclip.cpp \
    lutclient.cpp \
    sensorpickarm.cpp \
    sensorloadermodule.cpp \
    sutclient.cpp \
    aacorenew.cpp \
    sensortrayloadermodule.cpp \
    sensorclip.cpp \
    checkprocessitem.cpp \
    checkprocessmodel.cpp \
    machinestatemonitor.cpp \
    machinestatemonitorcontroller.cpp \
    singleheadmachinematerialpickarm.cpp \
    singleheadmachinematerialloadermodule.cpp \
    singlehead_lsut_module.cpp
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

HEADERS += \
    baslerpyloncamera.h \
    config.h \
    highsprrow.h \
    errorcode.h \
    aaheadmodule.h \
    visionmodule.h \
    commonutils.h \
    logger.h \
    motorspositionmodel.h \
    propertybase.h \
    basemodule.h \
    xtadcmodule.h \
    XtGeneralOutput.h \
    XtGeneralInput.h \
    sfrworker.h \
    aaheadparameters.h \
    wordoplight.h \
    logicmanager.h \
    filecontent.h \
    basemodulemanager.h \
    LontryLight.h \
    XtCylinder.h \
    xtmotor.h \
    XtVcMotor.h \
    material_carrier.h \
    XtVacuum.h \
    sut_parameter.h \
    material_carrier_parameter.h \
    lut_module.h \
    lut_parameter.h \
    sut_module.h \
    aadata.h \
    calibration.h \
    pixel2mech.h \
    Matrix/Matrix.h \
    calibration_parameter.h \
    chart_calibration.h \
    imagegrabbingworkerthread.h\
    dothinkey.h \
    iniparser.h \
    imageprovider.h \
    XtVacuum.h \
    dispenser.h \
    dispenser_parameter.h \
    dispense_module.h \
    dispense_parameter.h \
    position_define.h \
    vision_location_parameter.h \
    vision_location.h \
    unitlog.h \
    workers_manager.h \
    thread_worker_base.h \
    materialtray.h \
    unitlog.h \
    material_picker.h \
    traystandardsparameter.h \
    trayparameter.h \
    aacoreparameters.h \
    sparrowqserver.h \
    sparrowqclient.h \
    lenspickarm.h \
    lenspickarmparameter.h \
    lensloaderparameter.h \
    lensloadermodule.h \
    xtvcmotorparameter.h \
    trayloadermodule.h \
    trayclip.h \
    trayloaderparameter.h \
    trayclipstandardsparameter.h\
    xtvacuumparameter.h \
    xtcylinderparameter.h \
    lutclient.h \
    sensorpickarm.h \
    sensorpickarmparameter.h \
    sensorloadermodule.h \
    sensorloaderparameter.h \
    sutclient.h \
    aacorenew.h \
    sendmessagetool.h \
    sensortrayloadermodule.h \
    sensortrayloaderparameter.h \
    sensorclip.h \
    sensorclipparameter.h \
    iolimitparameter.h \
    parallellimitparameter.h \
    verticallimitparameter.h \
    modulemanagerparameter.h \
    xtmotorparameter.h \
    checkprocessitem.h \
    checkprocessmodel.h \
    machinestatemonitor.h \
    machinestatemonitorcontroller.h \
    singleheadmachinematerialpickarm.h \
    singleheadmachinematerialpickarmparameter.h \
    singleheadmachinematerialloadermodule.h \
    singleheadmachinematerialloadermoduleparameter.h \
    pickarmposition.h \
    singlehead_lsut_module.h \
    singleheadlsutparameter.h

DISTFILES += \
    icons/index.theme
