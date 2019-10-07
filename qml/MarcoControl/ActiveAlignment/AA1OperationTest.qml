import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11
import QtQuick.Dialogs 1.2
import LogicManagerLib 1.1
import SomeLib 1.1

Column {
    RowLayout {
        Label { text: qsTr("AA 1 测试") }
    }

    RowLayout {
        Button {
            text: qsTr("SUT 1 Load Position")
        }
        Button {
            text: qsTr("Place Sensor To SUT 1")
        }
        Button {
            text: qsTr("Pick Sensor From SUT 1")
        }
        Button {
            text: qsTr("AA 1 Head Pick Lens Position")
        }
        Button {
            text: qsTr("AA 1 Head Pick Lens")
        }
    }
    RowLayout {
        Button {
            text: qsTr("AA1 Head Place Lens")
        }
        Button {
            text: qsTr("AA1 Dispense PR")
        }
        Button {
            text: qsTr("AA1 Lens Up Look PR")
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
            text: qsTr("SUT 1 VAC")
        }
        RoundButton{
            background: Rectangle {
                radius: 6
                color: "red"
            }
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
            text: qsTr("AA 1 Gripper")
        }
        RoundButton{
            background: Rectangle {
                radius: 6
                color: "red"
            }
        }
    }
}
