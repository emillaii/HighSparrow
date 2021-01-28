import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11

ColumnLayout {
    RowLayout {
        Label {
            text: qsTr("OpenPos")
        }
        TextField {
            text: aaHeadParams.lightPanelOpenPos
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                aaHeadParams.setLightPanelOpenPos(text)
            }
        }
        Button {
            text: title_move_to
            width: 40
            height: 40
            onClicked: {
                aaNewCore.lightPanelOpen()
            }
        }
        Button {
            text: title_read_encoder
            width: 20
            height: 40
            onClicked: {
                var z = baseModuleManager.getMotorFeedbackPos(aaHeadParams.motorLightPanelName)
                aaHeadParams.setLightPanelOpenPos(z)
            }
        }
    }
    RowLayout {
        Label {
            text: qsTr("ClosePos")
        }
        TextField {
            text: aaHeadParams.lightPanelClosePos
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                aaHeadParams.setLightPanelClosePos(text)
            }
        }
        Button {
            text: title_move_to
            width: 40
            height: 40
            onClicked: {
                aaNewCore.lightPanelClose()
            }
        }
        Button {
            text: title_read_encoder
            width: 20
            height: 40
            onClicked: {
                var z = baseModuleManager.getMotorFeedbackPos(aaHeadParams.motorLightPanelName)
                aaHeadParams.setLightPanelClosePos(z)
            }
        }
    }
}
