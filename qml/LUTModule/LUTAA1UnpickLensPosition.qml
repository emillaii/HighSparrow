import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11
import LutModuleLib 1.1

ColumnLayout {
    RowLayout {
        Label {
            text: qsTr("LUT_X")
        }
        TextField {
            text: lutUnPickLensPositionAA1.X
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                lutUnPickLensPositionAA1.setX(text)
            }
        }
        Label {
            text: qsTr("LUT_Y")
        }
        TextField {
            text: lutUnPickLensPositionAA1.Y
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                lutUnPickLensPositionAA1.setY(text)
            }
        }
        Label {
            text: qsTr("LUT_Z")
        }
        TextField {
            text: lutUnPickLensPositionAA1.Z
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                lutUnPickLensPositionAA1.setZ(text)
            }
        }
    }

    RowLayout {
        Button {
            text: title_move_to
            width: 40
            height: 40
            onClicked: {
                lutModule.moveToAA1UnPickLensPos(false,true)
            }
        }
        Button {
            text: title_read_encoder
            width: 20
            height: 40
            onClicked: {
                var x = baseModuleManager.getMotorFeedbackPos(lutParams.motorXName)
                var y = baseModuleManager.getMotorFeedbackPos(lutParams.motorYName)
                var z = baseModuleManager.getMotorFeedbackPos(lutParams.motorZName)
                lutUnPickLensPositionAA1.setX(x)
                lutUnPickLensPositionAA1.setY(y)
                lutUnPickLensPositionAA1.setZ(z)
            }
        }
        Button {
            text: qsTr("UnPick")
            width: 40
            height: 40
            onClicked: {
                //logicManager.lutPickLensToAA1()
//                lutModule.performHandling(3);
                logicManager.performHandling(lutParams.moduleName,LutModule.AA1_UNPICK_LENS)
//                lutModule.moveToAA1UnPickLens()
//                lutModule.moveToAA1PickLens(false,true)
            }
        }
    }
}
