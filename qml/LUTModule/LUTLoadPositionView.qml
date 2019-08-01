import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11

ColumnLayout {
    RowLayout {
        Label {
            text: qsTr("LUT_X")
        }
        TextField {
            text: lutLoadPosition.X
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                lutLoadPosition.setX(text)
            }
        }
        Label {
            text: qsTr("LUT_Y")
        }
        TextField {
            text: lutLoadPosition.Y
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                lutLoadPosition.setY(text)
            }
        }
        Label {
            text: qsTr("LUT_Z")
        }
        TextField {
            text: lutLoadPosition.Z
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                lutLoadPosition.setZ(text)
            }
        }
    }

    RowLayout {
        Button {
            text: title_move_to
            width: 40
            height: 40
            onClicked: {
                lutModule.moveToLoadPos(false,true)
            }
        }
        Button {
            text: title_read_encoder
            width: 20
            height: 40
            onClicked: {
                var x = baseModuleManager.getMotorFeedbackPos(lutParams.motorXName)
                var y = baseModuleManager.getMotorFeedbackPos(lutParams.motorYName)
                var z = baseModuleManager.getMotorFeedbackPos(lutParams.motorZName)
                lutLoadPosition.setX(x)
                lutLoadPosition.setY(y)
                lutLoadPosition.setZ(z)
            }
        }
    }
}
