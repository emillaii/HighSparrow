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
                modelData:"AA_A"
            }
            ListElement{
                modelData:"AA_B"
            }
            ListElement{
                modelData:"AA_C"
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
        }
    }
}

