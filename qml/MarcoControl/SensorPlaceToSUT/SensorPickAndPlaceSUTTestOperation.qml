import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11
import FileContentItem 1.0
import QtQuick.Dialogs 1.2

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
            }
        }

        Button {
            text: qsTr("Pick")
        }

        Button {
            text: qsTr("Place")
        }

        //-------------------
        Label {
            text: qsTr("料盘2")
        }
        Label {
            text: qsTr("Row")
        }
        TextField {
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
        }
        RoundButton{
            background: Rectangle {
                radius: 6
                color: "red"
            }
        }
        Button {
            text: qsTr("SUT 1 VAC")
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
        }
        RoundButton{
            background: Rectangle {
                radius: 6
                color: "red"
            }
        }
        Button {
            text: qsTr("SUT 2 VAC")
        }
        RoundButton{
            background: Rectangle {
                radius: 6
                color: "red"
            }
        }
    }
}
