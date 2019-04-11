import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11

ColumnLayout {
    RowLayout {
        Label {
            text: qsTr("LUT_X")
        }
        TextField {
            text: lutParams.LoadX
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                bottom: -100
                top: 100
                decimals: 3
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                lutParams.setLoadX(text)
            }
        }
        Label {
            text: qsTr("LUT_Y")
        }
        TextField {
            text: lutParams.LoadY
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                bottom: -100
                top: 100
                decimals: 3
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                lutParams.setLoadY(text)
            }
        }
        Label {
            text: qsTr("LUT_Z")
        }
        TextField {
            text: lutParams.LoadZ
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                bottom: -100
                top: 100
                decimals: 3
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                lutParams.setLoadZ(text)
            }
        }
    }

    RowLayout {
        Button {
            text: title_move_to
            width: 40
            height: 40
            onClicked: {
                lutModule.moveToLoadPos()
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
                lutParams.setLoadX(x)
                lutParams.setLoadY(y)
                lutParams.setLoadZ(z)
            }
        }
    }
}
