import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11
import FileContentItem 1.0
import QtQuick.Dialogs 1.2

ItemDelegate {
    width: parent.width
    contentItem: ColumnLayout {
        ListModel{
            id:motorsModel
            ListElement{
                modelData:"SUT_X"
            }
            ListElement{
                modelData:"SUT_Y"
            }
            ListElement{
                modelData:"SUT_Z"
            }
        }
        MotionPopup{
            id:sut_model_motions
        }
        ColumnLayout {
            RowLayout {
                Label {
                    text: qsTr("")
                }
                Button{
                    text:qsTr("手动操作")
                    onClicked:{
                        sut_model_motions.open()
                    }
                }
            }

            GroupBox{
                title: qsTr("Mushroom Position")
                SUTMushroomPosition {}
            }
            GroupBox{
                title: qsTr("Gripper Position")
                SUTGripperPosition {}
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
                title: qsTr("Calibration Position")
                SUTToollookPosition {}
            }
            GroupBox{
                title: qsTr("Safety Position")
                SUTSafetyPositionView {}
            }
            GroupBox{
                title: qsTr("Pick Lens Position")
                SUTPicklensPosition {}
            }
            GroupBox{
                title: qsTr("Unpick Lens Position")
                SUTUnPicklensPosition {}
            }
        }
    }
}
