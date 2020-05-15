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
    }

    RowLayout {

        Label {
            text: qsTr("积累计数")
        }
        TextField {
            text: aaCoreParams.dispenseCount
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                aaCoreParams.setDispenseCount(text)
            }
        }

        Button {
            text: qsTr("Clear 积累计数")
            onClicked: {
                aaNewCore.clearCurrentDispenseCount()
            }
        }

        Label {
            text: qsTr("积累计数限制")
        }
        TextField {
            text: aaCoreParams.dispenseCountLimit
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                aaCoreParams.setDispenseCountLimit(text)
            }
        }
    }

    RowLayout{
        CheckBox{
            text: qsTr("排胶提醒")
            checked: dispenseParams.enableDispenseTimerAlarm
            onCheckedChanged: {
                dispenseParams.setEnableDispenseTimerAlarm(checked)
            }
        }

        Label{
            text:qsTr("时间间隔(min)")
        }
        TextField{
            text:dispenseParams.dispenseAlarmMinute
            horizontalAlignment: TextInput.AlignHCenter
            validator: IntValidator{
                bottom: 1
            }
            onEditingFinished: {
                dispenseParams.setDispenseAlarmMinute(text)
            }
        }
    }
}
