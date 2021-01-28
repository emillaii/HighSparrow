import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Dialogs 1.2
import FileContentItem 1.0
import QtQuick.Layouts 1.11
import AACoreNew 1.1
import LogicManagerLib 1.1
import UserMng 1.0
import LanguageConfig 1.0
import "qml"
import "qml/UserManagement"
import "qml/ConfigManager"


ApplicationWindow {
    id: applicationWindow
    visible: true
    width: 1480
    height: 720
    title: qsTr("High Sparrow")

    visibility: "Maximized"

    readonly property string version: "1.0.3"

    readonly property string title_move_to: "移动"
    readonly property string title_read_encoder: "读取"

    readonly property string m_AA1_X: "AA1_X";
    readonly property string m_AA1_Y: "AA1_Y"
    readonly property string m_AA1_Z: "AA1_Z"
    readonly property string m_AA1_A: "AA1_A"
    readonly property string m_AA1_B: "AA1_B"
    readonly property string m_AA1_C: "AA1_C"
    readonly property string m_SUT1_X: "SUT1_X"
    readonly property string m_SUT1_Y: "SUT1_Y"
    readonly property string m_SUT1_Z: "SUT_Z"
    readonly property string m_LUT1_X: "LUT_X"
    readonly property string m_LUT1_Y: "LUT_Y"
    readonly property string m_LUT1_Z: "LUT_Z"

    readonly property string m_LTL_X: "LPA_X" //X
    readonly property string m_LPA_X: "LPA_X" //visonpositionX
    readonly property string m_LPA_Y: "LPA_Y" //Y
    readonly property string m_LPA_Z: "LPA_Z" //物料高度

    property string aaCoreTestItemName: ""
    property string aaCoreTestParams: ""
    property bool isWindowMaximized: true

    //flags: Qt.Dialog | Qt.WindowCancelButtonHint | Qt.WindowCloseButtonHint

    FileDialog {
        id: loadfileDialog
        title: qsTr("选择加载文件")
        selectExisting: true
        selectFolder: false
        selectMultiple: false

        nameFilters: ["json文件 (*.json)"]
        onAccepted: {
            file.setSource(loadfileDialog.fileUrl)
            var result = file.read()
            var command = "document.getElementById('flowchart_data').value ='" + result + "'";
            flowChartPage.webView.runJavaScript(command, function(result) {
                console.log("Load flowchart success")
                command = "document.getElementById('set_data').click()"
                flowChartPage.webView.runJavaScript(command)
                baseModuleManager.loadFlowchart(result, loadfileDialog.fileUrl)
            })
        }
    }

    FileDialog {
        id: saveFileDialog
        title: "选择保存文件"
        selectExisting: false
        selectFolder: false
        selectMultiple: false
        nameFilters: ["json文件 (*.json)"]
        onAccepted: {
            file.setSource(saveFileDialog.fileUrl)
            var command = "document.getElementById('get_data').click()";
            flowChartPage.webView.runJavaScript(command, function(result) {
                command = "document.getElementById('flowchart_data').value";
                flowChartPage.webView.runJavaScript(command, function(result) {
                    baseModuleManager.loadFlowchart(result, saveFileDialog.fileUrl)
                    result = result.replace(/\\n/g, '\\\\n')
                    file.setData(result)
                    file.write()
                })
            })

        }
    }

    MotionDialog {
        id: motionDialog
    }

    TcpMotionDialog {
        id: tcpMotionDialog
    }

    IODialog {
        id: ioDialog
    }

    BusyDialog {
        id: busyDialog
        visible: false
    }

    AlarmDialog {
        id: alarmDialog
        visible: false
    }

    PopupMessageView {
        id: messageDialog
    }

    FileContentItem {
        id: file
        onError: console.log(msg)
    }

    SilicolMsgBox{
        id: silicolMsgBox
    }

    UserManagement{
        id: popupUserManagement
    }

    Login{
        id: popupLogin
    }

    LanguageSelector{
        id: languageSelector
    }

    Popup{
        id: popupConfigManager

        ColumnLayout{
            Button{
                text: "VisionLocationParameter"
                onClicked: {
                    popupConfigManager.close()
                    popupVisionLocationParameter.open()
                }
            }
        }
    }

    Popup{
        id: popupVisionLocationParameter

//        contentItem: ArrayEditor{
//            sectionName: qsTr("VisionLocationParameter")
//            configModel: vlParameters
//            listMaxWidth: 1500
//            listMaxHeight: 900
//            Component.onCompleted: {
//                init()
//            }
//        }
    }

    Connections{
        target: msgBoxModel
        onMsgBoxCountChanged:{
            if(count > 0)
            {
                silicolMsgBox.msgBoxCount = count
                silicolMsgBox.open()
            }
            else{
                silicolMsgBox.close()
            }
        }
    }

    header:
        ToolBar {
        id: toolBar
        height: 60
        background: Rectangle {
            color: "black"
        }

        Timer {
            id:timer
            interval: 600; running: true; repeat: true
        }
        RowLayout {
//            ToolButton {
//                text: qsTr("ConfigManager")
//                transformOrigin: Item.Center
//                display: Button.TextUnderIcon
//                icon.width: 30
//                icon.height: 30
//                icon.source: "icons/configManager.png"
//                icon.color: "deepskyblue"
//                onClicked: {
//                    popupConfigManager.open()
//                }
//            }

            ToolButton {
                text: qsTr("Language")
                transformOrigin: Item.Center
                display: Button.TextUnderIcon
                icon.width: 30
                icon.height: 30
                icon.source: "icons/language.png"
                icon.color: "deepskyblue"
                onClicked: {
                    languageSelector.open()
                }
            }

            ToolButton {
                text: qsTr("用户管理")
                transformOrigin: Item.Center
                display: Button.TextUnderIcon
                icon.width: 30
                icon.height: 30
                icon.source: "icons/userManagement.png"
                icon.color: "deepskyblue"
                onClicked: {
                    popupUserManagement.clearText()
                    popupUserManagement.open()
                }
            }

//            ToolButton {
//                text: qsTr("Undo")
//                transformOrigin: Item.Center
//                display: Button.TextUnderIcon
//                icon.width: 30
//                icon.height: 30
//                icon.source: "icons/undo.png"
//                icon.color: cfgManager.commandCount > 0 ? "transparent" : "gray"
//                onClicked: {
//                    cfgManager.undo()
//                }
//            }
//            ToolButton {
//                text: qsTr("Redo")
//                transformOrigin: Item.Center
//                display: Button.TextUnderIcon
//                icon.width: 30
//                icon.height: 30
//                icon.source: "icons/redo.png"
//                icon.color: cfgManager.undoCommandCount > 0 ? "transparent" : "gray"
//                onClicked: {
//                    cfgManager.redo()
//                }
//            }

            MessageDialog {
                id: logoutDialog
                text: "是否确定要登出?"
                standardButtons: StandardButton.Yes|StandardButton.No
                icon:StandardIcon.Question
                onYes: {
                    userManagement.logout()
                }
            }

            ToolButton {
                id: loginButton
                text: {
                    if(userManagement.hasLogin)
                    {
                        return qsTr("Logout")
                    }
                    else
                    {
                        return qsTr("Login")
                    }
                }
                transformOrigin: Item.Center
                display: Button.TextUnderIcon
                icon.width: 30
                icon.height: 30
                icon.source: "icons/login.png"
                icon.color: "red"
                onClicked: {
                    if(userManagement.hasLogin)
                    {
                        logoutDialog.open()
                    }
                    else
                    {
                        popupLogin.clearText()
                        popupLogin.open()
                    }
                }
                Connections{
                    target: timer
                    onTriggered:{
                        if(userManagement.hasLogin) {  loginButton.icon.color = "lightGreen" }
                        else {  loginButton.icon.color = "red" }
                    }
                }
            }

            ToolButton {
                id: initButton
                text: qsTr("初始化")
                transformOrigin: Item.Center
                display: Button.TextUnderIcon
                icon.width: 30
                icon.height: 30
                icon.source: "icons/initial.png"
                icon.color: "red"
                onClicked: {
                    logicManager.performHandling("",LogicManager.MOTION_INIT)
                    //visionModule.testVision()
                }
                Connections{
                    target: timer
                    onTriggered:{
                        initButton.icon.color = baseModuleManager.InitState?"deepskyblue":"red"
                    }
                }
            }

            MessageDialog {
                id: qmessageDialog
                text: "是否确定要进行Home操作？"
                standardButtons: StandardButton.Yes|StandardButton.No
                icon:StandardIcon.Question
                onYes: {
                    console.log("home start!")
                    logicManager.performHandling("",LogicManager.MOTION_HOME)
                }
                //Component.onCompleted: visible = true
            }

            MessageDialog {
                id: aaCorePerformTestDialog
                standardButtons: StandardButton.Yes|StandardButton.No
                icon:StandardIcon.Question
                onYes: {
                    var obj = JSON.parse(aaCoreTestParams)
                    if (aaCoreTestItemName.indexOf("MTF") !== -1) {
                        console.log("Perform MTF")
                        aaNewCore.performHandling(AACoreNew.MTF, aaCoreTestParams)
                    } else if (aaCoreTestItemName.indexOf("OC") !== -1) {
                        console.log("Perform OC")
                        aaNewCore.performHandling(AACoreNew.OC, aaCoreTestParams)
                    } else if (aaCoreTestItemName.indexOf("AA") !== -1) {
                        console.log("Perform AA")
                        aaNewCore.performHandling(AACoreNew.AA, aaCoreTestParams)
                    } else if (aaCoreTestItemName.indexOf("Init_Lens") !== -1) {
                        console.log("Perform Init LENS")
                        aaNewCore.performHandling(AACoreNew.MOVE_LENS, aaCoreTestParams)
                    } else if (aaCoreTestItemName.indexOf("Init Camera") !== -1) {
                        console.log("Perform Init Camera")
                        aaNewCore.performHandling(AACoreNew.INIT_CAMERA, aaCoreTestParams)
                    } else if (aaCoreTestItemName.indexOf("Y_Level") !== -1) {
                        console.log("Perform Y Level")
                        aaNewCore.performHandling(AACoreNew.Y_LEVEL, aaCoreTestParams)
                    } else if (aaCoreTestItemName.indexOf("UV") !== -1) {
                        console.log("Perform UV")
                        aaNewCore.performHandling(AACoreNew.UV, aaCoreTestParams)
                    } else if (aaCoreTestItemName.indexOf("OTP") !== -1) {
                        console.log("Perform OTP")
                        aaNewCore.performHandling(AACoreNew.OTP, aaCoreTestParams)
                    }else if (aaCoreTestItemName.indexOf("Unload Camera") !== -1) {
                        console.log("Perform Unload Camera")
                        aaNewCore.performHandling(AACoreNew.UNLOAD_CAMERA, aaCoreTestParams)
                    }else if (aaCoreTestItemName.indexOf("Dispense") !== -1) {
                        console.log("Perform Dispense")
                        aaNewCore.performHandling(AACoreNew.Dispense, aaCoreTestParams)
                    }else if (aaCoreTestItemName.indexOf("PR To Bond") !== -1) {
                        console.log("Perform PR To Bond")
                        aaNewCore.performHandling(AACoreNew.PR_To_Bond, aaCoreTestParams)
                    }else if (aaCoreTestItemName.indexOf("Z Offset") !== -1) {
                        console.log("Perform Z Offset")
                        aaNewCore.performHandling(AACoreNew.Z_OFFSET, aaCoreTestParams)
                    }else if (aaCoreTestItemName.indexOf("XY Offset") !== -1) {
                        console.log("Perform XY Offset")
                        aaNewCore.performHandling(AACoreNew.XY_OFFSET, aaCoreTestParams)
                    }else if (aaCoreTestItemName.indexOf("Partical Check") !== -1) {
                        console.log("Perform Partical Check")
                        aaNewCore.performHandling(AACoreNew.PARTICAL_CHECK, aaCoreTestParams)
                    }

                    aaCoreTestParams = ""
                    aaCoreTestItemName = ""
                }
                onNo: {
                    aaCoreTestParams = ""
                    aaCoreTestItemName = ""
                }
            }

            ToolButton {
                id: homeAllButton
                objectName: "HomeButtonObject"
                text: qsTr("Home")
                transformOrigin: Item.Center
                display: Button.TextUnderIcon
                icon.width: 30
                icon.height: 30
                icon.source: "icons/home.png"
                icon.color: "red"
                onClicked: {
                    qmessageDialog.open()
                    //logicManager.sendMessageTest()
                }
                Connections{
                    target: timer
                    onTriggered:{
                        homeAllButton.icon.color = baseModuleManager.HomeState?"deepskyblue":"red"
                    }
                }
            }
            ToolButton {
                id: loadFlowChartButton
                text: qsTr("加载流程图")
                transformOrigin: Item.Center
                display: Button.TextUnderIcon
                icon.width: 30
                icon.height: 30
                icon.source: "icons/flowchart.png"
                icon.color: "deepskyblue"
                onClicked: {
                    loadfileDialog.open()
                }
            }
            ToolButton {
                id: saveFlowChart
                text: qsTr("保存流程图")
                transformOrigin: Item.Center
                display: Button.TextUnderIcon
                icon.width: 30
                icon.height: 30
                icon.source: "icons/save.png"
                icon.color: "deepskyblue"
                onClicked: {
                    saveFileDialog.open()
                }
            }
            ToolButton {
                id: loadParamsButton
                text: qsTr("加载参数")
                transformOrigin: Item.Center
                display: Button.TextUnderIcon
                icon.width: 30
                icon.height: 30
                icon.source: "icons/flowchart.png"
                icon.color: "deepskyblue"
                onClicked: {
                    logicManager.performHandling("", LogicManager.LOAD_PARAMETERS)
                }
            }
            ToolButton {
                id: saveParams
                text: qsTr("保存参数")
                transformOrigin: Item.Center
                display: Button.TextUnderIcon
                icon.width: 30
                icon.height: 30
                icon.source: "icons/save.png"
                icon.color: "deepskyblue"
                onClicked: {
                    logicManager.performHandling("", LogicManager.SAVE_PARAMETERS)
                }
            }

            ToolButton {
                text: qsTr("AA1电机")
                visible: {
                    if (baseModuleManager.getServerMode() === 0) return false
                    else return true
                }

                transformOrigin: Item.Center
                display: Button.TextUnderIcon
                icon.width: 30
                icon.height: 30
                icon.source: "icons/machine.png"
                icon.color: "yellow"
                onClicked: {
                    tcpMotionDialog.open()
                }
            }

            ToolButton {
                text: {
                    if (baseModuleManager.getServerMode() === 0)
                        return qsTr("AA1电机")
                    else
                        return qsTr("AA2电机")
                }
                transformOrigin: Item.Center
                display: Button.TextUnderIcon
                icon.width: 30
                icon.height: 30
                icon.source: "icons/machine.png"
                icon.color: {
                    if (baseModuleManager.getServerMode() === 0)
                        return "yellow"
                    else
                        return "lightGreen"
                }
                onClicked: {
                    motionDialog.open()
                }
            }
            ToolButton {
                text: qsTr("IO")
                transformOrigin: Item.Center
                display: Button.TextUnderIcon
                icon.width: 30
                icon.height: 30
                icon.source: "icons/settings_input_composite.png"
                icon.color: "deepskyblue"
                onClicked: {
                    ioDialog.open()
                }
            }
            ToolButton {
                text: qsTr("运行流程图")
                transformOrigin: Item.Center
                display: Button.TextUnderIcon
                icon.width: 30
                icon.height: 30
                icon.source: "icons/flowchart_1.png"
                icon.color: "deepskyblue"
                onClicked: {
                    workersManager.startAllWorkers(3)
                }
            }

            //           ToolButton {
            //               text: qsTr("Map")
            //               transformOrigin: Item.Center
            //               display: Button.TextUnderIcon
            //               icon.width: 30
            //               icon.height: 30
            //               icon.source: "icons/machine.png"
            //               icon.color: "yellow"
            //               onClicked: {
            //                   baseModuleManager.showMachineMap()
            //               }
            //           }

            ToolButton {
                text: qsTr("Auto")
                transformOrigin: Item.Center
                display: Button.TextUnderIcon
                icon.width: 30
                icon.height: 30
                icon.source: "icons/auto-run.png"
                icon.color: "deepskyblue"
                onClicked: {
                    var command = "document.getElementById('get_data').click()";
                    flowChartPage.webView.runJavaScript(command, function(result) {
                        command = "document.getElementById('flowchart_data').value";
                        flowChartPage.webView.runJavaScript(command, function(result) {
                            baseModuleManager.updateCurrentFlowchart(result)
                            workersManager.startAllWorkers(moduleManagerParam.runMode)
                        })
                    })
                }
            }
            ToolButton {
                text: qsTr("Stop")
                transformOrigin: Item.Center
                display: Button.TextUnderIcon
                icon.width: 30
                icon.height: 30
                icon.source: "icons/stop.png"
                icon.color: "red"
                onClicked: {
                    workersManager.stopAllWorkers(true)
                    logicManagerState.setIsHandling(false)
                }
            }
            //Main Timer
            Timer {
                id: mainTimer
                interval: 500; running: true; repeat: true
                onTriggered: {
                    timeString.text =  Qt.formatTime(new Date(), "现在时间: hh:mm:ss")
                    busyDialog.updateBusyDialogStatus()
                    //Used for consuming the flowchart double click command
                    if (flowChartPage.webView.loadProgress == 100) {
                        var command = "document.getElementById('flowchart_running_cmd').value";
                        if (flowChartPage.webView.title === "Flow chart") {
                            flowChartPage.webView.runJavaScript(command, function(result) {
                                command = "document.getElementById('flowchart_running_cmd').value = ''";
                                if (result.length > 0) {
                                    var obj = JSON.parse(result)
                                    var operatorId = obj["operatorId"]
                                    aaCoreTestItemName = operatorId
                                    aaCoreTestParams = result
                                    aaCorePerformTestDialog.text = "Perform " + operatorId + "?"
                                    aaCorePerformTestDialog.open()
                                    flowChartPage.webView.runJavaScript(command, function(result) {
                                        //This is just reset the command in flowchart js
                                    })
                                }
                            })
                        }
                    }
                }
            }

            Button{
                text: "报警信息"
                onClicked: {
                    workersManager.changeAlarmShow()
                }
            }

            MessageDialog {
                id: closeDialog
                text: "是否确定要退出应用?"
                standardButtons: StandardButton.Yes|StandardButton.No
                icon:StandardIcon.Question
                onYes: {
                    applicationWindow.close()
                }
            }

            ToolButton {
                focusPolicy: Qt.TabFocus
                display: AbstractButton.IconOnly
                icon.width: 30
                icon.height: 30
                icon.source: "icons/full-screen.png"
                icon.color: "orange"
                onClicked: {
                    if (!isWindowMaximized) {
                        applicationWindow.showMaximized()
                    }
                    else {
                        applicationWindow.showNormal()
                    }
                    isWindowMaximized = !isWindowMaximized
                }
            }
            ToolButton {
                display: AbstractButton.IconOnly
                icon.width: 30
                icon.height: 30
                icon.source: "icons/close_program.png"
                icon.color: "red"
                onClicked: {
                    closeDialog.open()
                }
            }

            GridLayout{
                rows: 2
                columns: 3

                Label{
                    text: qsTr("LotNumber:") + aaCoreParams.lotNumber
                    color: "white"
                    Layout.fillWidth: true
                }

                Label{
                    text: qsTr("硅酷科技 版本:" + version)
                    color: "white"
                    Layout.fillWidth: true
                }

                Label{
                    id: timeString
                    color: "cyan"
                    text: qsTr(" 现在时间:")
                    Layout.fillWidth: true
                }

                RowLayout{
                    Label{
                        text: qsTr("User:")
                    }
                    Label{
                        text: userManagement.currentUserName
                    }
                }
                RowLayout{
                    Label{
                        text: qsTr("Authority:")
                    }
                    Label{
                        text: {
                            switch(userManagement.currentAuthority)
                            {
                            case UserMng.Admin:
                                return "Admin"
                            case UserMng.Engineer:
                                return "Engineer"
                            case UserMng.Operator:
                                return "Operator"
                            case UserMng.None:
                                return "None"
                            }
                        }
                    }
                }

            }
        }
    }



    StackLayout {
        id: swipeView
        anchors.fill: parent
        currentIndex: tabBar.currentIndex
        Page9Form {

        }

        Page1Form {
            featureButton.onClicked: {
                highSprrow.callQProcess("GenericNCCavproj.avexe")
            }
            navigationFeatureButton.onClicked: {
                highSprrow.callQProcess("edgeFittingHMI.avexe")
            }
            drawPathButton.onClicked: {
                highSprrow.callQProcess("DrawPath.avexe")
            }
            yLevelDrawPathButton.onClicked: {
                highSprrow.callQProcess("Y_Level_Editor.avexe")
            }
        }

        Page7Form {
            featureButton.onClicked: {
                highSprrow.callQProcess("GenericNCCavproj.avexe")
            }
            navigationFeatureButton.onClicked: {
                highSprrow.callQProcess("edgeFittingHMI.avexe")
            }
            drawPathButton.onClicked: {
                highSprrow.callQProcess("DrawPath.avexe")
            }
            yLevelDrawPathButton.onClicked: {
                highSprrow.callQProcess("Y_Level_Editor.avexe")
            }
        }

        Page2Form {}
        Page3Form {
            id: flowChartPage
        }
        Page4FormForm {}
        Page5Form {}
        Page6Form {}
        Page8Form {}
    }

    footer: TabBar {
        id: tabBar
        currentIndex: swipeView.currentIndex
        Component.onCompleted: {
            if (baseModuleManager.getServerMode() === 1) {
                tabBar.currentIndex = 0
            }
            else
            {
                tabBar.currentIndex = 1
            }
        }
        TabButton {
            text: qsTr("操作")
            enabled: {
                if (baseModuleManager.getServerMode() === 1)
                    return true
                else
                    return false
            }
        }

        TabButton {
            text: {
                if (baseModuleManager.getServerMode() === 1)
                    return "右工位"
                else
                    return "左工位"
            }
        }
        TabButton {
            text: {
                if (baseModuleManager.getServerMode() === 1)
                    return "左工位"
                else
                    return "右工位"
            }
            enabled: {
                if (baseModuleManager.getServerMode() === 1)
                    return true
                else
                    return false
            }
        }
        TabButton {
            text: qsTr("数据")
        }
        TabButton {
            text: qsTr("流程图")
//            enabled: {
//                if (userManagement.currentAuthority >= 2)
//                    return true
//                else
//                    return false
//            }
        }
        TabButton {
            text: qsTr("AA 控制面板")
        }
        TabButton {
            text: qsTr("MTF")
        }
        TabButton {
            text: qsTr("AA 核心参数")
        }
        TabButton {
            text: qsTr("Report")
        }
    }
}
