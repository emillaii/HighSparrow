import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11

ColumnLayout {
    RowLayout {
        Label {
            text: qsTr("SUT_X")
        }
        TextField {
            text: sutToolUplookPosition.X
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 3
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                sutToolUplookPosition.setX(text)
            }
        }
        Label {
            text: qsTr("SUT_Y")
        }
        TextField {
            text: sutToolUplookPosition.Y
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 3
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                sutToolUplookPosition.setY(text)
            }
        }
        Label {
            text: qsTr("SUT_Z")
        }
        TextField {
            text: sutToolUplookPosition.Z
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 3
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                sutToolUplookPosition.setZ(text)
            }
        }
    }

    RowLayout {
        Button {
            text: title_move_to
            width: 40
            height: 40
            onClicked: {
                sutModule.moveToToolUplookPos()
            }
        }
        Button {
            text: title_read_encoder
            width: 20
            height: 40
            onClicked: {
                var x = baseModuleManager.getMotorFeedbackPos(m_SUT1_X)
                var y = baseModuleManager.getMotorFeedbackPos(m_SUT1_Y)
                var z = baseModuleManager.getMotorFeedbackPos(m_SUT1_Z)
                sutToolUplookPosition.setX(x)
                sutToolUplookPosition.setY(y)
                sutToolUplookPosition.setZ(z)
            }
        }
    }
}
