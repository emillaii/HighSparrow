import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11
import SomeLib 1.1
import LogicManagerLib 1.1
import AACoreNew 1.1
ColumnLayout {
    RowLayout {
        Label {
            text: qsTr("AA_A TCP")
        }
        TextField {
            text: tcpAAHeadMushroomPosition.A
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                tcpAAHeadMushroomPosition.setA(text)
            }
        }
        Label {
            text: qsTr("AA_B")
        }
        TextField {
            text: tcpAAHeadMushroomPosition.B
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                tcpAAHeadMushroomPosition.setB(text)
            }
        }
        Label {
            text: qsTr("AA_C")
        }
        TextField {
            text: tcpAAHeadMushroomPosition.C
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                tcpAAHeadMushroomPosition.setC(text)
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
                //aaHeadModule.moveToMushroomPosition()
                logicManager.performHandling(tcpAACoreParams.moduleName, AACoreNew.MOVE_LENS);
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

                aaHeadMushroomPosition.setX(x)
                aaHeadMushroomPosition.setY(y)
                aaHeadMushroomPosition.setZ(z)
                aaHeadMushroomPosition.setA(a)
                aaHeadMushroomPosition.setB(b)
                aaHeadMushroomPosition.setC(c)
            }
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
                logicManager.performHandling(lutParams.moduleName,1)
            }
        }
        Button {
            text: qsTr("AA2 uplook PR")
            visible: baseModuleManager.getServerMode() === 0
            width: 40
            height: 40
            onClicked: {
//                lutModule.performHandling(2);
                logicManager.performHandling(lutParams.moduleName,2)
            }
        }
        Button {
            text: qsTr("downlook PR")
            width: 40
            height: 40
            onClicked: {
//                sutModule.performHandling(1);
                logicManager.performHandling(sutParams.moduleName,1)
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
}
