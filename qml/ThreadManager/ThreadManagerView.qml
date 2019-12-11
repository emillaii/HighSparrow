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

        ColumnLayout{
            RowLayout {
                Button{
                    text: qsTr("生成参数")
                    onClicked: {
                        baseModuleManager.generateConfigFiles()
                    }
                }
                Button{
                    text: qsTr("保存状态")
                    onClicked: {
                        baseModuleManager.saveStates()
                    }
                }
                Button{
                    text: qsTr("显示界面")
                    onClicked:
                    {
                        baseModuleManager.testPRTest()
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
                        ComboBox {
                            id: aaCoreProfile
                            Layout.preferredWidth: 200
                            model: [ qsTr("常规模式"), qsTr("空跑模式"), qsTr("振动测试模式"), "流程图测试模式" ]
                            currentIndex: moduleManagerParam.runMode
                            onCurrentIndexChanged: {
                                moduleManagerParam.setRunMode(currentIndex)
                            }
                        }
                        Button{
                            text: qsTr("重置逻辑")
                            onClicked:{
                                workersManager.resetLogics()
                            }
                        }
                    }

                    RowLayout {
                        CheckBox {
                            text: qsTr("禁用工位");
                            checked: moduleManagerParam.disableStation
                            onClicked: {
                                moduleManagerParam.setDisableStation(checked)
                            }
                        }
                        CheckBox {
                            visible: baseModuleManager.getServerMode() === 0
                            text: qsTr("手动换Lens盘");
                            checked: tray_loader_module_parameters.handlyChangeLensTray
                            onClicked: {
                                tray_loader_module_parameters.setHandlyChangeLensTray(checked)
                            }
                        }
                        CheckBox {
                            visible: baseModuleManager.getServerMode() === 1
                            text: qsTr("手动换Sensor盘");
                            checked: sensor_tray_loader_module_parameter.handlyChangeSensorTray
                            onClicked: {
                                sensor_tray_loader_module_parameter.setHandlyChangeSensorTray(checked)
                            }
                        }
                    }
                    RowLayout {
                        CheckBox {
                            text: qsTr("任务累计计数");
                            checked: aaCoreParams.taskAccumulate
                            onClicked: {
                                aaCoreParams.setTaskAccumulate(checked)
                            }
                        }
                    }
                    RowLayout {
                        RadioButton
                        {
                            id:fsj_task
                            text: qsTr("一次首件")
                            checked: aaCoreParams.taskMode === 1
                            onClicked: {
                                if(checked)
                                    aaCoreParams.setTaskMode(1);
                            }
                        }
                        TextField {
                            horizontalAlignment: TextInput.AlignHCenter
                            text: aaCoreParams.taskNumberFSJ
                            onEditingFinished: {
                                aaCoreParams.setTaskNumberFSJ(text);
                            }
                        }
                    }
                    RowLayout {
                        RadioButton
                        {
                            id:ssj_task
                            text: qsTr("多次首件")
                            checked: aaCoreParams.taskMode === 2
                            onClicked: {
                                if(checked)
                                    aaCoreParams.setTaskMode(2);
                            }
                        }
                        TextField {
                            horizontalAlignment: TextInput.AlignHCenter
                            text: aaCoreParams.taskNumberSSJ
                            onEditingFinished: {
                                aaCoreParams.setTaskNumberSSJ(text);
                            }
                        }
                    }
                    RowLayout {
                        RadioButton
                        {
                            id:normal_task
                            text: qsTr("一般打料")
                            checked: aaCoreParams.taskMode === 3
                            onClicked: {
                                if(checked)
                                    aaCoreParams.setTaskMode(3);
                            }
                        }
                        TextField {
                            horizontalAlignment: TextInput.AlignHCenter
                            text: aaCoreParams.taskNumber
                            onEditingFinished: {
                                aaCoreParams.setTaskNumber(text);
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
                            workersManager.startWorker("SensorTrayLoaderModule",moduleManagerParam.runMode)
                            workersManager.startWorker(sutParams.moduleName,moduleManagerParam.runMode)
                            workersManager.startWorker("SensorLoaderModule",moduleManagerParam.runMode)
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
                title: qsTr("Lens")
                RowLayout{
                    Button {
                        visible: !baseModuleManager.getServerMode()
                        text: "开始"
                        onClicked: {
                            workersManager.startWorker("TrayLoaderModule",moduleManagerParam.runMode)
                            workersManager.startWorker("LensLoaderModule",moduleManagerParam.runMode)
                            workersManager.startWorker("LUTModule",moduleManagerParam.runMode)
                        }
                    }
                    Button {
                        text: "放"
                        onClicked: {
//                            lutClient.sendLensRequest(false,false)
                            //                            baseModuleManager.sendLoadLens(false)
                        }
                    }
                    Button {
                        text: "放 取ng"
                        onClicked: {
//                            lutClient.sendLensRequest(true,false)
                            //                            baseModuleManager.sendLoadLens(true)
                        }
                    }
                    Button {
                        visible: !baseModuleManager.getServerMode()
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
                            workersManager.startWorker("SensorTrayLoaderModule",moduleManagerParam.runMode)
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
                            workersManager.startWorker("LensTrayLoaderModule",moduleManagerParam.runMode)
                        }
                    }
                    Button{
                        enabled: true
                        text:qsTr("允许进盘")
                        onClicked:{
                            lensLoaderModule.sendMessageToModule("LensTrayLoaderModule","ReadyTrayResquest");
                        }
                    }
                    Button{
                        enabled: true
                        text:qsTr("换盘")
                        onClicked:{
                            lensLoaderModule.sendMessageToModule("LensTrayLoaderModule","ChangeTrayResquest");
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
//            GroupBox{
//                title:qsTr("消息路由测试")
//                RowLayout{
//                    TextField{
//                        horizontalAlignment: TextInput.AlignHCenter
//                        id:module_name
//                        width: 100
//                        text: "Sut1Module"
//                    }

//                    TextField{
//                        horizontalAlignment: TextInput.AlignHCenter
//                        id:target_module_name
//                        width: 100
//                        text: "AA1CoreNew"
//                    }

//                    Button{
//                        text: qsTr("发送")
//                        onClicked: {
//                            if(module_name.text === "Sut1Module")
//                            {
//                                sutModule.sendMessageToModule(target_module_name.text,module_name.text+"TestMessage")
//                            }
//                            else if(module_name.text === "AA1CoreNew")
//                            {
//                                aaNewCore.sendMessageToModule(target_module_name.text,module_name.text+"TestMessage")
//                            }
//                            else if(module_name.text === "AA2CoreNew")
//                            {
//                                aaNewCore.sendMessageToModule(target_module_name.text,module_name.text+"TestMessage")
//                            }
//                            else if(module_name.text === "SUT2Module")
//                            {
//                                aaNewCore.sendMessageToModule(target_module_name.text,module_name.text+"TestMessage")
//                            }
//                        }
//                    }
//                    Button{
//                        text: qsTr("读取")
//                        onClicked: {
//                            if(target_module_name.text === "Sut1Module")
//                                receive_message.text = sutModule.getModuleMessage()
//                            else if(target_module_name.text === "AA1CoreNew")
//                                receive_message.text = aaNewCore.getModuleMessage()
//                            else if(module_name.text === "AA2CoreNew")
//                            {
//                                receive_message.text = aaNewCore.getModuleMessage()
//                            }
//                            else if(module_name.text === "Sut2Module")
//                            {
//                                receive_message.text = sutModule.getModuleMessage()
//                            }
//                        }
//                    }
//                    Label
//                    {
//                        id:receive_message
//                        text: "无"
//                    }
//                }
//            }


            UPHView{}
        }
    }
}
