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
            text: tcpLUTPickLensPositionAA2.X
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                tcpLUTPickLensPositionAA2.setX(text)
            }
        }
        Label {
            text: qsTr("LUT_Y")
        }
        TextField {
            text: tcpLUTPickLensPositionAA2.Y
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                tcpLUTPickLensPositionAA2.setY(text)
            }
        }
        Label {
            text: qsTr("LUT_Z")
        }
        TextField {
            text: tcpLUTPickLensPositionAA2.Z
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                tcpLUTPickLensPositionAA2.setZ(text)
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
                tcpLUTPickLensPositionAA2.setX(x)
                tcpLUTPickLensPositionAA2.setY(y)
                tcpLUTPickLensPositionAA2.setZ(z)
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
                //logicManager.lutPickLensToAA2()
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
