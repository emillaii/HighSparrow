import QtQuick.Controls 2.4
import QtQuick.Layouts 1.11
import QtQuick 2.11
import QtQuick.Window 2.0
import AACoreNew 1.1


RowLayout {
    property string aaCoreTestString : ""

    Popup {
       id: aaCoreTestPopup
       x: 200
       width: 200
       height: 300
       modal: true
       focus: true
       closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutsideParent
       Label {
           text: qsTr("选择测试项目")
       }

       ListModel {
           id: aaCoreTestModel
       }
       ListView {
           anchors.fill: parent
           anchors.margins: 20
           clip: true
           model: aaCoreTestModel
           delegate: ColumnLayout {
                RowLayout {
                    RoundButton {
                        text: operatorTitle
                        onClicked: {
                            var obj = JSON.parse(aaCoreTestString)
                            var params = JSON.stringify(obj["operators"][operatorName]["properties"]["params"])
                            if (operatorName.indexOf("AA") !== -1) {
                                aaNewCore.performHandling(AACoreNew.AA, params)
                            } else if (operatorName.indexOf("Init_Lens") !== -1) {
                                aaNewCore.performHandling(AACoreNew.MOVE_LENS, params)
                            } else if (operatorName.indexOf("UV") !== -1) {
                                aaNewCore.performHandling(AACoreNew.UV, params)
                            }
                        }
                    }
                }
           }
       }
       onClosed: {
           console.log("close is called")
           for (var i=0; i<10; i++)
           {
                aaCoreTestModel.remove(0);
           }
       }
    }
    function loadFlowchartSetting(target, check){
        console.log("load flowchart setting is called")
        var command = "document.getElementById('get_data').click()";
        flowChartPage.webView.runJavaScript(command, function(result) {
            command = "document.getElementById('flowchart_data').value";
            flowChartPage.webView.runJavaScript(command, function(result) {
                var obj = JSON.parse(result)
                aaCoreTestString = result
                for( var link in obj["links"] ) {
                    console.log(obj["links"][link]["toOperator"])
                    var operatorName = obj["links"][link]["toOperator"]
                    if (operatorName.includes(target)) {
                       aaCoreTestModel.append({
                            "operatorTitle": obj["operators"][operatorName]["properties"]["title"],
                            "operatorName": operatorName
                       })
                    }
                }
                aaCoreTestPopup.open()
            })
        })
    }
    Layout.fillWidth: false
    transformOrigin: Item.Left
    RoundButton {
        text: "Init Camera"
        transformOrigin: Item.Center
        display: Button.TextBesideIcon
        icon.width: 30
        icon.height: 30
        icon.source: "../../icons/camera.png"
        icon.color: "lightGreen"
        onClicked: {
            aaNewCore.performHandling(AACoreNew.INIT_CAMERA, "")
        }
    }
    RoundButton {
        text: "光学中心"
        transformOrigin: Item.Center
        display: Button.TextBesideIcon
        icon.width: 30
        icon.height: 30
        icon.source: "../../icons/target.png"
        icon.color: "lightGreen"
        onClicked: {
            aaNewCore.performHandling(AACoreNew.OC, "")
        }
    }
    RoundButton {
        text: "AA"
        transformOrigin: Item.Center
        display: Button.TextBesideIcon
        icon.width: 30
        icon.height: 30
        icon.source: "../../icons/alignment.png"
        icon.color: "lightGreen"
        onClicked: {
            loadFlowchartSetting("AA")
        }
    }
    RoundButton {
        text: "计算 MTF"
        transformOrigin: Item.Center
        display: Button.TextBesideIcon
        icon.width: 30
        icon.height: 30
        icon.source: "../../icons/calculate.png"
        icon.color: "lightGreen"
        onClicked: {
            aaNewCore.performHandling(AACoreNew.MTF, "")
        }
    }
    RoundButton {
        text: "画胶"
        transformOrigin: Item.Center
        display: Button.TextBesideIcon
        icon.width: 30
        icon.height: 30
        icon.source: "../../icons/draw.png"
        icon.color: "lightGreen"
        onClicked: {
            aaNewCore.performHandling(AACoreNew.Dispense, "")
        }
    }
    RoundButton {
        text: "VCM 移动"
        transformOrigin: Item.Center
        display: Button.TextBesideIcon
        icon.width: 30
        icon.height: 30
        icon.source: "../../icons/move-vertical.png"
        icon.color: "lightGreen"
        onClicked: {
            loadFlowchartSetting("Init_Lens")
        }
    }
    RoundButton {
        text: "UV"
        transformOrigin: Item.Center
        display: Button.TextBesideIcon
        icon.width: 30
        icon.height: 30
        icon.source: "../../icons/LC_icon_light_line.png"
        icon.color: "lightGreen"
        onClicked: {
            loadFlowchartSetting("UV")
        }
    }

    Label {
        text: aaCoreParams.aaCoreRunningTest
        font.pointSize: 20
    }
}
