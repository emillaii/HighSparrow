import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11

ColumnLayout {
    RowLayout {
        Label {
            text: qsTr("SUT_X")
        }
        TextField {
            text: sutCarrierParams.SafetyX
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                bottom: -100
                top: 100
                decimals: 3
                notation: DoubleValidator.StandardNotation
            }
            onFocusChanged: {
                if(!focus) sutCarrierParams.setSafetyX(text)
            }
        }
        Label {
            text: qsTr("SUT_Y")
        }
        TextField {
            text: sutCarrierParams.SafetyY
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                bottom: -100
                top: 100
                decimals: 3
                notation: DoubleValidator.StandardNotation
            }
            onFocusChanged: {
                if (!focus) sutCarrierParams.setSafetyY(text)
            }
        }
        Label {
            text: qsTr("SUT_Z")
        }
        TextField {
            text: sutCarrierParams.SafetyZ
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 3
                notation: DoubleValidator.StandardNotation
            }
            onFocusChanged: {
                if(!focus) sutCarrierParams.setSafetyZ(text)
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
