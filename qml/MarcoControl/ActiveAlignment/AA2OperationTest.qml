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
        Label { text: qsTr("AA 2 测试") }
    }

    RowLayout {
        Button {
            text: qsTr("SUT 2 Load Position")
            onClicked: {
                logicManager.performHandling(sutParams.moduleName, SutModule.LOAD_POS)
            }
        }
        Button {
            text: qsTr("Place Sensor To SUT 2")
            onClicked: {
                logicManager.performHandling(sensorLoaderParameter.moduleName,SensorLoaderModule.SUT_POS2
                                             +SensorLoaderModule.TO_PLACE_SENSOR_POS2
                                             +SensorLoaderModule.PLACE_SENSOR_TO_SUT2)
            }
        }
        Button {
            text: qsTr("Pick Sensor From SUT 2")
            onClicked: {
                logicManager.performHandling(sensorLoaderParameter.moduleName,SensorLoaderModule.SUT_POS2
                                             +SensorLoaderModule.TO_PICK_NG_SENSOR_POS2
                                             +SensorLoaderModule.PICK_NG_SENSOR_FROM_SUT2)
            }
        }
        Button {
            text: qsTr("AA 2 Head Pick Lens Position")
            onClicked: {
                logicManager.performHandling(tcpLUTParams.moduleName,LutModule.AA2_PICK_LENS_POS)
            }
        }
        Button {
            text: qsTr("AA 2 Head Pick Lens")
            onClicked: {
                logicManager.performHandling(tcpLUTParams.moduleName,LutModule.AA2_PICK_LENS_POS+LutModule.AA2_PICK_LENS)
            }
        }
    }
    RowLayout {
        Button {
            text: qsTr("AA 2 Head Place Lens")
            onClicked: {
                logicManager.performHandling(tcpLUTParams.moduleName,LutModule.AA2_UNPICK_LENS_POS + LutModule.AA2_UNPICK_LENS)
            }
        }
        Button {
            text: qsTr("AA 2 Dispense PR")
            onClicked: {
                logicManager.performHandling(sutParams.moduleName, SutModule.DOWNLOOK_PR_POS+SutModule.DOWNLOOK_PR)
            }
        }
        Button {
            text: qsTr("AA 2 Lens Up Look PR")
            onClicked: {
                logicManager.performHandling(tcpLUTParams.moduleName,LutModule.AA2_UPLOOK_POS+LutModule.AA2_UPLOOK_PR)
            }
        }
        Button {
            text: qsTr("PR To Bond")
            onClicked: {
                logicManager.performHandling(aaCoreParams.moduleName,AACoreNew.PR_To_Bond)
            }
        }
        Button {
            text: qsTr("AA Batch Run")
            enabled: false
        }
    }

    RowLayout {
        Button {
            text: qsTr("SUT 2 VAC")
            onClicked: {
                baseModuleManager.toogleIoState(sutParams.tcpSUTVaccumName)
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
                baseModuleManager.toogleIoState(sutParams.tcpSUTPOGOPINName)
            }
        }
        RoundButton{
            background: Rectangle {
                radius: 6
                color: {
                    if (sutState.pogopinState) return "lightGreen"
                    else return "red"
                }
            }
        }
        Button {
            text: qsTr("LUT Pocket 1 VAC")
            onClicked: {
                baseModuleManager.toogleIoState(tcpLUTParams.tcpLutVacuum1Name)
            }
        }
        RoundButton{
            background: Rectangle {
                radius: 6
                color: {
                    if (tcpLUTState.tcpVaccum1State) return "lightGreen"
                    else return "red"
                }
            }
        }
        Button {
            text: qsTr("LUT Pocket 2 VAC")
            onClicked: {
                baseModuleManager.toogleIoState(tcpLUTParams.tcpLutVacuum2Name)
            }
        }
        RoundButton{
            background: Rectangle {
                radius: 6
                color: {
                    if (tcpLUTState.tcpVaccum2State) return "lightGreen"
                    else return "red"
                }
            }
        }
        Button {
            text: qsTr("AA 2 Gripper")
            onClicked: {
                baseModuleManager.toogleIoState(aaHeadParams.gripperName)
            }
        }
        RoundButton{
            background: Rectangle {
                radius: 6
                color: {
                    if (aaCoreStates.aaGripperState) return "lightGreen"
                    else return "red"
                }
            }
        }
    }
}
