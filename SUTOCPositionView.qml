import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11

ColumnLayout {
    RowLayout {
        Label {
            text: qsTr("SUT_X")
        }
        TextField {
            text: sutParams.OCX
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onFocusChanged: {
                if(!focus) sutParams.setOCX(text)
            }
        }
        Label {
            text: qsTr("SUT_Y")
        }
        TextField {
            text: sutParams.OCY
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onFocusChanged: {
                if (!focus) sutParams.setOCY(text)
            }
        }
        Label {
            text: qsTr("SUT_Z")
        }
        TextField {
            text: sutParams.OCZ
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onFocusChanged: {
                if(!focus) sutParams.setOCZ(text)
            }
        }
    }

    RowLayout {
        Button {
            text: title_move_to
            width: 40
            height: 40
            onClicked: {
                sutModule.moveToOCPos()
            }
        }
        Button {
            text: title_read_encoder
            width: 20
            height: 40
            onClicked: {
                var x = baseModuleManager.getMotorFeedbackPos(m_SUT1_X)
                var y = baseModuleManager.getMotorFeedbackPos(m_SUT1_Y)
                var z = baseModuleManager.getMotorFeedbackPos(m_SUT1_Z)
                sutParams.setOCX(x)
                sutParams.setOCY(y)
                sutParams.setOCZ(z)
            }
        }
    }
}
