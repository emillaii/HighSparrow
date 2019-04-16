import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11

ColumnLayout {
    RowLayout {
        Label {
            text: qsTr("Offset_X")
        }
        TextField {
            text: dispenseParams.dispenseXOffset
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 3
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                dispenseParams.setDispenseXOffset(text)
            }
        }

        Label {
            text: qsTr("Offset_Y")
        }
        TextField {
            text: dispenseParams.dispenseYOffset
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 3
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                dispenseParams.setDispenseYOffset(text)
            }
        }
    }
    RowLayout {
        Button {
            text: qsTr("去喷胶")
            width: 20
            height: 40
            onClicked: {
                dispenseModule.moveToDispenseDot(false)
            }
        }
        Button {
            text: qsTr("计算")
            width: 20
            height: 40
            onClicked: {
                dispenseModule.calulateOffset()
            }
        }
    }

}
