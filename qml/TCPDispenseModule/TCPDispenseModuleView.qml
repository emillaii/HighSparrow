import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11
import FileContentItem 1.0
import QtQuick.Dialogs 1.2


ItemDelegate {
    width: parent.width
    contentItem: ColumnLayout {
        ColumnLayout {
            RowLayout {
                Label {
                    text: qsTr("")
                }
            }
            Button{
                text: qsTr("打开点胶调试软件")
                onClicked: {
                    highSprrow.callQProcess("ML808FX_UI.exe")
                }
            }

            GroupBox{
                title: qsTr("Dispense Param")
                TCPDispenseParam {}
            }
            GroupBox{
                title: qsTr("Dispense X Y Offset")
                TCPDispenseXYOffset {}
            }
            GroupBox{
                title: qsTr("Dispense Z")
                TCPDispenseZPos {}
            }
            GroupBox{
                title: qsTr("Dispenser Param")
                TCPDispenserParam {}
            }
        }
    }
}
