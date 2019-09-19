import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11

ColumnLayout {
    RowLayout {
        Label {
            text: qsTr("Offset_X")
        }
        TextField {
            text: tcpDispenseParams.dispenseXOffset
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                tcpDispenseParams.setDispenseXOffset(text)
            }
        }

        Label {
            text: qsTr("Offset_Y")
        }
        TextField {
            text: tcpDispenseParams.dispenseYOffset
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                tcpDispenseParams.setDispenseYOffset(text)
            }
        }

        Label {
            text: qsTr("OpenTime")
        }
        TextField {
            text: tcpDispenseParams.openTime
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                tcpDispenseParams.setOpenTime(text)
            }
        }
    }
    RowLayout {
        Button {
            text: qsTr("视觉")
            width: 20
            height: 40
            onClicked: {
                highSprrow.performSUTDownlookPR()
            }
        }
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
            spacing: 3
            width: 20
            height: 40
            onClicked: {
                dispenseModule.calulateOffset()
            }
        }
    }

}
