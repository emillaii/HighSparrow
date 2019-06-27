import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11
import FileContentItem 1.0
import QtQuick.Dialogs 1.2

ItemDelegate {
    width: parent.width

    contentItem: ColumnLayout{
        spacing: 0
        ListModel{
            id:motorsModel
            ListElement{
                modelData:"LTL_X"
            }
            ListElement{
                modelData:"LPA_X"
            }
            ListElement{
                modelData:"LPA_Y"
            }
            ListElement{
                modelData:"LPA_Z"
            }
            ListElement{
                modelData:"LPA_R"
            }
        }
        MotionPopup{
            id:lens_pickup_module_motions
        }

        ColumnLayout{
            RowLayout {
                Label {
                    text: qsTr("")
                }
                Button{
                    text:qsTr("手动操作")
                    onClicked:{
                        lens_pickup_module_motions.open()
                    }
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
