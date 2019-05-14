import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11

ColumnLayout {
    RowLayout {
        Label {
            text: qsTr("AA_X")
        }
        TextField {
            text: aaHeadParams.PickLensPositionX
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                bottom: -100
                top: 100
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onFocusChanged: {
                if(!focus) aaHeadParams.setPickLensPositionX(text)
            }
        }
        Label {
            text: qsTr("AA_Y")
        }
        TextField {
            text: aaHeadParams.PickLensPositionY
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                bottom: -100
                top: 100
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onFocusChanged: {
                if(!focus) aaHeadParams.setPickLensPositionY(text)
            }
        }
        Label {
            text: qsTr("AA_Z")
        }
        TextField {
            text: aaHeadParams.PickLensPositionZ
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                bottom: -100
                top: 100
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onFocusChanged: {
                if(!focus) aaHeadParams.setPickLensPositionZ(text)
            }
        }
    }

    RowLayout {
        Label {
            text: qsTr("AA_A")
        }
        TextField {
            text: aaHeadParams.PickLensPositionA
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                bottom: -100
                top: 100
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onFocusChanged: {
                if(!focus) aaHeadParams.setPickLensPositionA(text)
            }
        }
        Label {
            text: qsTr("AA_B")
        }
        TextField {
            text: aaHeadParams.PickLensPositionB
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                bottom: -100
                top: 100
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onFocusChanged: {
                if(!focus) aaHeadParams.setPickLensPositionB(text)
            }
        }
        Label {
            text: qsTr("AA_C")
        }
        TextField {
            text: aaHeadParams.PickLensPositionC
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                bottom: -100
                top: 100
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onFocusChanged: {
                if(!focus) aaHeadParams.setPickLensPositionC(text)
            }
        }
    }

    RowLayout {
        Button {
            text: title_move_to
            width: 40
            height: 40
            onClicked: {
                aaHeadModule.moveToPickLensPosition()
            }
        }
        Button {
            text: title_read_encoder
            width: 20
            height: 40
            onClicked: {
                var x = baseModuleManager.getMotorFeedbackPos(aaHeadParams.motorXName)
                var y = baseModuleManager.getMotorFeedbackPos(aaHeadParams.motorYName)
                var z = baseModuleManager.getMotorFeedbackPos(aaHeadParams.motorZName)
                var a = baseModuleManager.getMotorFeedbackPos(aaHeadParams.motorAName)
                var b = baseModuleManager.getMotorFeedbackPos(aaHeadParams.motorBName)
                var c = baseModuleManager.getMotorFeedbackPos(aaHeadParams.motorCName)

                aaHeadParams.setPickLensPositionX(x)
                aaHeadParams.setPickLensPositionY(y)
                aaHeadParams.setPickLensPositionZ(z)
                aaHeadParams.setPickLensPositionA(a)
                aaHeadParams.setPickLensPositionB(b)
                aaHeadParams.setPickLensPositionC(c)
            }
        }
    }
}
