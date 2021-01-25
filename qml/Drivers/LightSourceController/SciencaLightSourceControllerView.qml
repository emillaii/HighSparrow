import QtQuick 2.0
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0

Popup{
    contentItem: ColumnLayout{
        RowLayout{
            Label{
                text: qsTr("Com:")
            }
            ColumnLayout {
                TextField {
                    id: txtPortName
                    readOnly: true
                    text: lightPanelController.portName
                    onTextChanged: {
                        lightPanelController.setPortName(text)
                    }
                    MouseArea{
                        anchors.fill: parent
                        onClicked: {
                            cmbPortNames.popup.open()
                        }
                    }
                }
                ComboBox{
                    id: cmbPortNames

                    visible: false
                    model: 50
                    delegate:
                        Text {
                        text: "COM" + (index + 1)
                        color: "white"
                        MouseArea{
                            anchors.fill: parent
                            onClicked: {
                                txtPortName.text = text
                                cmbPortNames.popup.close()
                            }
                        }
                    }
//                    popup.onOpenedChanged: {
//                        visible = popup.opened
//                    }
                }
            }
            Button{
                text: qsTr("Open")
                onClicked: {
                    lightPanelController.open()
                }
            }
            Button{
                text: qsTr("Close")
                onClicked: {
                    lightPanelController.close()
                }
            }
        }

        RowLayout{
            Label{
                text: qsTr("Brightness:")
            }
            TextField {
                text: lightPanelController.brightness
                validator: IntValidator{}
                onEditingFinished: {
                    lightPanelController.setBrightness(text)
                }
            }
            Button{
                text: qsTr("Set")
                onClicked: {
                    lightPanelController.setIntensity(lightPanelController.brightness)
                }
            }
        }
    }
}
