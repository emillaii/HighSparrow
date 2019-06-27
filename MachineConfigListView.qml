﻿import QtQuick 2.0
import QtQuick.Layouts 1.11
import QtQuick.Controls 2.4

ScrollView {
    id: machineConfigListView
    width: 600

    ColumnLayout{
        Switch {id: aaViewSwitch; text:"AA 头机器配置"}
        AAHeadModuleView{
            Layout.fillWidth: true
            visible: aaViewSwitch.checked
        }

        Switch {id: sutModuleViewSwitch; text:"SUT 配置"}
        SUTModuleView{
            Layout.fillWidth: true
            visible: sutModuleViewSwitch.checked
        }

        Switch {id: lutModuleViewSwitch; text:"LUT 配置"}
        LUTModuleView{
            Layout.fillWidth: true
            visible: {
                if (baseModuleManager.getServerMode() === 0)
                    lutModuleViewSwitch.checked
                else
                    false
            }
        }

        Switch {id: dispensViewSwitch; text:"Dispense 配置"}
        DispenseModuleView{
            Layout.fillWidth: true
            visible: {dispensViewSwitch.checked}
        }

        Switch {id: imgGrabberViewSwitch; text:"Image Grabber 配置"}
        ImageGrabberView{
            Layout.fillWidth: true
            visible: {imgGrabberViewSwitch.checked}
        }

        Switch {id: calibrationViewSwitch; text:"Calibration 配置"}
        CalibrationView{
            Layout.fillWidth: true
            visible: {calibrationViewSwitch.checked}
        }

        Switch {id: aaCoreViewSwitch; text:"AA Core 配置"}
        AACoreView{
            Layout.fillWidth: true
            visible: {aaCoreViewSwitch.checked}
        }

        Switch {id: systemViewSwitch; text:"System 配置"}
        SystemConfigView{
            Layout.fillWidth: true
            visible: {systemViewSwitch.checked}
        }

        Switch {id: materialLoderViewSwitch; text:"MaterialLoder 配置"}
        Loader{
            source: {
                if (baseModuleManager.getServerMode() === 0)
                    return "LensPickArmModuleView.qml"
                else
                    return "SensorLoaderModuleView.qml"

            }

            visible: materialLoderViewSwitch.checked
        }

        Switch {id: trayLoderViewSwitch; text:"TrayLoader 配置"}
        Loader{
            source: {
                if (baseModuleManager.getServerMode() === 0)
                    return "TrayLoaderModuleView.qml"
                else
                    return "SensorTrayLoaderViewModule.qml"

            }

            visible: trayLoderViewSwitch.checked
        }

        Switch {id: threadMgrSwitch; text:"Thread Manager"}
        ThreadManagerView{
            Layout.fillWidth: true
            visible: {threadMgrSwitch.checked}
        }

        Switch {id: commonCtrlSwitch; text:"常用操作"}
        CommonControl{
            Layout.fillWidth: true
            visible: {commonCtrlSwitch.checked}
        }

        Switch {id: checkProcessSwitch; text:"校正过程"}
        CheckProcess{
            Layout.fillWidth: true
            visible: {checkProcessSwitch.checked}
        }
    }
}


//ListView {
//    id: machineConfigListView
//    model: MachineSettingModel {
//    }
//    delegate:  Component {
//        Loader {
//            source: switch(index) {
//                case 1 : return "AAHeadModuleView.qml"
//                case 2 :
//                    return "SUTModuleView.qml"
//                case 3 :
//                    if (baseModuleManager.getServerMode() === 0)
//                        return "LUTModuleView.qml"
//                    return "";
//                case 4 : return "DispenseModuleView.qml"
//                case 5 : return "ImageGrabberView.qml"
//                case 6 : return "CalibrationView.qml"
//                case 7 : return "AACoreView.qml"
//                case 8 : return "SystemConfigView.qml"
//                case 9 :
//                    if (baseModuleManager.getServerMode() === 0)
//                        return "LensPickArmModuleView.qml"
//                    else
//                        return "SensorLoaderModuleView.qml"
//                case 10:
//                    if (baseModuleManager.getServerMode() === 0)
//                        return "TrayLoaderModuleView.qml"
//                    else
//                        return "SensorTrayLoaderViewModule.qml"
//                case 11: return "ThreadManagerView.qml"
//                case 12:return "CommonControl.qml"
//                case 13:return "CheckProcess.qml"
//            }
//        }
//    }
//}
