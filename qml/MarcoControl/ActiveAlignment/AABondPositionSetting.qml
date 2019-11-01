import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11
import QtQuick.Dialogs 1.2
import LogicManagerLib 1.1
import SomeLib 1.1
import AACoreNew 1.1
import SutModuleLib 1.1

Column {
    RowLayout { Label { text: qsTr("Bond Position") } }
    RowLayout {
        Label {
            text: qsTr("SUT 1 Bond Position")
        }
        Label{
           text:qsTr("X")
        }
        TextField{
           text: tcpSUTMushroomPosition.X
           horizontalAlignment: TextInput.AlignHCenter
           validator: DoubleValidator{
               decimals: 6
               notation: DoubleValidator.StandardNotation
           }
           onEditingFinished: {
               tcpSUTMushroomPosition.setX(text)
           }
        }
        Label{
           text:qsTr("Y")
        }
        TextField{
           text: tcpSUTMushroomPosition.Y
           horizontalAlignment: TextInput.AlignHCenter
           validator: DoubleValidator{
               decimals: 6
               notation: DoubleValidator.StandardNotation
           }
           onEditingFinished: {
               tcpSUTMushroomPosition.setY(text)
           }
        }
        Label{
           text:qsTr("Z")
        }
        TextField{
           text: tcpSUTMushroomPosition.Z
           horizontalAlignment: TextInput.AlignHCenter
           validator: DoubleValidator{
               decimals: 6
               notation: DoubleValidator.StandardNotation
           }
           onEditingFinished: {
               tcpSUTMushroomPosition.setZ(text)
           }
        }
        Button{
           text:title_move_to
           onClicked: {
               logicManager.performHandling(tcpAACoreParams.moduleName, AACoreNew.AA_HEAD_MOVE_TO_MUSHROOM)
           }
        }
        Button{
           text:title_read_encoder
           onClicked: {
               var x = baseModuleManager.getMotorFeedbackPos(tcpSUTParams.motorXName)
               var y = baseModuleManager.getMotorFeedbackPos(tcpSUTParams.motorYName)
               var z = baseModuleManager.getMotorFeedbackPos(tcpSUTParams.motorZName)
               tcpSUTMushroomPosition.setX(x);
               tcpSUTMushroomPosition.setY(y);
               tcpSUTMushroomPosition.setZ(z);
           }
        }
    }
    RowLayout {
        Label {
            text: qsTr("SUT 2 Bond Position")
        }
        Label{
           text:qsTr("X")
        }
        TextField{
           text: sutMushroomPosition.X
           horizontalAlignment: TextInput.AlignHCenter
           validator: DoubleValidator{
               decimals: 6
               notation: DoubleValidator.StandardNotation
           }
           onEditingFinished: {
               sutMushroomPosition.setX(text)
           }
        }
        Label{
           text:qsTr("Y")
        }
        TextField{
           text: sutMushroomPosition.Y
           horizontalAlignment: TextInput.AlignHCenter
           validator: DoubleValidator{
               decimals: 6
               notation: DoubleValidator.StandardNotation
           }
           onEditingFinished: {
               sutMushroomPosition.setY(text)
           }
        }
        Label{
           text:qsTr("Z")
        }
        TextField{
           text: sutMushroomPosition.Z
           horizontalAlignment: TextInput.AlignHCenter
           validator: DoubleValidator{
               decimals: 6
               notation: DoubleValidator.StandardNotation
           }
           onEditingFinished: {
               sutMushroomPosition.setZ(text)
           }
        }
        Button{
           text:title_move_to
           onClicked: {
               logicManager.performHandling(sutParams.moduleName, SutModule.MUSHROOM_POS)
           }
        }
        Button{
           text:title_read_encoder
           onClicked: {
               var x = baseModuleManager.getMotorFeedbackPos(sutParams.motorXName)
               var y = baseModuleManager.getMotorFeedbackPos(sutParams.motorYName)
               var z = baseModuleManager.getMotorFeedbackPos(sutParams.motorZName)
               sutMushroomPosition.setX(x);
               sutMushroomPosition.setY(y);
               sutMushroomPosition.setZ(z);
           }
        }
    }
    RowLayout {
        Label { text: qsTr("AA 1 Head Position") }
        Label{
           text:qsTr("A")
        }
        TextField{
           text: tcpAAHeadMushroomPosition.A
           horizontalAlignment: TextInput.AlignHCenter
           validator: DoubleValidator{
               decimals: 6
               notation: DoubleValidator.StandardNotation
           }
           onEditingFinished: {
               tcpAAHeadMushroomPosition.setA(text)
           }
        }
        Label{
           text:qsTr("B")
        }
        TextField{
            text: tcpAAHeadMushroomPosition.B
           horizontalAlignment: TextInput.AlignHCenter
           validator: DoubleValidator{
               decimals: 6
               notation: DoubleValidator.StandardNotation
           }
           onEditingFinished: {
               tcpAAHeadMushroomPosition.setB(text)
           }
        }
        Label{
           text:qsTr("C")
        }
        TextField{
           text: tcpAAHeadMushroomPosition.C
           horizontalAlignment: TextInput.AlignHCenter
           validator: DoubleValidator{
               decimals: 6
               notation: DoubleValidator.StandardNotation
           }
           onEditingFinished: {
               tcpAAHeadMushroomPosition.setC(text)
           }
        }
        Button{
           text:title_read_encoder
           onClicked: {
               var a = baseModuleManager.getMotorFeedbackPos(tcpAAHeadParams.motorXName)
               var b = baseModuleManager.getMotorFeedbackPos(tcpAAHeadParams.motorYName)
               var c = baseModuleManager.getMotorFeedbackPos(tcpAAHeadParams.motorZName)
               tcpAAHeadMushroomPosition.setA(a);
               tcpAAHeadMushroomPosition.setB(b);
               tcpAAHeadMushroomPosition.setC(c);
           }
        }
        Button{
           text:title_move_to
           onClicked: {
               logicManager.performHandling(tcpAACoreParams.moduleName, AACoreNew.AA_HEAD_MOVE_TO_MUSHROOM)
           }
        }
        Button{
           text: qsTr("Home Tilt")
           onClicked: {
               logicManager.performHandling(tcpAACoreParams.moduleName, AACoreNew.HOME_TILT)
           }
        }
    }
    RowLayout {
        Label { text: qsTr("AA 2 Head Position") }
        Label{
           text:qsTr("A")
        }
        TextField{
           text: aaHeadMushroomPosition.A
           horizontalAlignment: TextInput.AlignHCenter
           validator: DoubleValidator{
               decimals: 6
               notation: DoubleValidator.StandardNotation
           }
           onEditingFinished: {
               aaHeadMushroomPosition.setA(text)
           }
        }
        Label{
           text:qsTr("B")
        }
        TextField{
           text: aaHeadMushroomPosition.B
           horizontalAlignment: TextInput.AlignHCenter
           validator: DoubleValidator{
               decimals: 6
               notation: DoubleValidator.StandardNotation
           }
           onEditingFinished: {
               aaHeadMushroomPosition.setB(text)
           }
        }
        Label{
           text:qsTr("C")
        }
        TextField{
           text: aaHeadMushroomPosition.C
           horizontalAlignment: TextInput.AlignHCenter
           validator: DoubleValidator{
               decimals: 6
               notation: DoubleValidator.StandardNotation
           }
           onEditingFinished: {
               aaHeadMushroomPosition.setC(text)
           }
        }
        Button{
           text:title_read_encoder
           onClicked: {
               var a = baseModuleManager.getMotorFeedbackPos(aaHeadParams.motorXName)
               var b = baseModuleManager.getMotorFeedbackPos(aaHeadParams.motorYName)
               var c = baseModuleManager.getMotorFeedbackPos(aaHeadParams.motorZName)
               aaHeadMushroomPosition.setA(a);
               aaHeadMushroomPosition.setB(b);
               aaHeadMushroomPosition.setC(c);
           }
        }
        Button{
           text:title_move_to
           onClicked: {
               logicManager.performHandling(aaCoreParams.moduleName, AACoreNew.AA_HEAD_MOVE_TO_MUSHROOM)
           }
        }
        Button{
           text: qsTr("Home Tilt")
           onClicked: {
               logicManager.performHandling(aaCoreParams.moduleName, AACoreNew.HOME_TILT)
           }
        }
    }

    AA1OperationTest {}
    AA2OperationTest {}
}
