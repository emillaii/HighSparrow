import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11
import QtQuick.Dialogs 1.2
import LogicManagerLib 1.1
import SomeLib 1.1

ColumnLayout {
    RowLayout {
        Label { text: qsTr("取放参数") }
    }
    SensorPickAndPlaceFromCarrierParameterView {}
    RowLayout {
        Label { text: qsTr("Sensor Collet Pick Offset") }
        Label{
            text:qsTr("X")
        }
        TextField{
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator{
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
            }
        }
        Label{
            text:qsTr("Y")
        }
        TextField{
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator{
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
            }
        }
        Label{
            text:qsTr("Theta")
        }
        TextField{
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator{
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
            }
        }
        Button{
            text:title_read_encoder
            width: 40
            height: 40
            onClicked: {
            }
        }
        Button{
            text:title_move_to
            width: 40
            height: 40
            onClicked: {
            }
        }
    }

    RowLayout {
        Label { text: qsTr("Carrier 1 Pick And Place 高度") }
        TextField {
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator{
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
            }
        }
        Button {
            text: qsTr("sensor视觉")
        }
        Button {
            text: qsTr("移动到取放位置")
        }
        Button {
            text: qsTr("测高")
        }

    }
    RowLayout {
        Label { text: qsTr("Carrier 2 Pick And Place 高度") }
        TextField {
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator{
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
            }
        }
        Button {
            text: qsTr("sensor视觉")
        }
        Button {
            text: qsTr("移动到取放位置")
        }
        Button {
            text: qsTr("测高")
        }
    }
    SensorCarrierInfoAndPositionTestOperation {}
    RowLayout {
        Button {
            text: qsTr("Carrier 1 Pick Sensor")
        }
        Button {
            text: qsTr("Carrier 1 Place Sensor")
        }
    }
    RowLayout {
        Button {
            text: qsTr("Carrier 2 Pick Sensor")
        }
        Button {
            text: qsTr("Carrier 2 Place Sensor")
        }
    }
    RowLayout{
        Button{ text: qsTr("SPA Collet VAC") }
        RoundButton{
            background: Rectangle {
                radius: 6
                color: "red"
            }
        }
    }
}
