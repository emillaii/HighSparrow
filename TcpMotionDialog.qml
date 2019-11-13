import QtQuick 2.3
import QtQuick.Controls 2.4
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.11
import QtQuick 2.11
import LogicManagerLib 1.1
// 2，5，0.2 0.5 0.02 0.05
Popup {
    property double selectedStepSize: 0.1

    id: dialog
    visible: false
    contentItem: Rectangle {
        id: rectangle
        color: "black"
        implicitWidth: 500
        implicitHeight: 800

        RowLayout {
            ColumnLayout {
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
            ScrollView {
                Timer {
                    id:timer
                    interval: 600; running: true; repeat: true
                }

                implicitWidth:  400
                implicitHeight: 600
                clip: true
                ColumnLayout {
                    RowLayout {
                        Text {
                            text: "AA_1_A_Inter "
                            color: "white"
                        }
                        RoundButton {
                            text: "-"
                            onClicked: {
                                highSprrow.tcp_aa_A_Inter(-selectedStepSize);
                            }
                        }
                        RoundButton {
                            text: "+"
                            onClicked: {
                                highSprrow.tcp_aa_A_Inter(selectedStepSize);
                            }
                        }
                    }
                    RowLayout {
                        id:inter_layout_b
                        Text {
                            text: "AA_1_B_Inter "
                            color: "white"
                        }
                        RoundButton {
                            text: "-"
                            onClicked: {
                                highSprrow.tcp_aa_B_Inter(-selectedStepSize);
                            }
                        }
                        RoundButton {
                            text: "+"
                            onClicked: {
                                highSprrow.tcp_aa_B_Inter(selectedStepSize);
                            }
                        }
                    }

                    ListView{
                        id:list
                        height: 50*count
                        model: tcpMotorNames
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
                            RoundButton{
                                id:motorOnOffButton
                                text:"off"
                                onClicked: {
                                    if(motorOnOffButton.text === "off")
                                        baseModuleManager.enableMotor(modelData)
                                    else
                                        baseModuleManager.disableMotor(modelData)
                                }
                                Connections{
                                   target: timer
                                   onTriggered:{
                                       if(motorOnOffButton.visible)
                                           motorOnOffButton.text = baseModuleManager.getMotorEnableState(modelData)? "on":"off"
                                   }
                                }
                            }
                            RoundButton {
                                id:homeButton
                                onClicked: {
                                    baseModuleManager.motorSeekOrigin(modelData)
                                }
                                transformOrigin: Item.Center
                                display: Button.IconOnly
                                icon.source: "icons/home.png"
                                icon.color: "red"
                                Connections{
                                   target: timer
                                   onTriggered:{
                                       if(homeButton.visible)
                                          homeButton.icon.color = baseModuleManager.getMotorHomeState(modelData)?"cyan":"red"
                                   }
                                }
                            }
                            RoundButton{
                                text:"-"
                                onClicked: {
                                   var res = baseModuleManager.stepMove(modelData,selectedStepSize,false)
                                   console.log("result: "+res)
                                }
                            }
                            RoundButton{
                                text:"+"
                                onClicked: {
                                   var res = baseModuleManager.stepMove(modelData,selectedStepSize,true)
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
