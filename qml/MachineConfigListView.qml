import QtQuick 2.0
import QtQuick.Layouts 1.11
import QtQuick.Controls 2.4
import "AAHeadModule"
import "DispenseModule"
import "SUTModule"
import "ThreadManager"
import "SingleStationMaterialLoaderModule"
import "DOE"

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

        Switch {id: materialLoaderViewSwitch; text:"MaterialLoader 配置"; font.bold: true}
        SingleStationMaterialLoaderModuleView{
            Layout.fillWidth: true
            visible: materialLoaderViewSwitch.checked
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

        Switch {id: systemViewSwitch; text:"System 配置"; font.bold: true}
        SystemConfigView{
            Layout.fillWidth: true
            visible: {systemViewSwitch.checked}
        }



        Switch {id: threadMgrSwitch; text:"Thread Manager"; font.bold: true}
        ThreadManagerView{
            Layout.fillWidth: true
            visible: {threadMgrSwitch.checked}
        }

//        Switch {id: commonCtrlSwitch; text:"常用操作"; font.bold: true}
//        CommonControl{
//            Layout.fillWidth: true
//            visible: {commonCtrlSwitch.checked}
//        }

        Switch {id: doeSwitch; text:"DOE"; font.bold: true}
        StartCameraDoe{
            Layout.fillWidth: true
            visible: {doeSwitch.checked}
        }

//        Switch {id: checkProcessSwitch; text:"校正过程"; font.bold: true}
//        CheckProcess{
//            Layout.fillWidth: true
//            visible: {checkProcessSwitch.checked}
//        }
    }
}
