import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11
import FileContentItem 1.0
import QtQuick.Dialogs 1.2

ItemDelegate {
    width: parent.width

    contentItem: ColumnLayout{
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
                id: sensorLoaderModuleViewSwitch
            }
        }

        ColumnLayout{
            visible:  sensorLoaderModuleViewSwitch.checked
            RowLayout {
                Button{
                    text: qsTr("生成参数")
                    onClicked: {
                        baseModuleManager.generatefileConfigs()
                    }
                }
                Button{
                    text: qsTr("显示界面")
                    onClicked:
                    {

                    }
                }

                Label {
                    text: qsTr("")
                }
            }
            GroupBox
            {
                visible: baseModuleManager.getServerMode()
                title: qsTr("Sensor")
                RowLayout{
                    Button{
                        text: qsTr("load")
                        onClicked: {
                            baseModuleManager.loadSensorLoaderParameter()
                        }
                    }
                    Button {
                        text: "开始"
                        onClicked: {
                            workersManager.startWorker("SutModule",0)
                            workersManager.startWorker("SensorLoader",0)
                        }
                    }
                    Button {
                        text: "放"
                        onClicked: {
                            baseModuleManager.sendLoadSensor(false,false)
                        }
                    }
                    Button {
                        text: "放 取ng"
                        onClicked: {
                            baseModuleManager.sendLoadSensor(false,true)
                        }
                    }
                    Button {
                        text: "放 取成品"
                        onClicked: {
                            baseModuleManager.sendLoadSensor(true,false)
                        }
                    }
                    Button {
                        text: "停止"
                        onClicked: {
                            workersManager.stopWorkers()
                        }
                    }
                }
            }

            GroupBox
            {
                visible: !baseModuleManager.getServerMode()
                title: qsTr("Lens")
                RowLayout{
                    Button {
                        text: "开始"
                        onClicked: {
                            workersManager.startWorker("LensPickArmModule",0)
                            workersManager.startWorker("LUTModule",0)
                        }
                    }
                    Button {
                        text: "放"
                        onClicked: {
                            lutClient.sendLensRequest(false,false)
//                            baseModuleManager.sendLoadLens(false)
                        }
                    }
                    Button {
                        text: "放 取ng"
                        onClicked: {
                            lutClient.sendLensRequest(true,false)
//                            baseModuleManager.sendLoadLens(true)
                        }
                    }
                    Button {
                        text: "停止"
                        onClicked: {
                            workersManager.stopWorkers()
                        }
                    }
                }
            }
            GroupBox{
                visible: baseModuleManager.getServerMode()
                title: qsTr("SensorTryLoader")
                RowLayout{
                    Button {
                        text: "Load"
                        onClicked: {
                            baseModuleManager.loadSensorTrayLoaderMuduleParameter()
                        }
                    }
                    Button {
                        text: "开始"
                        onClicked: {
                            workersManager.startWorker("SensorTrayLoaderModule",0)
                        }
                    }
                    Button {
                        text: "换盘"
                        onClicked: {
                            baseModuleManager.sendChangeSensorTray()
                        }
                    }
                    Button {
                        text: "停止"
                        onClicked: {
                            workersManager.stopWorkers()
                        }
                    }
                }
            }
            GroupBox{
                visible: {
                    return !baseModuleManager.getServerMode()
                }
                title:qsTr("TrayLoader")
                RowLayout{
                    Button{
                        enabled: true
                        id:start_btn
                        text:qsTr("开始")
                        onClicked:{
                            workersManager.startWorker("TrayLoaderModule",2)
                        }
                    }
                    Button{
                        enabled: true
                        id:change_tray
                        text:qsTr("换盘")
                        onClicked:{
                            logicManager.trayLoaderModuleEmitTestTrayUsed()
                        }
                    }
                    Button{
                        enabled: true
                        id:end_btn
                        text:qsTr("结束")
                        onClicked: {
                            workersManager.stopWorkers()
                        }
                    }
                }
            }
        }
    }
}
