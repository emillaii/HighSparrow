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
            text: lsutPRPosition.X
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                lsutPRPosition.setX(text)
            }
        }
        Label {
            text: qsTr("SUT_Y")
        }
        TextField {
            text: lsutPRPosition.Y
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                lsutPRPosition.setY(text)
            }
        }
        Label {
            text: qsTr("SUT_Z")
        }
        TextField {
            text: lsutPRPosition.Z
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                lsutPRPosition.setZ(text)
            }
        }
    }

    RowLayout {
        Button {
            text: title_move_to
            width: 40
            height: 40
            onClicked: {
                console.log("LSUT Move To PR Position")
                sh_lsut_Module.performHandling(SingleheadLSutModule.MOVE_TO_PR_POSITION)
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
                lsutPRPosition.setX(x)
                lsutPRPosition.setY(y)
                lsutPRPosition.setZ(z)
            }
        }
        Button {
            text: qsTr("SENSOR视觉")
            width: 40
            height: 40
            onClicked: {
                console.log("Execute sensor pr")
                sh_lsut_Module.performHandling(SingleheadLSutModule.DOWNLOOK_SENSOR_PR)
            }
        }
    }
}
