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
        Label{ text: qsTr("AA1 取放料位置") }
    }
    RowLayout{
        Label { text: qsTr("LUT Pick Position") }
        Label {
            text: qsTr("X")
        }
        TextField {
            text: tcpLUTPickLensPositionAA1.X
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                tcpLUTPickLensPositionAA1.setX(text)
            }
        }
        Label {
            text: qsTr("Y")
        }
        TextField {
            text: tcpLUTPickLensPositionAA1.Y
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                tcpLUTPickLensPositionAA1.setY(text)
            }
        }
        Label {
            text: qsTr("Z")
        }
        TextField {
            text: tcpLUTPickLensPositionAA1.Z
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                tcpLUTPickLensPositionAA1.setZ(text)
            }
        }
        Button {
            text: title_move_to
            width: 40
            height: 40
            onClicked: {
                logicManager.performHandling(tcpLUTParams.moduleName,LutModule.AA1_PICK_LENS_POS)
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
                tcpLUTPickLensPositionAA1.setX(x)
                tcpLUTPickLensPositionAA1.setY(y)
                tcpLUTPickLensPositionAA1.setZ(z)
            }
        }
        Button {
            text: qsTr("测高")
            width: 40
            height: 40
            onClicked: {
                //lutModule.moveToAAMeasurePickHeight(true,true,true)
                logicManager.performHandling(tcpLUTParams.moduleName,LutModule.AA1_PICK_LENS_POS+
                                             LutModule.Measure_AA1_PICK_POS)
            }
        }
        Label { text: qsTr("AA Head Position") }
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
        Button {
            text: title_move_to
            width: 40
            height: 40
            onClicked: {
                logicManager.performHandling(tcpAAHeadParams.moduleName, AAHeadModule.PICK_LENS_POS)
            }
        }
        Button {
            text: title_read_encoder
            width: 20
            height: 40
            onClicked: {
                var a = baseModuleManager.getMotorFeedbackPos(tcpAAHeadParams.motorAName)
                var b = baseModuleManager.getMotorFeedbackPos(tcpAAHeadParams.motorBName)
                var c = baseModuleManager.getMotorFeedbackPos(tcpAAHeadParams.motorCName)
                tcpAAHeadMushroomPosition.setA(a)
                tcpAAHeadMushroomPosition.setB(b)
                tcpAAHeadMushroomPosition.setC(c)
            }
        }
        Button {
            text: qsTr("Home Tilt")
            onClicked: {
                logicManager.performHandling(tcpAACoreParams.moduleName, AACoreNew.HOME_TILT)
            }
        }
    }
    RowLayout{
        Label { text: qsTr("LUT2 Pick Position") }
        Label {
            text: qsTr("X")
        }
        TextField {
            text: tcpLUTUnPickLensPositionAA1.X
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                tcpLUTUnPickLensPositionAA1.setX(text)
            }
        }
        Label {
            text: qsTr("Y")
        }
        TextField {
            text: tcpLUTUnPickLensPositionAA1.Y
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                tcpLUTUnPickLensPositionAA1.setY(text)
            }
        }
        Label {
            text: qsTr("Z")
        }
        TextField {
            text: tcpLUTUnPickLensPositionAA1.Z
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                tcpLUTUnPickLensPositionAA1.setZ(text)
            }
        }
        Button {
            text: title_move_to
            width: 40
            height: 40
            onClicked: {
                logicManager.performHandling(tcpLUTParams.moduleName,LutModule.AA1_UNPICK_LENS_POS)
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
                tcpLUTPickLensPositionAA1.setX(x)
                tcpLUTPickLensPositionAA1.setY(y)
                tcpLUTPickLensPositionAA1.setZ(z)
            }
        }
        Button {
            text: qsTr("测高")
            width: 40
            height: 40
            onClicked: {
                logicManager.performHandling(tcpLUTParams.moduleName,LutModule.AA1_UNPICK_LENS_POS + LutModule.Measure_AA2_PICK_POS)
            }
        }
        Label { text: qsTr("AA Head Position") }
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
        Button {
            text: title_move_to
            width: 40
            height: 40
            onClicked: {
                logicManager.performHandling(tcpAACoreParams.moduleName, AACoreNew.AA_HEAD_MOVE_TO_PICK_LENS)
            }
        }
        Button {
            text: title_read_encoder
            width: 20
            height: 40
            onClicked: {
                var a = baseModuleManager.getMotorFeedbackPos(tcpAAHeadParams.motorAName)
                var b = baseModuleManager.getMotorFeedbackPos(tcpAAHeadParams.motorBName)
                var c = baseModuleManager.getMotorFeedbackPos(tcpAAHeadParams.motorCName)
                tcpAAHeadPickLensPosition.setA(a)
                tcpAAHeadPickLensPosition.setB(b)
                tcpAAHeadPickLensPosition.setC(c)
            }
        }
        Button {
            text: qsTr("Home Tilt")
            onClicked: {
                logicManager.performHandling(tcpAACoreParams.moduleName, AACoreNew.HOME_TILT)
            }
        }
    }
}
