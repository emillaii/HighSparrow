import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11

ColumnLayout {
    RowLayout {
        Label {
            text: qsTr("SUT_X")
        }
        TextField {
            text: sutCarrierParams.VisionX
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                bottom: -100
                top: 100
                decimals: 3
                notation: DoubleValidator.StandardNotation
            }
            onFocusChanged: {
                if(!focus) sutCarrierParams.setVisionX(text)
            }
        }
        Label {
            text: qsTr("SUT_Y")
        }
        TextField {
            text: sutCarrierParams.VisionY
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                bottom: -100
                top: 100
                decimals: 3
                notation: DoubleValidator.StandardNotation
            }
            onFocusChanged: {
                if (!focus) sutCarrierParams.setVisionY(text)
            }
        }
        Label {
            text: qsTr("SUT_Z")
        }
        TextField {
            text: sutCarrierParams.VisionZ
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                bottom: -100
                top: 100
                decimals: 3
                notation: DoubleValidator.StandardNotation
            }
            onFocusChanged: {
                if(!focus) sutCarrierParams.setVisionZ(text)
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
                sutModule.moveToPR()
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
