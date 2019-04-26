import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11
import FileContentItem 1.0
import QtQuick.Dialogs 1.2

ItemDelegate {
    width: parent.width
    contentItem: ColumnLayout {
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
                id: lutViewSwitch
            }
        }
        ColumnLayout {
            visible: lutViewSwitch.checked
            RowLayout {
                Label {
                    text: qsTr("")
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
                title: qsTr("AA_1 Uplook Position")
                LUTAA1UplookPosition {
                }
            }

            GroupBox{
                title: qsTr("AA_1 Pick Lens Position")
                LUTAA1PickLensPosition {
                }
            }
//            GroupBox{
//                title: qsTr("AA_1 UnPick Lens Position")
//                LUTAA1UnpickLensPosition {
//                }
//            }
//            GroupBox{
//                title: qsTr("AA_1 UpDownlook Position")
//                LUTAA1UpDownlookPosition {
//                }
//            }

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
//            GroupBox{
//                title: qsTr("AA_2 UnPick Lens Position")
//                LUTAA2UnPickLensPosition {
//                }
//            }
//            GroupBox{
//                title: qsTr("AA_2 UpDownlook Position")
//                LUTAA2UpDownlookPosition {
//                }
//            }
            GroupBox{
                title: qsTr("LUT Safety Position")
                LUTSafetyPositionView{
                }
            }
        }
    }
}
