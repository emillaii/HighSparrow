import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11
import QtQuick.Dialogs 1.2
import LogicManagerLib 1.1
import SomeLib 1.1
import AACoreNew 1.1
import SutModuleLib 1.1
ColumnLayout {
    AA1DispensingSetting {}
    AA2DispensingSetting {}

    RowLayout {
        Button {
            text: qsTr("AA1 画胶PR位置")
            onClicked: {
                logicManager.performHandling(tcpSUTParams.moduleName, SutModule.DOWNLOOK_PR_POS+SutModule.DOWNLOOK_PR)
            }
        }

        Button {
            text: qsTr("AA1 点胶")
            onClicked: {
                logicManager.performHandling(tcpAACoreParams.moduleName, AACoreNew.Dispense)
            }
        }

        Button {
            text: qsTr("AA1 去UV")
            onClicked: {
                logicManager.performHandling(tcpAACoreParams.moduleName, AACoreNew.UV)
            }
        }

        Button {
            text: qsTr("SUT 1 VAC")
            onClicked: {
                baseModuleManager.toogleIoState(tcpSUTParams.tcpSUTVaccumName)
            }
        }

        RoundButton{
            background: Rectangle {
                radius: 6
                color: {
                    if (tcpSUTState.tcpVaccumState) return "lightGreen"
                    else return "red"
                }
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
                color: {
                    if (tcpSUTState.tcpPogopinState) return "lightGreen"
                    else return "red"
                }
            }
        }
    }

    RowLayout {
        Button {
            text: qsTr("AA2 画胶PR位置")
            onClicked: {
                logicManager.performHandling(sutParams.moduleName, SutModule.DOWNLOOK_PR_POS+SutModule.DOWNLOOK_PR)
            }
        }

        Button {
            text: qsTr("AA2 点胶")
            onClicked: {
                logicManager.performHandling(aaCoreParams.moduleName, AACoreNew.Dispense)
            }
        }

        Button {
            text: qsTr("AA2 去UV")
            onClicked: {
                logicManager.performHandling(aaCoreParams.moduleName, AACoreNew.UV)
            }
        }

        Button {
            text: qsTr("SUT 2 VAC")
            onClicked: {
                baseModuleManager.toogleIoState(sutParams.sutVaccumName)
            }
        }

        RoundButton{
            background: Rectangle {
                radius: 6
                color: {
                    if (sutState.vaccumState) return "lightGreen"
                    else return "red"
                }
            }
        }
        Button {
            text: qsTr("SUT 2 Clamp")
            onClicked: {
                baseModuleManager.toogleIoState(sutParams.sutPOGOPINName)
            }
        }
        RoundButton{
            background: Rectangle {
                radius: 6
                color: {
                    if (sutState.gripperState) return "lightGreen"
                    else return "red"
                }
            }
        }
    }
}
