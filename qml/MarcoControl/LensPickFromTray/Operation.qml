import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11
import TrayLoaderModuleLib 1.1
import SomeLib 1.1
ColumnLayout{
    RowLayout{ Label { text: qsTr("弹夹操作"); font.bold: true } }
    RowLayout{
        Label{
            text: qsTr("Load Lens Tray")
        }
        TextField{
            id: layerIndex_in
            text:"1"
            horizontalAlignment:TextInput.AlignHCenter
            validator: IntValidator{

            }
        }
        Button{
            text: title_move_to
            onClicked: {
                logicManager.performHandling(tcp_tray_loader_module_parameters.moduleName,TrayLoaderModule.ENTRANCE_CLIP_POS,layerIndex_in.text-1)
            }
        }
        Button{
            text: qsTr("加载")
            enabled: false
            onClicked: {
                // TBD: 集成动作，入料弹夹到指定层，拉出lens tray到轨道工作位置
            }
        }
    }
    RowLayout{
        Label{
            text: qsTr("Unload Lens Tray")
        }
        TextField{
            id: layerIndex_out
            text:"1"
            horizontalAlignment:TextInput.AlignHCenter
            validator: IntValidator{

            }
        }
        Button{
            text: title_move_to
            onClicked: {
                logicManager.performHandling(tcp_tray_loader_module_parameters.moduleName,TrayLoaderModule.EXIT_CLIP_POS,layerIndex_out.text-1)
            }
        }
        Button{
            text: qsTr("卸载")
            enabled: false
            onClicked: {
                // TBD: 集成动作，出料弹夹到指定层，从轨道工作位置推出lens tray
            }
        }
    }
    RowLayout{ Label { text: qsTr("料盘信息"); font.bold: true } }
    RowLayout{
        Label{
            text: qsTr("Tray1目标位置")
        }
        Label{
            text: qsTr("Row")
        }
        TextField{
            id:t_nrow_1
            text: "1"
            horizontalAlignment: TextInput.AlignHCenter
            validator: IntValidator{

            }
        }
        Label{
            text: qsTr("COL")
        }
        TextField{
            id:t_ncol_1
            text: "1"
            horizontalAlignment: TextInput.AlignHCenter
            validator: IntValidator{

            }
        }
        Button{
            text: qsTr("Move")
            onClicked: {
                tcp_material_tray.setTrayCurrent(t_ncol_1.text-1,t_nrow_1.text-1,0)
                var param = {}
                param["col"] = t_ncol_1.text-1
                param["row"] = t_nrow_1.text-1
                param["tray_index"] = 0
                console.log(param)
                logicManager.performHandling(tcp_lens_loader_parameter.moduleName,LensLoaderModule.LENS_TRAY1, JSON.stringify(param))
            }
        }
        Button{
            text: qsTr("Pick")
            onClicked: {
                logicManager.performHandling(tcp_lens_loader_parameter.moduleName,LensLoaderModule.LENS_TRAY1+
                                             LensLoaderModule.LENS_PR+
                                             LensLoaderModule.ToWork+
                                             LensLoaderModule.PICK_LENS_FROM_TRAY)
            }
        }
        Button{
            text: qsTr("Place")
            onClicked: {
                logicManager.performHandling(tcp_lens_loader_parameter.moduleName,LensLoaderModule.LENS_TRAY1+
                                             LensLoaderModule.VACANCY_PR+
                                             LensLoaderModule.ToWork+
                                             LensLoaderModule.PLACE_NG_LENS_TO_TRAY)
            }
        }
        Button{
            text: qsTr("下次取料位置")
            onClicked: {
                material_tray.setTrayFirst(t_ncol_1.text-1,t_nrow_1.text-1,0)
            }
        }
    }
    RowLayout{
        Label{
            text: qsTr("Tray2目标位置")
        }
        Label{
            text: qsTr("Row")
        }
        TextField{
            id:t_nrow_2
            text: "1"
            horizontalAlignment: TextInput.AlignHCenter
            validator: IntValidator{

            }
        }
        Label{
            text: qsTr("COL")
        }
        TextField{
            id:t_ncol_2
            text: "1"
            horizontalAlignment: TextInput.AlignHCenter
            validator: IntValidator{

            }
        }
        Button{
            text: qsTr("Move")
            onClicked: {
                tcp_material_tray.setTrayCurrent(t_ncol_2.text-1,t_nrow_2.text-1,1)
                var param = {}
                param["col"] = t_ncol_2.text-1
                param["row"] = t_nrow_2.text-1
                param["tray_index"] = 1
                logicManager.performHandling(tcp_lens_loader_parameter.moduleName,LensLoaderModule.LENS_TRAY2, JSON.stringify(param))
            }
        }
        Button{
            text: qsTr("Pick")
            onClicked: {
                logicManager.performHandling(tcp_lens_loader_parameter.moduleName,LensLoaderModule.LENS_TRAY2+
                                             LensLoaderModule.LENS_PR+
                                             LensLoaderModule.ToWork+
                                             LensLoaderModule.PICK_LENS_FROM_TRAY)
            }
        }
        Button{
            text: qsTr("Place")
            onClicked: {
                logicManager.performHandling(tcp_lens_loader_parameter.moduleName,LensLoaderModule.LENS_TRAY2+
                                             LensLoaderModule.VACANCY_PR+
                                             LensLoaderModule.ToWork+
                                             LensLoaderModule.PLACE_NG_LENS_TO_TRAY)
            }
        }
        Button{
            text: qsTr("下次取料位置")
            onClicked: {
                material_tray.setTrayFirst(t_ncol_2.text-1,t_nrow_2.text-1,1)
            }
        }
    }
}
