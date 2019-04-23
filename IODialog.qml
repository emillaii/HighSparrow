import QtQuick 2.3
import QtQuick.Controls 2.4
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.11
import QtQuick 2.11

Popup {
    property double selectedStepSize: 0.1

    id: dialog
    visible: false
    contentItem: Rectangle {
        id: rectangle
        color: "black"
        implicitWidth: 400
        implicitHeight: 800
        RowLayout {
            Timer {
                id:timer
                interval: 300; running: true; repeat: true
            }
            ScrollView {
                id:output_sv
                implicitWidth:  200
                implicitHeight: 600
                clip: true
                ColumnLayout{
                    Switch {
                        id:special_ouput
                        text: qsTr("SUT Presser")
                        Connections{
                            target: timer
                            onTriggered: {
                                if (dialog.visible) {
                                    if(special_ouput.checked != baseModuleManager.getOutput("SUT1补充_1"))
                                        special_ouput.toggle()
                                    //if(index%2==0&&!swich_id.checked)swich_id.toggle()
                                }
                            }
                        }
                        onCheckedChanged: {
                            baseModuleManager.setOutput("SUT1补充_1", checked)
                            baseModuleManager.setOutput("SUT1补充_2", !checked)
                        }
                    }
                    ListView{
                        model: outputList
                        height: 1100
                        anchors.top: special_ouput.bottom
                        delegate: Switch{
                            id:output_switch_id
                            Connections{
                                target: timer
                                onTriggered: {
                                    if (dialog.visible) {
                                        if(output_switch_id.checked != baseModuleManager.getOutput(modelData))
                                            output_switch_id.toggle()
                                        //if(index%2==0&&!swich_id.checked)swich_id.toggle()
                                    }
                                }
                            }
                            text:qsTr(modelData)
                            onCheckableChanged: {
                                baseModuleManager.setOutput(modelData,checked);
                            }
                        }
                    }
                }
/*
                ColumnLayout {

                    Switch {
                        text: qsTr("SUT Presser")
                        onCheckedChanged: {
                            baseModuleManager.setOutput("SUT1补充_1", checked)
                            baseModuleManager.setOutput("SUT1补充_2", !checked)
                        }
                    }
                    Switch {
                        text: qsTr("SUT1吸真空")
                        onCheckedChanged: {
                                baseModuleManager.setOutput("SUT1吸真空", !checked)
                        }
                    }
                    Switch {
                        text: qsTr("AA1_Gripper")
                        onCheckedChanged: {
                                baseModuleManager.setOutput("AA1_GripON", checked)
                        }
                    }
                    Switch {
                        text: qsTr("LUT吸真空")
                        onCheckedChanged: {
                                baseModuleManager.setOutput("LUT吸真空", checked)
                        }
                    }
                    Switch {
                        text: qsTr("LUT破真空")
                        onCheckedChanged: {
                                baseModuleManager.setOutput("LUT破真空", checked)
                        }
                    }
                    Switch {
                        text: qsTr("SUT1点胶阀")
                        onCheckedChanged: {
                                baseModuleManager.setOutput("SUT1点胶阀", checked)
                        }
                    }

                }
//*/
            }
            ScrollView{
                id:input_sv
                anchors.left:output_sv.right
                implicitWidth:  200
                implicitHeight: 600
                clip: true
                ListView{
                    model:inputList
                    anchors.fill: parent
                    delegate: Switch{
                        id:input_swich_id
                        Connections{
                            target: timer
                            onTriggered: {
                                if (dialog.visible) {
                                    if(input_swich_id.checked != baseModuleManager.getOutput(modelData))
                                        input_swich_id.toggle()
                                    //if(index%2==0&&!swich_id.checked)swich_id.toggle()
                                }
                            }
                        }
                        text:qsTr(modelData)
                        onCheckableChanged: {
                            baseModuleManager.setOutput(modelData,checked);
                        }
                    }
                }
            }
        }
     }
}
