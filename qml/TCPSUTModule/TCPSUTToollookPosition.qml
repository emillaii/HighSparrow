import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11

ColumnLayout {
    RowLayout {
        Label {
            text: qsTr("SUT_X")
        }
        TextField {
            text: tcpSUTToollookPosition.X
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                sutToollookPosition.setX(text)
            }
        }
        Label {
            text: qsTr("SUT_Y")
        }
        TextField {
            text: tcpSUTToollookPosition.Y
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                tcpSUTToollookPosition.setY(text)
            }
        }
        Label {
            text: qsTr("SUT_Z")
        }
        TextField {
            text: tcpSUTToollookPosition.Z
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                tcpSUTToollookPosition.setZ(text)
            }
        }
    }

    RowLayout {
        Button {
            text: title_move_to
            width: 40
            height: 40
            onClicked: {

            }
        }
        Button {
            text: title_read_encoder
            width: 20
            height: 40
            onClicked: {
//                var x = baseModuleManager.getMotorFeedbackPos(sutParams.motorXName)
//                var y = baseModuleManager.getMotorFeedbackPos(sutParams.motorYName)
//                var z = baseModuleManager.getMotorFeedbackPos(sutParams.motorZName)
//                sutToollookPosition.setX(x)
//                sutToollookPosition.setY(y)
//                sutToollookPosition.setZ(z)
            }
        }
    }
}
