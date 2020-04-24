import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11
import LutModuleLib 1.1
import SutModuleLib 1.1
ColumnLayout {
//    RowLayout {
//        Label {
//            text: qsTr("AA_X")
//        }
//        TextField {
//            text: aaHeadMushroomPosition.X
//            horizontalAlignment: TextInput.AlignHCenter
//            validator: DoubleValidator {
//                decimals: 6
//                notation: DoubleValidator.StandardNotation
//            }
//            onEditingFinished: {
//                aaHeadMushroomPosition.setX(text)
//            }
//        }
//        Label {
//            text: qsTr("AA_Y")
//        }
//        TextField {
//            text: aaHeadMushroomPosition.Y
//            horizontalAlignment: TextInput.AlignHCenter
//            validator: DoubleValidator {
//                decimals: 6
//                notation: DoubleValidator.StandardNotation
//            }
//            onEditingFinished: {
//                aaHeadMushroomPosition.setY(text)
//            }
//        }
//        Label {
//            text: qsTr("AA_Z")
//        }
//        TextField {
//            text: aaHeadMushroomPosition.Z
//            horizontalAlignment: TextInput.AlignHCenter
//            validator: DoubleValidator {
//                decimals: 6
//                notation: DoubleValidator.StandardNotation
//            }
//            onEditingFinished: {
//                aaHeadMushroomPosition.setZ(text)
//            }
//        }
//    }

    RowLayout {
        Label {
            text: qsTr("AA_A")
        }
        TextField {
            text: aaHeadMushroomPosition.A
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                aaHeadMushroomPosition.setA(text)
            }
        }
        Label {
            text: qsTr("AA_B")
        }
        TextField {
            text: aaHeadMushroomPosition.B
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                aaHeadMushroomPosition.setB(text)
            }
        }
        Label {
            text: qsTr("AA_C")
        }
        TextField {
            text: aaHeadMushroomPosition.C
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                aaHeadMushroomPosition.setC(text)
            }
        }
    }

    RowLayout {
        Button {
            text: title_move_to
            width: 40
            height: 40
            onClicked: {
                //logicManager.aaMoveToMushroomPos()
                aaHeadModule.moveToMushroomPosition()
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
                //var c = baseModuleManager.getMotorFeedbackPos(aaHeadParams.motorCName)

                aaHeadMushroomPosition.setX(x)
                aaHeadMushroomPosition.setY(y)
                aaHeadMushroomPosition.setZ(z)
                aaHeadMushroomPosition.setA(a)
                aaHeadMushroomPosition.setB(b)
                //aaHeadMushroomPosition.setC(c)
            }
            // enabled: userManagement.currentAuthority >= 2 // At least engineer authority
        }
    }
    RowLayout
    {
        Button {
            text: qsTr("AA1 uplook PR")
            visible: baseModuleManager.getServerMode() === 0
            width: 40
            height: 40
            onClicked: {
//                lutModule.performHandling(1);
                logicManager.performHandling(lutParams.moduleName,LutModule.AA1_UPLOOK_POS)
            }
        }
        Button {
            text: qsTr("AA2 uplook PR")
            visible: baseModuleManager.getServerMode() === 0
            width: 40
            height: 40
            onClicked: {
//                lutModule.performHandling(2);
                logicManager.performHandling(lutParams.moduleName,LutModule.AA2_UPLOOK_POS)
            }
        }
        Button {
            text: qsTr("downlook PR")
            width: 40
            height: 40
            onClicked: {
//                sutModule.performHandling(1);
                logicManager.performHandling(sutParams.moduleName,SutModule.DOWNLOOK_PR_POS+SutModule.DOWNLOOK_PR)
            }
        }
        Button {
            text: qsTr("PR To Bond")
            width: 40
            height: 40
            onClicked: {
//                aaNewCore.performHandling(2, "");
                logicManager.performHandling(aaCoreParams.moduleName,2)
            }
        }
    }
    RowLayout {
        Label {
            text: qsTr("offset_X")
        }
        TextField {
            text: aaHeadPr2BondOffset.X
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                aaHeadPr2BondOffset.setX(text)
            }
        }
        Label {
            text: qsTr("offset_Y")
        }
        TextField {
            text: aaHeadPr2BondOffset.Y
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                aaHeadPr2BondOffset.setY(text)
            }
        }
        Label {
            text: qsTr("offset_Theta")
        }
        TextField {
            text: aaHeadPr2BondOffset.Theta
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                aaHeadPr2BondOffset.setTheta(text)
            }
        }
    }
}
