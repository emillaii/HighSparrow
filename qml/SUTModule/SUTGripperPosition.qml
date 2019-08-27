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
            text: lsutGripperPosition.X
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                lsutGripperPosition.setX(text)
            }
        }
        Label {
            text: qsTr("SUT_Y")
        }
        TextField {
            text: lsutGripperPosition.Y
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                lsutGripperPosition.setY(text)
            }
        }
        Label {
            text: qsTr("SUT_Z")
        }
        TextField {
            text: lsutGripperPosition.Z
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                lsutGripperPosition.setZ(text)
            }
        }
    }

    RowLayout {
        Button {
            text: title_move_to
            width: 40
            height: 40
            onClicked: {
                console.log("lsut move to gripper pos")
                sh_lsut_Module.performHandling(SingleheadLSutModule.MOVE_TO_GRIPPER_POSITION)
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
                lsutGripperPosition.setX(x)
                lsutGripperPosition.setY(y)
                lsutGripperPosition.setZ(z)
            }
        }
    }
    RowLayout {
        Label{
            text: qsTr("Lens offset X")
        }
        TextField{
            text: lsutLensOffset.X
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator{
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                lsutLensOffset.setX(text)
            }
        }
        Label{
            text: qsTr("Y")
        }
        TextField{
            text: lsutLensOffset.Y
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator{
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                lsutLensOffset.setY(text)
            }
        }
    }


    RowLayout {
        Label{
            text: qsTr("Gripper高度")
        }
        TextField{
            text: lsutParameter.ZOffset
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator{
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                lsutParameter.setZOffset(text)
            }
        }
        Button {
            text: qsTr("测高")
            width: 40
            height: 40
            onClicked: {
                console.log("Measure height from lens to gripper")
                sh_lsut_Module.performHandling(SingleheadLSutModule.LENS_GRIPPER_MEASURE_HEIGHT)
            }
        }
    }
    RowLayout {
        Button {
            text: qsTr("LENS TO GRIPER CENTER")
            width: 40
            height: 40
            onClicked: {
                console.log("Move lens to center of gripper")
                sh_lsut_Module.performHandling(SingleheadLSutModule.LENS_TO_GRIPPER_CENTER)
            }
        }
        Button {
            text: qsTr("LENS TO GRIPER")
            width: 40
            height: 40
            onClicked: {
                console.log("Move lens to gripper position")
                sh_lsut_Module.performHandling(SingleheadLSutModule.LENS_TO_GRIPPER)
            }
        }
        Button {
            text: qsTr("夹Lens")
            width: 40
            height: 40
            onClicked: {
                console.log("Move Lens to gripper")
                sh_lsut_Module.performHandling(SingleheadLSutModule.GRAB_LENS_TO_GRIPPER)
            }
        }
        Button {
            text: qsTr("UnPick Lens")
            width: 40
            height: 40
            onClicked: {
                console.log("UnpickLens to LUT")
                sh_lsut_Module.performHandling(SingleheadLSutModule.UNPICK_LENS_TO_LUT)
            }
        }
    }
}

