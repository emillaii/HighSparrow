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
    }
}
