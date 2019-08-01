import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11

ColumnLayout {
    RowLayout {
        Label {
            text: qsTr("LUT_X")
        }
        TextField {
            text: lutUplookPositionAA1.X
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                lutUplookPositionAA1.setX(text)
            }
        }
        Label {
            text: qsTr("LUT_Y")
        }
        TextField {
            text: lutUplookPositionAA1.Y
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                lutUplookPositionAA1.setY(text)
            }
        }
        Label {
            text: qsTr("LUT_Z")
        }
        TextField {
            text: lutUplookPositionAA1.Z
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                lutUplookPositionAA1.setZ(text)
            }
        }
    }

    RowLayout {
        Button {
            text: title_move_to
            width: 40
            height: 40
            onClicked: {
                lutModule.moveToAA1UplookPos(false,true)
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
                lutUplookPositionAA1.setX(x)
                lutUplookPositionAA1.setY(y)
                lutUplookPositionAA1.setZ(z)
            }
        }
    }
}
