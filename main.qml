import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Dialogs 1.2
import FileContentItem 1.0


ApplicationWindow {
    visible: true
    width: 1320
    height: 720
    title: qsTr("High Sparrow")

    FileDialog {
        id: loadfileDialog
        title: "选择加载文件"
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
        onRejected: {
            console.log("Canceled")
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
        onRejected: {
            console.log("Canceled")
        }
    }

    FileContentItem {
        id: file
        onError: console.log(msg)
    }

    header: ToolBar {
        id: toolBar
        height: 80
        anchors.right: parent.right
        anchors.rightMargin: 8
        anchors.left: parent.left
        anchors.leftMargin: 8
        anchors.top: parent.top
        anchors.topMargin: 8

        ToolButton {
            id: homeButton
            objectName: "HomeButtonObject"
            text: qsTr("Home")
            transformOrigin: Item.Center
            display: Button.TextUnderIcon
            anchors.top: parent.top
            anchors.topMargin: 0
            anchors.left: parent.left
            anchors.leftMargin: 8
            icon.width: 50
            icon.height: 50
            icon.source: "icons/home.png"
            onClicked: {
                logicManager.start()
            }
        }
        ToolButton {
            id: loadFlowChartButton
            text: qsTr("加载流程图")
            anchors.left: parent.left
            anchors.leftMargin: 76
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
            anchors.leftMargin: 173
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

    }

    SwipeView {
        id: swipeView
        anchors.fill: parent
        currentIndex: tabBar.currentIndex
        interactive: false

        Page1Form {
            navigationFeatureButton.onClicked: {
                baseModuleManager.testVision()
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
            text: qsTr("Page 1")
        }
        TabButton {
            text: qsTr("Page 2")
        }
        TabButton {
            text: qsTr("Page 3")
        }
    }
}
