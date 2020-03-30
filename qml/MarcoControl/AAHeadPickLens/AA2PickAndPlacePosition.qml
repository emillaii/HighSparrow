import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11
import QtQuick.Dialogs 1.2
import LogicManagerLib 1.1
import SomeLib 1.1
import LutModuleLib 1.1
import AACoreNew 1.1
Column{
    RowLayout{
        Label{ text: qsTr("AA2 取放料位置") }
    }
    RowLayout{
        Label { text: qsTr("LUT Pick Position") }
        Label {
            text: qsTr("X")
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
            text: qsTr("Y")
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
            text: qsTr("Z")
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
        Button {
            text: title_move_to
            width: 40
            height: 40
            onClicked: {
                logicManager.performHandling(tcpLUTParams.moduleName,LutModule.AA2_PICK_LENS_POS)
            }
        }
        Button {
            text: title_read_encoder
            width: 20
            height: 40
            onClicked: {
                var x = baseModuleManager.getMotorFeedbackPos(tcpLUTParams.motorXName)
                var y = baseModuleManager.getMotorFeedbackPos(tcpLUTParams.motorYName)
                var z = baseModuleManager.getMotorFeedbackPos(tcpLUTParams.motorZName)
                tcpLUTPickLensPositionAA2.setX(x)
                tcpLUTPickLensPositionAA2.setY(y)
                tcpLUTPickLensPositionAA2.setZ(z)
            }
        }
        Button {
            text: qsTr("测高")
            width: 40
            height: 40
            onClicked: {
                logicManager.performHandling(tcpLUTParams.moduleName,LutModule.AA2_PICK_LENS_POS+
                                             LutModule.Measure_AA2_PICK_POS)
            }
        }
        Label { text: qsTr("AA Head Position") }
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
        Button {
            text: title_move_to
            width: 40
            height: 40
            onClicked: {
                logicManager.performHandling(aaHeadParams.moduleName, AAHeadModule.PICK_LENS_POS)
            }
        }
        Button {
            text: title_read_encoder
            width: 20
            height: 40
            onClicked: {
                var a = baseModuleManager.getMotorFeedbackPos(aaHeadParams.motorAName)
                var b = baseModuleManager.getMotorFeedbackPos(aaHeadParams.motorBName)
                var c = baseModuleManager.getMotorFeedbackPos(aaHeadParams.motorCName)

                aaHeadPickLensPosition.setA(a)
                aaHeadPickLensPosition.setB(b)
                aaHeadPickLensPosition.setC(c)
            }
        }
        Button {
            text: qsTr("Home Tilt")
            onClicked: {
                logicManager.performHandling(aaCoreParams.moduleName,AACoreNew.HOME_TILT)
            }
        }
    }
    RowLayout{
        Label { text: qsTr("LUT2 Pick Position") }
        Label {
            text: qsTr("X")
        }
        TextField {
            text: tcpLUTUnPickLensPositionAA2.X
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                tcpLUTUnPickLensPositionAA2.setX(text)
            }
        }
        Label {
            text: qsTr("Y")
        }
        TextField {
            text: tcpLUTUnPickLensPositionAA2.Y
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                tcpLUTUnPickLensPositionAA2.setY(text)
            }
        }
        Label {
            text: qsTr("Z")
        }
        TextField {
            text: tcpLUTUnPickLensPositionAA2.Z
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                tcpLUTUnPickLensPositionAA2.setZ(text)
            }
        }
        Button {
            text: title_move_to
            width: 40
            height: 40
            onClicked: {
                logicManager.performHandling(tcpLUTParams.moduleName, LutModule.AA2_UNPICK_LENS_POS)
            }
        }
        Button {
            text: title_read_encoder
            width: 20
            height: 40
            onClicked: {
                var x = baseModuleManager.getMotorFeedbackPos(tcpLUTParams.motorXName)
                var y = baseModuleManager.getMotorFeedbackPos(tcpLUTParams.motorYName)
                var z = baseModuleManager.getMotorFeedbackPos(tcpLUTParams.motorZName)

                tcpLUTPickLensPositionAA2.setX(x)
                tcpLUTPickLensPositionAA2.setY(y)
                tcpLUTPickLensPositionAA2.setZ(z)
            }
        }
        Button {
            text: qsTr("测高")
            width: 40
            height: 40
            onClicked: {
                logicManager.performHandling(tcpLUTParams.moduleName,LutModule.AA2_PICK_LENS_POS+
                                             LutModule.Measure_AA2_PICK_POS)
            }
        }
        Label { text: qsTr("AA Head Position") }
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
        Button {
            text: title_move_to
            width: 40
            height: 40
            onClicked: {
                logicManager.performHandling(aaCoreParams.moduleName, AACoreNew.AA_HEAD_MOVE_TO_PICK_LENS)
            }
        }
        Button {
            text: title_read_encoder
            width: 20
            height: 40
            onClicked: {
                var a = baseModuleManager.getMotorFeedbackPos(aaHeadParams.motorAName)
                var b = baseModuleManager.getMotorFeedbackPos(aaHeadParams.motorBName)
                var c = baseModuleManager.getMotorFeedbackPos(aaHeadParams.motorCName)

                aaHeadPickLensPosition.setA(a)
                aaHeadPickLensPosition.setB(b)
                aaHeadPickLensPosition.setC(c)
            }
        }
        Button {
            text: qsTr("Home Tilt")
            onClicked: {
                logicManager.performHandling(aaCoreParams.moduleName,AACoreNew.HOME_TILT)
            }
        }
    }
}
