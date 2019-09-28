import QtQuick 2.3
import QtQuick.Controls 2.4
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.11
import QtQuick 2.11
Popup {
    property double selectedStepSize: 0.1
    id:dialog
    visible: false
    contentItem: Rectangle{
        id:rectangle
        color: "black"
        implicitWidth: 500
        implicitHeight: 800

        RowLayout{
            ColumnLayout {
                Layout.alignment: Qt.AlignTop
                RadioButton {
                    id: step20Button
                    text: qsTr("20")
                    font.pixelSize: 10
                    onCheckedChanged: {
                        if (step20Button.checked)
                        {
                            selectedStepSize = 20
                        }
                    }
                }
                RadioButton {
                    id: step10Button
                    text: qsTr("10")
                    font.pixelSize: 10
                    onCheckedChanged: {
                        if (step10Button.checked)
                        {
                            selectedStepSize = 10
                        }
                    }
                }
                RadioButton {
                    id: step5Button
                    text: qsTr("5")
                    font.pixelSize: 10
                    onCheckedChanged: {
                        if (step5Button.checked)
                        {
                            selectedStepSize = 5
                        }
                    }
                }
                RadioButton {
                    id: step2Button
                    text: qsTr("2")
                    font.pixelSize: 10
                    onCheckedChanged: {
                        if (step2Button.checked)
                        {
                            selectedStepSize = 2
                        }
                    }
                }
                RadioButton {
                    id: step1Button
                    text: qsTr("1")
                    font.pixelSize: 10
                    onCheckedChanged: {
                        if (step1Button.checked)
                        {
                            selectedStepSize = 1
                        }
                    }
                }
                RadioButton {
                    id: step05Button
                    text: qsTr("0.5")
                    font.pixelSize: 10
                    onCheckedChanged: {
                        if (step05Button.checked)
                        {
                            selectedStepSize = 0.5
                        }
                    }
                }
                RadioButton {
                    id: step02Button
                    text: qsTr("0.2")
                    font.pixelSize: 10
                    onCheckedChanged: {
                        if (step02Button.checked)
                        {
                            selectedStepSize = 0.2
                        }
                    }
                }
                RadioButton {
                    id: step01Button
                    text: qsTr("0.1")
                    font.pixelSize: 10
                    checked: true
                    onCheckedChanged: {
                        if (step01Button.checked)
                        {
                            selectedStepSize = 0.1
                        }
                    }
                }
                RadioButton {
                    id: step005Button
                    text: qsTr("0.05")
                    font.pixelSize: 10
                    onCheckedChanged: {
                        if (step005Button.checked)
                        {
                            selectedStepSize = 0.05
                        }
                    }
                }
                RadioButton {
                    id: step002Button
                    text: qsTr("0.02")
                    font.pixelSize: 10
                    onCheckedChanged: {
                        if (step002Button.checked)
                        {
                            selectedStepSize = 0.02
                        }
                    }
                }
                RadioButton {
                    id: step001Button
                    text: qsTr("0.01")
                    font.pixelSize: 10
                    onCheckedChanged: {
                        if (step001Button.checked)
                        {
                            selectedStepSize = 0.01
                        }
                    }
                }
                RadioButton {
                    id: step0001Button
                    text: qsTr("0.001")
                    font.pixelSize: 10
                    onCheckedChanged: {
                        if (step0001Button.checked)
                        {
                            selectedStepSize = 0.001
                        }
                    }
                }
            }
            ScrollView{

                implicitWidth:  400
                implicitHeight: 600
                clip: true
                ColumnLayout{
                    Timer{
                            id:timer
                            interval: 600;
                            running: true;
                            repeat: true;
                        }
                    ListView{
                            id:list
                            height: 50*count
                            model: motorsModel
                            delegate: RowLayout{
                                Rectangle{
                                    width: 65
                                    Text{
                                       anchors.verticalCenter: parent.verticalCenter
                                       text:modelData
                                       wrapMode: width
                                       color:"white"
                                    }
                                }
                                RoundButton {
                                    onClicked: {
                                        //console.log("move "+modelData);
                                        baseModuleManager.motorSeekOrigin(modelData)
                                    }
                                    transformOrigin: Item.Center
                                    display: Button.IconOnly
                                    icon.source: "icons/home.png"
                                    icon.color: "cyan"
                                }
                                RoundButton{
                                    text:"-"
                                    onClicked: {
                                       //console.log("move "+modelData);
                                       var res = baseModuleManager.stepMove(modelData,selectedStepSize,false)
                                       //console.log("result: "+res)
                                    }
                                }
                                RoundButton{
                                    text:"+"
                                    onClicked: {
                                       //console.log("move "+modelData);
                                       var res = baseModuleManager.stepMove(modelData,selectedStepSize,true)
                                       //console.log(motorsNames.length)
                                       //console.log("result: "+res)
                                    }
                                }
                                Label{
                                    id:lbl
                                    color:"lightskyblue"
                                    Connections{
                                       target: timer
                                       onTriggered:{
                                           if(lbl.visible)
                                            lbl.text = baseModuleManager.getMotorFeedbackPos(modelData)
                                       }
                                    }
                                }
                            }
                        }
                }

                }
        }
    }
}





/*##^## Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
 ##^##*/
