import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11
import FileContentItem 1.0
import QtQuick.Dialogs 1.2
import "../"
ItemDelegate {
    width: parent.width

    contentItem: ColumnLayout{
        spacing: 0
        ListModel{
            id:motorsModel
            ListElement{
                modelData:"TL_X"
            }
            ListElement{
                modelData:"PA_T1"
            }
            ListElement{
                modelData:"PA_T2"
            }
            ListElement{
                modelData:"PA_Z1"
            }
            ListElement{
                modelData:"PA_Z2"
            }
            ListElement{
                modelData:"PA_Y"
            }
            ListElement{
                modelData:"PA_X"
            }
        }
        MotionPopup{
            id:single_station_material_loader_motions
        }
        ColumnLayout{
            RowLayout {
                Label {
                    text: qsTr("")
                }
                Button{
                    text:qsTr("手动操作")
                    onClicked:{
                        single_station_material_loader_motions.open()
                    }
                }
                Button{
                    text:qsTr("换盘位置")
                    width: 40
                    height: 40
                    onClicked: {
                        sh_materialLoaderModule.moveToChangeTrayPos()
                    }
                }
                Button{
                    text:qsTr("towerlight test")
                    width: 40
                    height: 40
                    onClicked: {
                        sh_materialLoaderModule.towerLightBuzzerTest()
                    }
                }
                Button{
                    text:qsTr("CLOSE ALL towerlight")
                    width: 40
                    height: 40
                    onClicked: {
                        sh_materialLoaderModule.closeAllTowerLightBuzzer()
                    }
                }
            }
            GroupBox{
                title:qsTr("头参数")
                MPAHeadParameter{}
            }
            GroupBox{
                title:qsTr("lens盘规格")
                LensTraySpecs{
                    enabled: userManagement.currentAuthority >= 2 //At least engineer authority
                }

            }

            GroupBox{
                title:qsTr("lens盘")
                SingleHeadLensTray{}
            }
            GroupBox{
                title:qsTr("sensor盘规格")
                SensorTraySpecs{
                    enabled: userManagement.currentAuthority >= 2 //At least engineer authority
                }
            }

            GroupBox{
                title:qsTr("sensor盘")
                SingleHeadSensorTray{}
            }
            GroupBox{
                title:qsTr("reject盘规格")
                RejectTraySpecs{
                    enabled: userManagement.currentAuthority >= 2 //At least engineer authority
                }
            }

            GroupBox{
                title:qsTr("reject盘")
                SingleHeadRejectTray{}
            }

            GroupBox{
                title:qsTr("sut和lut操作")
                MaterialLoaderModulueUTView{}
            }
        }
    }
}
