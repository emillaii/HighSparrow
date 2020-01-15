import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11
import FileContentItem 1.0
import QtQuick.Dialogs 1.2
import "../../"
import "../Drivers/LightSourceController"

ItemDelegate {
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
        SciencaLightSourceControllerView{
            id: slsc
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
                    enabled: userManagement.currentAuthority >= 2 // At least engineer autority
                }
                Button{
                    text:qsTr("背光板操作")
                    onClicked:{
                        slsc.open()
                    }
                }
                Button{
                    text:qsTr("加载")
                    onClicked: {
                        baseModuleManager.loadAAHeadParameter()
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

//            // Comment out, not used
//            GroupBox{
//                title: qsTr("OC Position")
//                AAHeadOCPosition {}
//            }
        }
    }
}
