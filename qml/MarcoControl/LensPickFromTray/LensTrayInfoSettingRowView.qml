import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11
import "../../../"
ColumnLayout{
    RowLayout{
        Label { text: qsTr("料盘信息") }
    }
    RowLayout{
        Button{
           text:qsTr("料盘1起点")
           onClicked: {
               if (baseModuleManager.getServerMode() === 0){
                   logicManager.performHandling(lens_loader_parameter.moduleName,LensLoaderModule.LENS_TRAY1_START_POS)
               }else{
                   logicManager.performHandling(sensorLoaderParameter.moduleName,SensorLoaderModule.SENSOR_TRAY_1_START_POS)
               }
           }
       }
       Label{
            text: qsTr("X")
       }
       TextField{
            text: tray_start_point1.X
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                tray_start_point1.setX(text)
            }
       }
       Label{
            text: qsTr("Y")
       }
       TextField{
            text: tray_start_point1.Y
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                tray_start_point1.setY(text)
            }
       }
       Button{
           text:title_read_encoder
           width: 40
           height: 40
           onClicked: {
               var x =0;
               var x1 =0;
               var y=0;
               if (baseModuleManager.getServerMode() === 0){
                   x = baseModuleManager.getMotorFeedbackPos(lensPickArmParams.motorTrayName)
                   x1 = baseModuleManager.getMotorFeedbackPos(lensPickArmParams.motorXName)
                   y = baseModuleManager.getMotorFeedbackPos(lensPickArmParams.motorYName)
                   tray_start_point1.setX(x + lensPickArmModuleParameter.visonPositionX - x1);
                   tray_start_point1.setY(y);
               }else{

                   x = baseModuleManager.getMotorFeedbackPos(sensorPickArmParams.motorXName)
                   y = baseModuleManager.getMotorFeedbackPos(sensorPickArmParams.motorYName)
                   tray_start_point1.setX(x);
                   tray_start_point1.setY(y);
               }
           }
       }
       Button{
           text: qsTr("料盘1终点")
           onClicked: {
               if (baseModuleManager.getServerMode() === 0){
                   logicManager.performHandling(lens_loader_parameter.moduleName,LensLoaderModule.LENS_TRAY1_END_POS)
               }else{
                   logicManager.performHandling(sensorLoaderParameter.moduleName,SensorLoaderModule.SENSOR_TRAY_1_END_POS)
               }
           }
       }
       Label{
           text: qsTr("X")
       }
       TextField{
           text: first_tray_end_position.X
           horizontalAlignment: TextInput.AlignHCenter
           validator: DoubleValidator {
               decimals: 6
               notation: DoubleValidator.StandardNotation
           }
           onEditingFinished: {
               first_tray_end_position.setX(text)
           }
       }
       Label{
           text: qsTr("Y")
       }
       TextField{
           text: first_tray_end_position.Y
           horizontalAlignment: TextInput.AlignHCenter
           validator: DoubleValidator {
               decimals: 6
               notation: DoubleValidator.StandardNotation
           }
           onEditingFinished: {
               first_tray_end_position.setY(text)
           }
       }
       Button{
           text:title_read_encoder
           onClicked: {
               var x =0;
               var x1 =0;
               var y=0;
               if (baseModuleManager.getServerMode() === 0){
                   x = baseModuleManager.getMotorFeedbackPos(lensPickArmParams.motorTrayName)
                   x1 = baseModuleManager.getMotorFeedbackPos(lensPickArmParams.motorXName)
                   y = baseModuleManager.getMotorFeedbackPos(lensPickArmParams.motorYName)
                   first_tray_end_position.setX(x + lensPickArmModuleParameter.visonPositionX - x1);
                   first_tray_end_position.setY(y);
               }else{

                   x = baseModuleManager.getMotorFeedbackPos(sensorPickArmParams.motorXName)
                   y = baseModuleManager.getMotorFeedbackPos(sensorPickArmParams.motorYName)
                   first_tray_end_position.setX(x);
                   first_tray_end_position.setY(y);
               }
           }
       }
    }
    RowLayout{
        Button{
            text:qsTr("料盘2起点")
            onClicked: {
                if (baseModuleManager.getServerMode() === 0){
                    logicManager.performHandling(lens_loader_parameter.moduleName,LensLoaderModule.LENS_TRAY2_START_POS)
                }else{
                    logicManager.performHandling(sensorLoaderParameter.moduleName,SensorLoaderModule.SENSOR_TRAY_2_START_POS)
                }
            }
        }
        Label{
            text: qsTr("X")
        }
        TextField{
            text: tray_start_point2.X
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                tray_start_point2.setX(text)
            }
        }
        Label{
            text: qsTr("Y")
        }
        TextField{
            text: tray_start_point2.Y
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                tray_start_point2.setY(text)
            }
        }
        Button{
            text:title_read_encoder
            width: 40
            height: 40
            onClicked: {
                var x =0;
                var y =0;
                if (baseModuleManager.getServerMode() === 0){
                    x = baseModuleManager.getMotorFeedbackPos(lensPickArmParams.motorTrayName)
                    var x1 = baseModuleManager.getMotorFeedbackPos(lensPickArmParams.motorXName)
                    y = baseModuleManager.getMotorFeedbackPos(lensPickArmParams.motorYName)
                    tray_start_point2.setX(x + lensPickArmModuleParameter.visonPositionX - x1);
                    tray_start_point2.setY(y);
                }else{

                    x = baseModuleManager.getMotorFeedbackPos(sensorPickArmParams.motorXName)
                    y = baseModuleManager.getMotorFeedbackPos(sensorPickArmParams.motorYName)
                    tray_start_point2.setX(x);
                    tray_start_point2.setY(y);
                }
            }
        }
    }
    RowLayout{
        TraySpecs {}
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
                material_tray.setTrayCurrent(t_ncol.text-1,t_nrow.text-1,0)
                if (baseModuleManager.getServerMode() === 0){
                    logicManager.performHandling(lens_loader_parameter.moduleName,LensLoaderModule.LENS_TRAY1)
                }else{
                    logicManager.performHandling(sensorLoaderParameter.moduleName,SensorLoaderModule.SENSOR_TRAY_1_POS)
                }
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
                material_tray.setTrayCurrent(t_ncol_2.text-1,t_nrow_2.text-1,1)

                if (baseModuleManager.getServerMode() === 0){
                    logicManager.performHandling(lens_loader_parameter.moduleName,LensLoaderModule.LENS_TRAY2)
                }else{
                    logicManager.performHandling(sensorLoaderParameter.moduleName,SensorLoaderModule.SENSOR_TRAY_2_POS)
                }
            }
        }
    }
}
