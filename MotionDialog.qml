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
            ColumnLayout {
                RadioButton {
                    id: step10Button
                    text: qsTr("10")
                    font.pixelSize: 10
                    onCheckedChanged: {
                        if (step10Button.checked)
                        {
                            console.log(selectedStepSize)
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
                            console.log(selectedStepSize)
                            selectedStepSize = 10
                        }
                    }
                }
                RadioButton {
                    text: qsTr("0.1")
                    font.pixelSize: 10
                    checked: true
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
                                console.log("AA X move +");
                                var res = baseModuleManager.stepMove("AA_X", 0.1, true)
                                console.log("result: " + res)
                            }
                        }
                        RoundButton {
                            text: "-"
                            onClicked: {
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
                        }
                        RoundButton {
                            text: "-"
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
                        }
                        RoundButton {
                            text: "-"
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
                        }
                        RoundButton {
                            text: "-"
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
                        }
                        RoundButton {
                            text: "-"
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
                        }
                        RoundButton {
                            text: "-"
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
                        }
                        RoundButton {
                            text: "-"
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
                        }
                        RoundButton {
                            text: "-"
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
                        }
                        RoundButton {
                            text: "-"
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
