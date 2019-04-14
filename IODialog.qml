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
            ScrollView {
                Timer {
                    interval: 300; running: true; repeat: true
                    onTriggered: {
                        if (dialog.visible) {
                        }
                    }
                }

                implicitWidth:  150
                implicitHeight: 600
                clip: true
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
                }
            }
        }
     }
}
