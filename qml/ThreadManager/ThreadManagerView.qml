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
                title: qsTr("测试参数")
                visible: baseModuleManager.getServerMode() === 0
                RowLayout{
                    CheckBox {
                        text: qsTr("静态测试")
                        checked: lens_loader_parameter.staticTest
                        onClicked: {
                            lens_loader_parameter.setStaticTest(checked)
                            lutParams.setStaticTest(checked)
                        }
                    }
                    Label{
                        text: qsTr("重复次数")
                    }
                    TextField{
                        text: lens_loader_parameter.repeatTime
                        onEditingFinished: {
                            lens_loader_parameter.setRepeatTime(text)
                            lutParams.setRepeatTime(text)
                        }
                    }
                    Label{
                        text: qsTr("测试个数")
                    }
                    TextField{
                        text: lens_loader_parameter.testLensCount
                        onEditingFinished: {
                            lens_loader_parameter.setTestLensCount(text)
                            lutParams.setTestLensCount(text)
                        }
                    }
                }
            }
//            GroupBox{
//                title: qsTr("测试参数")
//                visible: baseModuleManager.getServerMode() !== 0
//                ColumnLayout{
//                    CheckBox {
//                        text: qsTr("静态测试")
//                        checked: lens_loader_parameter.staticTest
//                        onClicked: {
//                            sensorLoaderParameter.setStaticTest(checked)
//                            sutParams.setStaticTest(checked)
//                        }
//                    }
//                    Label{
//                        text: qsTr("重复次数")
//                    }
//                    TextField{
//                        text: sensorLoaderParameter.repeatTime
//                        onEditingFinished: {
//                            sensorLoaderParameter.setRepeatTime(text)
//                            sutParams.setRepeatTime(text)
//                        }
//                    }
//                    Label{
//                        text: qsTr("测试个数")
//                    }
//                    TextField{
//                        text: sensorLoaderParameter.testLensCount
//                        onEditingFinished: {
//                            sensorLoaderParameter.setTestLensCount(text)
//                            sutParams.setTestLensCount(text)
//                        }
//                    }
//                }
//            }
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
                        CheckBox {
                            visible: baseModuleManager.getServerMode() === 0
                            text: qsTr("手动换Lens盘");
                            checked: tray_loader_module_parameters.isHandly
                            onClicked: {
                                tray_loader_module_parameters.setIsHandly(checked)
                            }
                        }
                        CheckBox {
                            visible: baseModuleManager.getServerMode() !== 0
                            text: qsTr("手动换Sensor盘");
                            checked: sensor_tray_loader_module_parameter.isHandly
                            onClicked: {
                                sensor_tray_loader_module_parameter.setIsHandly(checked)
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
                            workersManager.startWorker("SensorTrayLoaderModule",systerm_param.runMode)
                            workersManager.startWorker(sutParams.moduleName,systerm_param.runMode)
                            workersManager.startWorker("SensorLoaderModule",systerm_param.runMode)
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
                            workersManager.startWorker("TrayLoaderModule",systerm_param.runMode)
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
        GroupBox{
            title:qsTr("消息路由测试")
            RowLayout{
                TextField{
                    id:module_name
                    width: 100
                    text: "SUT1Module"
                }

                TextField{
                    id:target_module_name
                    width: 100
                    text: "AA1CoreNew"
                }

                Button{
                    text: qsTr("发送")
                    onClicked: {
                        if(module_name.text === "SUT1Module")
                        {
                            sutModule.sendMessageToModule(target_module_name.text,module_name.text+"TestMessage")
                        }
                        else if(module_name.text === "AA1CoreNew")
                        {
                            aaNewCore.sendMessageToModule(target_module_name.text,module_name.text+"TestMessage")
                        }
                        else if(module_name.text === "AA2CoreNew")
                        {
                            aaNewCore.sendMessageToModule(target_module_name.text,module_name.text+"TestMessage")
                        }
                        else if(module_name.text === "SUT2Module")
                        {
                            aaNewCore.sendMessageToModule(target_module_name.text,module_name.text+"TestMessage")
                        }
                    }
                }
                Button{
                    text: qsTr("读取")
                    onClicked: {
                        if(target_module_name.text === "SUT1Module")
                            receive_message.text = sutModule.getModuleMessage()
                        else if(target_module_name.text === "AA1CoreNew")
                            receive_message.text = aaNewCore.getModuleMessage()
                        else if(module_name.text === "AA2CoreNew")
                        {
                            receive_message.text = aaNewCore.getModuleMessage()
                        }
                        else if(module_name.text === "SUT2Module")
                        {
                            receive_message.text = sutModule.getModuleMessage()
                        }
                    }
                }
                Label
                {
                    id:receive_message
                    text: "无"
                }
                }
        }
            UPHView{}
        }
    }
}
