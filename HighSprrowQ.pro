QT += quick
QT += serialport
QT += webengine
QT += widgets
QT += charts
QT += qml
QT += quick
QT += websockets
QT += sql
QT += remoteobjects
CONFIG += c++11
CONFIG+=qtquickcompiler
# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Refer to the documentation for the
# deprecated API to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += QT_MESSAGELOGCONTEXT

REPC_REPLICA += ./QTRORep/vision.rep
REPC_SOURCE += ./QTRORep/vision.rep
TRANSLATIONS = languagePackage/chinese.ts languagePackage/english.ts

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Drivers/LightSourceController/sciencalightsourcecontroller.cpp \
    devicestatesgeter.cpp \
    i2cControl/i2ccontrol.cpp \
    main.cpp \
    utils/commonmethod.cpp \
    utils/configManager/configarray.cpp \
    utils/configManager/configobject.cpp \
    utils/configManager/configobjectarray.cpp \
    utils/loging/loging.cpp \
    utils/loging/logmodel.cpp \
    utils/loging/rollbackfile.cpp \
    utils/singletoninstances.cpp \
    utils/uiHelper/msgboxmodel.cpp \
    utils/uiHelper/uioperation.cpp \
    utils/userManagement/mysqltablemodel.cpp \
    utils/userManagement/usermanagement.cpp \
    vision/baslerpyloncamera.cpp \
    highsprrow.cpp \
    utils/errorcode.cpp \
    aaHeadModule/aaheadmodule.cpp \
    traymapmodel.cpp \
    utils/commonutils.cpp \
    motorspositionmodel.cpp \
    propertybase.cpp \
    basemodule.cpp \
    vision/visionserver.cpp \
    xtadcmodule.cpp \
    XtGeneralOutput.cpp \
    XtGeneralInput.cpp \
    AACore/sfrworker.cpp \
    logicmanager.cpp \
    utils/filecontent.cpp \
    basemodulemanager.cpp \
    utils/LontryLight.cpp \
    XtCylinder.cpp \
    XtMotor.cpp \
    XtVcMotor.cpp \
    XtVacuum.cpp \
    lutModule/lut_module.cpp \
    sutModule/sut_module.cpp \
    AACore/aadata.cpp \
    material_carrier.cpp \
    utils/pixel2mech.cpp \
    Matrix/Matrix.cpp \
    calibration/calibration.cpp \
    calibration/chart_calibration.cpp \
    imageGrabber/imagegrabbingworkerthread.cpp\
    imageGrabber/dothinkey.cpp \
    imageGrabber/iniparser.cpp \
    utils/imageprovider.cpp \
    dispenseModule/dispenser.cpp \
    dispenseModule/dispense_module.cpp \
    vision/wordoplight.cpp \
    vision/visionmodule.cpp \
    vision/vision_location.cpp \
    utils/unitlog.cpp \
    workers_manager.cpp \
    thread_worker_base.cpp \
    materialtray.cpp \
    network/sparrowqserver.cpp \
    network/sparrowqclient.cpp \
    lenspickarm.cpp \
    lensloadermodule.cpp \
    trayloadermodule.cpp \
    trayclip.cpp \
    lutModule/lutclient.cpp \
    sensorpickarm.cpp \
    sensorloadermodule.cpp \
    sutModule/sutclient.cpp \
    AACore/aacorenew.cpp \
    sensortrayloadermodule.cpp \
    sensorclip.cpp \
    checkprocessitem.cpp \
    checkprocessmodel.cpp \
    machinestatemonitor.cpp \
    machinestatemonitorcontroller.cpp \
    uphhelper.cpp \
    tcpmessager.cpp \
    tcpclientmessager.cpp \
    tcpmanager.cpp \
    tcpservermessager.cpp \ 
    uiControl/edge.cpp \
    uiControl/graphwidget.cpp \
    uiControl/node.cpp \
    alarmmessageshower.cpp

RESOURCES += myqml.qrc \
    language.qrc
RESOURCES += icons.qrc
#QTQUICK_COMPILER_SKIPPED_RESOURCES += myqml.qrc
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

LIBS += -L$$PWD/../libs/UsbI2cIo/libs/ -lusbi2cio64
INCLUDEPATH += $$PWD/../libs/UsbI2cIo/libs
DEPENDPATH += $$PWD/../libs/UsbI2cIo/libs

HEADERS += \
    Drivers/LightSourceController/sciencalightsourcecontroller.h \
    UnitTest/SilicolMsgBoxTest.h \
    basicconfig.h \
    i2cControl/i2ccontrol.h \
    utils/commonmethod.h \
    utils/configManager/configarray.h \
    utils/configManager/configbase.h \
    utils/configManager/configfile.h \
    utils/configManager/configmanager.h \
    utils/configManager/configmanagerunittest.h \
    utils/configManager/configobject.h \
    utils/configManager/configobjectarray.h \
    utils/configManager/qobjectfactory.h \
    utils/languageManager/languageconfig.h \
    utils/languageManager/languagemanager.h \
    utils/loging/logconfig.h \
    utils/loging/loging.h \
    utils/loging/logmodel.h \
    utils/loging/rollbackfile.h \
    utils/singletoninstances.h \
    utils/uiHelper/msgboxmodel.h \
    utils/uiHelper/uioperation.h \
    utils/userManagement/mysqltablemodel.h \
    utils/userManagement/usermanagement.h \
    vision/baslerpyloncamera.h \
    config.h \
    devicestatesgeter.h \
    highsprrow.h \
    utils/errorcode.h \
    aaHeadModule/aaheadmodule.h \
    traymapmodel.h \
    vision/visionmodule.h \
    utils/commonutils.h \
    motorspositionmodel.h \
    propertybase.h \
    basemodule.h \
    vision/visionserver.h \
    xtadcmodule.h \
    XtGeneralOutput.h \
    XtGeneralInput.h \
    AACore/sfrworker.h \
    aaHeadModule/aaheadparameters.h \
    logicmanager.h \
    utils/filecontent.h \
    basemodulemanager.h \
    utils/LontryLight.h \
    XtCylinder.h \
    xtmotor.h \
    XtVcMotor.h \
    material_carrier.h \
    XtVacuum.h \
    sutModule/sut_parameter.h \
    material_carrier_parameter.h \
    lutModule/lut_module.h \
    lutModule/lut_parameter.h \
    sutModule/sut_module.h \
    AACore/aadata.h \
    utils/pixel2mech.h \
    Matrix/Matrix.h \
    calibration/calibration.h \
    calibration/calibration_parameter.h \
    calibration/chart_calibration.h \
    imageGrabber/imagegrabbingworkerthread.h\
    imageGrabber/dothinkey.h \
    imageGrabber/iniparser.h \
    utils/ \
    XtVacuum.h \
    dispenseModule/dispenser.h \
    dispenseModule/dispenser_parameter.h \
    dispenseModule/dispense_module.h \
    dispenseModule/dispense_parameter.h \
    position_define.h \
    vision/wordoplight.h \
    vision/vision_location_parameter.h \
    vision/vision_location.h \
    utils/unitlog.h \
    workers_manager.h \
    thread_worker_base.h \
    materialtray.h \
    material_picker.h \
    traystandardsparameter.h \
    trayparameter.h \
    AACore/aacoreparameters.h \
    network/sparrowqserver.h \
    network/sparrowqclient.h \
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
    lutModule/lutclient.h \
    sensorpickarm.h \
    sensorpickarmparameter.h \
    sensorloadermodule.h \
    sensorloaderparameter.h \
    sutModule/sutclient.h \
    AACore/aacorenew.h \
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
    uphhelper.h \
    tcpmessager.h \
    tcpmessagerparameter.h \
    tcpmanager.h \
    tcpmanagerparameter.h \
    tcpclientmessager.h \
    tcpservermessager.h \
    workersmanagerparameter.h \
    uiControl/edge.h \
    uiControl/graphwidget.h \
    uiControl/node.h \
    logicmanagerparameter.h \
    alarmmessageshower.h \
    ipcclient.h

DISTFILES += \
    icons/index.theme \
    index.html

RC_ICONS= icons/sparrow.ico
