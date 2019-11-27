import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11

ColumnLayout {
    RowLayout {
        Label {
            text: qsTr("测量力")
        }
        TextField {
            text: dispenseParams.testForce
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                dispenseParams.setTestForce(text)
            }
        }

        Label {
            text: qsTr("Offset_Z")
        }
        TextField {
            text: dispenseParams.dispenseZOffset
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                dispenseParams.setDispenseZOffset(text)
            }
        }
    }

    RowLayout {

        Label {
            text: qsTr("模板角度")
        }
        TextField {
            text: dispenseParams.initTheta
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                dispenseParams.setInitTheta(text)
            }
        }

        Label {
            text: qsTr("人工检胶频率")
        }
        TextField {
            text: aaCoreParams.FrequencyForCheckGlue
            horizontalAlignment: TextInput.AlignHCenter
            validator: IntValidator {
                bottom: 1
            }
            onEditingFinished: {
                aaCoreParams.setFrequencyForCheckGlue(text)
            }
        }
    }
}
