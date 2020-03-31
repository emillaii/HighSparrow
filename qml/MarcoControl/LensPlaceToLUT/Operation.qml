import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11
import TrayLoaderModuleLib 1.1
import LutModuleLib 1.1
import SomeLib 1.1
ColumnLayout{
    RowLayout{ Label { text: qsTr("LUT操作") } }
    RowLayout{
        Button{
            text: qsTr("LUT Pocket1 POS")
            onClicked: {
                var param = {}
                param["skip_dialog"] = true
                logicManager.performHandling(tcp_lens_loader_parameter.moduleName,LensLoaderModule.LUT_POS1)
            }
        }
        Button{
            text: qsTr("Place to Pocket1")
            onClicked: {
                var param = {}
                param["skip_dialog"] = true
                logicManager.performHandling(tcp_lens_loader_parameter.moduleName,LensLoaderModule.LUT_POS1+
                                             LensLoaderModule.LUT_PR+
                                             LensLoaderModule.ToWork+
                                             LensLoaderModule.PLACE_LENS_TO_LUT, JSON.stringify(param))
            }
        }
        Button{
            text: qsTr("Pick From Pocket1")
            onClicked: {
                var param = {}
                param["skip_dialog"] = true
                logicManager.performHandling(tcp_lens_loader_parameter.moduleName,LensLoaderModule.LUT_POS1+
                                             LensLoaderModule.LUT_LENS_PR+
                                             LensLoaderModule.ToWork+
                                             LensLoaderModule.PICK_NG_LENS_FROM_LUT1, JSON.stringify(param))
            }
        }
    }
    RowLayout{
        Button{
            text: qsTr("LUT Pocket2 POS")
            onClicked: {
                var param = {}
                param["skip_dialog"] = true
                logicManager.performHandling(tcp_lens_loader_parameter.moduleName,LensLoaderModule.LUT_POS2, JSON.stringify(param))
            }
        }
        Button{
            text: qsTr("Place to Pocket2")
            onClicked: {
                var param = {}
                param["skip_dialog"] = true
                logicManager.performHandling(tcp_lens_loader_parameter.moduleName,LensLoaderModule.LUT_POS2+
                                             LensLoaderModule.LUT_NG_SLOT_PR+
                                             LensLoaderModule.ToWork+
                                             LensLoaderModule.PLACE_LENS_TO_LUT2, JSON.stringify(param))
            }
        }
        Button{
            text: qsTr("Pick From Pocket2")
            onClicked: {
                var param = {}
                param["skip_dialog"] = true
                logicManager.performHandling(tcp_lens_loader_parameter.moduleName,LensLoaderModule.LUT_POS2+
                                             LensLoaderModule.LUT_LENS_PR+
                                             LensLoaderModule.ToWork+
                                             LensLoaderModule.PICK_NG_LENS_FROM_LUT2, JSON.stringify(param))
            }
        }
    }
}
