import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11

ColumnLayout {
    RowLayout {
        Layout.fillHeight: false
        Label {
            text: qsTr("LUT_X")
        }
        TextField {
            text: lutPickLensPositionAA2.X
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                lutPickLensPositionAA2.setX(text)
            }
        }
        Label {
            text: qsTr("LUT_Y")
        }
        TextField {
            text: lutPickLensPositionAA2.Y
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                lutPickLensPositionAA2.setY(text)
            }
        }
        Label {
            text: qsTr("LUT_Z")
        }
        TextField {
            text: lutPickLensPositionAA2.Z
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                lutPickLensPositionAA2.setZ(text)
            }
        }
    }

    RowLayout {
        Button {
            text: title_move_to
            width: 40
            height: 40
            onClicked: {
                lutModule.moveToAA2PickLensPos(false,true)
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
                lutPickLensPositionAA2.setX(x)
                lutPickLensPositionAA2.setY(y)
                lutPickLensPositionAA2.setZ(z)
            }
        }
    }
    RowLayout {
        Button {
            text: qsTr("去测高")
            width: 40
            height: 40
            onClicked: {
                lutModule.moveToAAMeasurePickHeight(false,false,true)
            }
        }
        Button {
            text: qsTr("Pick")
            width: 40
            height: 40
            onClicked: {
                lutModule.moveToAA2PickLens(false,false,true)
            }
        }
        Button {
            text: qsTr("Return")
            width: 40
            height: 40
            onClicked: {
                lutModule.vcmReturn()
            }
        }
    }
}
