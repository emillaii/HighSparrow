import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Dialogs 1.2
import FileContentItem 1.0
import QtQuick.Layouts 1.11

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

    IODialog {
        id: ioDialog
    }

    BusyDialog {
        id: busyDialog
        visible: false
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
            height: 60
            background: Rectangle {
                color: "black"
            }

        RowLayout {
            ToolButton {
                text: qsTr("初始化")
                transformOrigin: Item.Center
                display: Button.TextUnderIcon
                icon.width: 30
                icon.height: 30
                icon.source: "icons/initial.png"
                icon.color: "deepskyblue"
                onClicked: {
                    logicManager.init()
                }
            }
            ToolButton {
                objectName: "HomeButtonObject"
                text: qsTr("Home")
                transformOrigin: Item.Center
                display: Button.TextUnderIcon
                icon.width: 30
                icon.height: 30
                icon.source: "icons/home.png"
                icon.color: "deepskyblue"
                onClicked: {
                    logicManager.home()
                }
            }
            ToolButton {
               id: stopHomeButton
               objectName: "StopHomeButtonObject"
               text: qsTr("停Home")
               transformOrigin: Item.Center
               display: Button.TextUnderIcon
               icon.width: 30
               icon.height: 30
               icon.source: "icons/home.png"
               icon.color: "red"
               onClicked: {
                   logicManager.stopHome()
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
               text: qsTr("保存参数")
               transformOrigin: Item.Center
               display: Button.TextUnderIcon
               icon.width: 30
               icon.height: 30
               icon.source: "icons/save.png"
               icon.color: "deepskyblue"
               onClicked: {
                   baseModuleManager.updateParams()
//                   messageDialog.messageText.text = "Save Complete"
//                   messageDialog.open()
               }
           }
           ToolButton {
               text: qsTr("电机")
               transformOrigin: Item.Center
               display: Button.TextUnderIcon
               icon.width: 30
               icon.height: 30
               icon.source: "icons/machine.png"
               icon.color: "deepskyblue"
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
               text: qsTr("Auto")
               transformOrigin: Item.Center
               display: Button.TextUnderIcon
               icon.width: 30
               icon.height: 30
               icon.source: "icons/auto-run.png"
               icon.color: "deepskyblue"
               onClicked: {
                   var command = "document.getElementsByClassName('get_data')[0].click()";
                   flowChartPage.webView.runJavaScript(command, function(result) {
                       command = "document.getElementById('flowchart_data').value";
                       flowChartPage.webView.runJavaScript(command, function(result) {
                           logicManager.autoRun(result)
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
               icon.source: "icons/auto-run.png"
               icon.color: "deepskyblue"
               onClicked: {
                    logicManager.stop()
               }
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
                var command = "document.getElementsByClassName('get_data')[0].click()";
                flowChartPage.webView.runJavaScript(command, function(result) {
                    command = "document.getElementById('flowchart_data').value";
                    flowChartPage.webView.runJavaScript(command, function(result) {
                        console.log(result)
                        logicManager.loadFlowchart(result)
                        logicManager.start()
                    })
                })
            }
        }

        Page2Form {}
        Page3Form {
            id: flowChartPage
        }
        Page4FormForm {}
        Page5Form {}
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
        TabButton {
            text: qsTr("AA Data")
        }
        TabButton {
            text: qsTr("Dignostics")
        }
    }
}
