import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11
import FileContentItem 1.0
import QtQuick.Dialogs 1.2

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
                    onTextChanged: {
                        aaHeadParams.setPickLensPositionX(text)
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
                    text: title_move_to
                    width: 40
                    height: 40
                    visible: root.checked
                    onClicked: {
                        console.log("Move Tsso");
                    }
                }
                Button {
                    text: title_read_encoder
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
                    text: title_move_to
                    width: 40
                    height: 40
                    visible: root.checked
                    onClicked: {
                        console.log("Move Tsso");
                    }
                }
                Button {
                    text: title_read_encoder
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
        title: qsTr("Uplook PR Config")
        ColumnLayout {
            RowLayout {
                Label {
                    text: qsTr("Load PR Name")
                }
                TextField {
                    id: uplookCameraPR
                    text: aaHeadParams.visionUplookPR
                    horizontalAlignment: TextInput.AlignHCenter
                    onTextChanged: {
                        aaHeadParams.setVisionUplookPR(text)
                    }
                }
                FileDialog {
                    id: loadfileDialog
                    title: "选择加载PR文件"
                    selectExisting: true
                    selectFolder: false
                    selectMultiple: false

                    nameFilters: ["avdata文件 (*.avdata)"]
                    onAccepted: {
                        uplookCameraPR.text = loadfileDialog.fileUrl
                        aaHeadParams.setVisionUplookPR(uplookCameraPR.text)
                    }
                }

                Button {
                    text: qsTr("Load PR Result")
                    width: 20
                    height: 40
                    onClicked: {
                        loadfileDialog.open()
                    }
                }
            }
        }
    }
}
