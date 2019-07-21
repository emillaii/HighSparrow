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
