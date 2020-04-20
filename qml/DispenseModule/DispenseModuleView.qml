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
                Button {
                    text: qsTr("点胶机调试软件")
                    onClicked: {
                        highSprrow.callQProcess("ML808FX_UI.exe")
                    }
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
                DispenseParam {}
            }
            GroupBox{
                title: qsTr("Dispense X Y Offset")
                DispenseXYOffset {}
            }
            GroupBox{
                title: qsTr("Dispense Z")
                DispenseZPos {}
            }
            GroupBox{
                title: qsTr("Dispenser Param")
                DispenserParam {}
            }
        }
    }
}
