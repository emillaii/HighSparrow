import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11
import TrayLoaderModuleLib 1.1
import SomeLib 1.1
import LogicManagerLib 1.1
import LutModuleLib 1.1
import SutModuleLib 1.1
import "../BaseUIModule"

ColumnLayout{
    RowLayout{ Label { text: qsTr("取放操作") } }
    RowLayout{
        Button{
            text: qsTr("Sensor料盘1位置")
            onClicked: {
                var selectedRow = sensorTray1Selector.txtRow.text
                var selectedCol = sensorTray1Selector.txtCol.text
                material_tray.setTrayCurrent(selectedCol-1,selectedRow-1,0)
                logicManager.performHandling(sensorLoaderParameter.moduleName,SensorLoaderModule.SENSOR_TRAY_1_POS)
            }
        }
        RowColSelector{
            id: sensorTray1Selector
        }
        Button{
            text: qsTr("Pick")
            onClicked: {
                var selectedRow = sensorTray1Selector.txtRow
                var selectedCol = sensorTray1Selector.txtCol
                material_tray.setTrayCurrent(selectedCol-1,selectedRow-1,0)
                logicManager.performHandling(sensorLoaderParameter.moduleName,SensorLoaderModule.SENSOR_TRAY_1_POS
                                             +SensorLoaderModule.TRAY_SENSOR_PR
                                             +SensorLoaderModule.TO_PICK_SENSOR_POS1
                                             +SensorLoaderModule.PICK_SENSOR_FROM_TRAY1)
            }
        }
        Button{
            text: qsTr("Place")
            onClicked: {
                var selectedRow = sensorTray1Selector.txtRow
                var selectedCol = sensorTray1Selector.txtCol
                material_tray.setTrayCurrent(selectedCol-1,selectedRow-1,0)
                sensorLoaderModule.performHandling(SensorLoaderModule.SENSOR_TRAY_1_POS
                                                   +SensorLoaderModule.TRAY_EMPTY_PR
                                                   +SensorLoaderModule.TO_PICK_SENSOR_POS1
                                                   +SensorLoaderModule.PLACE_SENSOR_BACK_TO_TRAY1)
            }
        }

        Button{
            text: qsTr("Sensor料盘2位置")
            onClicked: {
                var selectedRow = sensorTray2Selector.txtRow.text
                var selectedCol = sensorTray2Selector.txtCol.text
                material_tray.setTrayCurrent(selectedCol-1,selectedRow-1,1)
                logicManager.performHandling(sensorLoaderParameter.moduleName,SensorLoaderModule.SENSOR_TRAY_2_POS)
            }
        }
        RowColSelector{
            id: sensorTray2Selector
        }
        Button{
            text: qsTr("Pick")
            onClicked: {
                var selectedRow = sensorTray2Selector.txtRow
                var selectedCol = sensorTray2Selector.txtCol
                material_tray.setTrayCurrent(selectedCol-1,selectedRow-1,1)
                logicManager.performHandling(sensorLoaderParameter.moduleName,SensorLoaderModule.SENSOR_TRAY_2_POS
                                             +SensorLoaderModule.TRAY_SENSOR_PR
                                             +SensorLoaderModule.TO_PICK_SENSOR_POS2
                                             +SensorLoaderModule.PICK_SENSOR_FROM_TRAY2)
            }
        }
        Button{
            text: qsTr("Place")
            onClicked: {
                var selectedRow = sensorTray2Selector.txtRow
                var selectedCol = sensorTray2Selector.txtCol
                material_tray.setTrayCurrent(selectedCol-1,selectedRow-1,1)
                logicManager.performHandling(sensorLoaderParameter.moduleName,SensorLoaderModule.SENSOR_TRAY_2_POS
                                             +SensorLoaderModule.TRAY_SENSOR_PR
                                             +SensorLoaderModule.TO_PICK_SENSOR_POS2
                                             +SensorLoaderModule.PLACE_SENSOR_BACK_TO_TRAY2)
            }
        }
    }


    RowLayout{
        Button{
            text: qsTr("Lens料盘1位置")
            onClicked: {
                var selectedRow = lensTray1Selector.txtRow.text
                var selectedCol = lensTray1Selector.txtRow.text
                tcp_material_tray.setTrayCurrent(selectedCol-1,selectedRow-1,0)
                var param = {}
                param["col"] = selectedCol-1
                param["row"] = selectedRow-1
                param["tray_index"] = 0
                logicManager.performHandling(tcp_lens_loader_parameter.moduleName,LensLoaderModule.LENS_TRAY1, JSON.stringify(param))
            }
        }
        RowColSelector{
            id: lensTray1Selector
        }
        Button{
            text: qsTr("Pick")
            onClicked: {
                var selectedRow = lensTray1Selector.txtRow.text
                var selectedCol = lensTray1Selector.txtRow.text
                tcp_material_tray.setTrayCurrent(selectedCol-1,selectedRow-1,0)
                var param = {}
                param["col"] = selectedCol-1
                param["row"] = selectedRow-1
                param["tray_index"] = 0
                param["skip_dialog"] = true
                logicManager.performHandling(tcp_lens_loader_parameter.moduleName,LensLoaderModule.LENS_TRAY1+
                                             LensLoaderModule.LENS_PR+
                                             LensLoaderModule.ToWork+
                                             LensLoaderModule.PICK_LENS_FROM_TRAY, JSON.stringify(param))
            }
        }
        Button{
            text: qsTr("Place")
            onClicked: {
                var selectedRow = lensTray1Selector.txtRow.text
                var selectedCol = lensTray1Selector.txtRow.text
                tcp_material_tray.setTrayCurrent(selectedCol-1,selectedRow-1,0)
                var param = {}
                param["col"] = selectedCol-1
                param["row"] = selectedRow-1
                param["tray_index"] = 0
                param["skip_dialog"] = true
                logicManager.performHandling(tcp_lens_loader_parameter.moduleName,LensLoaderModule.LENS_TRAY1+
                                             LensLoaderModule.VACANCY_PR+
                                             LensLoaderModule.ToWork+
                                             LensLoaderModule.PLACE_NG_LENS_TO_TRAY,JSON.stringify(param))
            }
        }

        Button{
            text: qsTr("Lens料盘2位置")
            onClicked: {
                var selectedRow = lensTray2Selector.txtRow.text
                var selectedCol = lensTray2Selector.txtRow.text
                tcp_material_tray.setTrayCurrent(selectedCol-1,selectedRow-1,1)
                var param = {}
                param["col"] = selectedCol-1
                param["row"] = selectedRow-1
                param["tray_index"] = 1
                logicManager.performHandling(tcp_lens_loader_parameter.moduleName,LensLoaderModule.LENS_TRAY2, JSON.stringify(param))
            }
        }
        RowColSelector{
            id: lensTray2Selector
        }
        Button{
            text: qsTr("Pick")
            onClicked: {
                var selectedRow = lensTray2Selector.txtRow.text
                var selectedCol = lensTray2Selector.txtRow.text
                tcp_material_tray.setTrayCurrent(selectedCol-1,selectedRow-1,1)
                var param = {}
                param["col"] = selectedCol-1
                param["row"] = selectedRow-1
                param["tray_index"] = 1
                param["skip_dialog"] = true
                logicManager.performHandling(tcp_lens_loader_parameter.moduleName,LensLoaderModule.LENS_TRAY2+
                                             LensLoaderModule.LENS_PR+
                                             LensLoaderModule.ToWork+
                                             LensLoaderModule.PICK_LENS_FROM_TRAY, JSON.stringify(param))
            }
        }
        Button{
            text: qsTr("Place")
            onClicked: {
                var selectedRow = lensTray2Selector.txtRow.text
                var selectedCol = lensTray2Selector.txtRow.text
                tcp_material_tray.setTrayCurrent(selectedCol-1,selectedRow-1,1)
                var param = {}
                param["col"] = selectedCol-1
                param["row"] = selectedRow-1
                param["tray_index"] = 1
                param["skip_dialog"] = true
                logicManager.performHandling(tcp_lens_loader_parameter.moduleName,LensLoaderModule.LENS_TRAY2+
                                             LensLoaderModule.VACANCY_PR+
                                             LensLoaderModule.ToWork+
                                             LensLoaderModule.PLACE_NG_LENS_TO_TRAY,JSON.stringify(param))
            }
        }
    }

    RowLayout{
        Button{
            text: qsTr("SUT1 Load POS")
            onClicked: {
                logicManager.performHandling(sensorLoaderParameter.moduleName,SensorLoaderModule.SUT_POS1)
            }
        }
        Button{
            text: qsTr("Place Sensor to SUT1")
            onClicked: {
                logicManager.performHandling(sensorLoaderParameter.moduleName,SensorLoaderModule.SUT_POS1
                                             +SensorLoaderModule.TO_PLACE_SENSOR_POS1
                                             +SensorLoaderModule.PLACE_SENSOR_TO_SUT1)
            }
        }
        Button{
            text: qsTr("Pick Sensor from SUT1")
            onClicked: {
                logicManager.performHandling(sensorLoaderParameter.moduleName,SensorLoaderModule.SUT_POS1
                                             +SensorLoaderModule.SUT_SENSOR_PR
                                             +SensorLoaderModule.TO_PICK_NG_SENSOR_POS1
                                             +SensorLoaderModule.PICK_NG_SENSOR_FROM_SUT1)
            }
        }
    }

    RowLayout{
        Button{
            text: qsTr("SUT2 Load POS")
            onClicked: {
                logicManager.performHandling(sensorLoaderParameter.moduleName,SensorLoaderModule.SUT_POS2)
            }
        }
        Button{
            text: qsTr("Place Sensor to SUT2")
            onClicked: {
                logicManager.performHandling(sensorLoaderParameter.moduleName,SensorLoaderModule.SUT_POS2
                                             +SensorLoaderModule.TO_PLACE_SENSOR_POS2
                                             +SensorLoaderModule.PLACE_SENSOR_TO_SUT2)
            }
        }
        Button{
            text: qsTr("Pick Sensor from SUT2")
            onClicked: {
                logicManager.performHandling(sensorLoaderParameter.moduleName,SensorLoaderModule.SUT_POS2
                                             +SensorLoaderModule.SUT_SENSOR_PR
                                             +SensorLoaderModule.TO_PICK_NG_SENSOR_POS2
                                             +SensorLoaderModule.PICK_NG_SENSOR_FROM_SUT2)
            }
        }
    }

    RowLayout{
        Button{
            text: qsTr("LUT Load POS")
            onClicked: {
                logicManager.performHandling(tcpLUTParams.moduleName,LutModule.LOAD_POS)
            }
        }
        Button{
            text: qsTr("Place Lens to LUT")
            onClicked: {
                var param = {}
                param["skip_dialog"] = true
                logicManager.performHandling(tcp_lens_loader_parameter.moduleName,LensLoaderModule.LUT_POS1+
                                             LensLoaderModule.LUT_NG_SLOT_PR+
                                             LensLoaderModule.ToWork+
                                             LensLoaderModule.PLACE_LENS_TO_LUT, JSON.stringify(param))
            }
        }
        Button{
            text: qsTr("Pick from LUT")
            onClicked: {
                var param = {}
                param["skip_dialog"] = true
                logicManager.performHandling(tcp_lens_loader_parameter.moduleName,LensLoaderModule.LUT_POS1+
                                             LensLoaderModule.LUT_LENS_PR+
                                             LensLoaderModule.ToWork+
                                             LensLoaderModule.PICK_NG_LENS_FROM_LUT2, JSON.stringify(param))
            }
        }
    }

    RowLayout{
        Button{
            text: qsTr("AA1 Place Lens Pocket 1")
            onClicked: {
                logicManager.performHandling(tcpLUTParams.moduleName, LutModule.AA1_PICK_LENS_POS + LutModule.AA1_UNPICK_LENS)
            }
        }
        Button{
            text: qsTr("AA1 Pick Lens Pocket 1")
            onClicked: {
                logicManager.performHandling(tcpLUTParams.moduleName, LutModule.AA1_PICK_LENS_POS + LutModule.AA1_PICK_LENS)
            }
        }
        Button{
            text: qsTr("AA1 Place Lens Pocket 2")
            onClicked: {
                logicManager.performHandling(tcpLUTParams.moduleName, LutModule.AA1_UNPICK_LENS_POS + LutModule.AA1_UNPICK_LENS)
            }
        }
        Button{
            text: qsTr("AA1 Pick Lens Pocket 2")
            onClicked: {
                logicManager.performHandling(tcpLUTParams.moduleName, LutModule.AA1_UNPICK_LENS_POS + LutModule.AA1_PICK_LENS)
            }
        }
    }

    RowLayout{
        Button{
            text: qsTr("AA2 Place Lens Pocket 1")
            onClicked: {
                logicManager.performHandling(tcpLUTParams.moduleName,LutModule.AA2_PICK_LENS + LutModule.AA2_UNPICK_LENS)
            }
        }
        Button{
            text: qsTr("AA2 Pick Lens Pocket 1")
            onClicked: {
                logicManager.performHandling(tcpLUTParams.moduleName,LutModule.AA2_PICK_LENS + LutModule.AA2_PICK_LENS)
            }
        }
        Button{
            text: qsTr("AA2 Place Lens Pocket 2")
            onClicked: {
                logicManager.performHandling(tcpLUTParams.moduleName,LutModule.AA2_UNPICK_LENS_POS + LutModule.AA2_UNPICK_LENS)
            }
        }
        Button{
            text: qsTr("AA2 Pick Lens Pocket 2")
            onClicked: {
                logicManager.performHandling(tcpLUTParams.moduleName,LutModule.AA2_UNPICK_LENS_POS + LutModule.AA2_PICK_LENS)
            }
        }
    }

    RowLayout{ Label {text: qsTr("AA操作") } }

    RowLayout{
        Button{
            text: qsTr("AA1 Dispense PR")
            onClicked: {
                logicManager.performHandling(tcpSUTParams.moduleName, SutModule.DOWNLOOK_PR_POS + SutModule.DOWNLOOK_PR)
            }
        }
        Button{
            text: qsTr("AA1 Lens Uplook PR")
            onClicked: {
                logicManager.performHandling(tcpLUTParams.moduleName, LutModule.AA1_UPLOOK_POS + LutModule.AA1_UPLOOK_PR)
            }
        }
        Button{
            text: qsTr("AA1 PR To Bond")
            onClicked:  {
                logicManager.performHandling(tcpAACoreParams.moduleName, 2)
            }
        }
        Button{
            text: qsTr("AA1 Batch Run")
            enabled: false
        }
    }

    RowLayout{
        Button{
            text: qsTr("AA1 Gripper")
            onClicked: {
                baseModuleManager.toogleIoState(tcpAAHeadParams.gripperName)
            }
        }
        RoundButton{
            background: Rectangle {
                radius: 6
                color: {
                    if (tcpAACoreStates.tcpAAGripperState) return "lightGreen"
                    else return "red"
                }
            }
            onClicked: {
                 baseModuleManager.toogleIoState(tcpAAHeadParams.gripperName)
            }
        }

        BaseSut1UI{
        }

        Button{
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
    }

    RowLayout{
        Button{
            text: qsTr("AA2 Dispense PR")
            onClicked: {
                logicManager.performHandling(sutParams.moduleName, SutModule.DOWNLOOK_PR_POS + SutModule.DOWNLOOK_PR)
            }
        }
        Button{
            text: qsTr("AA2 Lens Uplook PR")
            onClicked: {
                logicManager.performHandling(tcpLUTParams.moduleName, LutModule.AA2_UPLOOK_POS + LutModule.AA2_UPLOOK_PR)
            }
        }
        Button{
            text: qsTr("AA2 PR To Bond")
            onClicked:  {
                logicManager.performHandling(aaCoreParams.moduleName, 2)
            }
        }
        Button{
            text: qsTr("AA2 Batch Run")
            enabled: false
        }
    }

    RowLayout{
        Button{
            text: qsTr("AA2 Gripper")
            onClicked: {
                baseModuleManager.toogleIoState(aaHeadParams.gripperName)
            }
        }
        RoundButton{
            background: Rectangle {
                radius: 6
                color: "red"
            }
            onClicked: {
                 baseModuleManager.toogleIoState(aaHeadParams.gripperName)
            }
        }

        BaseSut2UI{

        }

        Button{
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
    }
}
