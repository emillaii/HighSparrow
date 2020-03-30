import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11
import "../BaseUIModule"
Column {
    RowLayout { Label { text: qsTr("测试") } }
    RowLayout {
        Label { text: qsTr("Buffer盘位置") }
        RowColSelector {
            id: bufferTraySelector
        }
        Button{
            text: title_move_to
            onClicked: {
                material_tray.setTrayCurrent(bufferTraySelector.txtCol.text-1,bufferTraySelector.txtRow.text-1,3)
                logicManager.performHandling(sensorLoaderParameter.moduleName,SensorLoaderModule.BUFFER_TRAY_POS)
            }
        }
    }
    RowLayout {
        Button {
            text: qsTr("Place To Buffer Carrier")
            onClicked: {
                material_tray.setTrayCurrent(bufferTraySelector.txtCol.text-1,bufferTraySelector.txtRow.text-1,3)
                logicManager.performHandling(sensorLoaderParameter.moduleName,SensorLoaderModule.BUFFER_TRAY_POS
                                             +SensorLoaderModule.TRAY_EMPTY_PR
                                             +SensorLoaderModule.TO_PLACE_BUFFER_POS
                                             +SensorLoaderModule.PLACE_PRODUCT_TO_BUFFER_TRAY)
            }
        }
    }

    RowLayout {
        Button {
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
