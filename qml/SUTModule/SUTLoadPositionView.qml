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
            text: lsutLoadSensorPosition.X
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                lsutLoadSensorPosition.setX(text)
            }
        }
        Label {
            text: qsTr("SUT_Y")
        }
        TextField {
            text: lsutLoadSensorPosition.Y
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                lsutLoadSensorPosition.setY(text)
            }
        }
        Label {
            text: qsTr("SUT_Z")
        }
        TextField {
            text: lsutLoadSensorPosition.Z
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                lsutLoadSensorPosition.setZ(text)
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
                sh_lsut_Module.performHandling(SingleheadLSutModule.MOVE_TO_LOAD_SENSOR_POSITION)
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
                lsutLoadSensorPosition.setX(x)
                lsutLoadSensorPosition.setY(y)
                lsutLoadSensorPosition.setZ(z)
            }
            enabled: userManagement.currentAuthority >= 2 //At least engineer authority
        }
    }
//    RowLayout {
//        Label {
//            text: qsTr("LUT_X")
//        }
//        TextField {
//            text: lsutLoadLensPosition.X
//            horizontalAlignment: TextInput.AlignHCenter
//            validator: DoubleValidator {
//                decimals: 6
//                notation: DoubleValidator.StandardNotation
//            }
//            onEditingFinished: {
//                lsutLoadLensPosition.setX(text)
//            }
//        }
//        Label {
//            text: qsTr("LUT_Y")
//        }
//        TextField {
//            text: lsutLoadLensPosition.Y
//            horizontalAlignment: TextInput.AlignHCenter
//            validator: DoubleValidator {
//                decimals: 6
//                notation: DoubleValidator.StandardNotation
//            }
//            onEditingFinished: {
//                lsutLoadLensPosition.setY(text)
//            }
//        }
//        Label {
//            text: qsTr("LUT_Z")
//        }
//        TextField {
//            text: lsutLoadLensPosition.Z
//            horizontalAlignment: TextInput.AlignHCenter
//            validator: DoubleValidator {
//                decimals: 6
//                notation: DoubleValidator.StandardNotation
//            }
//            onEditingFinished: {
//                lsutLoadLensPosition.setZ(text)
//            }
//        }
//    }

//    RowLayout {
//        Button {
//            text: title_move_to
//            width: 40
//            height: 40
//            onClicked: {
//                console.log("LSUT Move To Load Position")
//                sh_lsut_Module.performHandling(SingleheadLSutModule.MOVE_TO_LOAD_LENS_POSITION)
//            }
//        }
//        Button {
//            text: title_read_encoder
//            width: 20
//            height: 40
//            onClicked: {
//                var x = baseModuleManager.getMotorFeedbackPos(lsutParams.motorXName)
//                var y = baseModuleManager.getMotorFeedbackPos(lsutParams.motorYName)
//                var z = baseModuleManager.getMotorFeedbackPos(lsutParams.motorZName)
//                lsutLoadLensPosition.setX(x)
//                lsutLoadLensPosition.setY(y)
//                lsutLoadLensPosition.setZ(z)
//            }
//        }
//    }
}
