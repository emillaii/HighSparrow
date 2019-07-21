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
                modelData:"LUT_X"
            }
            ListElement{
                modelData:"LUT_Y"
            }
            ListElement{
                modelData:"LUT_Z"
            }
        }
        MotionPopup{
            id:lut_model_motions
        }

        ColumnLayout {
            RowLayout {
                Label {
                    text: qsTr("")
                }
                Button{
                    text:qsTr("手动操作")
                    onClicked:{
                        lut_model_motions.open()
                    }
                }
            }
            GroupBox{
                title: qsTr("Load Position")
                LUTLoadPositionView {
                }
            }

            GroupBox{
                title: qsTr("Unload Position")
                LUTUnloadPosition {
                }
            }

            GroupBox{
                title: qsTr("Load Uplook Position")
                LUTLoadUplookPosition {
                }
            }

            GroupBox{
                title: qsTr("Downlook Load Position")
                LUTDownlookLoadPosition {
                }
            }

            GroupBox{
                title: qsTr("Downlook Unload Position")
                LUTDownlookUnloadPosition {
                }
            }

            GroupBox{
                title: qsTr("AA_1 Uplook Position")
                LUTAA1UplookPosition {
                }
            }

            GroupBox{
                title: qsTr("AA_1 Pick Lens Position")
                LUTAA1PickLensPosition {
                }
            }
            GroupBox{
                title: qsTr("AA_1 UnPick Lens Position")
                LUTAA1UnpickLensPosition {
                }
            }
            GroupBox{
                title: qsTr("AA_2 Uplook Position")
                LUTAA2UplookPosition {
                }
            }

            GroupBox{
                title: qsTr("AA_2 Pick Lens Position")
                LUTAA2PickLensPosition {
                }
            }
            GroupBox{
                title: qsTr("AA_2 UnPick Lens Position")
                LUTAA2UnPickLensPosition {
                }
            }
            GroupBox{
                title: qsTr("LUT Safety Position")
                LUTSafetyPositionView{
                }
            }
        }
    }
}
