import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11

ColumnLayout {
    RowLayout {
        Label {
            text: qsTr("LUT_X")
        }
        TextField {
            text: lutPickLensPositionAA1.X
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 3
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                lutPickLensPositionAA1.setX(text)
            }
        }
        Label {
            text: qsTr("LUT_Y")
        }
        TextField {
            text: lutPickLensPositionAA1.Y
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 3
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                lutPickLensPositionAA1.setY(text)
            }
        }
        Label {
            text: qsTr("LUT_Z")
        }
        TextField {
            text: lutPickLensPositionAA1.Z
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 3
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                lutPickLensPositionAA1.setZ(text)
            }
        }
    }

    RowLayout {
        Button {
            text: title_move_to
            width: 40
            height: 40
            onClicked: {
                lutModule.moveToAA1PickLensPos(true)
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
                lutPickLensPositionAA1.setX(x)
                lutPickLensPositionAA1.setY(y)
                lutPickLensPositionAA1.setZ(z)
            }
        }

        Label {
            text: qsTr("PickForce")
        }
        TextField {
            text: lutParams.pickForce
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 3
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                lutParams.setPickForce(text)
            }
        }
        Button {
            text: qsTr("Pick")
            width: 40
            height: 40
            onClicked: {
                lutModule.moveToAA1PickLens(false,true)
            }
        }
        Button {
            text: qsTr("Return")
            width: 40
            height: 40
            onClicked: {
                lutModule.vcmReturn()
            }
        }
    }
}
