import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11
import TrayLoaderModuleLib 1.1
ColumnLayout{
    RowLayout{ Label { text: qsTr("取放操作") } }
    RowLayout{
        Button{
            text: qsTr("SUT1 Place sensor")
        }
        Button{
            text: qsTr("SUT1 Pick sensor")
        }
        Button{
            text: qsTr("SUT1 Clamp")
        }
        RoundButton{
            background: Rectangle {
                radius: 6
                color: "red"
            }
        }
        Button{
            text: qsTr("SUT1 VAC")
        }
        RoundButton{
            background: Rectangle {
                radius: 6
                color: "red"
            }
        }
    }
    RowLayout{
        Button{
            text: qsTr("SUT2 Place sensor")
        }
        Button{
            text: qsTr("SUT2 Pick sensor")
        }
        Button{
            text: qsTr("SUT2 Clamp")
        }
        RoundButton{
            background: Rectangle {
                radius: 6
                color: "red"
            }
        }
        Button{
            text: qsTr("SUT2 VAC")
        }
        RoundButton{
            background: Rectangle {
                radius: 6
                color: "red"
            }
        }
    }
}
