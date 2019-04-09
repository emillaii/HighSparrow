import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Dialogs 1.2
import FileContentItem 1.0


ApplicationWindow {
    visible: true
    width: 1320
    height: 720
    title: qsTr("High Sparrow")

    readonly property string title_move_to: "移動"
    readonly property string title_read_encoder: "讀取"

    readonly property string m_AA1_X: "AA1_X"
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
                command = "document.getElementsByClassName('set_data')[0].click()";
                flowChartPage.webView.runJavaScript(command);
                logicManager.loadFlowchart(result)
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
            file.write()
        }
    }

    MotionDialog {
        id: motionDialog
    }

    PopupMessageView {
        id: messageDialog
    }

    FileContentItem {
        id: file
        onError: console.log(msg)
    }

    header:
        ToolBar {
        id: toolBar
        height: 80
        anchors.right: parent.right
        anchors.rightMargin: 8
        anchors.left: parent.left
        anchors.leftMargin: 8
        anchors.top: parent.top
        anchors.topMargin: 8

        ToolButton {
            id: initButton
            text: qsTr("Init")
            transformOrigin: Item.Center
            display: Button.TextUnderIcon
            anchors.top: parent.top
            anchors.topMargin: 0
            anchors.left: parent.left
            anchors.leftMargin: 8
            icon.width: 50
            icon.height: 50
            icon.source: "icons/initial.png"
            onClicked: {
                console.log("Initial Device")
                baseModuleManager.initialDevice();
            }
        }

        ToolButton {
            id: homeButton
            objectName: "HomeButtonObject"
            text: qsTr("Home")
            transformOrigin: Item.Center
            display: Button.TextUnderIcon
            anchors.top: parent.top
            anchors.topMargin: 0
            anchors.left: parent.left
            anchors.leftMargin: 90
            icon.width: 50
            icon.height: 50
            icon.source: "icons/home.png"
            onClicked: {
                baseModuleManager.allMotorsSeekOrigin();
            }
        }

        ToolButton {
            id: stopHomeButton
            objectName: "StopHomeButtonObject"
            text: qsTr("StopHome")
            transformOrigin: Item.Center
            display: Button.TextUnderIcon
            anchors.top: parent.top
            anchors.topMargin: 0
            anchors.left: parent.left
            anchors.leftMargin: 90*2
            icon.width: 50
            icon.height: 50
            icon.source: "icons/home.png"
            onClicked: {
                baseModuleManager.stopSeeking();
            }
        }
        ToolButton {
            id: loadFlowChartButton
            text: qsTr("加载流程图")
            anchors.left: parent.left
            anchors.leftMargin: 90*3
            transformOrigin: Item.Center
            display: Button.TextUnderIcon
            anchors.top: parent.top
            anchors.topMargin: 0
            icon.width: 50
            icon.height: 50
            icon.source: "icons/flowchart.png"
            onClicked: {
                loadfileDialog.open()
            }
        }

        ToolButton {
            id: saveFlowChart
            text: qsTr("保存流程图")
            anchors.left: parent.left
            anchors.leftMargin: 90*4
            transformOrigin: Item.Center
            display: Button.TextUnderIcon
            anchors.top: parent.top
            anchors.topMargin: 0
            icon.width: 50
            icon.height: 50
            icon.source: "icons/save.png"
            onClicked: {
                var command = "document.getElementsByClassName('get_data')[0].click()";
                flowChartPage.webView.runJavaScript(command, function(result) {
                    command = "document.getElementById('flowchart_data').value";
                    flowChartPage.webView.runJavaScript(command, function(result) {
                        console.log(result)
                        file.setData(result)
                        saveFileDialog.open()
                    })
                })
            }
        }

        ToolButton {
            id: saveParams
            text: qsTr("保存")
            anchors.left: parent.left
            anchors.leftMargin: 90*5
            transformOrigin: Item.Center
            display: Button.TextUnderIcon
            anchors.top: parent.top
            anchors.topMargin: 0
            icon.width: 50
            icon.height: 50
            icon.source: "icons/save.png"
            onClicked: {
                baseModuleManager.updateParams()
                messageDialog.messageText.text = "Save Complete"
                messageDialog.open()
            }
        }

        ToolButton {
            text: qsTr("Dialog")
            anchors.left: parent.left
            anchors.leftMargin: 90*6
            transformOrigin: Item.Center
            display: Button.TextUnderIcon
            anchors.top: parent.top
            anchors.topMargin: 0
            icon.width: 50
            icon.height: 50
            icon.source: "icons/machine.png"
            onClicked: {
                motionDialog.open()
            }
        }

    }

    SwipeView {
        id: swipeView
        anchors.fill: parent
        currentIndex: tabBar.currentIndex
        interactive: false

        Page1Form {
            featureButton.onClicked: {
                highSprrow.callQProcess("GenericNCCavproj.avexe")
            }
            navigationFeatureButton.onClicked: {
                //logicManager.start()
                visionModule.testVision()
            }
        }

        Page2Form {
        }

        Page3Form {
            id: flowChartPage
        }
    }

    footer: TabBar {
        id: tabBar
        currentIndex: swipeView.currentIndex

        TabButton {
            text: qsTr("Main Page")
        }
        TabButton {
            text: qsTr("Data")
        }
        TabButton {
            text: qsTr("流程图")
        }
    }
}
