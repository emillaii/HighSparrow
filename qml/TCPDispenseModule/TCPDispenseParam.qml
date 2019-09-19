import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11

ColumnLayout {
    RowLayout {
        Label {
            text: qsTr("测量力")
        }
        TextField {
            text: tcpDispenseParams.testForce
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                tcpDispenseParams.setTestForce(text)
            }
        }

        Label {
            text: qsTr("Offset_Z")
        }
        TextField {
            text: tcpDispenseParams.dispenseZOffset
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                tcpDispenseParams.setDispenseZOffset(text)
            }
        }
    }

    RowLayout {

        Label {
            text: qsTr("模板角度")
        }
        TextField {
            text: tcpDispenseParams.initTheta
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                tcpDispenseParams.setInitTheta(text)
            }
        }
    }
}
