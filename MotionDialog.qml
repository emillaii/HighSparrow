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
        implicitWidth: 650
        implicitHeight: 1000

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

                implicitWidth:  550
                implicitHeight: 600
                clip: true
                ColumnLayout {
                    RowLayout {
                        Text {
                            text: "AA_A_Inter "
                            color: "white"
                        }
                        RoundButton {
                            text: "-"
                            onClicked: {
                                highSprrow.aa_A_Inter(-selectedStepSize);
                            }
                        }
                        RoundButton {
                            text: "+"
                            onClicked: {
                                highSprrow.aa_A_Inter(selectedStepSize);
                            }
                        }
                    }
                    RowLayout {
                        id:inter_layout_b
                        Text {
                            text: "AA_B_Inter "
                            color: "white"
                        }
                        RoundButton {
                            text: "-"
                            onClicked: {
                                highSprrow.aa_B_Inter(-selectedStepSize);
                            }
                        }
                        RoundButton {
                            text: "+"
                            onClicked: {
                                highSprrow.aa_B_Inter(selectedStepSize);
                            }
                        }
                    }

                    ListView{
                        //anchors.top:inter_layout_b.bottom
                        id:list
                        //anchors.top:  inter_layout_b.bottom
                        height: 50*count
                        model: motorsNames
                        delegate: RowLayout{
                            RoundButton{
                                background: Rectangle {
                                    id: err
                                    radius: 6
                                    Connections{
                                       target: timer
                                       onTriggered:{
//                                            if (baseModuleManager.getMotorAlarmState(modelData))
//                                                err.color = "red"
//                                            else
                                                err.color = "green"
                                       }
                                    }
                                }
                                onClicked: {
//                                    baseModuleManager.clearMotorAlarmState(modelData)
                                }
                            }
                            Rectangle{
                                width: 40
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
                                    logicManagerState.setCurrentChannelName(modelData)
                                    logicManager.performHandling("", LogicManager.MOTION_SINGLE_MOTOR_HOME)
                                    //baseModuleManager.motorSeekOrigin(modelData)
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
                                   //console.log("move "+modelData);
                                   var res = baseModuleManager.stepMove(modelData,selectedStepSize,false)
                                   console.log("result: "+res)
                                }
                            }
                            RoundButton{
                                text:"+"
                                onClicked: {
                                   //console.log("move "+modelData);
                                   var res = baseModuleManager.stepMove(modelData,selectedStepSize,true)
                                   console.log(motorsNames.length)
                                   console.log("result: "+res)
                                }
                            }
                            TextField{
                                id: pos
                                horizontalAlignment: TextInput.AlignHCenter
                                validator: DoubleValidator {
                                    bottom: -100
                                    top: 100
                                    decimals: 3
                                    notation: DoubleValidator.StandardNotation
                                }
                            }
                            RoundButton{
                                transformOrigin: Item.Center
                                display: Button.IconOnly
                                icon.source: "icons/target.png"
                                icon.color: "cyan"
                                onClicked: {
                                    baseModuleManager.moveToPos(modelData, pos.text)
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
