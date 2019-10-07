import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11
import QtQuick.Dialogs 1.2
import LogicManagerLib 1.1
import SomeLib 1.1

Column {
    RowLayout {
        Label { text: qsTr("AA 2 测试") }
    }

    RowLayout {
        Button {
            text: qsTr("SUT 2 Load Position")
        }
        Button {
            text: qsTr("Place Sensor To SUT 2")
        }
        Button {
            text: qsTr("Pick Sensor From SUT 2")
        }
        Button {
            text: qsTr("AA 2 Head Pick Lens Position")
        }
        Button {
            text: qsTr("AA 2 Head Pick Lens")
        }
    }
    RowLayout {
        Button {
            text: qsTr("AA 2 Head Place Lens")
        }
        Button {
            text: qsTr("AA 2 Dispense PR")
        }
        Button {
            text: qsTr("AA 2 Lens Up Look PR")
        }
        Button {
            text: qsTr("PR To Bond")
        }
        Button {
            text: qsTr("AA Batch Run")
        }
    }

    RowLayout {
        Button {
            text: qsTr("SUT 2 VAC")
        }
        RoundButton{
            background: Rectangle {
                radius: 6
                color: "red"
            }
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
            text: qsTr("LUT Pocket 1 VAC")
        }
        RoundButton{
            background: Rectangle {
                radius: 6
                color: "red"
            }
        }
        Button {
            text: qsTr("LUT Pocket 2 VAC")
        }
        RoundButton{
            background: Rectangle {
                radius: 6
                color: "red"
            }
        }
        Button {
            text: qsTr("AA 2 Gripper")
        }
        RoundButton{
            background: Rectangle {
                radius: 6
                color: "red"
            }
        }
    }
}
