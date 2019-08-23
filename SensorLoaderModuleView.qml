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
                modelData:"SPA_R2"
            }
            ListElement{
                modelData:"SPA_R1"
            }
            ListElement{
                modelData:"SPA_X"
            }
            ListElement{
                modelData:"SPA_Y"
            }
            ListElement{
                modelData:"SPA_Z2"
            }
            ListElement{
                modelData:"SPA_Z1"
            }
        }
        MotionPopup{
            id:sensor_loader_module_motionss
        }

        ColumnLayout{
            RowLayout {
                Label {
                    text: qsTr("")
                }
                Button{
                    text:qsTr("音圈设置窗口")
                    onClicked:{
                        baseModuleManager.showSettingDialog()
                    }
                }
                Button{
                    text:qsTr("手动操作")
                    onClicked:{
                        sensor_loader_module_motionss.open()
                    }
                }
                Button{
                    text: qsTr("load")
                    onClicked: {
                        baseModuleManager.loadSensorLoaderParameter()
                    }
                }
            }
            GroupBox{
                title:qsTr("sensor 头参数")
                SensorHeadParameter{}
            }
            GroupBox{
                title: qsTr("SUT")
                SensorPickArmSUT{}
            }
            GroupBox{
                title:qsTr("sensor 料盘1")
                MaterialTray1{}
            }
            GroupBox{
                title:qsTr("料盘规格")
                TraySpecs{}
            }
            GroupBox{
                title:qsTr("sensor 料盘2")
                MaterialTray2{}
            }
            GroupBox{
                title:qsTr("Ng 盘")
                MaterialTray3{}
            }
            GroupBox{
                title:qsTr("缓存盘")
                MaterialTray4{}
            }
        }
    }
}
