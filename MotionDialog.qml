import QtQuick 2.3
import QtQuick.Controls 2.4
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.11
import QtQuick 2.11
Popup {
    id: dialog
    visible: false
    contentItem: Rectangle {
        id: rectangle
        color: "black"
        implicitWidth: 400
        implicitHeight: 800
        ScrollView {
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
