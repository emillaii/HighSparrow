import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11
import SomeLib 1.1
import LogicManagerLib 1.1
import "../BaseUIModule"

ColumnLayout {
   RowLayout{ Label { text: qsTr("取放操作") } }
    RowLayout{
        Label{
            text: qsTr("料盘1位置")
        }
        RowColSelector{
            id: sensorTray1
        }
        Button{
            text: qsTr("Move")
            onClicked: {
                material_tray.setTrayFirst(sensorTray1.txtCol.text-1,sensorTray1.txtRow.text-1,0)
            }
        }
        Button{
            text: qsTr("Pick")
            onClicked: {
                material_tray.setTrayCurrent(sensorTray1.txtCol.text-1,sensorTray1.txtRow.text-1,0)
                logicManager.performHandling(sensorLoaderParameter.moduleName,SensorLoaderModule.SENSOR_TRAY_1_POS
                                             +SensorLoaderModule.TRAY_SENSOR_PR
                                             +SensorLoaderModule.TO_PICK_SENSOR_POS1
                                             +SensorLoaderModule.PICK_SENSOR_FROM_TRAY1)
            }
        }
        Button{
            text: qsTr("Place")
            onClicked: {
                material_tray.setTrayCurrent(sensorTray1.txtCol.text-1,sensorTray1.txtRow.text-1,0)
                sensorLoaderModule.performHandling(SensorLoaderModule.SENSOR_TRAY_1_POS
                                                   +SensorLoaderModule.TRAY_EMPTY_PR
                                                   +SensorLoaderModule.TO_PICK_SENSOR_POS1
                                                   +SensorLoaderModule.PLACE_SENSOR_BACK_TO_TRAY1)
            }
        }
    }

    RowLayout{
        Label{
            text: qsTr("料盘2位置")
        }
        RowColSelector{
            id: sensorTray2
        }
        Button{
            text: qsTr("Move")
            onClicked: {
                material_tray.setTrayFirst(sensorTray2.txtCol.text-1,sensorTray2.txtRow.text-1,1)
            }
        }
        Button{
            text: qsTr("Pick")
            onClicked: {
                material_tray.setTrayCurrent(sensorTray2.txtCol.text-1,sensorTray2.txtRow.text-1,1)
                logicManager.performHandling(sensorLoaderParameter.moduleName,SensorLoaderModule.SENSOR_TRAY_2_POS
                                             +SensorLoaderModule.TRAY_SENSOR_PR
                                             +SensorLoaderModule.TO_PICK_SENSOR_POS2
                                             +SensorLoaderModule.PICK_SENSOR_FROM_TRAY2)
            }
        }
        Button{
            text: qsTr("Place")
            onClicked: {
                material_tray.setTrayCurrent(sensorTray1.txtCol.text-1,sensorTray1.txtRow.text-1,1)
                sensorLoaderModule.performHandling(SensorLoaderModule.SENSOR_TRAY_2_POS
                                                   +SensorLoaderModule.TRAY_EMPTY_PR
                                                   +SensorLoaderModule.TO_PICK_SENSOR_POS2
                                                   +SensorLoaderModule.PLACE_SENSOR_BACK_TO_TRAY2)
            }
        }
    }

    RowLayout{
        Button{
            text: qsTr("SPA Collet VAC")
            onClicked: {
                baseModuleManager.toogleIoState(sensorPickArmParams.spaVaccum1OutIoName)
            }
        }
        RoundButton{
            background: Rectangle {
                radius: 6
                color: {
                    if (sensorLoaderModuleState.spaVaccum1State) return "lightGreen"
                    else return "red"
                }
            }
        }
        Button{
            text: qsTr("CPA Collet VAC")
            onClicked: {
                baseModuleManager.toogleIoState(sensorPickArmParams.spaVaccum2OutIoName)
            }
        }
        RoundButton{
            background: Rectangle {
                radius: 6
                color: {
                    if (sensorLoaderModuleState.spaVaccum2State) return "lightGreen"
                    else return "red"
                }
            }
        }
    }
}
