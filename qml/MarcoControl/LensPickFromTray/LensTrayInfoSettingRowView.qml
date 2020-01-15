import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11
import "../../"
import SomeLib 1.1

ColumnLayout{
    RowLayout{
        Label { text: qsTr("料盘信息") }
    }
    RowLayout{
       Label{
           text:qsTr("料盘1起点")
       }
       Label{
            text: qsTr("X")
       }
       TextField{
            text: tcp_tray_start_point1.X
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                tcp_tray_start_point1.setX(text)
            }
       }
       Label{
            text: qsTr("Y")
       }
       TextField{
            text: tcp_tray_start_point1.Y
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                tcp_tray_start_point1.setY(text)
            }
       }
       Button{
           text: title_move_to
           onClicked: {
               logicManager.performHandling(tcp_lens_loader_parameter.moduleName, LensLoaderModule.LENS_TRAY1_START_POS)
           }
       }

       Button{
           text:title_read_encoder
           width: 40
           height: 40
           onClicked: {
               //ToDo: Get the motor name from parameters
               var x  = baseModuleManager.getMotorFeedbackPos(tcp_lens_loader_parameter.motorTrayName)
               var y = baseModuleManager.getMotorFeedbackPos(tcp_lens_loader_parameter.motorYName);
               tcp_tray_start_point1.setX(x);
               tcp_tray_start_point1.setY(y);
           }
       }
       Label{
           text: qsTr("料盘1终点")
       }
       Label{
           text: qsTr("X")
       }
       TextField{
           text: tcp_first_tray_end_position.X
           horizontalAlignment: TextInput.AlignHCenter
           validator: DoubleValidator {
               decimals: 6
               notation: DoubleValidator.StandardNotation
           }
           onEditingFinished: {
               tcp_first_tray_end_position.setX(text)
           }
       }
       Label{
           text: qsTr("Y")
       }
       TextField{
           text: tcp_first_tray_end_position.Y
           horizontalAlignment: TextInput.AlignHCenter
           validator: DoubleValidator {
               decimals: 6
               notation: DoubleValidator.StandardNotation
           }
           onEditingFinished: {
               tcp_first_tray_end_position.setY(text)
           }
       }
       Button{
           text:title_move_to
           onClicked: {
               logicManager.performHandling(tcp_lens_loader_parameter.moduleName, LensLoaderModule.LENS_TRAY1_END_POS)
           }
       }

       Button{
           text:title_read_encoder
           onClicked: {
               var x  = baseModuleManager.getMotorFeedbackPos(tcp_lens_loader_parameter.motorTrayName)
               var y = baseModuleManager.getMotorFeedbackPos(tcp_lens_loader_parameter.motorYName);
               tcp_first_tray_end_position.setX(x);
               tcp_first_tray_end_position.setY(y);
           }
       }
    }
    RowLayout{
        Label{
            text:qsTr("料盘2起点")
        }
        Label{
            text: qsTr("X")
        }
        TextField{
            text: tcp_tray_start_point2.X
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                tcp_tray_start_point2.setX(text)
            }
        }
        Label{
            text: qsTr("Y")
        }
        TextField{
            text: tcp_tray_start_point2.Y
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                tcp_tray_start_point2.setY(text)
            }
        }
        Button{
            text: title_move_to
            onClicked: {
                logicManager.performHandling(tcp_lens_loader_parameter.moduleName, LensLoaderModule.LENS_TRAY2_START_POS)
            }
        }

        Button{
            text:title_read_encoder
            width: 40
            height: 40
            onClicked: {
                var x  = baseModuleManager.getMotorFeedbackPos("LTL_X")
                var y = baseModuleManager.getMotorFeedbackPos("LPA_Y");
                tcp_tray_start_point2.setX(x);
                tcp_tray_start_point2.setY(y);
            }
        }
    }
    RowLayout{
        TCPTraySpec {}
    }
    RowLayout{
        Label { text: qsTr("测试") }
    }
    RowLayout{
        Label{ text: qsTr("料盘1") }
        Label{
            text: qsTr("目标行")
        }
        TextField{
            id:t_nrow
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
            id:t_ncol
            text: "1"
            horizontalAlignment: TextInput.AlignHCenter
            validator: IntValidator{
                bottom: 1
            }
        }
        Button{
            text:title_move_to
            width: 40
            height: 40
            onClicked: {
                tcp_material_tray.setTrayCurrent(t_ncol.text-1,t_nrow.text-1,0)
                var param = {}
                param["col"] = t_ncol.text-1
                param["row"] = t_nrow.text-1
                param["tray_index"] = 0
                console.log(param)
                logicManager.performHandling(tcp_lens_loader_parameter.moduleName,LensLoaderModule.LENS_TRAY1, JSON.stringify(param))
            }
        }
    }
    RowLayout{
        Label{ text: qsTr("料盘2") }
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
            text:title_move_to
            width: 40
            height: 40
            onClicked: {
                tcp_material_tray.setTrayCurrent(t_ncol_2.text-1,t_nrow_2.text-1,1)
                logicManager.performHandling(tcp_lens_loader_parameter.moduleName,LensLoaderModule.LENS_TRAY2)
            }
        }
    }
}
