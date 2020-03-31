import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11

Column {
    RowLayout {
        Label { text: qsTr("料盘1位置") }
        Label { text: qsTr("Row") }
        TextField{
           id:t1_nrow
           horizontalAlignment: TextInput.AlignHCenter
           validator: DoubleValidator{
               decimals: 6
               notation: DoubleValidator.StandardNotation
           }
           onEditingFinished: {
               material_tray.setTrayCurrent(t1_ncol.text-1,t1_nrow.text-1,0)
           }
        }
        Label { text: qsTr("Col") }
        TextField{
           id:t1_ncol
           horizontalAlignment: TextInput.AlignHCenter
           validator: DoubleValidator{
               decimals: 6
               notation: DoubleValidator.StandardNotation
           }
           onEditingFinished: {
               material_tray.setTrayCurrent(t1_ncol.text-1,t1_nrow.text-1,0)
           }
        }
        Button {
            text: title_move_to
            onClicked: {
                material_tray.setTrayCurrent(t1_ncol.text-1,t1_nrow.text-1,0)
                logicManager.performHandling(sensorLoaderParameter.moduleName,SensorLoaderModule.SENSOR_TRAY_1_POS)
            }
        }
        Button{
            text: qsTr("Pick Camera")
            onClicked: {
                material_tray.setTrayCurrent(t1_ncol.text-1,t1_nrow.text-1,0)
                logicManager.performHandling(sensorLoaderParameter.moduleName,SensorLoaderModule.SENSOR_TRAY_1_POS
                                             +SensorLoaderModule.TRAY_SENSOR_PR
                                             +SensorLoaderModule.TO_PICK_SENSOR_POS1
                                             +SensorLoaderModule.PICK_SENSOR_FROM_TRAY1)
            }
        }
        Button {
            text: qsTr("Place Camera")
            onClicked: {
                material_tray.setTrayCurrent(t1_ncol.text-1,t1_nrow.text-1,0)
                logicManager.performHandling(sensorLoaderParameter.moduleName,SensorLoaderModule.SENSOR_TRAY_1_POS
                                             +SensorLoaderModule.TRAY_EMPTY_PR
                                             +SensorLoaderModule.TO_PLACE_PRODUCT_POS1
                                             +SensorLoaderModule.PLACE_PRODUCT_TO_TRAY1)
            }
        }
    }

    RowLayout {
        Label { text: qsTr("料盘2位置") }
        Label { text: qsTr("Row") }
        TextField{
           id:t2_nrow
           horizontalAlignment: TextInput.AlignHCenter
           validator: DoubleValidator{
               decimals: 6
               notation: DoubleValidator.StandardNotation
           }
           onEditingFinished: {
               material_tray.setTrayCurrent(t2_ncol.text-1,t2_nrow.text-1,1)
           }
        }
        Label { text: qsTr("Col") }
        TextField{
           id:t2_ncol
           horizontalAlignment: TextInput.AlignHCenter
           validator: DoubleValidator{
               decimals: 6
               notation: DoubleValidator.StandardNotation
           }
           onEditingFinished: {
               material_tray.setTrayCurrent(t2_ncol.text-1,t2_nrow.text-1,1)
           }
        }
        Button {
            text: title_move_to
            onClicked: {
                material_tray.setTrayCurrent(t2_ncol.text-1,t2_nrow.text-1,1)
                logicManager.performHandling(sensorLoaderParameter.moduleName,SensorLoaderModule.SENSOR_TRAY_2_POS)
            }
        }
        Button{
            text: qsTr("Pick Camera")
            onClicked: {
                material_tray.setTrayCurrent(t2_ncol.text-1,t2_nrow.text-1,1)
                logicManager.performHandling(sensorLoaderParameter.moduleName,SensorLoaderModule.SENSOR_TRAY_1_POS
                                             +SensorLoaderModule.TRAY_SENSOR_PR
                                             +SensorLoaderModule.TO_PICK_SENSOR_POS1
                                             +SensorLoaderModule.PICK_SENSOR_FROM_TRAY1)
            }
        }
        Button {
            text: qsTr("Place Camera")
            onClicked: {
                material_tray.setTrayCurrent(t2_ncol.text-1,t2_nrow.text-1,1)
                logicManager.performHandling(sensorLoaderParameter.moduleName,SensorLoaderModule.SENSOR_TRAY_2_POS
                                             +SensorLoaderModule.TRAY_EMPTY_PR
                                             +SensorLoaderModule.TO_PLACE_PRODUCT_POS2
                                             +SensorLoaderModule.PLACE_PRODUCT_TO_TRAY2)
            }
        }
    }

    RowLayout {
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



