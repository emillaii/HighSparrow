import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11
import QtQuick.Dialogs 1.2
import LogicManagerLib 1.1
import SomeLib 1.1
import AACoreNew 1.1
import SutModuleLib 1.1

Column {
    RowLayout {
        Label { text : qsTr("AA1点胶设置") }
    }
    RowLayout {
        Label { text : qsTr("测高力度") }
        TextField{
            text: tcpDispenseParams.testForce
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator{
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                tcpDispenseParams.setTestForce(text)
            }
        }
        Label { text : qsTr("针头抬高") }
        TextField{
            text: tcpDispenseParams.dispenseZOffset
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator{
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                tcpDispenseParams.setDispenseZOffset(text)
            }
        }
        Label { text : qsTr("模板角度") }
        TextField{
            text: tcpDispenseParams.initTheta
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator{
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                tcpDispenseParams.setInitTheta(text)
            }
        }
        Label { text : qsTr("UV Time（ms）") }
        TextField{
            text: tcpDispenseParams.uvTimeMs
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator{
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                tcpDispenseParams.setUvTimeMs(text)
            }
        }
    }

    RowLayout {
        Label { text : qsTr("出胶延时") }
        TextField{
            text: tcpDispenserParams.openOffset
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator{
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                tcpDispenseParams.setOpenTime(text)
            }
        }
        Label { text : qsTr("收胶延时") }
        TextField{
            text: tcpDispenserParams.closeOffset
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator{
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                tcpDispenserParams.setCloseOffset(text)
            }
        }
        Button {
            text : qsTr("Dispensing Path Edit")
            enabled: false
        }
        Button {
            text : qsTr("Dispensing Line Speed")
            enabled: false
        }
    }

    RowLayout {
        Label { text : qsTr("Dispensing Offset") }
        Label { text : qsTr("SUT X") }
        TextField{
            text: tcpDispenseParams.dispenseXOffset
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator{
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                tcpDispenseParams.setDispenseXOffset(text)
            }
        }
        Label { text : qsTr("Y") }
        TextField{
            text: tcpDispenseParams.dispenseYOffset
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator{
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                tcpDispenseParams.setDispenseYOffset(text)
            }
        }
        Label { text : qsTr("Purge Time") }
        TextField{
            text: tcpDispenseParams.openTime
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator{
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                tcpDispenseParams.setOpenTime(text)
            }
        }
        Button {
            text: qsTr("视觉")
            onClicked: {
                 logicManager.performHandling(tcpSUTParams.moduleName, SutModule.DOWNLOOK_PR_POS+SutModule.DOWNLOOK_PR)
            }
        }
        Button {
            text: qsTr("喷胶")
            onClicked: {
                 logicManager.performHandling(tcpAACoreParams.moduleName, AACoreNew.DISPENSE_XY_OFFSET_TEST)
            }
        }
        Button {
            text: qsTr("读取")
            enabled: false
        }
    }

    RowLayout {
        Label { text : qsTr("点胶高度 SUT Z") }
        TextField{
            text: tcpDispenseParams.dispenseZPos
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator{
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                tcpDispenseParams.setDispenseZPos(text)
            }
        }
        Button {
            text: qsTr("测高")
            onClicked: {
                 logicManager.performHandling(tcpAACoreParams.moduleName, AACoreNew.DISPENSE_Z_TEST)
            }
        }
    }
}
