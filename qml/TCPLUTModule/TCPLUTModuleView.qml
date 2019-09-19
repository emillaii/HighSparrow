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
                TCPLUTLoadPositionView {
                }
            }

            GroupBox{
                title: qsTr("Unload Position")
                TCPLUTUnloadPosition {
                }
            }

            GroupBox{
                title: qsTr("Load Uplook Position")
                TCPLUTLoadUplookPosition {
                }
            }

            GroupBox{
                title: qsTr("Downlook Load Position")
                TCPLUTDownlookLoadPosition {
                }
            }

            GroupBox{
                title: qsTr("Downlook Unload Position")
                TCPLUTDownlookUnloadPosition {
                }
            }

            GroupBox{
                title: qsTr("AA_1 Uplook Position")
                TCPLUTAA1UplookPosition {
                }
            }

            GroupBox{
                title: qsTr("AA_1 Pick Lens Position")
                TCPLUTAA1PickLensPosition {
                }
            }
            GroupBox{
                title: qsTr("AA_1 UnPick Lens Position")
                TCPLUTAA1UnpickLensPosition {
                }
            }
            GroupBox{
                title: qsTr("AA_2 Uplook Position")
                TCPLUTAA2UplookPosition {
                }
            }

            GroupBox{
                title: qsTr("AA_2 Pick Lens Position")
                TCPLUTAA2PickLensPosition {
                }
            }
            GroupBox{
                title: qsTr("AA_2 UnPick Lens Position")
                TCPLUTAA2UnPickLensPosition {
                }
            }
            GroupBox{
                title: qsTr("LUT Safety Position")
                TCPLUTSafetyPositionView{
                }
            }
        }
    }
}
