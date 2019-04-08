import QtQuick 2.3
import QtQuick.Controls 2.4
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.11
import QtQuick 2.11
Popup {
    property double selectedStepSize: 0.1

    readonly property string m_AA1_X: "AA1_X"
    readonly property string m_AA1_Y: "AA1_Y"
    readonly property string m_AA1_Z: "AA1_Z"
    readonly property string m_AA1_A: "AA1_A"
    readonly property string m_AA1_B: "AA1_B"
    readonly property string m_AA1_C: "AA1_C"
    readonly property string m_SUT1_X: "SUT1_X"
    readonly property string m_SUT1_Y: "SUT1_Y"
    readonly property string m_SUT1_Z: "SUT_Z"
    readonly property string m_LUT1_X: "LUT_X"
    readonly property string m_LUT1_Y: "LUT_Y"
    readonly property string m_LUT1_Z: "LUT_Z"

    id: dialog
    visible: false
    contentItem: Rectangle {
        id: rectangle
        color: "black"
        implicitWidth: 400
        implicitHeight: 800
        RowLayout {
            ColumnLayout {
                RadioButton {
                    id: step10Button
                    text: qsTr("10")
                    font.pixelSize: 10
                    onCheckedChanged: {
                        if (step10Button.checked)
                        {
                            selectedStepSize = 10
                        }
                        console.log(selectedStepSize)
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
                        console.log(selectedStepSize)
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
                        console.log(selectedStepSize)
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
                implicitHeight: 300
                clip: true
                ColumnLayout {
                    RowLayout {
                        Text {
                            text: " AA_X"
                            color: "white"
                        }
                        RoundButton {
                            text: "+"
                            onClicked: {
                                var res = baseModuleManager.stepMove(m_AA1_X, selectedStepSize, true)
                                console.log("result: " + res)
                            }
                        }
                        RoundButton {
                            text: "-"
                            onClicked: {
                                var res = baseModuleManager.stepMove(m_AA1_X, selectedStepSize, false)
                                console.log("AA X move -");
                            }
                        }
                        Label {
                            text: qsTr("100")
                            color: "lightskyblue"
                        }
                    }
                    RowLayout {
                        Text {
                            text: " AA_Y"
                            color: "white"
                        }
                        RoundButton {
                            text: "+"
                            onClicked: {
                                 var res = baseModuleManager.stepMove(m_AA1_Y, selectedStepSize, true)
                            }
                        }
                        RoundButton {
                            text: "-"
                            onClicked: {
                                 var res = baseModuleManager.stepMove(m_AA1_Y, selectedStepSize, false)
                            }
                        }
                        Label {
                            text: qsTr("100")
                            color: "lightskyblue"
                        }
                    }
                    RowLayout {
                        Text {
                            text: " AA_Z"
                            color: "white"
                        }
                        RoundButton {
                            text: "+"
                            onClicked: {
                                 var res = baseModuleManager.stepMove(m_AA1_Z, selectedStepSize, true)
                            }
                        }
                        RoundButton {
                            text: "-"
                            onClicked: {
                                 var res = baseModuleManager.stepMove(m_AA1_Z, selectedStepSize, false)
                            }
                        }
                        Label {
                            text: qsTr("100")
                            color: "lightskyblue"
                        }
                    }
                    RowLayout {
                        Text {
                            text: " AA_A"
                            color: "white"
                        }
                        RoundButton {
                            text: "+"
                            onClicked: {
                                 var res = baseModuleManager.stepMove(m_AA1_A, selectedStepSize, true)
                            }
                        }
                        RoundButton {
                            text: "-"
                            onClicked: {
                                 var res = baseModuleManager.stepMove(m_AA1_A, selectedStepSize, false)
                            }
                        }
                        Label {
                            text: qsTr("100")
                            color: "lightskyblue"
                        }
                    }
                    RowLayout {
                        Text {
                            text: " AA_B"
                            color: "white"
                        }
                        RoundButton {
                            text: "+"
                            onClicked: {
                                 var res = baseModuleManager.stepMove(m_AA1_B, selectedStepSize, true)
                            }
                        }
                        RoundButton {
                            text: "-"
                            onClicked: {
                                 var res = baseModuleManager.stepMove(m_AA1_B, selectedStepSize, false)
                            }
                        }
                        Label {
                            text: qsTr("100")
                            color: "lightskyblue"
                        }
                    }
                    RowLayout {
                        Text {
                            text: " AA_C"
                            color: "white"
                        }
                        RoundButton {
                            text: "+"
                            onClicked: {
                                 var res = baseModuleManager.stepMove(m_AA1_C, selectedStepSize, true)
                            }
                        }
                        RoundButton {
                            text: "-"
                            onClicked: {
                                 var res = baseModuleManager.stepMove(m_AA1_C, selectedStepSize, false)
                            }
                        }
                        Label {
                            text: qsTr("100")
                            color: "lightskyblue"
                        }
                    }
                    RowLayout {
                        Text {
                            text: " SUT_X"
                            color: "white"
                        }
                        RoundButton {
                            text: "+"
                            onClicked: {
                                 var res = baseModuleManager.stepMove(m_SUT1_X, selectedStepSize, true)
                            }
                        }
                        RoundButton {
                            text: "-"
                            onClicked: {
                                 var res = baseModuleManager.stepMove(m_SUT1_X, selectedStepSize, false)
                            }
                        }
                        Label {
                            text: qsTr("100")
                            color: "lightskyblue"
                        }
                    }
                    RowLayout {
                        Text {
                            text: " SUT_Y"
                            color: "white"
                        }
                        RoundButton {
                            text: "+"
                            onClicked: {
                                 var res = baseModuleManager.stepMove(m_SUT1_Y, selectedStepSize, true)
                            }
                        }
                        RoundButton {
                            text: "-"
                            onClicked: {
                                 var res = baseModuleManager.stepMove(m_SUT1_Y, selectedStepSize, false)
                            }
                        }
                        Label {
                            text: qsTr("100")
                            color: "lightskyblue"
                        }
                    }
                    RowLayout {
                        Text {
                            text: " SUT_Z"
                            color: "white"
                        }
                        RoundButton {
                            text: "+"
                            onClicked: {
                                 var res = baseModuleManager.stepMove(m_SUT1_Z, selectedStepSize, true)
                            }
                        }
                        RoundButton {
                            text: "-"
                            onClicked: {
                                 var res = baseModuleManager.stepMove(m_SUT1_Z, selectedStepSize, false)
                            }
                        }
                        Label {
                            text: qsTr("100")
                            color: "lightskyblue"
                        }
                    }
                }
            }
        }
     }
}
