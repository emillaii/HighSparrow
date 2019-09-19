import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11


ColumnLayout {
    RowLayout {
        Label {
            text: qsTr("AA_A")
        }
        TextField {
            text: tcpAAHeadPickLensPosition.A
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                tcpAAHeadPickLensPosition.setA(text)
            }
        }
        Label {
            text: qsTr("AA_B")
        }
        TextField {
            text: tcpAAHeadPickLensPosition.B
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                tcpAAHeadPickLensPosition.setB(text)
            }
        }
        Label {
            text: qsTr("AA_C")
        }
        TextField {
            text: tcpAAHeadPickLensPosition.C
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                tcpAAHeadPickLensPosition.setC(text)
            }
        }
    }

    RowLayout {
        Button {
            text: title_move_to
            width: 40
            height: 40
            onClicked: {
                //aaHeadModule.moveToPickLensPosition()
            }
        }
        Button {
            text: title_read_encoder
            width: 20
            height: 40
            onClicked: {
//                var a = baseModuleManager.getMotorFeedbackPos(aaHeadParams.motorAName)
//                var b = baseModuleManager.getMotorFeedbackPos(aaHeadParams.motorBName)
//                var c = baseModuleManager.getMotorFeedbackPos(aaHeadParams.motorCName)

//                aaHeadMushroomPosition.setA(a)
//                aaHeadMushroomPosition.setB(b)
//                aaHeadMushroomPosition.setC(c)
            }
        }
    }
}
