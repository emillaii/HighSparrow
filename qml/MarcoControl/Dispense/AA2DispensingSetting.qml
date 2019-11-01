import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11
import QtQuick.Dialogs 1.2
import LogicManagerLib 1.1
import SomeLib 1.1
import SutModuleLib 1.1
import AACoreNew 1.1
Column {
    RowLayout {
        Label { text : qsTr("AA2 点胶设置") }
    }
    RowLayout {
        Label { text : qsTr("测高力度") }
        TextField{
            text: dispenseParams.testForce
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator{
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                dispenseParams.setTestForce(text)
            }
        }
        Label { text : qsTr("针头抬高") }
        TextField{
            text: dispenseParams.dispenseZOffset
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator{
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                dispenseParams.setDispenseZOffset(text)
            }
        }
        Label { text : qsTr("模板角度") }
        TextField{
            text: dispenseParams.initTheta
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator{
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                dispenseParams.setInitTheta(text)
            }
        }
        Label { text : qsTr("UV Time（ms）") }
        TextField{
            text: dispenseParams.uvTimeMs
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator{
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                dispenseParams.setUvTimeMs(text)
            }
        }
    }

    RowLayout {
        Label { text : qsTr("出胶延时") }
        TextField{
            text: dispenserParams.openOffset
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator{
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                dispenserParams.setOpenOffset(text)
            }
        }
        Label { text : qsTr("收胶延时") }
        TextField{
            text: dispenserParams.closeOffset
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator{
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                dispenserParams.setCloseOffset(text)
            }
        }
        Button { text : qsTr("Dispensing Path Edit") }
        Button { text : qsTr("Dispensing Line Speed") }
    }

    RowLayout {
        Label { text : qsTr("Dispensing Offset") }
        Label { text : qsTr("SUT X") }
        TextField{
            text: dispenseParams.dispenseXOffset
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator{
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                dispenseParams.setDispenseXOffset(text)
            }
        }
        Label { text : qsTr("Y") }
        TextField{
            text: dispenseParams.dispenseYOffset
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator{
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                dispenseParams.setDispenseYOffset(text)
            }
        }
        Label { text : qsTr("Purge Time") }
        TextField{
            text: dispenseParams.openTime
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator{
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                dispenseParams.setOpenTime(text)
            }
        }
        Button {
            text: qsTr("视觉")
            onClicked: {
                logicManager.performHandling(sutParams.moduleName, SutModule.DOWNLOOK_PR_POS+SutModule.DOWNLOOK_PR)
            }
        }
        Button {
            text: qsTr("喷胶")
            onClicked: {
                logicManager.performHandling(aaCoreParams.moduleName, AACoreNew.DISPENSE_XY_OFFSET_TEST)
            }
        }
        Button {
            text: qsTr("读取")
            onClicked: {
                dispenseModule.calulateOffset()
            }
        }
    }

    RowLayout {
        Label { text : qsTr("点胶高度 SUT Z") }
        TextField{
            text: dispenseParams.dispenseZPos
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator{
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                dispenseParams.setDispenseZPos(text)
            }
        }
        Button {
            text: qsTr("测高")
            onClicked: {
                dispenseModule.moveToDispenseDot()
            }
        }
    }
}
