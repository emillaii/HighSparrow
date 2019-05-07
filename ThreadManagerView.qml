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
                Label {
                    text: qsTr("")
                }
            }
            GroupBox
            {
                visible: baseModuleManager.ServerMode
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
                            workersManager.startWorker("SutModule",true,2)
                            workersManager.startWorker("SensorLoader",true,2)
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
                visible: !baseModuleManager.ServerMode
                title: qsTr("Lens")
                RowLayout{
                    Button {
                        text: "开始"
                        onClicked: {
                            workersManager.startWorker("LensPickArmModule",true,2)
                            workersManager.startWorker("LUTModule",true,2)
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
        }
    }
}
