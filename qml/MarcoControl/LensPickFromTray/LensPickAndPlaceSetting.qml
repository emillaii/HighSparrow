import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11
import "../../../"
import SomeLib 1.1
import LogicManagerLib 1.1
import LutModuleLib 1.1
ColumnLayout{
    RowLayout { Label { text: qsTr("取放参数") } }
    RowLayout {
        Label {
            text:qsTr("力限制")
        }
        TextField{
            text:tcp_lens_loader_parameter.vcmWorkForce
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator{
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                tcp_lens_loader_parameter.setVcmWorkForce(text)
            }
        }
        Label{
            text:qsTr("速度")
        }
        TextField{
            text:tcp_lens_loader_parameter.vcmWorkSpeed
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator{
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                tcp_lens_loader_parameter.setVcmWorkSpeed(text)
            }
        }
        Label{
            text:qsTr("限力区间")
        }
        TextField{
            text:tcp_lens_loader_parameter.vcmMargin
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator{
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                tcp_lens_loader_parameter.setVcmMargin(text)
            }
        }
    }
    RowLayout{
        Label{
            text:qsTr("Lens 物料高度")
        }
        TextField{
            text:tcp_lens_loader_parameter.placeLensZ
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator{
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                tcp_lens_loader_parameter.setPlaceLensZ(text)
            }
        }
        Button{
            text:qsTr("lens视觉")
            width:40
            height:40
            onClicked: {
                logicManager.performHandling(tcp_lens_loader_parameter.moduleName,LensLoaderModule.LENS_PR)
            }
        }
        Button{
            text:qsTr("移动吸头")
            width: 40
            height: 40
            onClicked: {
                logicManager.performHandling(tcp_lens_loader_parameter.moduleName,LensLoaderModule.LENS_PR+
                                             LensLoaderModule.ToWork)
            }
        }
        Button{
            text:qsTr("测高")
            width: 40
            height: 40
            onClicked: {
                logicManager.performHandling(tcp_lens_loader_parameter.moduleName,LensLoaderModule.MeasureLensInTray)
            }
        }
    }
    RowLayout{
        Label { text:qsTr("Pick Lens Offset") }
        Label{
            text:qsTr("X")
        }
        TextField{
            text: tcp_picker_offset.X
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator{
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                tcp_picker_offset.setX(text)
            }
        }
        Label{
            text:qsTr("Y")
        }
        TextField{
            text: tcp_picker_offset.Y
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator{
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                tcp_picker_offset.setY(text)
            }
        }
    }
    RowLayout {
        Label { text: qsTr("Place Lens Offset") }
        RowLayout{
            Label{
                text:qsTr("X")
            }
            TextField{
                text: tcp_picker_offset.X
                horizontalAlignment: TextInput.AlignHCenter
                validator: DoubleValidator{
                    decimals: 6
                    notation: DoubleValidator.StandardNotation
                }
                onEditingFinished: {
                    tcp_picker_offset.setX(text)
                }
            }
            Label{
                text:qsTr("Y")
            }
            TextField{
                text: tcp_picker_offset.Y
                horizontalAlignment: TextInput.AlignHCenter
                validator: DoubleValidator{
                    decimals: 6
                    notation: DoubleValidator.StandardNotation
                }
                onEditingFinished: {
                    tcp_picker_offset.setY(text)
                }
            }
        }
    }
    RowLayout { Label { text: qsTr("测试") }}
    RowLayout {
        Label { text: qsTr("Tray1 目标位置") }
        Label { text: qsTr("目标行") }
        TextField{
            id:t_nrow_1
            text: "1"
            horizontalAlignment: TextInput.AlignHCenter
            validator: IntValidator{
                bottom: 1
            }
        }
        Label{
            text: qsTr("目标列")
        }
        TextField{
            id:t_ncol_1
            text: "1"
            horizontalAlignment: TextInput.AlignHCenter
            validator: IntValidator{
                bottom: 1
            }
        }
        Button{
            text:qsTr("下次取料位置")
            width: 40
            height: 40
            onClicked: {
                tcp_material_tray.setTrayFirst(t_ncol_1.text-1,t_nrow_1.text-1,0)
            }
        }
        Button{
            text:title_move_to
            width: 40
            height: 40
            onClicked: {
                var param = {}
                param["col"] = t_ncol_1.text-1
                param["row"] = t_nrow_1.text-1
                param["tray_index"] = 0
                logicManager.performHandling(tcp_lens_loader_parameter.moduleName,LensLoaderModule.LENS_TRAY1, JSON.stringify(param))
            }
        }
        Button{
            text:qsTr("取 Tray Lens")
            width: 40
            height: 40
            onClicked: {
                tcp_material_tray.setTrayCurrent(t_ncol_1.text-1,t_nrow_1.text-1,0)
                var param = {}
                param["col"] = t_ncol_1.text-1
                param["row"] = t_nrow_1.text-1
                param["tray_index"] = 0
                param["skip_dialog"] = true
                logicManager.performHandling(tcp_lens_loader_parameter.moduleName,LensLoaderModule.LENS_TRAY1+
                                             LensLoaderModule.LENS_PR+
                                             LensLoaderModule.ToWork+
                                             LensLoaderModule.PICK_LENS_FROM_TRAY, JSON.stringify(param))
            }
        }
        Button{
            text:qsTr("放 Lens To Tray")
            onClicked: {
                tcp_material_tray.setTrayCurrent(t_ncol_1.text-1,t_nrow_1.text-1,0)
                var param = {}
                param["col"] = t_ncol_1.text-1
                param["row"] = t_nrow_1.text-1
                param["tray_index"] = 0
                param["skip_dialog"] = true
                logicManager.performHandling(tcp_lens_loader_parameter.moduleName,LensLoaderModule.LENS_TRAY1+
                                             LensLoaderModule.VACANCY_PR+
                                             LensLoaderModule.ToWork+
                                             LensLoaderModule.PLACE_NG_LENS_TO_TRAY,JSON.stringify(param))
                console.log("Finish perform handling")
            }
        }
    }
    RowLayout {
        Label { text: qsTr("Tray2 目标位置") }
        Label{
            text: qsTr("目标行")
        }
        TextField{
            id:t_nrow_2
            text: "1"
            horizontalAlignment: TextInput.AlignHCenter
            validator: IntValidator{
                bottom: 1
            }
        }
        Label{
            text: qsTr("目标列")
        }
        TextField{
            id:t_ncol_2
            text: "1"
            horizontalAlignment: TextInput.AlignHCenter
            validator: IntValidator{
                bottom: 1
            }
        }
        Button{
            text:qsTr("下次取料位置")
            width: 40
            height: 40
            onClicked: {
                material_tray.setTrayFirst(t_ncol_2.text-1,t_nrow_2.text-1,1)
            }
        }
        Button{
            text:title_move_to
            width: 40
            height: 40
            onClicked: {
                var param = {}
                param["col"] = t_ncol_2.text-1
                param["row"] = t_nrow_2.text-1
                param["tray_index"] = 0
                logicManager.performHandling(tcp_lens_loader_parameter.moduleName,LensLoaderModule.LENS_TRAY2, JSON.stringify(param))
            }
        }
        Button{
            text:qsTr("取 Tray Lens")
            width: 40
            height: 40
            onClicked: {
                tcp_material_tray.setTrayCurrent(t_ncol_1.text-1,t_nrow_1.text-1,0)
                var param = {}
                param["col"] = t_ncol_1.text-1
                param["row"] = t_nrow_1.text-1
                param["tray_index"] = 0
                param["skip_dialog"] = true
                logicManager.performHandling(tcp_lens_loader_parameter.moduleName,LensLoaderModule.LENS_TRAY2+
                                             LensLoaderModule.LENS_PR+
                                             LensLoaderModule.ToWork+
                                             LensLoaderModule.PICK_LENS_FROM_TRAY, JSON.stringify(param))
            }
        }
        Button{
            text:qsTr("放 Lens To Tray")
            onClicked: {
                tcp_material_tray.setTrayCurrent(t_ncol_1.text-1,t_nrow_1.text-1,0)
                var param = {}
                param["col"] = t_ncol_1.text-1
                param["row"] = t_nrow_1.text-1
                param["tray_index"] = 0
                param["skip_dialog"] = true
                logicManager.performHandling(tcp_lens_loader_parameter.moduleName,LensLoaderModule.LENS_TRAY2+
                                             LensLoaderModule.VACANCY_PR+
                                             LensLoaderModule.ToWork+
                                             LensLoaderModule.PLACE_NG_LENS_TO_TRAY,JSON.stringify(param))
            }
        }
    }
    RowLayout {
        Button{
            text: qsTr("LUT 移动到 Load 位置")
            onClicked: {
                logicManager.performHandling(tcpLUTParams.moduleName,LutModule.LOAD_POS)
            }
        }
        Button{
            text:qsTr("Lens 视觉")
            width:40
            height:40
            onClicked: {
                logicManager.performHandling(tcp_lens_loader_parameter.moduleName,LensLoaderModule.LENS_PR)
            }
        }

        Button{
            text:qsTr("取 LUT1 Lens")
            width: 40
            height: 40
            onClicked: {
                tcp_material_tray.setTrayCurrent(t_ncol_1.text-1,t_nrow_1.text-1,0)
                var param = {}
                param["skip_dialog"] = true
                logicManager.performHandling(tcp_lens_loader_parameter.moduleName,LensLoaderModule.LUT_POS1+
                                             LensLoaderModule.LUT_LENS_PR+
                                             LensLoaderModule.ToWork+
                                             LensLoaderModule.PICK_NG_LENS_FROM_LUT1, JSON.stringify(param))
            }
        }
        Button{
            text:qsTr("取 LUT2 Lens")
            width: 40
            height: 40
            onClicked: {
                var param = {}
                param["skip_dialog"] = true
                tcp_material_tray.setTrayCurrent(t_ncol_1.text-1,t_nrow_1.text-1,0)
                logicManager.performHandling(tcp_lens_loader_parameter.moduleName,LensLoaderModule.LUT_POS2+
                                             LensLoaderModule.LUT_LENS_PR+
                                             LensLoaderModule.ToWork+
                                             LensLoaderModule.PICK_NG_LENS_FROM_LUT2, JSON.stringify(param))
            }
        }
    }
    RowLayout {
        Button{
            text:qsTr("Lens Tray 空位视觉")
            onClicked: {
                logicManager.performHandling(tcp_lens_loader_parameter.moduleName,
                                             LensLoaderModule.VACANCY_PR)
            }
        }
        Button{
            text:qsTr("放 Lens To LUT1")
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
            text:qsTr("放 Lens To LUT2")
            onClicked: {
                var param = {}
                param["skip_dialog"] = true
                logicManager.performHandling(tcp_lens_loader_parameter.moduleName,LensLoaderModule.LUT_POS2+
                                             LensLoaderModule.LUT_NG_SLOT_PR+
                                             LensLoaderModule.ToWork+
                                             LensLoaderModule.PLACE_LENS_TO_LUT, JSON.stringify(param))
            }
        }
    }
    RowLayout {
        Button {
            text: qsTr("AA1 Unload Lens")
            onClicked: {
                logicManager.performHandling(tcpLUTParams.moduleName,LutModule.AA1_UNPICK_LENS)
            }
        }
        Button {
            text: qsTr("AA2 Unload Lens")
            onClicked: {
                logicManager.performHandling(tcpLUTParams.moduleName,LutModule.AA2_UNPICK_LENS)
            }
        }
    }

    RowLayout {
        Button{
            text: qsTr("LPA VAC Open/Closed")
            onClicked: {
                baseModuleManager.toogleIoState(tcp_lens_loader_parameter.pickarmVaccumName)
            }
        }
        RoundButton{
            background: Rectangle {
                radius: 6
                color: {
                    if (tcpLensLoaderModuleState.tcpVaccumState) return "green"
                    else return "red"
                }
            }
        }
    }
}
