import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11

ColumnLayout {
    RowLayout {
        Label {
            text: qsTr("SUT_X")
        }
        TextField {
            text: tcpSUTDownlookPosition.X
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                tcpSUTDownlookPosition.setX(text)
            }
        }
        Label {
            text: qsTr("SUT_Y")
        }
        TextField {
            text: tcpSUTDownlookPosition.Y
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                tcpSUTDownlookPosition.setY(text)
            }
        }
        Label {
            text: qsTr("SUT_Z")
        }
        TextField {
            text: tcpSUTDownlookPosition.Z
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                tcpSUTDownlookPosition.setZ(text)
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
//                sutDownlookPosition.setX(x)
//                sutDownlookPosition.setY(y)
//                sutDownlookPosition.setZ(z)
            }
        }
    }
}
