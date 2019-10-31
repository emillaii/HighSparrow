import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11
import FileContentItem 1.0
import QtQuick.Dialogs 1.2
import LogicManagerLib 1.1
import SomeLib 1.1

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
                sensorLoaderModule.performHandling(SensorLoaderModule.SENSOR_TRAY_1_POS)
                logicManager.performHandling(sensorLoaderParameter.moduleName,SensorLoaderModule.SENSOR_TRAY_1_POS)
            }
        }

        Button {
            text: qsTr("Pick")
        }

        Button {
            text: qsTr("Place")
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
            onEditingFinished: {
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
            onEditingFinished: {
            }
        }
        Button {
            text: title_move_to
            width: 40
            height: 40
            onClicked: {
                lutModule.moveToAA2PickLensPos(false,true)
            }
        }

        Button {
            text: qsTr("Pick")
        }

        Button {
            text: qsTr("Place")
        }
    }

    RowLayout {
        Button {
            text: qsTr("SUT 1 Place Sensor")
        }
        Button {
            text: qsTr("SUT 1 Pick Sensor")
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
                color: "red"
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
                color: "red"
            }
        }
    }
    RowLayout {
        Button {
            text: qsTr("SUT 2 Place Sensor")
        }
        Button {
            text: qsTr("SUT 2 Pick Sensor")
        }
        Button {
            text: qsTr("SUT 2 Clamp")
            onClicked: {
                baseModuleManager.toogleIoState(sutParams.tcpSUTPOGOPINName)
            }
        }
        RoundButton{
            background: Rectangle {
                radius: 6
                color: "red"
            }
        }
        Button {
            text: qsTr("SUT 2 VAC")
            onClicked: {
                baseModuleManager.toogleIoState(sutParams.tcpSUTVaccumName)
            }
        }
        RoundButton{
            background: Rectangle {
                radius: 6
                color: "red"
            }
        }
    }
}
