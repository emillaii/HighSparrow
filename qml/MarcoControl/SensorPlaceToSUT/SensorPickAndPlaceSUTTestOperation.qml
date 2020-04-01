import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11
import FileContentItem 1.0
import QtQuick.Dialogs 1.2
import LogicManagerLib 1.1
import SomeLib 1.1
import SutModuleLib 1.1
Column{
    RowLayout {
        Label { text: qsTr("测试") }
    }

    RowLayout {
        Label {
            text: qsTr("料盘1")
        }
        Label {
            text: qsTr("Row")
        }
        TextField {
            id:t_nrow1
            text: "1"
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
        }
        Label {
            text: qsTr("Col")
        }
        TextField {
            text: "1"
            id:t_ncol1
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
        }
        Button {
            text: title_move_to
            width: 40
            height: 40
            onClicked: {
                material_tray.setTrayCurrent(t_ncol1.text-1,t_nrow1.text-1,0)
                logicManager.performHandling(sensorLoaderParameter.moduleName,SensorLoaderModule.SENSOR_TRAY_1_POS)
            }
        }

        Button {
            text: qsTr("Pick")
            onClicked: {
                material_tray.setTrayCurrent(t_ncol1.text-1,t_nrow1.text-1,0)
                logicManager.performHandling(sensorLoaderParameter.moduleName,SensorLoaderModule.SENSOR_TRAY_1_POS
                                             +SensorLoaderModule.TRAY_SENSOR_PR
                                             +SensorLoaderModule.TO_PICK_SENSOR_POS1
                                             +SensorLoaderModule.PICK_SENSOR_FROM_TRAY1)
            }
        }

        Button {
            text: qsTr("Place")
            onClicked: {
                material_tray.setTrayCurrent(t_ncol1.text-1,t_nrow1.text-1,0)
                sensorLoaderModule.performHandling(SensorLoaderModule.SENSOR_TRAY_1_POS
                                                   +SensorLoaderModule.TRAY_EMPTY_PR
                                                   +SensorLoaderModule.TO_PICK_SENSOR_POS1
                                                   +SensorLoaderModule.PLACE_SENSOR_BACK_TO_TRAY1)
            }
        }

        Button {
            text: qsTr("Place NG Sensor")
            onClicked: {
                material_tray.setTrayCurrent(t_ncol1.text-1,t_nrow1.text-1,0)
                sensorLoaderModule.performHandling(SensorLoaderModule.NG_SENSOR_TRAY_POS
                                                   +SensorLoaderModule.TRAY_EMPTY_PR
                                                   +SensorLoaderModule.TO_PLACE_NG_POS
                                                   +SensorLoaderModule.PLACE_NG_SENSOR_TO_TRAY)
            }
        }

        Button {
            text: qsTr("Place Product")
            onClicked: {
                material_tray.setTrayCurrent(t_ncol1.text-1,t_nrow1.text-1,0)
                sensorLoaderModule.performHandling(SensorLoaderModule.SENSOR_TRAY_1_POS
                                                   +SensorLoaderModule.TRAY_EMPTY_PR
                                                   +SensorLoaderModule.TO_PLACE_PRODUCT_POS1
                                                   +SensorLoaderModule.PLACE_PRODUCT_TO_TRAY1)
            }
        }

        Label {
            text: qsTr("料盘2")
        }
        Label {
            text: qsTr("Row")
        }
        TextField {
            text: "1"
            id:t_nrow2
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
        }
        Label {
            text: qsTr("Col")
        }
        TextField {
            text: "1"
            id:t_ncol2
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
        }
        Button {
            text: title_move_to
            width: 40
            height: 40
            onClicked: {
                material_tray.setTrayCurrent(t_ncol2.text-1,t_nrow2.text-1,1)
                logicManager.performHandling(sensorLoaderParameter.moduleName,SensorLoaderModule.SENSOR_TRAY_2_POS)
            }
        }

        Button {
            text: qsTr("Pick")
            onClicked: {
                material_tray.setTrayCurrent(t_ncol2.text-1,t_nrow2.text-1,1)
                logicManager.performHandling(sensorLoaderParameter.moduleName,SensorLoaderModule.SENSOR_TRAY_2_POS
                                             +SensorLoaderModule.TRAY_SENSOR_PR
                                             +SensorLoaderModule.TO_PICK_SENSOR_POS2
                                             +SensorLoaderModule.PICK_SENSOR_FROM_TRAY2)
            }
        }

        Button {
            text: qsTr("Place")
            onClicked: {
                material_tray.setTrayCurrent(t_ncol2.text-1,t_nrow2.text-1,1)
                sensorLoaderModule.performHandling(SensorLoaderModule.SENSOR_TRAY_2_POS
                                                   +SensorLoaderModule.TRAY_EMPTY_PR
                                                   +SensorLoaderModule.TO_PICK_SENSOR_POS2
                                                   +SensorLoaderModule.PLACE_SENSOR_BACK_TO_TRAY2)
            }
        }
        Button {
            text: qsTr("Place NG Sensor")
            onClicked: {
                material_tray.setTrayCurrent(t_ncol1.text-1,t_nrow1.text-1,1)
                sensorLoaderModule.performHandling(SensorLoaderModule.SENSOR_TRAY_2_POS
                                                   +SensorLoaderModule.TRAY_EMPTY_PR
                                                   +SensorLoaderModule.TO_PICK_SENSOR_POS2
                                                   +SensorLoaderModule.PLACE_NG_SENSOR_TO_TRAY)
            }
        }

        Button {
            text: qsTr("Place Product")
            onClicked: {
                material_tray.setTrayCurrent(t_ncol1.text-1,t_nrow1.text-1,1)
                sensorLoaderModule.performHandling(SensorLoaderModule.SENSOR_TRAY_2_POS
                                                   +SensorLoaderModule.TRAY_EMPTY_PR
                                                   +SensorLoaderModule.TO_PLACE_PRODUCT_POS2
                                                   +SensorLoaderModule.PLACE_PRODUCT_TO_TRAY2)
            }
        }
    }

    RowLayout {
        Button {
            text: qsTr("SUT 1 Move To Load Pos")
            onClicked: {
                logicManager.performHandling(tcpSUTParams.moduleName,SutModule.LOAD_POS)
            }
        }
        Button {
            text: qsTr("SUT 1 Place Sensor")
            onClicked: {
                logicManager.performHandling(sensorLoaderParameter.moduleName,SensorLoaderModule.SUT_POS1
                                             +SensorLoaderModule.TO_PLACE_SENSOR_POS1
                                             +SensorLoaderModule.PLACE_SENSOR_TO_SUT1)
            }
        }
        Button {
            text: qsTr("SUT 1 Pick Sensor")
            onClicked: {
                logicManager.performHandling(sensorLoaderParameter.moduleName,SensorLoaderModule.SUT_POS1
                                             +SensorLoaderModule.SUT_SENSOR_PR
                                             +SensorLoaderModule.TO_PICK_NG_SENSOR_POS1
                                             +SensorLoaderModule.PICK_NG_SENSOR_FROM_SUT1)
            }
        }
        Button {
            text: qsTr("SUT 1 Clamp")
            onClicked: {
                baseModuleManager.toogleIoState(tcpSUTParams.tcpSUTPOGOPINName)
            }
        }
        RoundButton{
            background: Rectangle {
                radius: 6
                color: {
                    if (tcpSUTState.tcpPogopinState) return "lightGreen"
                    else return "red"
                }
            }
        }
        Button {
            text: qsTr("SUT 1 VAC")
            onClicked: {
                baseModuleManager.toogleIoState(tcpSUTParams.tcpSUTVaccumName)
            }
        }
        RoundButton{
            background: Rectangle {
                radius: 6
                color: {
                    if (tcpSUTState.tcpVaccumState) return "lightGreen"
                    else return "red"
                }
            }
        }
    }
    RowLayout {
        Button {
            text: qsTr("SUT 2 Move To Load Pos")
            onClicked: {
                logicManager.performHandling(sutParams.moduleName,SutModule.LOAD_POS)
            }
        }
        Button {
            text: qsTr("SUT 2 Place Sensor")
            onClicked: {
                logicManager.performHandling(sensorLoaderParameter.moduleName,SensorLoaderModule.SUT_POS2
                                             +SensorLoaderModule.TO_PLACE_SENSOR_POS2
                                             +SensorLoaderModule.PLACE_SENSOR_TO_SUT2)
            }
        }
        Button {
            text: qsTr("SUT 2 Pick Sensor")
            onClicked: {
                logicManager.performHandling(sensorLoaderParameter.moduleName,SensorLoaderModule.SUT_POS2
                                             +SensorLoaderModule.SUT_SENSOR_PR
                                             +SensorLoaderModule.TO_PICK_NG_SENSOR_POS2
                                             +SensorLoaderModule.PICK_NG_SENSOR_FROM_SUT2)
            }
        }
        Button {
            text: qsTr("SUT 2 Clamp")
            onClicked: {
                baseModuleManager.toogleIoState(sutParams.sutPOGOPINName)
            }
        }
        RoundButton{
            background: Rectangle {
                radius: 6
                color: {
                    if (sutState.pogopinState) return "lightGreen"
                    else return "red"
                }
            }
        }
        Button {
            text: qsTr("SUT 2 VAC")
            onClicked: {
                baseModuleManager.toogleIoState(sutParams.sutVaccumName)
            }
        }
        RoundButton{
            background: Rectangle {
                radius: 6
                color: {
                    if (sutState.vaccumState) return "lightGreen"
                    else return "red"
                }
            }
        }
    }
}
