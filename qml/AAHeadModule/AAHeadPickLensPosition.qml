import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11


ColumnLayout {
    RowLayout {
        Label {
            text: qsTr("AA_A")
        }
        TextField {
            text: aaHeadPickLensPosition.A
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                aaHeadPickLensPosition.setA(text)
            }
        }
        Label {
            text: qsTr("AA_B")
        }
        TextField {
            text: aaHeadPickLensPosition.B
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                aaHeadPickLensPosition.setB(text)
            }
        }
        Label {
            text: qsTr("AA_C")
        }
        TextField {
            text: aaHeadPickLensPosition.C
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                aaHeadPickLensPosition.setC(text)
            }
        }
    }

//    RowLayout {
//        Label {
//            text: qsTr("AA_Z")
//        }
//        TextField {
//            text: aaHeadParams.PickLensPositionZ
//            horizontalAlignment: TextInput.AlignHCenter
//            validator: DoubleValidator {
//                decimals: 6
//                notation: DoubleValidator.StandardNotation
//            }
//            onEditingFinished: {
//                aaHeadParams.setPickLensPositionZ(text)
//            }
//        }
//    }

    RowLayout {
        Button {
            text: title_move_to
            width: 40
            height: 40
            onClicked: {
                //logicManager.aaMoveToPickLensPos()
                aaHeadModule.moveToPickLensPosition()
            }
        }
        Button {
            text: title_read_encoder
            width: 20
            height: 40
            onClicked: {
//                var z = baseModuleManager.getMotorFeedbackPos(aaHeadParams.motorZName)
//                aaHeadParams.setPickLensPositionZ(z)
                var a = baseModuleManager.getMotorFeedbackPos(aaHeadParams.motorAName)
                var b = baseModuleManager.getMotorFeedbackPos(aaHeadParams.motorBName)
                var c = baseModuleManager.getMotorFeedbackPos(aaHeadParams.motorCName)

//                aaHeadMushroomPosition.setA(a)
//                aaHeadMushroomPosition.setB(b)
//                aaHeadMushroomPosition.setC(c)
                aaHeadPickLensPosition.setA(a)
                aaHeadPickLensPosition.setB(b)
                aaHeadPickLensPosition.setC(c)
            }
        }
    }
}
