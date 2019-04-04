import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11

ColumnLayout {
    RowLayout {
        Label {
            text: qsTr("")
        }
    }
    GroupBox{
        title: qsTr("Uplook Pick Lens")
        ColumnLayout {
            RowLayout {
                Label {
                    text: qsTr("AA_X")
                }
                TextField {
                    text: aaHeadParams.pickLensPositionX
                    horizontalAlignment: TextInput.AlignHCenter
                    validator: DoubleValidator {
                        bottom: -100
                        top: 100
                        decimals: 3
                        notation: DoubleValidator.StandardNotation
                    }
                }
                Label {
                    text: qsTr("AA_Y")
                }
                TextField {
                    text: aaHeadParams.pickLensPositionY
                    horizontalAlignment: TextInput.AlignHCenter
                    validator: DoubleValidator {
                        bottom: -100
                        top: 100
                        decimals: 3
                        notation: DoubleValidator.StandardNotation
                    }
                }
                Label {
                    text: qsTr("AA_Z")
                }
                TextField {
                    text: aaHeadParams.pickLensPositionY
                    horizontalAlignment: TextInput.AlignHCenter
                    validator: DoubleValidator {
                        bottom: -100
                        top: 100
                        decimals: 3
                        notation: DoubleValidator.StandardNotation
                    }
                }
            }
            RowLayout {
                Label {
                    text: qsTr("AA_A")
                }
                TextField {
                    text: aaHeadParams.pickLensPositionY
                    horizontalAlignment: TextInput.AlignHCenter
                    validator: DoubleValidator {
                        bottom: -100
                        top: 100
                        decimals: 3
                        notation: DoubleValidator.StandardNotation
                    }
                }
                Label {
                    text: qsTr("AA_B")
                }
                TextField {
                    text: aaHeadParams.pickLensPositionY
                    horizontalAlignment: TextInput.AlignHCenter
                    validator: DoubleValidator {
                        bottom: -100
                        top: 100
                        decimals: 3
                        notation: DoubleValidator.StandardNotation
                    }
                }
                Label {
                    text: qsTr("AA_C")
                }
                TextField {
                    text: aaHeadParams.pickLensPositionY
                    horizontalAlignment: TextInput.AlignHCenter
                    validator: DoubleValidator {
                        bottom: -100
                        top: 100
                        decimals: 3
                        notation: DoubleValidator.StandardNotation
                    }
                }
            }

            RowLayout {
                Button {
                    text: qsTr("Move To")
                    width: 40
                    height: 40
                    visible: root.checked
                    onClicked: {
                        console.log("Move Tsso");
                    }
                }
                Button {
                    text: qsTr("Read Encoder")
                    width: 20
                    height: 40
                    visible: root.checked
                    onClicked: {
                        console.log("Read Encdoer");
                    }
                }
            }
        }
    }
    GroupBox{
        title: qsTr("Uplook Pick Lens")
        ColumnLayout {
            RowLayout {
                Label {
                    text: qsTr("AA_X")
                }
                TextField {
                    text: aaHeadParams.pickLensPositionX
                    horizontalAlignment: TextInput.AlignHCenter
                    validator: DoubleValidator {
                        bottom: -100
                        top: 100
                        decimals: 3
                        notation: DoubleValidator.StandardNotation
                    }
                }
                Label {
                    text: qsTr("AA_Y")
                }
                TextField {
                    text: aaHeadParams.pickLensPositionY
                    horizontalAlignment: TextInput.AlignHCenter
                    validator: DoubleValidator {
                        bottom: -100
                        top: 100
                        decimals: 3
                        notation: DoubleValidator.StandardNotation
                    }
                }
                Label {
                    text: qsTr("AA_Z")
                }
                TextField {
                    text: aaHeadParams.pickLensPositionY
                    horizontalAlignment: TextInput.AlignHCenter
                    validator: DoubleValidator {
                        bottom: -100
                        top: 100
                        decimals: 3
                        notation: DoubleValidator.StandardNotation
                    }
                }
            }
            RowLayout {
                Label {
                    text: qsTr("AA_A")
                }
                TextField {
                    text: aaHeadParams.pickLensPositionY
                    horizontalAlignment: TextInput.AlignHCenter
                    validator: DoubleValidator {
                        bottom: -100
                        top: 100
                        decimals: 3
                        notation: DoubleValidator.StandardNotation
                    }
                }
                Label {
                    text: qsTr("AA_B")
                }
                TextField {
                    text: aaHeadParams.pickLensPositionY
                    horizontalAlignment: TextInput.AlignHCenter
                    validator: DoubleValidator {
                        bottom: -100
                        top: 100
                        decimals: 3
                        notation: DoubleValidator.StandardNotation
                    }
                }
                Label {
                    text: qsTr("AA_C")
                }
                TextField {
                    text: aaHeadParams.pickLensPositionY
                    horizontalAlignment: TextInput.AlignHCenter
                    validator: DoubleValidator {
                        bottom: -100
                        top: 100
                        decimals: 3
                        notation: DoubleValidator.StandardNotation
                    }
                }
            }

            RowLayout {
                Button {
                    text: qsTr("Move To")
                    width: 40
                    height: 40
                    visible: root.checked
                    onClicked: {
                        console.log("Move Tsso");
                    }
                }
                Button {
                    text: qsTr("Read Encoder")
                    width: 20
                    height: 40
                    visible: root.checked
                    onClicked: {
                        console.log("Read Encdoer");
                    }
                }
            }
        }
    }
}
