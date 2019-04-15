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
                    interval: 300; running: true; repeat: true
                    onTriggered: {
                        if (dialog.visible) {
                            aa1_x_label.text = baseModuleManager.getMotorFeedbackPos(m_AA1_X)
                            aa1_y_label.text = baseModuleManager.getMotorFeedbackPos(m_AA1_Y)
                            aa1_z_label.text = baseModuleManager.getMotorFeedbackPos(m_AA1_Z)
                            aa1_a_label.text = baseModuleManager.getMotorFeedbackPos(m_AA1_A)
                            aa1_b_label.text = baseModuleManager.getMotorFeedbackPos(m_AA1_B)
                            aa1_c_label.text = baseModuleManager.getMotorFeedbackPos(m_AA1_C)
                            sut_x_label.text = baseModuleManager.getMotorFeedbackPos(m_SUT1_X)
                            sut_y_label.text = baseModuleManager.getMotorFeedbackPos(m_SUT1_Y)
                            sut_z_label.text = baseModuleManager.getMotorFeedbackPos(m_SUT1_Z)
                            lut_x_label.text = baseModuleManager.getMotorFeedbackPos(m_LUT1_X)
                            lut_y_label.text = baseModuleManager.getMotorFeedbackPos(m_LUT1_Y)
                            lut_z_label.text = baseModuleManager.getMotorFeedbackPos(m_LUT1_Z)
                        }
                    }
                }

                implicitWidth:  400
                implicitHeight: 600
                clip: true
                ColumnLayout {
                    RowLayout {
                        Text {
                            text: " AA_X "
                            color: "white"
                        }
                        RoundButton {
                            onClicked: {
                                baseModuleManager.homeMotor(m_AA1_X)
                            }
                            transformOrigin: Item.Center
                            display: Button.IconOnly
                            icon.source: "icons/home.png"
                            icon.color: "cyan"
                        }
                        RoundButton {
                            text: "-"
                            onClicked: {
                                var res = baseModuleManager.stepMove(m_AA1_X, selectedStepSize, false)
                            }
                        }
                        RoundButton {
                            text: "+"
                            onClicked: {
                                var res = baseModuleManager.stepMove(m_AA1_X, selectedStepSize, true)
                            }
                        }
                        Label {
                            id: aa1_x_label
                            color: "lightskyblue"
                        }
                    }
                    RowLayout {
                        Text {
                            text: " AA_Y "
                            color: "white"
                        }
                        RoundButton {
                            onClicked: {
                                baseModuleManager.homeMotor(m_AA1_Y)
                            }
                            transformOrigin: Item.Center
                            display: Button.IconOnly
                            icon.source: "icons/home.png"
                            icon.color: "cyan"
                        }
                        RoundButton {
                            text: "-"
                            onClicked: {
                                 var res = baseModuleManager.stepMove(m_AA1_Y, selectedStepSize, false)
                            }
                        }
                        RoundButton {
                            text: "+"
                            onClicked: {
                                 var res = baseModuleManager.stepMove(m_AA1_Y, selectedStepSize, true)
                            }
                        }
                        Label {
                            id: aa1_y_label
                            color: "lightskyblue"
                        }
                    }
                    RowLayout {
                        Text {
                            text: " AA_Z "
                            color: "white"
                        }
                        RoundButton {
                            onClicked: {
                                baseModuleManager.homeMotor(m_AA1_Z)
                            }
                            transformOrigin: Item.Center
                            display: Button.IconOnly
                            icon.source: "icons/home.png"
                            icon.color: "cyan"
                        }
                        RoundButton {
                            text: "-"
                            onClicked: {
                                 var res = baseModuleManager.stepMove(m_AA1_Z, selectedStepSize, false)
                            }
                        }
                        RoundButton {
                            text: "+"
                            onClicked: {
                                 var res = baseModuleManager.stepMove(m_AA1_Z, selectedStepSize, true)
                            }
                        }
                        Label {
                            id: aa1_z_label
                            color: "lightskyblue"
                        }
                    }
                    RowLayout {
                        Text {
                            text: " AA_A "
                            color: "white"
                        }
                        RoundButton {
                            onClicked: {
                                baseModuleManager.homeMotor(m_AA1_A)
                            }
                            transformOrigin: Item.Center
                            display: Button.IconOnly
                            icon.source: "icons/home.png"
                            icon.color: "cyan"
                        }
                        RoundButton {
                            text: "-"
                            onClicked: {
                                 var res = baseModuleManager.stepMove(m_AA1_A, selectedStepSize, false)
                            }
                        }
                        RoundButton {
                            text: "+"
                            onClicked: {
                                 var res = baseModuleManager.stepMove(m_AA1_A, selectedStepSize, true)
                            }
                        }
                        Label {
                            id: aa1_a_label
                            color: "lightskyblue"
                        }
                    }

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
                        Text {
                            text: " AA_B "
                            color: "white"
                        }
                        RoundButton {
                            onClicked: {
                                baseModuleManager.homeMotor(m_AA1_B)
                            }
                            transformOrigin: Item.Center
                            display: Button.IconOnly
                            icon.source: "icons/home.png"
                            icon.color: "cyan"
                        }
                        RoundButton {
                            text: "-"
                            onClicked: {
                                 var res = baseModuleManager.stepMove(m_AA1_B, selectedStepSize, false)
                            }
                        }
                        RoundButton {
                            text: "+"
                            onClicked: {
                                 var res = baseModuleManager.stepMove(m_AA1_B, selectedStepSize, true)
                            }
                        }
                        Label {
                            id: aa1_b_label
                            color: "lightskyblue"
                        }
                    }
                    RowLayout {
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
                    RowLayout {
                        Text {
                            text: " AA_C "
                            color: "white"
                        }
                        RoundButton {
                            onClicked: {
                                baseModuleManager.homeMotor(m_AA1_C)
                            }
                            transformOrigin: Item.Center
                            display: Button.IconOnly
                            icon.source: "icons/home.png"
                            icon.color: "cyan"
                        }
                        RoundButton {
                            text: "-"
                            onClicked: {
                                var res = baseModuleManager.stepMove(m_AA_C, selectedStepSize, false)
                            }
                        }
                        RoundButton {
                            text: "+"
                            onClicked: {
                                var res = baseModuleManager.stepMove(m_AA_C, selectedStepSize, true)
                            }
                        }
                        Label {
                            id: aa1_c_label
                            color: "lightskyblue"
                        }
                    }
                    RowLayout {
                        Text {
                            text: " SUT_X"
                            color: "white"
                        }
                        RoundButton {
                            onClicked: {
                                baseModuleManager.homeMotor(m_SUT1_X)
                            }
                            transformOrigin: Item.Center
                            display: Button.IconOnly
                            icon.source: "icons/home.png"
                            icon.color: "cyan"
                        }
                        RoundButton {
                            text: "-"
                            onClicked: {
                                var res = baseModuleManager.stepMove(m_SUT1_X, selectedStepSize, false)
                            }
                        }
                        RoundButton {
                            text: "+"
                            onClicked: {
                                var res = baseModuleManager.stepMove(m_SUT1_X, selectedStepSize, true)
                            }
                        }
                        Label {
                            id: sut_x_label
                            color: "lightskyblue"

                        }
                    }
                    RowLayout {
                        Text {
                            text: " SUT_Y"
                            color: "white"
                        }
                        RoundButton {
                            onClicked: {
                                baseModuleManager.homeMotor(m_SUT1_Y)
                            }
                            transformOrigin: Item.Center
                            display: Button.IconOnly
                            icon.source: "icons/home.png"
                            icon.color: "cyan"
                        }
                        RoundButton {
                            text: "-"
                            onClicked: {
                                var res = baseModuleManager.stepMove(m_SUT1_Y, selectedStepSize, false)
                            }
                        }
                        RoundButton {
                            text: "+"
                            onClicked: {
                                var res = baseModuleManager.stepMove(m_SUT1_Y, selectedStepSize, true)
                            }
                        }
                        Label {
                            id: sut_y_label
                            color: "lightskyblue"
                        }
                    }
                    RowLayout {
                        Text {
                            text: " SUT_Z"
                            color: "white"
                        }
                        RoundButton {
                            onClicked: {
                                baseModuleManager.homeMotor(m_SUT1_Z)
                            }
                            transformOrigin: Item.Center
                            display: Button.IconOnly
                            icon.source: "icons/home.png"
                            icon.color: "cyan"
                        }
                        RoundButton {
                            text: "-"
                            onClicked: {
                                var res = baseModuleManager.stepMove(m_SUT1_Z, selectedStepSize, false)
                            }
                        }
                        RoundButton {
                            text: "+"
                            onClicked: {
                                var res = baseModuleManager.stepMove(m_SUT1_Z, selectedStepSize, true)
                            }
                        }
                        Label {
                            id: sut_z_label
                            color: "lightskyblue"
                        }
                    }
                    RowLayout {
                        Text {
                            text: " LUT_X"
                            color: "white"
                        }
                        RoundButton {
                            onClicked: {
                                baseModuleManager.homeMotor(m_LUT1_X)
                            }
                            transformOrigin: Item.Center
                            display: Button.IconOnly
                            icon.source: "icons/home.png"
                            icon.color: "cyan"
                        }
                        RoundButton {
                            text: "-"
                            onClicked: {
                                var res = baseModuleManager.stepMove(m_LUT1_X, selectedStepSize, false)
                            }
                        }
                        RoundButton {
                            text: "+"
                            onClicked: {
                                var res = baseModuleManager.stepMove(m_LUT1_X, selectedStepSize, true)
                            }
                        }
                        Label {
                            id: lut_x_label
                            color: "lightskyblue"
                        }
                    }
                    RowLayout {
                        Text {
                            text: " LUT_Y"
                            color: "white"
                        }
                        RoundButton {
                            onClicked: {
                                baseModuleManager.homeMotor(m_LUT1_Y)
                            }
                            transformOrigin: Item.Center
                            display: Button.IconOnly
                            icon.source: "icons/home.png"
                            icon.color: "cyan"
                        }
                        RoundButton {
                            text: "-"
                            onClicked: {
                                var res = baseModuleManager.stepMove(m_LUT1_Y, selectedStepSize, false)
                            }
                        }
                        RoundButton {
                            text: "+"
                            onClicked: {
                                var res = baseModuleManager.stepMove(m_LUT1_Y, selectedStepSize, true)
                            }
                        }
                        Label {
                            id: lut_y_label
                            color: "lightskyblue"
                        }
                    }
                    RowLayout {
                        Text {
                            text: " LUT_Z"
                            color: "white"
                        }
                        RoundButton {
                            onClicked: {
                                baseModuleManager.homeMotor(m_LUT1_Z)
                            }
                            transformOrigin: Item.Center
                            display: Button.IconOnly
                            icon.source: "icons/home.png"
                            icon.color: "cyan"
                        }
                        RoundButton {
                            text: "-"
                            onClicked: {
                                var res = baseModuleManager.stepMove(m_LUT1_Z, selectedStepSize, false)
                            }
                        }
                        RoundButton {
                            text: "+"
                            onClicked: {
                                var res = baseModuleManager.stepMove(m_LUT1_Z, selectedStepSize, true)
                            }
                        }
                        Label {
                            id: lut_z_label
                            color: "lightskyblue"
                        }
                    }
                }
            }
        }
     }
}
