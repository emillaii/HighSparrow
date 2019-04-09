import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11

ColumnLayout {
    RowLayout {
        Label {
            text: qsTr("LUT_X")
        }
        TextField {
            text: lutCarrierParams.VisionX
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                bottom: -100
                top: 100
                decimals: 3
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                lutCarrierParams.setVisionX(text)
            }
        }
        Label {
            text: qsTr("LUT_Y")
        }
        TextField {
            text: lutCarrierParams.VisionY
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                bottom: -100
                top: 100
                decimals: 3
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                lutCarrierParams.setVisionY(text)
            }
        }
        Label {
            text: qsTr("LUT_Z")
        }
        TextField {
            text: lutCarrierParams.VisionZ
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                bottom: -100
                top: 100
                decimals: 3
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                lutCarrierParams.setVisionZ(text)
            }
        }
    }

    RowLayout {
        Button {
            text: title_move_to
            width: 40
            height: 40
            visible: root.checked
            onClicked: {
                lutModule.moveToPR()
            }
        }
        Button {
            text: title_read_encoder
            width: 20
            height: 40
            visible: root.checked
            onClicked: {
                var x = baseModuleManager.getMotorFeedbackPos(m_LUT1_X)
                var y = baseModuleManager.getMotorFeedbackPos(m_LUT1_Y)
                var z = baseModuleManager.getMotorFeedbackPos(m_LUT1_Z)
                lutCarrierParams.setVisionX(x)
                lutCarrierParams.setVisionY(y)
                lutCarrierParams.setVisionZ(z)
            }
        }
    }
}
