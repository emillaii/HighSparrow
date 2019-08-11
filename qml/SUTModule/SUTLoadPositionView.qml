import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11

import SingleheadLSutModuleLib 1.0

ColumnLayout {
    RowLayout {
        Label {
            text: qsTr("SUT_X")
        }
        TextField {
            text: lsutLoadPosition.X
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                lsutLoadPosition.setX(text)
            }
        }
        Label {
            text: qsTr("SUT_Y")
        }
        TextField {
            text: lsutLoadPosition.Y
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                lsutLoadPosition.setY(text)
            }
        }
        Label {
            text: qsTr("SUT_Z")
        }
        TextField {
            text: lsutLoadPosition.Z
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                lsutLoadPosition.setZ(text)
            }
        }
    }

    RowLayout {
        Button {
            text: title_move_to
            width: 40
            height: 40
            onClicked: {
                console.log("LSUT Move To Load Position")
                sh_lsut_Module.performHandling(SingleheadLSutModule.MOVE_TO_LOAD_POSITION)
            }
        }
        Button {
            text: title_read_encoder
            width: 20
            height: 40
            onClicked: {
                var x = baseModuleManager.getMotorFeedbackPos(lsutParams.motorXName)
                var y = baseModuleManager.getMotorFeedbackPos(lsutParams.motorYName)
                var z = baseModuleManager.getMotorFeedbackPos(lsutParams.motorZName)
                lsutLoadPosition.setX(x)
                lsutLoadPosition.setY(y)
                lsutLoadPosition.setZ(z)
            }
        }
    }
}
