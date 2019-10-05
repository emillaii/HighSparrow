import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11
import QtQuick.Dialogs 1.2
import LogicManagerLib 1.1
import SomeLib 1.1
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
            text: lutPickLensPositionAA2.X
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                lutPickLensPositionAA2.setX(text)
            }
        }
        Label {
            text: qsTr("Y")
        }
        TextField {
            text: lutPickLensPositionAA2.Y
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                lutPickLensPositionAA2.setY(text)
            }
        }
        Label {
            text: qsTr("Z")
        }
        TextField {
            text: lutPickLensPositionAA2.Z
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                lutPickLensPositionAA2.setZ(text)
            }
        }
        Button {
            text: title_move_to
            width: 40
            height: 40
            onClicked: {
                lutModule.moveToAA2PickLensPos(false,true)
            }
        }
        Button {
            text: title_read_encoder
            width: 20
            height: 40
            onClicked: {
                var x = baseModuleManager.getMotorFeedbackPos(lutParams.motorXName)
                var y = baseModuleManager.getMotorFeedbackPos(lutParams.motorYName)
                var z = baseModuleManager.getMotorFeedbackPos(lutParams.motorZName)
                lutPickLensPositionAA2.setX(x)
                lutPickLensPositionAA2.setY(y)
                lutPickLensPositionAA2.setZ(z)
            }
        }
        Button {
            text: qsTr("测高")
            width: 40
            height: 40
            onClicked: {
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
                aaHeadModule.moveToPickLensPosition()
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

                aaHeadMushroomPosition.setA(a)
                aaHeadMushroomPosition.setB(b)
                aaHeadMushroomPosition.setC(c)
            }
        }
        Button {
            text: qsTr("Home Tilt")
        }
    }
    RowLayout{
        Label { text: qsTr("LUT2 Pick Position") }
        Label {
            text: qsTr("X")
        }
        TextField {
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
            }
        }
        Label {
            text: qsTr("Y")
        }
        TextField {
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
            }
        }
        Label {
            text: qsTr("Z")
        }
        TextField {
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
            }
        }
        Button {
            text: title_move_to
            width: 40
            height: 40
            onClicked: {
            }
        }
        Button {
            text: title_read_encoder
            width: 20
            height: 40
            onClicked: {
            }
        }
        Button {
            text: qsTr("测高")
            width: 40
            height: 40
            onClicked: {
            }
        }
        Label { text: qsTr("AA Head Position") }
        Label {
            text: qsTr("AA_A")
        }
        TextField {
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
            }
        }
        Label {
            text: qsTr("AA_B")
        }
        TextField {
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
            }
        }
        Label {
            text: qsTr("AA_C")
        }
        TextField {
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
            }
        }
        Button {
            text: title_move_to
            width: 40
            height: 40
            onClicked: {
            }
        }
        Button {
            text: title_read_encoder
            width: 20
            height: 40
            onClicked: {
            }
        }
        Button {
            text: qsTr("Home Tilt")
        }
    }
}
