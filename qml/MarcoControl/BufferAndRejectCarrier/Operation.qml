import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11
import SomeLib 1.1
import "../BaseUIModule"

ColumnLayout {
    RowLayout{ Label { text: qsTr("取放操作") } }
    RowLayout{
        Label{
            text: qsTr("Buffer盘位置")
        }
        RowColSelector{
            id: bufferTraySelector
        }
        Button{
            text: qsTr("Move")
            onClicked: {
                material_tray.setTrayCurrent(bufferTraySelector.txtCol.text-1,bufferTraySelector.txtRow.text-1,3)
                logicManager.performHandling(sensorLoaderParameter.moduleName,SensorLoaderModule.BUFFER_TRAY_POS)
            }
        }
        Button{
            text: qsTr("Place")
            onClicked: {
                material_tray.setTrayCurrent(bufferTraySelector.txtCol.text-1,bufferTraySelector.txtRow.text-1,3)
                logicManager.performHandling(sensorLoaderParameter.moduleName,SensorLoaderModule.BUFFER_TRAY_POS
                                             +SensorLoaderModule.TRAY_EMPTY_PR
                                             +SensorLoaderModule.TO_PLACE_BUFFER_POS
                                             +SensorLoaderModule.PLACE_PRODUCT_TO_BUFFER_TRAY)
            }
        }
    }

    RowLayout{
        Label{
            text: qsTr("Reject盘位置")
        }
        RowColSelector{
            id: rejectTraySelector
        }
        Button{
            text: qsTr("Move")
            onClicked: {
                material_tray.setTrayCurrent(rejectTraySelector.txtCol.text-1,rejectTraySelector.txtRow.text-1,2)
                logicManager.performHandling(sensorLoaderParameter.moduleName,SensorLoaderModule.NG_SENSOR_TRAY_POS)
            }
        }
        Button{
            text: qsTr("Place")
            onClicked: {
                material_tray.setTrayCurrent(rejectTraySelector.txtCol.text-1,rejectTraySelector.txtRow.text-1,2)
                logicManager.performHandling(sensorLoaderParameter.moduleName,SensorLoaderModule.NG_SENSOR_TRAY_POS
                                             +SensorLoaderModule.TRAY_EMPTY_PR
                                             +SensorLoaderModule.TO_PLACE_NG_POS
                                             +SensorLoaderModule.PLACE_NG_PRODUCT_TO_TRAY)
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
