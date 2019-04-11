import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11
import FileContentItem 1.0
import QtQuick.Dialogs 1.2


ItemDelegate {
    width: parent.width
    contentItem: ColumnLayout {
        RowLayout {
            ColumnLayout {
                RowLayout {
                    Label {
                        id: itemLabel
                        text: model.label
                        font.pixelSize: 20
                    }
                }
            }
            Switch {
                id: sutViewSwitch
            }
        }
        ColumnLayout {
            visible: sutViewSwitch.checked
            RowLayout {
                Label {
                    text: qsTr("")
                }
            }
            GroupBox{
                title: qsTr("Mushroom Position")
                SUTMushroomPosition {}
            }
            GroupBox{
                title: qsTr("Load Position")
                SUTLoadPositionView {}
            }
            GroupBox{
                title: qsTr("PR Position")
                SUTPRPositionView {}
            }
            GroupBox{
                title: qsTr("UpDn DownLook Calibration Position")
                SUTToollookPosition {}
            }
            GroupBox{
                title: qsTr("UpDn UpLook Calibration Position")
                SUTToolUplookPosition {}
            }
            GroupBox{
                title: qsTr("Safety Position")
                SUTSafetyPositionView {}
            }
        }
    }
}
