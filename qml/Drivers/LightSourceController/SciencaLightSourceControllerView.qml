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
                    text: sciencaLightSourceController.portName
                    onTextChanged: {
                        sciencaLightSourceController.setPortName(text)
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
                    sciencaLightSourceController.open()
                }
            }
            Button{
                text: qsTr("Close")
                onClicked: {
                    sciencaLightSourceController.close()
                }
            }
        }

        RowLayout{
            Label{
                text: qsTr("CTChannelIntensity:")
            }
            TextField {
                text: sciencaLightSourceController.colorTemperatureIntensity
                validator: IntValidator{}
                onEditingFinished: {
                    sciencaLightSourceController.setColorTemperatureIntensity(text)
                }
            }
            Button{
                text: qsTr("Set")
                onClicked: {
                    sciencaLightSourceController.setIntensity(sciencaLightSourceController.ColorTemperature)
                }
            }
        }

        RowLayout{
            Label{
                text: qsTr("WLChannelIntensity:")
            }
            TextField {
                text: sciencaLightSourceController.waveLengthIntensity
                validator: IntValidator{}
                onEditingFinished: {
                    sciencaLightSourceController.setWaveLengthIntensity(text)
                }
            }
            Button{
                text: qsTr("Set")
                onClicked: {
                    sciencaLightSourceController.setIntensity(sciencaLightSourceController.WaveLength)
                }
            }
        }
    }
}

/*##^## Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
 ##^##*/
