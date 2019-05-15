import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11
import FileContentItem 1.0
import QtQuick.Dialogs 1.2

ItemDelegate {
    width: parent.width
    contentItem: ColumnLayout {
        spacing: 0
        ListModel{
            id:motorsModel
            ListElement{
                modelData:"AA1_X"
            }
            ListElement{
                modelData:"AA1_Y"
            }
            ListElement{
                modelData:"AA1_Z"
            }
            ListElement{
                modelData:"AA1_A"
            }
            ListElement{
                modelData:"AA1_B"
            }
            ListElement{
                modelData:"AA1_C"
            }
        }
        MotionPopup{
            id:aa_head_motions
        }
        RowLayout {
            ColumnLayout {
                RowLayout {
                    Label {
                        text: model.label
                        font.pixelSize: 20
                    }
                }
            }
            Switch {
                id: aaViewSwitch
            }
        }

        ColumnLayout {
            visible: aaViewSwitch.checked
            RowLayout {
                Label {
                    text: qsTr("")
                }
                Button{
                    text:qsTr("手动操作")
                    onClicked:{
                        aa_head_motions.open()
                    }
                }
            }
            GroupBox{
                title: qsTr("Mushroom Position")
                AAHeadMushroomPosition {}
            }

            GroupBox{
                title: qsTr("Pick Lens Position")
                AAHeadPickLensPosition {}
            }

            GroupBox{
                title: qsTr("OC Position")
                AAHeadOCPosition {}
            }
        }
    }
}

