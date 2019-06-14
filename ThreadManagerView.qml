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
                        baseModuleManager.generateConfigFiles()
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

            GroupBox{
                title:qsTr("系统操作")
                ColumnLayout {
                    RowLayout {
                        Label {
                            text: qsTr("运行模式")
                        }
                        TextField {
                            text: systerm_param.runMode
                            onEditingFinished: {
                                systerm_param.setRunMode(text);
                            }
                        }
                        Button{
                            text: qsTr("重置逻辑")
                            onClicked:{
                                workersManager.resetLogics()
                            }
                        }
                    }
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
                            workersManager.startWorker(sutParams.moduleName,systerm_param.runMode)
                            workersManager.startWorker("SensorLoader",systerm_param.runMode)
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
                            workersManager.startWorker("LensPickArmModule",systerm_param.runMode)
                            workersManager.startWorker("LUTModule",systerm_param.runMode)
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
                        text: "加载"
                        onClicked: {
                            baseModuleManager.loadSensorTrayLoaderMuduleParameter()
                        }
                    }
                    Button {
                        text: "开始"
                        onClicked: {
                            workersManager.startWorker("SensorTrayLoaderModule",systerm_param.runMode)
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
                        text: qsTr("加载")
                        onClicked: {
                            baseModuleManager.loadLensTrayLoaderParameter()
                        }
                    }
                    Button{
                        enabled: true
                        id:start_btn
                        text:qsTr("开始")
                        onClicked:{
                            workersManager.startWorker("TrayLoaderModule",systerm_param.runMode)
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
        UPHView{}
        }
    }
}
