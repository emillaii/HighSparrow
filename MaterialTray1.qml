import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11

ColumnLayout {
    GroupBox{
        title:qsTr("料盘起点")
        ColumnLayout{
            RowLayout{
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
                    text:title_move_to
                    width: 40
                    height: 40
                    onClicked: {
                        if (baseModuleManager.getServerMode() === 0){
                            logicManager.lensPickArmMoveToTray1StartPos()
                        }else{
                            sensorLoaderModule.performHandling(SensorLoaderModule.SENSOR_TRAY1_START_POS)
                        }
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
            }
        }
    }

    GroupBox{
        title:qsTr("料盘终点")
        ColumnLayout{
            RowLayout{
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
                    text:title_move_to
                    width: 40
                    height: 40
                    onClicked: {
                        if (baseModuleManager.getServerMode() === 0){
                            logicManager.lensPickArmMoveToTray1EndPos()
                        }else{
                            sensorLoaderModule.performHandling(SensorLoaderModule.SENSOR_TRAY1_END_POS)
                        }
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
                        if (baseModuleManager.getServerMode() == 0){
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
        }
    }

    GroupBox{
        title:qsTr("料盘操作")
        ColumnLayout{
            RowLayout{
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
                            logicManager.lensPickArmMoveToTray1Pos()
                        }else{
                            sensorLoaderModule.performHandling(SensorLoaderModule.SENSOR_TRAY1)
                        }
                    }
                }
            }

            RowLayout{
                Label{
                    text: baseModuleManager.getServerMode()===0?qsTr("物料高度"):qsTr("sensor高度")
                }
                TextField{
                    text: baseModuleManager.getServerMode()===0 ?lensPickArmModuleParameter.pickLensZ:sensorLoaderParameter.pickSensorZ
                    horizontalAlignment: TextInput.AlignHCenter
                    validator: DoubleValidator{
                        decimals: 6
                        notation: DoubleValidator.StandardNotation
                    }
                    onEditingFinished: {
                        if (baseModuleManager.getServerMode()===0)
                            lensPickArmModuleParameter.setPickLensZ(text)
                        else {
                            sensorLoaderParameter.setPickSensorZ(text)
                        }
                    }
                }

                Button{
                    text:qsTr("测高")
                    width: 40
                    height: 40
                    onClicked: {

                        if (baseModuleManager.getServerMode() === 0){
                            logicManager.lensPickArmLensMeasureHeight()
                        }else{
                            sensorLoaderModule.performHandling(SensorLoaderModule.MEASURE_SENSOR_IN_TRAY)
                        }
                    }
                }
                Button{
                    text:qsTr("执行视觉")
                    width:40
                    height:40
                    onClicked: {
                        if(baseModuleManager.getServerMode()===0){
                            logicManager.performLocation(lens_loader_parameter.lensLocationName);
                        }else{
                            logicManager.performLocation(sensorLoaderParameter.sensorLocationName);
                        }
                    }
                }

                Button{
                    //text:baseModuleManager.getServerMode()==0?qsTr("lens视觉"):qsTr("sensor视觉")
                    text:qsTr("移动吸头")
                    width: 40
                    height: 40
                    onClicked: {
                        if (baseModuleManager.getServerMode() === 0){
                            logicManager.lensPickArmLensPR()
                        }else{
                            sensorLoaderModule.performHandling(SensorLoaderModule.TO_PICK1+SensorLoaderModule.SENSOR_PR)
                        }
                    }
                }

                Button{
                    text:baseModuleManager.getServerMode()===0?qsTr("取lens"):qsTr("取sensor")
                    width: 40
                    height: 40
                    onClicked: {
                        material_tray.setTrayCurrent(t_ncol.text-1,t_nrow.text-1,0)
                        if (baseModuleManager.getServerMode() === 0){
                            logicManager.lensPickArmMoveToPickLensFromTray1()
                        }else{
                            //1121
                            sensorLoaderModule.performHandling(SensorLoaderModule.PICK_SENSOR_FROM_TRAY
                                                               +SensorLoaderModule.TO_PICK1
                                                               +SensorLoaderModule.SENSOR_PR
                                                               +SensorLoaderModule.SENSOR_TRAY1)
                        }
                    }
                }
            }
            RowLayout{
                visible: baseModuleManager.getServerMode()===0
                Layout.alignment: Qt.AlignVCenter|Qt.AlignRight
                Button{
                    text:qsTr("执行空位视觉")
                    width:40
                    height:40
                    onClicked: {
                        logicManager.performLocation(lens_loader_parameter.vacancyLocationName)
                    }
                }

                Button{
                    text:qsTr("移动吸头")
                    width: 40
                    height: 40
                    onClicked: {
                        logicManager.lensPickArmVacancyTrayPR()
                    }
                }
                Button{
                    text:qsTr("放lens")
                    width: 40
                    height: 40
                    onClicked: {
                        material_tray.setTrayCurrent(t_ncol.text-1,t_nrow.text-1,0)
                        logicManager.lensPickArmMoveToPlaceLensToTray1()
                    }
                }
            }
        }
    }
}
