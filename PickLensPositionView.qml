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
                decimals: 3
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
                decimals: 3
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
                decimals: 3
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
                decimals: 3
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
                decimals: 3
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
                decimals: 3
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
                var x = baseModuleManager.getMotorFeedbackPos(m_AA1_X)
                var y = baseModuleManager.getMotorFeedbackPos(m_AA1_Y)
                var z = baseModuleManager.getMotorFeedbackPos(m_AA1_Z)
                var a = baseModuleManager.getMotorFeedbackPos(m_AA1_A)
                var b = baseModuleManager.getMotorFeedbackPos(m_AA1_B)
                var c = baseModuleManager.getMotorFeedbackPos(m_AA1_C)

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
