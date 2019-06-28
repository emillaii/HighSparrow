import QtQuick 2.0
import QtQuick.Layouts 1.11
import QtQuick.Controls 2.4

ScrollView {
    id: machineConfigListView
    width: 600

    ColumnLayout{
        Switch {id: aaViewSwitch; text:"AA 头机器配置"; font.bold: true}
        AAHeadModuleView{
            Layout.fillWidth: true
            visible: aaViewSwitch.checked
        }

        Switch {id: sutModuleViewSwitch; text:"SUT 配置"; font.bold: true}
        SUTModuleView{
            Layout.fillWidth: true
            visible: sutModuleViewSwitch.checked
        }

        Switch {id: dispensViewSwitch; text:"Dispense 配置"; font.bold: true}
        DispenseModuleView{
            Layout.fillWidth: true
            visible: {dispensViewSwitch.checked}
        }

        Switch {id: imgGrabberViewSwitch; text:"Image Grabber 配置"; font.bold: true}
        ImageGrabberView{
            Layout.fillWidth: true
            visible: {imgGrabberViewSwitch.checked}
        }

        Switch {id: calibrationViewSwitch; text:"Calibration 配置"; font.bold: true}
        CalibrationView{
            Layout.fillWidth: true
            visible: {calibrationViewSwitch.checked}
        }

        Switch {id: aaCoreViewSwitch; text:"AA Core 配置"; font.bold: true}
        AACoreView{
            Layout.fillWidth: true
            visible: {aaCoreViewSwitch.checked}
        }

        Switch {id: systemViewSwitch; text:"System 配置"; font.bold: true}
        SystemConfigView{
            Layout.fillWidth: true
            visible: {systemViewSwitch.checked}
        }

        Switch {id: materialLoaderViewSwitch; text:"MaterialLoader 配置"; font.bold: true}
        SingleStationMaterialLoaderModuleView{
            Layout.fillWidth: true
            visible: materialLoaderViewSwitch.checked
        }

        Switch {id: threadMgrSwitch; text:"Thread Manager"; font.bold: true}
        ThreadManagerView{
            Layout.fillWidth: true
            visible: {threadMgrSwitch.checked}
        }

        Switch {id: commonCtrlSwitch; text:"常用操作"; font.bold: true}
        CommonControl{
            Layout.fillWidth: true
            visible: {commonCtrlSwitch.checked}
        }

        Switch {id: checkProcessSwitch; text:"校正过程"; font.bold: true}
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
//                    else if(baseModuleManager.getServerMode()===1)
//                        return "SensorLoaderModuleView.qml"
//                    else
//                        return "SingleStationMaterialLoaderModuleView.qml"
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
