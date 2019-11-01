import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11
import QtQuick.Dialogs 1.2
import LogicManagerLib 1.1
import SomeLib 1.1
import SutModuleLib 1.1
import LutModuleLib 1.1
import AACoreNew 1.1

Column {
    RowLayout {
        Label { text: qsTr("AA 1 测试") }
    }

    RowLayout {
        Button {
            text: qsTr("SUT 1 Load Position")
            onClicked: {
                logicManager.performHandling(tcpSUTParams.moduleName, SutModule.LOAD_POS)
            }
        }
        Button {
            text: qsTr("Place Sensor To SUT 1")
            enabled:false
        }
        Button {
            text: qsTr("Pick Sensor From SUT 1")
            enabled:false
        }
        Button {
            text: qsTr("AA 1 Head Pick Lens Position")
            onClicked: {
                logicManager.performHandling(tcpLUTParams.moduleName,LutModule.AA1_PICK_LENS_POS)
            }
        }
        Button {
            text: qsTr("AA 1 Head Pick Lens")
            onClicked: {
                logicManager.performHandling(tcpLUTParams.moduleName,LutModule.AA1_PICK_LENS_POS + LutModule.AA1_PICK_LENS)
            }
        }
    }
    RowLayout {
        Button {
            text: qsTr("AA1 Head Place Lens")
            onClicked: {
                logicManager.performHandling(tcpLUTParams.moduleName,LutModule.AA1_UNPICK_LENS_POS+LutModule.AA1_UNPICK_LENS)
            }
        }
        Button {
            text: qsTr("AA1 Dispense PR")
            onClicked: {
                logicManager.performHandling(tcpSUTParams.moduleName, SutModule.DOWNLOOK_PR_POS+SutModule.DOWNLOOK_PR)
            }
        }
        Button {
            text: qsTr("AA1 Lens Up Look PR")
            onClicked: {
                logicManager.performHandling(tcpLUTParams.moduleName,LutModule.AA1_UPLOOK_POS+LutModule.AA1_UPLOOK_PR)
            }
        }
        Button {
            text: qsTr("PR To Bond")
            onClicked: {
                logicManager.performHandling(tcpAACoreParams.moduleName,AACoreNew.PR_To_Bond)
            }
        }
        Button {
            text: qsTr("AA Batch Run")
        }
    }

    RowLayout {
        Button {
            text: qsTr("SUT 1 VAC")
            onClicked: {
                baseModuleManager.toogleIoState(tcpSUTParams.tcpSUTVaccumName)
            }
        }
        RoundButton{
            background: Rectangle {
                radius: 6
                color: "red"
            }
        }
        Button {
            text: qsTr("SUT 1 Clamp")
            onClicked: {
                baseModuleManager.toogleIoState(tcpSUTParams.tcpSUTPOGOPINName)
            }
        }
        RoundButton{
            background: Rectangle {
                radius: 6
                color: "red"
            }
        }
        Button {
            text: qsTr("LUT Pocket 1 VAC")
            onClicked: {
                baseModuleManager.toogleIoState(tcpLUTParams.lutVacuum1Name)
            }
        }
        RoundButton{
            background: Rectangle {
                radius: 6
                color: "red"
            }
        }
        Button {
            text: qsTr("LUT Pocket 2 VAC")
            onClicked: {
                baseModuleManager.toogleIoState(tcpLUTParams.lutVacuum2Name)
            }
        }
        RoundButton{
            background: Rectangle {
                radius: 6
                color: "red"
            }
        }
        Button {
            text: qsTr("AA 1 Gripper")
            onClicked: {
                baseModuleManager.toogleIoState(tcpAAHeadParams.gripperName)
            }
        }
        RoundButton{
            background: Rectangle {
                radius: 6
                color: "red"
            }
        }
    }
}
