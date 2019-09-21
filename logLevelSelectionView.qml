import QtQuick 2.0
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.11

Frame {
    id: frame2
    width: 300
    height: 300

    ColumnLayout {
        Label {
            text: qsTr("Log Level")
        }

        CheckBox {
            text: qsTr("Debug")
            checked: displayDebugLog
            onCheckStateChanged: {
                baseModuleManager.setQtDebugMsgDisplay(checked)
            }
        }
        CheckBox {
            text: qsTr("Warning")
            checked: displayWarningLog
            onCheckStateChanged: {
                baseModuleManager.setQtWarningMsgDisplay(checked)
            }
        }
        CheckBox {
            text: qsTr("Critical")
            checked: displayCriticalLog
            onCheckStateChanged: {
                baseModuleManager.setQtCriticalMsgDisplay(checked)
            }
        }
        CheckBox {
            text: qsTr("Fatal")
            checked: displayFatalLog
            onCheckStateChanged: {
                baseModuleManager.setQtFatalMsgDisplay(checked)
            }
        }
        CheckBox {
            text: qsTr("Info")
            checked: displayInfoLog
            onCheckedChanged: {
                baseModuleManager.setQtInfoMsgDisplay(checked)
            }
        }
    }
}
