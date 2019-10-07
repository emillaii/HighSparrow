import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11
import QtQuick.Dialogs 1.2
import LogicManagerLib 1.1
import SomeLib 1.1

ColumnLayout {
    AA1DispensingSetting {}
    AA2DispensingSetting {}

    RowLayout {
        Button { text: qsTr("AA1 画胶PR位置") }

        Button { text: qsTr("AA1 点胶") }

        Button { text: qsTr("AA1 去UV") }

        Button { text: qsTr("SUT 1 VAC") }

        RoundButton{
            background: Rectangle {
                radius: 6
                color: "red"
            }
        }
        Button { text: qsTr("SUT 1 Clamp") }
        RoundButton{
            background: Rectangle {
                radius: 6
                color: "red"
            }
        }
    }

    RowLayout {
        Button { text: qsTr("AA2 画胶PR位置") }

        Button { text: qsTr("AA2 点胶") }

        Button { text: qsTr("AA2 去UV") }

        Button { text: qsTr("SUT 2 VAC") }

        RoundButton{
            background: Rectangle {
                radius: 6
                color: "red"
            }
        }
        Button { text: qsTr("SUT 2 Clamp") }
        RoundButton{
            background: Rectangle {
                radius: 6
                color: "red"
            }
        }
    }
}
