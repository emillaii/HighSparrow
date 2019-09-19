import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11
import FileContentItem 1.0
import QtQuick.Dialogs 1.2
import "../../"
ItemDelegate {
    contentItem: ColumnLayout {
        spacing: 0
        ListModel{
            id:motorsModel
            ListElement{
                modelData:"AA2_X"
            }
            ListElement{
                modelData:"AA2_Y"
            }
            ListElement{
                modelData:"AA2_Z"
            }
            ListElement{
                modelData:"AA2_A"
            }
            ListElement{
                modelData:"AA2_B"
            }
            ListElement{
                modelData:"AA2_C"
            }
        }

        MotionPopup{
            id:aa_head_motions
        }

        ColumnLayout {
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
                TCPAAHeadMushroomPosition {}
            }
            GroupBox{
                title: qsTr("Pick Lens Position")
                TCPAAHeadPickLensPosition {}
            }
        }
    }
}
