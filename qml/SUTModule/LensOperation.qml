import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11
import SingleheadLSutModuleLib 1.0


ColumnLayout{
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
        Label{
            text: qsTr("DelayBeforeGripLens")
        }
        TextField{
            text: lsutParameter.delayBeforGripLens
            horizontalAlignment: TextInput.AlignHCenter
            validator: IntValidator{}
            onEditingFinished: {
                lsutParameter.setDelayBeforGripLens(text)
            }
        }
        Label{
            text: qsTr("DelayAfterGripLens")
        }
        TextField{
            text: lsutParameter.delayAfterGripLens
            horizontalAlignment: TextInput.AlignHCenter
            validator: IntValidator{}
            onEditingFinished: {
                lsutParameter.setDelayAfterGripLens(text)
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
