import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11

ColumnLayout {
    RowLayout {
        Label {
            text: qsTr("LUT_X")
        }
        TextField {
            text: lutUnPickLensPositionAA1.X
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 3
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                lutUnPickLensPositionAA1.setX(text)
            }
        }
        Label {
            text: qsTr("LUT_Y")
        }
        TextField {
            text: lutUnPickLensPositionAA1.Y
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 3
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                lutUnPickLensPositionAA1.setY(text)
            }
        }
        Label {
            text: qsTr("LUT_Z")
        }
        TextField {
            text: lutUnPickLensPositionAA1.Z
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 3
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                lutUnPickLensPositionAA1.setZ(text)
            }
        }
    }

    RowLayout {
        Button {
            text: title_move_to
            width: 40
            height: 40
            onClicked: {
                lutModule.moveToAA1UnPickLens()
            }
        }
        Button {
            text: title_read_encoder
            width: 20
            height: 40
            onClicked: {
                var x = baseModuleManager.getMotorFeedbackPos(m_LUT1_X)
                var y = baseModuleManager.getMotorFeedbackPos(m_LUT1_Y)
                var z = baseModuleManager.getMotorFeedbackPos(m_LUT1_Z)
                lutUnPickLensPositionAA1.setX(x)
                lutUnPickLensPositionAA1.setY(y)
                lutUnPickLensPositionAA1.setZ(z)
            }
        }
    }
}