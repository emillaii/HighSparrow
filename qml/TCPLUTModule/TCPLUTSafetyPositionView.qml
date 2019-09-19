import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11

ColumnLayout {
    RowLayout {
        Label {
            text: qsTr("LUT_X")
        }
        TextField {
            text: tcpLUTCarrierParams.SafetyX
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                bottom: -100
                top: 100
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                tcpLUTCarrierParams.setSafetyX(text)
            }
        }
        Label {
            text: qsTr("LUT_Y")
        }
        TextField {
            text: tcpLUTCarrierParams.SafetyY
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                bottom: -100
                top: 100
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                tcpLUTCarrierParams.setSafetyY(text)
            }
        }
        Label {
            text: qsTr("LUT_Z")
        }
        TextField {
            text: tcpLUTCarrierParams.SafetyZ
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                bottom: -100
                top: 100
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                tcpLUTCarrierParams.setSafetyZ(text)
            }
        }
    }

    RowLayout {
//        Button {
//            text: title_move_to
//            width: 40
//            height: 40
//            onClicked: {

//            }
//        }
        Button {
            text: title_read_encoder
            width: 20
            height: 40
            onClicked: {
                var x = baseModuleManager.getMotorFeedbackPos(lutParams.motorXName)
                var y = baseModuleManager.getMotorFeedbackPos(lutParams.motorYName)
                var z = baseModuleManager.getMotorFeedbackPos(lutParams.motorZName)
                tcpLUTCarrierParams.setSafetyX(x)
                tcpLUTCarrierParams.setSafetyY(y)
                tcpLUTCarrierParamsss.setSafetyZ(z)
            }
        }
    }
}
