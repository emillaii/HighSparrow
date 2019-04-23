import QtQuick 2.3
import QtQuick.Controls 2.4
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.11
import QtQuick 2.11
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
                        if (step001Button.checked)
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
                        if (step001Button.checked)
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
                        if (step001Button.checked)
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
                        if (step001Button.checked)
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
                        if (step001Button.checked)
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
                        if (step001Button.checked)
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
            }
            ScrollView {
                Timer {
                    id:timer
                    interval: 300; running: true; repeat: true
                }

                implicitWidth:  400
                implicitHeight: 600
                clip: true

                ColumnLayout {
                    RowLayout {
                        Text {
                            text: " AA_A_Inter "
                            color: "white"
                        }
                        RoundButton {
                            text: "+"
                            onClicked: {
                                highSprrow.aa_A_Inter(selectedStepSize);
                            }
                        }
                        RoundButton {
                            text: "-"
                            onClicked: {
                                highSprrow.aa_A_Inter(-selectedStepSize);
                            }
                        }
                    }
                    RowLayout {
                        id:inter_layout_b
                        Text {
                            text: " AA_B_Inter "
                            color: "white"
                        }
                        RoundButton {
                            text: "+"
                            onClicked: {
                                highSprrow.aa_B_Inter(selectedStepSize);
                            }
                        }
                        RoundButton {
                            text: "-"
                            onClicked: {
                                highSprrow.aa_B_Inter(-selectedStepSize);
                            }
                        }
                    }

                    ListView{
                        anchors.top:inter_layout_b.bottom
                        id:list
                        //anchors.top:  inter_layout_b.bottom
                        height: 1000
                        model: motorsNames
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
                                    baseModuleManager.motorSeekOrigin(modelData)
                                }
                                transformOrigin: Item.Center
                                display: Button.IconOnly
                                icon.source: "icons/home.png"
                                icon.color: "cyan"
                            }
                            RoundButton{
                                text:"+"
                                onClicked: {
                                   var res = baseModuleManager.stepMove(modelData,selectedStepSize,true)
                                   console.log(motorsNames.length)
                                   console.log("result: "+res)
                                }
                            }
                            RoundButton{
                                text:"-"
                                onClicked: {
                                   var res = baseModuleManager.stepMove(modelData,selectedStepSize,false)
                                   console.log("result: "+res)
                                }
                            }
                            Label{
                                id:lbl
                                color:"lightskyblue"
                                Connections{
                                   target: timer
                                   onTriggered:{
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
