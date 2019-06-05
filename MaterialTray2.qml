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
                    text:title_move_to
                    width: 40
                    height: 40
                    onClicked: {
                        if (baseModuleManager.getServerMode() === 0){
                            logicManager.lensPickArmMoveToTray2StartPos()
                        }else{
                            sensorLoaderModule.performHandling(SensorLoaderModule.SENSOR_TRAY2_START_POS)
                        }
                    }
                }
                Button{
                    text:title_read_encoder
                    width: 40
                    height: 40
                    onClicked: {
                        var x =0;
                        var y=0;
                        if (baseModuleManager.getServerMode() === 0){
                            x = baseModuleManager.getMotorFeedbackPos(lensPickArmParams.motorTrayName)
                            y = baseModuleManager.getMotorFeedbackPos(lensPickArmParams.motorYName)
                        }else{

                            x = baseModuleManager.getMotorFeedbackPos(sensorPickArmParams.motorXName)
                            y = baseModuleManager.getMotorFeedbackPos(sensorPickArmParams.motorYName)
                        }
                        tray_start_point2.setX(x);
                        tray_start_point2.setY(y);
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
                        material_tray.setTrayCurrent(t_ncol.text-1,t_nrow.text-1,1)

                        if (baseModuleManager.getServerMode() == 0){
                            logicManager.lensPickArmMoveToTray2Pos()
                        }else{
                            sensorLoaderModule.performHandling(SensorLoaderModule.SENSOR_TRAY2)
                        }
                    }
                }
            }
            RowLayout{
                visible: baseModuleManager.getServerMode() !=0
                Label{
                    text:qsTr("成品高度")
                }
                TextField{
                    text: sensorLoaderParameter.placeProductZ
                    horizontalAlignment: TextInput.AlignHCenter
                    validator: DoubleValidator {
                        decimals: 6
                        notation: DoubleValidator.StandardNotation
                    }
                    onEditingFinished: {
                        sensorLoaderParameter.setPlaceProductZ(text)
                    }
                }
                Button{
                    text:qsTr("测高")
                    onClicked: {
                        sensorLoaderModule.performHandling(SensorLoaderModule.MEASURE_PRODUCT_IN_TRAY)
                    }
                }
            }
            RowLayout{
                visible: baseModuleManager.getServerMode() !=0
                Label{
                    text:qsTr("Ng Sensor高度")
                }
                TextField{
                    text: sensorLoaderParameter.placeNgSensorZ
                    horizontalAlignment: TextInput.AlignHCenter
                    validator: DoubleValidator {
                        decimals: 6
                        notation: DoubleValidator.StandardNotation
                    }
                    onEditingFinished: {
                        sensorLoaderParameter.setPlaceNgSensorZ(text)
                    }
                }
                Button{
                    text:qsTr("测高")
                    onClicked: {
                        sensorLoaderModule.performHandling(SensorLoaderModule.MEASURE_NG_SENSOR_IN_TRAY)
                    }
                }
            }
            RowLayout{
                Layout.alignment: Qt.AlignVCenter|Qt.AlignRight
                Button{
                    visible: baseModuleManager.getServerMode()===0
                    text:qsTr("lens视觉")
                    width: 40
                    height: 40
                    onClicked: {
                        logicManager.lensPickArmLensPR()
                    }
                }
                Button{
                    visible: baseModuleManager.getServerMode()==0
                    text:qsTr("取lens")
                    width: 40
                    height: 40
                    onClicked: {
                        logicManager.lensPickArmMoveToPickLensFromTray2()
                    }
                }
                Button{
                    text:qsTr("空位视觉")
                    width:40
                    height: 40
                    onClicked:{
                        if(baseModuleManager.getServerMode()==0){
                            logicManager.performLocation(lens_loader_parameter.vacancyLocationName)
                        }else{
                            logicManager.performLocation(sensorLoaderParameter.vacancyLocationName)
                        }
                    }
                }

                Button{
                    text:qsTr("移动吸头")
                    width:40
                    height: 40
                    onClicked: {
                        material_tray.setTrayCurrent(t_ncol.text-1,t_nrow.text-1,1)

                        if (baseModuleManager.getServerMode() == 0){
                            logicManager.lensPickArmVacancyTrayPR()
                        }else{
                            //232
                            sensorLoaderModule.performHandling(SensorLoaderModule.TO_PICK2
                                                               +SensorLoaderModule.VACANCY_PR
                                                               +SensorLoaderModule.SENSOR_TRAY2)
                        }
                    }
                }

                Button{
                    text:baseModuleManager.getServerMode()==0?qsTr("放lens"):qsTr("放成品")
                    width: 40
                    height: 40
                    onClicked: {
                        material_tray.setTrayCurrent(t_ncol.text-1,t_nrow.text-1,1)

                        if (baseModuleManager.getServerMode() == 0){
                            logicManager.lensPickArmMoveToPlaceLensToTray2()
                        }else{
                            //9232
                            sensorLoaderModule.performHandling(SensorLoaderModule.PLACE_PRODUCT_TO_TRAY
                                                               +SensorLoaderModule.TO_PICK2
                                                               +SensorLoaderModule.VACANCY_PR
                                                               +SensorLoaderModule.SENSOR_TRAY2)
                        }
                    }
                }
                Button{
                    visible: baseModuleManager.getServerMode()!=0
                    text:qsTr("放ng Sensor")
                    width:40
                    height: 40
                    onClicked: {
                        material_tray.setTrayCurrent(t_ncol.text-1,t_nrow.text-1,1)
                        //6232
                        sensorLoaderModule.performHandling(SensorLoaderModule.PLACE_NG_SENSOR_TO_TRAY
                                                           +SensorLoaderModule.TO_PICK2
                                                           +SensorLoaderModule.VACANCY_PR
                                                           +SensorLoaderModule.SENSOR_TRAY2)
                    }
                }
            }
        }
    }
}
