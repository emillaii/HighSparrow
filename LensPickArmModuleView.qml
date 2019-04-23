import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11
import FileContentItem 1.0
import QtQuick.Dialogs 1.2

ItemDelegate {
    width: parent.width

    contentItem: ColumnLayout{
        spacing: 0
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
                id: lensPickArmModuleViewSwitch
            }
        }

        ColumnLayout{
            visible:  lensPickArmModuleViewSwitch.checked
            RowLayout {
                Label {
                    text: qsTr("")
                }
            }

            GroupBox{
                title:qsTr("lens 头参数")
                LensHeadParameter{}
            }

            GroupBox{
                title:qsTr("lens 料盘1")
                MaterialTray1{}
            }
            GroupBox{
                title:qsTr("lens 料盘2")
                MaterialTray2{}
            }
            GroupBox{
                title:qsTr("料盘规格")
                TraySpecs{}
            }
            GroupBox{
                title:qsTr("LUT")
                LensPcikArmLUT{}
            }
        }
    }
}
