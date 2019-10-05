import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11
import QtQuick.Dialogs 1.2
import LogicManagerLib 1.1
import SomeLib 1.1

ColumnLayout{
    RowLayout{ Label { text: qsTr("AA取放参数") } }

    AAPickParameter {}
    AAPlaceParameter {}
    AA1PickAndPlacePosition {}
    AA2PickAndPlacePosition {}

    RowLayout {
        Label { text: qsTr("AA Head Pick LUT Safety Position") }
    }
    RowLayout {
        Label { text: qsTr("LUT Safety Position") }
        Label {
            text: qsTr("X")
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
            text: qsTr("Y")
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
            text: qsTr("Z")
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
    }

    AA1TestOperation {}
    AA2TestOperation {}

    RowLayout {
        Button{ text: qsTr("LUT Pocket 1 VAC") }
        RoundButton{
            background: Rectangle {
                radius: 6
                color: "red"
            }
        }
        Button{ text: qsTr("LUT Pocket 2 VAC") }
        RoundButton{
            background: Rectangle {
                radius: 6
                color: "red"
            }
        }
        Button{ text: qsTr("Gripper Open/Closed") }
        RoundButton{
            background: Rectangle {
                radius: 6
                color: "red"
            }
        }
    }
}
