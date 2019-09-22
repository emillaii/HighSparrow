import QtQuick.Controls 2.4
import QtQuick.Layouts 1.11
import QtQuick 2.11
import QtQuick.Window 2.0
import AACoreNew 1.1


RowLayout {
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
                        text: operatorName
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
    function loadFlowchartSetting(target){
        console.log("load flowchart setting is called")
        var command = "document.getElementById('get_data').click()";
        flowChartPage.webView.runJavaScript(command, function(result) {
            command = "document.getElementById('flowchart_data').value";
            flowChartPage.webView.runJavaScript(command, function(result) {
                //console.log(result)
                var obj = JSON.parse(result)
                for( var link in obj["links"] ) {
                    console.log(obj["links"][link]["toOperator"])
                    var operatorName = obj["links"][link]["toOperator"]
                    if (operatorName.includes(target)) {
                       console.log("Appending aaCoreTestMode: " + operatorName)
                       aaCoreTestModel.append({
                            "operatorName": obj["operators"][operatorName]["properties"]["title"]
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
            aaNewCore.performHandling(AACoreNew.UV, "")
        }
    }
}
