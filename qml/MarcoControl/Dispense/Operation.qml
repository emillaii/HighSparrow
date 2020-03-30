import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11
import TrayLoaderModuleLib 1.1
import "../BaseUIModule"
import LogicManagerLib 1.1
import SomeLib 1.1
import AACoreNew 1.1
import SutModuleLib 1.1

ColumnLayout{
    RowLayout{ Label { text: qsTr("点胶操作") } }
    RowLayout{
        Button{
            text: qsTr("AA1 画胶PR位置")
            onClicked: {
                logicManager.performHandling(tcpSUTParams.moduleName, SutModule.DOWNLOOK_PR_POS+SutModule.DOWNLOOK_PR)
            }
        }
        Button{
            text: qsTr("AA1点胶")
            onClicked: {
                logicManager.performHandling(tcpAACoreParams.moduleName, AACoreNew.Dispense)
            }
        }
        Button{
            text: qsTr("AA1去UV")
            onClicked: {
                logicManager.performHandling(tcpAACoreParams.moduleName, AACoreNew.UV)
            }
        }
        Button{
            text: qsTr("SUT1 Load POS")
            onClicked: {
                logicManager.performHandling(tcpSUTParams.moduleName, SutModule.LOAD_POS)
            }
        }
        BaseSut1UI{}
    }
    RowLayout{
        Button{
            text: qsTr("AA2 画胶PR位置")
            onClicked: {
                logicManager.performHandling(sutParams.moduleName, SutModule.DOWNLOOK_PR_POS+SutModule.DOWNLOOK_PR)
            }
        }
        Button{
            text: qsTr("AA2点胶")
            onClicked: {
                logicManager.performHandling(aaCoreParams.moduleName, AACoreNew.Dispense)
            }
        }
        Button{
            text: qsTr("AA2去UV")
            onClicked: {
                logicManager.performHandling(aaCoreParams.moduleName, AACoreNew.UV)
            }
        }
        Button{
            text: qsTr("SUT2 Load POS")
            onClicked: {
                logicManager.performHandling(sutParams.moduleName, SutModule.LOAD_POS)
            }
        }
        BaseSut2UI{}
    }

    RowLayout{ Label { text: qsTr("AA1点胶调试") } }
    RowLayout{
        Label{
            text: qsTr("点胶高度 SUT Z")
        }
        TextField{
            text: tcpDispenseParams.dispenseZPos
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator{
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                dispenseParams.setDispenseZPos(text)
            }
        }
        Button{
            text: qsTr("测高")
            onClicked: {
                 logicManager.performHandling(tcpAACoreParams.moduleName, AACoreNew.DISPENSE_Z_TEST)
            }
        }
        Label{
            text: qsTr("针头抬高")
        }
        TextField{
            text: tcpDispenseParams.dispenseZOffset
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator{
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                dispenseParams.setDispenseZOffset(text)
            }
        }
    }

    RowLayout{
        Button{
            text: qsTr("Dispensing Offset")
        }
        Label{
            text: qsTr("SUT X")
        }
        TextField{
            text: tcpDispenseParams.dispenseXOffset
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator{
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                dispenseParams.setDispenseXOffset(text)
            }
        }
        Label{
            text: qsTr("Y")
        }
        TextField{
            text: tcpDispenseParams.dispenseYOffset
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator{
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                dispenseParams.setDispenseYOffset(text)
            }
        }
        Button{
            text: qsTr("视觉")
            onClicked: {
                 logicManager.performHandling(tcpSUTParams.moduleName, SutModule.DOWNLOOK_PR_POS+SutModule.DOWNLOOK_PR)
            }
        }
        Button{
            text: qsTr("喷胶")
            onClicked: {
                 logicManager.performHandling(tcpAACoreParams.moduleName, AACoreNew.DISPENSE_XY_OFFSET_TEST)
            }
        }
        Button{
            text: title_read_encoder
            enabled: false
        }
    }

    RowLayout{
        Label{
            text: qsTr("出胶延时")
        }
        TextField{
            text: tcpDispenserParams.openOffset
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator{
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                tcpDispenserParams.setOpenOffset(text)
            }
        }
        Button{
            text: qsTr("收胶延时")
        }
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
        Button{
            text: qsTr("Dispensing Path")
            enabled: false
        }
        Button{
            text: qsTr("Dispensing Line Speed")
            enabled: false
        }
    }


    RowLayout{ Label { text: qsTr("AA2点胶调试") } }
    RowLayout{
        Label{
            text: qsTr("点胶高度")
        }
        Label{
            text: qsTr("SUT Z")
        }
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
        Button{
            text: qsTr("测高")
            onClicked: {
                 logicManager.performHandling(aaCore.moduleName, AACoreNew.DISPENSE_Z_TEST)
            }
        }
        Button{
            text: qsTr("针头抬高")
        }
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
    }

    RowLayout{
        Button{
            text: qsTr("Dispensing Offset")
        }
        Label{
            text: qsTr("SUT X")
        }
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
        Label{
            text: qsTr("Y")
        }
        TextField{
            text: dispenseParams.dispenseYOffset
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator{
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                dispenseParams.setDispenseXOffset(text)
            }
        }
        Button{
            text: qsTr("视觉")
            onClicked: {
                logicManager.performHandling(sutParams.moduleName, SutModule.DOWNLOOK_PR_POS+SutModule.DOWNLOOK_PR)
            }
        }
        Button{
            text: qsTr("喷胶")
            onClicked: {
                logicManager.performHandling(aaCoreParams.moduleName, AACoreNew.DISPENSE_XY_OFFSET_TEST)
            }
        }
        Button{
            text: title_read_encoder
            onClicked: {
                dispenseModule.calulateOffset()
            }
        }
    }

    RowLayout{
        Button{
            text: qsTr("出胶延时")
        }
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
        Button{
            text: qsTr("收胶延时")
        }
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
        Button{
            text: qsTr("Dispensing Path")
            enabled: false
        }
        Button{
            text: qsTr("Dispensing Line Speed")
            enabled: false
        }
    }
}
