import QtQuick 2.3
import QtQuick.Controls 2.4
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.11
import QtQuick 2.11
import LogicManagerLib 1.1
// 2，5，0.2 0.5 0.02 0.05
Popup {
    property double selectedStepSize: 0.1
    property double opacityLevel: 1

    id: dialog
    visible: false
    background: Rectangle {
        color: "white"
        opacity: 0.1    // number between 0-1
    }
    contentItem: Rectangle {
        id: rectangle
        //color: "black"
        color: Qt.rgba(0,0,0,opacityLevel)   // 0.5 means opacity level
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
                    id: step0005Button
                    text: qsTr("0.005")
                    font.pixelSize: 10
                    onCheckedChanged: {
                        if (step0005Button.checked)
                        {
                            selectedStepSize = 0.005
                        }
                    }
                }
                RadioButton {
                    id: step0002Button
                    text: qsTr("0.002")
                    font.pixelSize: 10
                    onCheckedChanged: {
                        if (step0002Button.checked)
                        {
                            selectedStepSize = 0.002
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
                RadioButton {
                    id: stepCustomButton
                    text: qsTr("Custom")
                    font.pixelSize: 10
                }
                TextField{
                    id: stepCustomText
                    width: 100
                    Layout.preferredWidth: 100
                    horizontalAlignment: TextInput.AlignHCenter
                    validator: DoubleValidator {
                        bottom: -100
                        top: 100
                        decimals: 6
                        notation: DoubleValidator.StandardNotation
                    }
                }
            }
            ScrollView {
                Timer {
                    id:timer
                    interval: 600; running: true; repeat: true
                }

                implicitWidth:  550
                implicitHeight: 800
                clip: true
                ColumnLayout {
                    RowLayout {
                        id: opacityView
                        Text {
                            text: qsTr("透明度")
                            color: "white"
                        }
                        Slider {
                            id: opacitySlider
                            width: 300
                            Layout.preferredWidth: 300
                            height: 48
                            stepSize: 0.05
                            to: 1
                            from: 0
                            value: 1
                            onValueChanged: {
                                opacityLevel = opacitySlider.value
                            }
                        }

                    }

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
//                                                err.color = "green"
                                           err.color = "green"
                                       }
                                    }
                                }
                                onClicked: {
                                    baseModuleManager.clearMotorAlarmState(modelData)
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
                                   var stepSize = stepCustomButton.checked ? stepCustomText.text : selectedStepSize
                                   var res = baseModuleManager.stepMove(modelData,stepSize,false)
                                   console.log("result: "+res)
                                }
                            }
                            RoundButton{
                                text:"+"
                                onClicked: {
                                   //console.log("move "+modelData);
                                   var stepSize = stepCustomButton.checked ? stepCustomText.text : selectedStepSize
                                   var res = baseModuleManager.stepMove(modelData,stepSize,true)
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
