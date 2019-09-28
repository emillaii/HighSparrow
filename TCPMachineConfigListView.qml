import QtQuick 2.0
import QtQuick.Layouts 1.11
import QtQuick.Controls 2.4
import "qml"
import "qml/TCPAAHeadModule"
import "qml/TCPSUTModule"
import "qml/TCPDispenseModule"
import "qml/TCPLUTModule"
import "qml/TCPSensorTrayLoaderModule"
ScrollView {
    id: tcpMachineConfigListView
    width: 600

    ColumnLayout{
        Switch {
            id: aaViewSwitch
            text:"AA 头机器配置"
        }
        TCPAAHeadModuleView{
            Layout.fillWidth: true
            visible: aaViewSwitch.checked
        }
        Switch {id: sutViewSwitch; text:"SUT 配置"}
        TCPSUTModuleView{
            Layout.fillWidth: true
            visible: sutViewSwitch.checked
        }
        Switch {id: dispensViewSwitch; text:"Dispense 配置"}
        TCPDispenseModuleView{
            Layout.fillWidth: true
            visible: {dispensViewSwitch.checked}
        }
        Switch {id: lutModuleViewSwitch; text:"LUT 配置"}
        TCPLUTModuleView{
            Layout.fillWidth: true
            visible: {
                if (baseModuleManager.getServerMode() === 1)
                    return lutModuleViewSwitch.checked
                else
                    return false
            }
        }
        Switch {
            id: materialLoderViewSwitch;
            text: {
                if (baseModuleManager.getServerMode() === 1)
                    return "Lens Material Loader 配置"
                else
                    return "Sensor Material Loader 配置"
            }
        }
        Loader{
            source: {
                if (baseModuleManager.getServerMode() === 1)
                    return "./qml/TCPLensPickArmModule/TCPLensPickArmModuleView.qml"
//                else
//                    return "SensorLoaderModuleView.qml"

            }
            visible: materialLoderViewSwitch.checked
        }
        Switch {
            id: trayLoderViewSwitch;
            text:{
                if (baseModuleManager.getServerMode() === 1)
                    return "Lens Tray Loader 配置"
                else
                    return "Sensor Tray Loader 配置"
            }
        }
        Loader{
            source: {
                if (baseModuleManager.getServerMode() === 1)
                    return "./qml/TCPTrayLoaderModule/TCPTrayLoaderModuleView.qml"
//                else
//                    return "SensorTrayLoaderViewModule.qml"

            }
            visible: trayLoderViewSwitch.checked
        }
    }
}
