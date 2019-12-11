import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11
import TrayLoaderModuleLib 1.1
import SomeLib 1.1
ColumnLayout{
    RowLayout{ Label { text: qsTr("弹夹操作") } }
    RowLayout{
        Label{
            text: qsTr("Load Lens Tray")
        }
        TextField{
            text:"1"
            horizontalAlignment:TextInput.AlignHCenter
            validator: IntValidator{

            }
        }
        Button{
            text: title_move_to
        }
    }
    RowLayout{
        Label{
            text: qsTr("Unload Lens Tray")
        }
        TextField{
            text:"1"
            horizontalAlignment:TextInput.AlignHCenter
            validator: IntValidator{

            }
        }
        Button{
            text: title_move_to
        }
    }
    RowLayout{ Label { text: qsTr("料盘信息") } }
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
        }
        Button{
            text: qsTr("Place")
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
                tcp_material_tray.setTrayCurrent(t_ncol_2.text-1,t_nrow_2.text-1,0)
                var param = {}
                param["col"] = t_ncol_2.text-1
                param["row"] = t_nrow_2.text-1
                param["tray_index"] = 0
                logicManager.performHandling(tcp_lens_loader_parameter.moduleName,LensLoaderModule.LENS_TRAY2, JSON.stringify(param))
            }
        }
        Button{
            text: qsTr("Pick")
        }
        Button{
            text: qsTr("Place")
        }
    }
}
