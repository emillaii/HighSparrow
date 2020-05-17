import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Dialogs 1.2
import FileContentItem 1.0
import QtQuick.Layouts 1.11
import AACoreNew 1.1
import UserMng 1.0
import "qml"
import "qml/UserManagement"

ApplicationWindow {
    visible: true
    width: 1320
    height: 720
    title: qsTr("High Sparrow")

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
    readonly property string m_LPA_X: "LPA_X"  //visonpositionX
    readonly property string m_LPA_Y: "LPA_Y" //Y
    readonly property string m_LPA_Z: "LPA_Z" //物料高度

    readonly property string versionString: "HW-1.0.3"

    property string aaCoreTestItemName: ""
    property string aaCoreTestParams: ""

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
                file.write()
            }
        }

        VisionLocationParameters{
            id: visionLocationParametersViewer
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

        RowLayout {
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
            ToolButton {
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
                icon.color: "deepskyblue"
                onClicked: {
                    if(userManagement.hasLogin)
                    {
                        userManagement.logout()
                    }
                    else
                    {
                        popupLogin.clearText()
                        popupLogin.open()
                    }
                }
            }

            Timer{
                id: timer
                interval: 500
                repeat: true
                running: true
            }

            ToolButton {
                id:inintButton
                text: qsTr("初始化")
                transformOrigin: Item.Center
                display: Button.TextUnderIcon
                icon.width: 30
                icon.height: 30
                icon.source: "icons/initial.png"
                icon.color: "red"
                onClicked: {
                    logicManager.init()
                }
                Connections{
                    target: timer
                    onTriggered:{
                        inintButton.icon.color = baseModuleManager.InitState?"deepskyblue":"red"
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
                    logicManager.home()
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
                    } else if (aaCoreTestItemName.indexOf("Init Camera") !== -1) {
                        console.log("Perform Init Camera")
                        aaNewCore.performHandling(AACoreNew.INIT_CAMERA, aaCoreTestParams)
                    } else if (aaCoreTestItemName.indexOf("Y_Level") !== -1) {
                        console.log("Perform Y Level")
                        aaNewCore.performHandling(AACoreNew.Y_LEVEL, aaCoreTestParams)
                    } else if (aaCoreTestItemName.indexOf("UV") !== -1) {
                        console.log("Perform UV")
                        aaNewCore.performHandling(AACoreNew.UV, aaCoreTestParams)
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
                    //logicManager.receiveMsgSignal()
                }
                Connections{
                    target: timer
                    onTriggered:{
//                        homeAllButton.icon.color = baseModuleManager.HomeState?"deepskyblue":"red"
                        homeAllButton.icon.color = baseModuleManager.HomeState? "deepskyblue":"red"
                    }
                }
            }
//            ToolButton {
//               id: stopHomeButton
//               objectName: "StopHomeButtonObject"
//               text: qsTr("停Home")
//               transformOrigin: Item.Center
//               display: Button.TextUnderIcon
//               icon.width: 30
//               icon.height: 30
//               icon.source: "icons/home.png"
//               icon.color: "red"
//               onClicked: {
//                   logicManager.stopHome()
//               }
//           }
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
                  var command = "document.getElementById('get_data').click()";
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
                id: openVisionLocationParameterViewer
                text: qsTr("VL参数")
                transformOrigin: Item.Center
                display: Button.TextUnderIcon
                icon.width: 30
                icon.height: 30
                icon.source: "icons/visionLocationParameterViewer.png"
                icon.color: "deepskyblue"
                onClicked: {
                    visionLocationParametersViewer.open()
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
                    baseModuleManager.loadParameters()
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
//                   logicManager.updateParams()
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
               text: qsTr("运行流程图")
               transformOrigin: Item.Center
               display: Button.TextUnderIcon
               icon.width: 30
               icon.height: 30
               icon.source: "icons/flowchart_1.png"
               icon.color: "deepskyblue"
               onClicked: {
                   var command = "document.getElementById('get_data').click()";
                   flowChartPage.webView.runJavaScript(command, function(result) {
                       command = "document.getElementById('flowchart_data').value";
                       flowChartPage.webView.runJavaScript(command, function(result) {
                            baseModuleManager.loadFlowchart(result)
                            workersManager.startWorkers(4)
                       })
                   })
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
                   var command = "document.getElementById('get_data').click()";
                   flowChartPage.webView.runJavaScript(command, function(result) {
                       command = "document.getElementById('flowchart_data').value";
                       flowChartPage.webView.runJavaScript(command, function(result) {
                           baseModuleManager.loadFlowchart(result)
                           workersManager.startWorkers(systerm_param.runMode)
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
                   workersManager.stopWorkers(true)
                   logicManager.stop()
               }
           }

           Timer {
               interval: 500; running: true; repeat: true
               onTriggered: {
                    timeString.text =  Qt.formatTime(new Date(), "现在时间: hh:mm:ss")
                    if (baseModuleManager.HomeState) {
                        homeSignal.color = "green"
                    }
                    else {
                        homeSignal.color = "red"
                    }
                    //Used for consuming the flowchart double click command
                    if (flowChartPage.webView.loadProgress == 100) {
                    var command = "document.getElementById('flowchart_running_cmd').value";
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

           RoundButton {
               text: qsTr("Home")
               background: Rectangle {
                   id: homeSignal
                   color: "red"
               }
           }

           GridLayout{
               rows: 2
               columns: 2

               Label{
                  text: qsTr("User:")
               }
               Label{
                  text: userManagement.currentUserName
               }
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

           Label{
               text: "Version: " + versionString
           }
           Label{
              id: timeString
              color: "cyan"
              text: qsTr("现在时间:")
              Layout.fillWidth: true
              Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
           }
        }
    }

    SwipeView {
        id: swipeView
        anchors.fill: parent
        currentIndex: tabBar.currentIndex
        interactive: false

//        UnitTestForm{}

        Page1Form {
            featureButton.onClicked: {
                //visionModule.testVision()
                highSprrow.callQProcess("GenericNCCavproj.avexe")
            }
            navigationFeatureButton.onClicked: {
                highSprrow.callQProcess("EdgeFinder.avexe")
            }
            drawPathButton.onClicked: {
                highSprrow.callQProcess("DrawPath.avexe")
            }
        }

        Page2Form {}
        Page3Form {
            id: flowChartPage
        }
        Page4FormForm {}
        Page5Form {}
        Page6Form {}
        Page7Form {}
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
            text: qsTr("MTF")
        }
        TabButton {
            text: qsTr("AA Core")
        }
        TabButton {
            text: qsTr("HWAA Core")
        }
    }
}
