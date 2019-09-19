import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11

ColumnLayout {
    RowLayout {
        Label {
            text: qsTr("SUT_X")
        }
        TextField {
            text: tcpSUTCarrierParams.SafetyX
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                bottom: -100
                top: 100
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                tcpSUTCarrierParams.setSafetyX(text)
            }
        }
        Label {
            text: qsTr("SUT_Y")
        }
        TextField {
            text: tcpSUTCarrierParams.SafetyY
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                bottom: -100
                top: 100
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                tcpSUTCarrierParams.setSafetyY(text)
            }
        }
        Label {
            text: qsTr("SUT_Z")
        }
        TextField {
            text: tcpSUTCarrierParams.SafetyZ
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                tcpSUTCarrierParams.setSafetyZ(text)
            }
        }
    }

    RowLayout {
        Button {
            text: title_move_to
            width: 40
            height: 40
            onClicked: {
                console.log("sut move to safety pos")
                sutModule.moveToSafetyPos()
            }
        }
        Button {
            text: title_read_encoder
            width: 20
            height: 40
            onClicked: {
                var x = baseModuleManager.getMotorFeedbackPos(sutParams.motorXName)
                var y = baseModuleManager.getMotorFeedbackPos(sutParams.motorYName)
                var z = baseModuleManager.getMotorFeedbackPos(sutParams.motorZName)
                tcpSUTCarrierParams.setSafetyX(x)
                tcpSUTCarrierParams.setSafetyY(y)
                tcpSUTCarrierParams.setSafetyZ(z)
            }
        }
    }
}
