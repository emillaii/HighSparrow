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
                title: qsTr("点胶头参数")
                DispenseParam {}
            }
            GroupBox{
                title: qsTr("视觉到点胶位置偏移")
                DispenseXYOffset {}
            }
            GroupBox{
                title: qsTr("点胶高度")
                DispenseZPos {}
            }
            GroupBox{
                title: qsTr("点胶轨迹参数")
                DispenserParam {}
            }
        }
    }
}
