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
            text: lsutUpDnDownlookCalibrationPosition.X
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                lsutUpDnDownlookCalibrationPosition.setX(text)
            }
        }
        Label {
            text: qsTr("SUT_Y")
        }
        TextField {
            text: lsutUpDnDownlookCalibrationPosition.Y
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                lsutUpDnDownlookCalibrationPosition.setY(text)
            }
        }
        Label {
            text: qsTr("SUT_Z")
        }
        TextField {
            text: lsutUpDnDownlookCalibrationPosition.Z
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                lsutUpDnDownlookCalibrationPosition.setZ(text)
            }
        }
    }

    RowLayout {
        Button {
            text: title_move_to
            width: 40
            height: 40
            onClicked: {
                sh_lsut_Module.performHandling(SingleheadLSutModule.MOVE_TO_UPDN_DOWNLOOK_CALIBRATION_POSITION)
            }
        }
        Button {
            text: title_read_encoder
            width: 20
            height: 40
            onClicked: {
                var x = baseModuleManager.getMotorFeedbackPos(sutParams.motorXName)
                var y = baseModuleManager.getMotorFeedbackPos(sutParams.motorYName)
                var z = baseModuleManager.getMotorFeedbackPos(sutParams.motorZName)
                lsutUpDnDownlookCalibrationPosition.setX(x)
                lsutUpDnDownlookCalibrationPosition.setY(y)
                lsutUpDnDownlookCalibrationPosition.setZ(z)
            }
        }
    }
}
