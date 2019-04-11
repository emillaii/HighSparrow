import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11

ColumnLayout {
    RowLayout {
        Label {
            text: qsTr("AA_X")
        }
        TextField {
            text: aaHeadParams.OCPositionX
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                bottom: -100
                top: 100
                decimals: 3
                notation: DoubleValidator.StandardNotation
            }
            onFocusChanged: {
                if(!focus) aaHeadParams.setOCPositionX(text)
            }
        }
        Label {
            text: qsTr("AA_Y")
        }
        TextField {
            text: aaHeadParams.OCPositionY
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                bottom: -100
                top: 100
                decimals: 3
                notation: DoubleValidator.StandardNotation
            }
            onFocusChanged: {
                if(!focus) aaHeadParams.setOCPositionY(text)
            }
        }
        Label {
            text: qsTr("AA_Z")
        }
        TextField {
            text: aaHeadParams.OCPositionZ
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                bottom: -100
                top: 100
                decimals: 3
                notation: DoubleValidator.StandardNotation
            }
            onFocusChanged: {
                if (!focus) aaHeadParams.setOCPositionZ(text)
            }
        }
    }

    RowLayout {
        Label {
            text: qsTr("AA_A")
        }
        TextField {
            text: aaHeadParams.OCPositionA
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                bottom: -100
                top: 100
                decimals: 3
                notation: DoubleValidator.StandardNotation
            }
            onFocusChanged: {
                if(!focus) aaHeadParams.setOCPositionA(text)
            }
        }
        Label {
            text: qsTr("AA_B")
        }
        TextField {
            text: aaHeadParams.OCPositionB
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                bottom: -100
                top: 100
                decimals: 3
                notation: DoubleValidator.StandardNotation
            }
            onFocusChanged: {
                if(!focus) aaHeadParams.setOCPositionB(text)
            }
        }
        Label {
            text: qsTr("AA_C")
        }
        TextField {
            text: aaHeadParams.OCPositionC
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                bottom: -100
                top: 100
                decimals: 3
                notation: DoubleValidator.StandardNotation
            }
            onFocusChanged: {
                if(!focus) aaHeadParams.setOCPositionC(text)
            }
        }
    }

    RowLayout {
        Button {
            text: title_move_to
            width: 40
            height: 40
            onClicked: {
                aaHeadModule.moveToOCPosition()
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

                aaHeadParams.setOCPositionX(x)
                aaHeadParams.setOCPositionY(y)
                aaHeadParams.setOCPositionZ(z)
                aaHeadParams.setOCPositionA(a)
                aaHeadParams.setOCPositionB(b)
                aaHeadParams.setOCPositionC(c)
            }
        }
    }
}
