import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11
import SomeLib 1.1
import LogicManagerLib 1.1
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
                            //logicManager.lensPickArmMoveToTray2StartPos()
//                            lensLoaderModule.performHandling(LensLoaderModule.LENS_TRAY2_START_POS)
                            logicManager.performHandling(lens_loader_parameter.moduleName,LensLoaderModule.LENS_TRAY2_START_POS)
                        }else{
//                            sensorLoaderModule.performHandling(SensorLoaderModule.SENSOR_TRAY_2_START_POS)
                            logicManager.performHandling(sensorLoaderParameter.moduleName,SensorLoaderModule.SENSOR_TRAY_2_START_POS)
                        }
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
                            tray_start_point2.setX(x + lens_loader_parameter.visonPositionX - x1);
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
                    text:qsTr("下次取料位置")
                    width: 40
                    height: 40
                    onClicked: {
                        material_tray.setTrayFirst(t_ncol.text-1,t_nrow.text-1,1)
                    }
                }
                Button{
                    text:title_move_to
                    width: 40
                    height: 40
                    onClicked: {
                        material_tray.setTrayCurrent(t_ncol.text-1,t_nrow.text-1,1)

                        if (baseModuleManager.getServerMode() === 0){
//                            lensLoaderModule.performHandling(LensLoaderModule.LENS_TRAY2)
                            logicManager.performHandling(lens_loader_parameter.moduleName,LensLoaderModule.LENS_TRAY2)
                        }else{
//                            sensorLoaderModule.performHandling(SensorLoaderModule.SENSOR_TRAY_2_POS)
                            logicManager.performHandling(sensorLoaderParameter.moduleName,SensorLoaderModule.SENSOR_TRAY_2_POS)
                        }
                    }
                }
            }

            RowLayout{
                visible: baseModuleManager.getServerMode()===1

                Label{
                    text: qsTr("吸料偏移:")
                }
                Label{
                    text: qsTr("X")
                }
                TextField{
                    text: sensorTray2PickOffset.X
                    horizontalAlignment: TextInput.AlignHCenter
                    validator: DoubleValidator{
                        decimals: 6
                        notation: DoubleValidator.StandardNotation
                    }
                    onEditingFinished: {
                        sensorTray2PickOffset.setX(text)
                    }
                }

                Label{
                    text: qsTr("Y")
                }
                TextField{
                    text: sensorTray2PickOffset.Y
                    horizontalAlignment: TextInput.AlignHCenter
                    validator: DoubleValidator{
                        decimals: 6
                        notation: DoubleValidator.StandardNotation
                    }
                    onEditingFinished: {
                        sensorTray2PickOffset.setY(text)
                    }
                }
                Button{
                    text:qsTr("视觉校正")
                    width:40
                    height:40
                    onClicked: {
                        material_tray.setTrayCurrent(t_ncol.text-1,t_nrow.text-1,1)
//                        sensorLoaderModule.performHandling(SensorLoaderModule.SENSOR_TRAY_1_POS
//                                                           +SensorLoaderModule.TRAY_SENSOR_OFFSET)
                        logicManager.performHandling(sensorLoaderParameter.moduleName,SensorLoaderModule.SENSOR_TRAY_2_POS
                                                     +SensorLoaderModule.TRAY_SENSOR_OFFSET)
                    }
                }
            }

            RowLayout{
                visible: baseModuleManager.getServerMode()===1
                Button{
                    text:qsTr("执行视觉")
                    width:40
                    height:40
                    onClicked: {
                        material_tray.setTrayCurrent(t_ncol.text-1,t_nrow.text-1,1)
//                        sensorLoaderModule.performHandling(SensorLoaderModule.SENSOR_TRAY_2_POS
//                                                           +SensorLoaderModule.TRAY_SENSOR_PR
//                                                           +SensorLoaderModule.TO_PR_OFFSET)
                        logicManager.performHandling(sensorLoaderParameter.moduleName,SensorLoaderModule.SENSOR_TRAY_2_POS
                                                     +SensorLoaderModule.TRAY_SENSOR_PR
                                                     +SensorLoaderModule.TO_PR_OFFSET)
                    }
                }

                Label{
                    text: qsTr("sensor高度")
                }
                TextField{
                    text: sensorLoaderParameter.pickSensorZ2
                    horizontalAlignment: TextInput.AlignHCenter
                    validator: DoubleValidator{
                        decimals: 6
                        notation: DoubleValidator.StandardNotation
                    }
                    onEditingFinished: {
                        sensorLoaderParameter.setPickSensorZ2(text)
                    }
                }
                Button{
                    text:qsTr("移动吸头")
                    width: 40
                    height: 40
                    onClicked: {
//                        material_tray.setTrayCurrent(t_ncol.text-1,t_nrow.text-1,1)
//                        sensorLoaderModule.performHandling(SensorLoaderModule.SENSOR_TRAY_2_POS
//                                                           +SensorLoaderModule.TRAY_SENSOR_PR
//                                                           +SensorLoaderModule.TO_PICK_SENSOR_POS2)
                        logicManager.performHandling(sensorLoaderParameter.moduleName,SensorLoaderModule.SENSOR_TRAY_2_POS
                                                     +SensorLoaderModule.TRAY_SENSOR_PR
                                                     +SensorLoaderModule.TO_PICK_SENSOR_POS2)
                    }
                }

                Button{
                    text:qsTr("测高")
                    width: 40
                    height: 40
                    onClicked: {
//                        sensorLoaderModule.performHandling(SensorLoaderModule.MEASURE_SENSOR_IN_TRAY2)
                        logicManager.performHandling(sensorLoaderParameter.moduleName,SensorLoaderModule.MEASURE_SENSOR_IN_TRAY2)
                    }
                }

                Button{
                    text:qsTr("取sensor")
                    width: 40
                    height: 40
                    onClicked: {
                        material_tray.setTrayCurrent(t_ncol.text-1,t_nrow.text-1,1)
//                        sensorLoaderModule.performHandling(SensorLoaderModule.SENSOR_TRAY_2_POS
//                                                           +SensorLoaderModule.TRAY_SENSOR_PR
//                                                           +SensorLoaderModule.TO_PICK_SENSOR_POS2
//                                                           +SensorLoaderModule.PICK_SENSOR_FROM_TRAY2)
                        logicManager.performHandling(sensorLoaderParameter.moduleName,SensorLoaderModule.SENSOR_TRAY_2_POS
                                                     +SensorLoaderModule.TRAY_SENSOR_PR
                                                     +SensorLoaderModule.TO_PICK_SENSOR_POS2
                                                     +SensorLoaderModule.PICK_SENSOR_FROM_TRAY2)
                    }
                }
            }
            RowLayout{
                visible: baseModuleManager.getServerMode()===1
                Button{
                    text:qsTr("空位视觉")
                    width:40
                    height:40
                    onClicked: {
                        material_tray.setTrayCurrent(t_ncol.text-1,t_nrow.text-1,1)
//                        sensorLoaderModule.performHandling(SensorLoaderModule.SENSOR_TRAY_2_POS
//                                                           +SensorLoaderModule.TRAY_EMPTY_PR
//                                                           +SensorLoaderModule.TO_PR_OFFSET)
                        logicManager.performHandling(sensorLoaderParameter.moduleName,SensorLoaderModule.SENSOR_TRAY_2_POS
                                                     +SensorLoaderModule.TRAY_EMPTY_PR
                                                     +SensorLoaderModule.TO_PR_OFFSET)
                    }
                }

                Label{
                    text: qsTr("成品  高度")
                }
                TextField{
                    text: sensorLoaderParameter.placeProductZ2
                    horizontalAlignment: TextInput.AlignHCenter
                    validator: DoubleValidator{
                        decimals: 6
                        notation: DoubleValidator.StandardNotation
                    }
                    onEditingFinished: {
                        sensorLoaderParameter.setPlaceProductZ2(text)
                    }
                }

                Button{
                    text:qsTr("移动吸头")
                    width: 40
                    height: 40
                    onClicked: {
                        material_tray.setTrayCurrent(t_ncol.text-1,t_nrow.text-1,1)
//                        sensorLoaderModule.performHandling(SensorLoaderModule.SENSOR_TRAY_2_POS
//                                                           +SensorLoaderModule.TRAY_EMPTY_PR
//                                                           +SensorLoaderModule.TO_PLACE_PRODUCT_POS2)
                        logicManager.performHandling(sensorLoaderParameter.moduleName,SensorLoaderModule.SENSOR_TRAY_2_POS
                                                     +SensorLoaderModule.TRAY_EMPTY_PR
                                                     +SensorLoaderModule.TO_PLACE_PRODUCT_POS2)
                    }
                }

                Button{
                    text:qsTr("测高")
                    width: 40
                    height: 40
                    onClicked: {
//                        sensorLoaderModule.performHandling(SensorLoaderModule.MEASURE_PRODUCT_IN_TRAY2)
                        logicManager.performHandling(sensorLoaderParameter.moduleName,SensorLoaderModule.MEASURE_PRODUCT_IN_TRAY2)
                    }
                }
            }
            RowLayout{
                visible: baseModuleManager.getServerMode()===1
                CheckBox{
                    text: qsTr("不限力")
                    checked: sensorLoaderParameter.disablePlaceToGoodTrayForceLimit
                    onCheckedChanged: {
                        sensorLoaderParameter.setDisablePlaceToGoodTrayForceLimit(checked)
                    }
                }

                Label{
                    text:qsTr("放料区间")
                }
                TextField{
                    text:sensorLoaderParameter.placeToGoodTrayMargin
                    horizontalAlignment: TextInput.AlignHCenter
                    validator: DoubleValidator{
                        decimals: 6
                        notation: DoubleValidator.StandardNotation
                    }
                    onEditingFinished: {
                        sensorLoaderParameter.setPlaceToGoodTrayMargin(text)
                    }
                }
                Button{
                    text:qsTr("退Sensor")
                    width: 40
                    height: 40
                    onClicked: {
                        material_tray.setTrayCurrent(t_ncol.text-1,t_nrow.text-1,1)
                        sensorLoaderModule.performHandling(SensorLoaderModule.SENSOR_TRAY_2_POS
                                                           +SensorLoaderModule.TRAY_EMPTY_PR
                                                           +SensorLoaderModule.TO_PICK_SENSOR_POS2
                                                           +SensorLoaderModule.PLACE_SENSOR_BACK_TO_TRAY2)
                    }
                }
                Button{
                    text:qsTr("退NG sensor")
                    width:40
                    height: 40
                    onClicked: {
                        material_tray.setTrayCurrent(t_ncol.text-1,t_nrow.text-1,1)
                        sensorLoaderModule.performHandling(SensorLoaderModule.SENSOR_TRAY_2_POS
                                                     +SensorLoaderModule.TRAY_EMPTY_PR
                                                     +SensorLoaderModule.TO_PLACE_NG_POS
                                                     +SensorLoaderModule.PLACE_NG_SENSOR_TO_TRAY)
                    }
                }
            }
            RowLayout{
                CheckBox{
                    text: qsTr("使能视觉")
                    checked: sensorLoaderParameter.enablePlaceProdcutPr
                    onCheckedChanged:
                        sensorLoaderParameter.setEnablePlaceProdcuPr(checked)
                }
                Button{
                    text:qsTr("放成品")
                    width: 40
                    height: 40
                    onClicked: {
                        material_tray.setTrayCurrent(t_ncol.text-1,t_nrow.text-1,1)
                        logicManager.performHandling(sensorLoaderParameter.moduleName,SensorLoaderModule.SENSOR_TRAY_2_POS
                                                     +SensorLoaderModule.TRAY_EMPTY_PR
                                                     +SensorLoaderModule.TO_PLACE_PRODUCT_POS2
                                                     +SensorLoaderModule.PLACE_PRODUCT_TO_TRAY2)
                    }
                }
            }

            RowLayout{
                visible: baseModuleManager.getServerMode()===1
                Label{
                    text:qsTr("放料offsetX")
                }
                TextField{
                    text:sensorTray2PlaceOffset.X
                    horizontalAlignment: TextInput.AlignHCenter
                    validator: DoubleValidator{
                        decimals: 6
                        notation: DoubleValidator.StandardNotation
                    }
                    onEditingFinished: {
                        sensorTray2PlaceOffset.setX(text)
                    }
                }
                Label{
                    text:qsTr("放料offsetY")
                }
                TextField{
                    text:sensorTray2PlaceOffset.Y
                    horizontalAlignment: TextInput.AlignHCenter
                    validator: DoubleValidator{
                        decimals: 6
                        notation: DoubleValidator.StandardNotation
                    }
                    onEditingFinished: {
                        sensorTray2PlaceOffset.setY(text)
                    }
                }
            }

            RowLayout{
                visible: baseModuleManager.getServerMode()===0
                Layout.alignment: Qt.AlignVCenter|Qt.AlignRight
                Button{
                    visible: baseModuleManager.getServerMode()===0
                    text:qsTr("lens视觉")
                    width: 40
                    height: 40
                    onClicked: {
                        //logicManager.lensPickArmLensPR()
//                        lensLoaderModule.performHandling(LensLoaderModule.LENS_PR+
//                                                         LensLoaderModule.ToWork)
                        logicManager.performHandling(lens_loader_parameter.moduleName,LensLoaderModule.LENS_PR+
                                                     LensLoaderModule.ToWork)
                    }
                }
                Button{
                    visible: baseModuleManager.getServerMode()===0
                    text:qsTr("取lens")
                    width: 40
                    height: 40
                    onClicked: {
                        //logicManager.lensPickArmMoveToPickLensFromTray2()
//                        lensLoaderModule.performHandling(LensLoaderModule.LENS_TRAY2+
//                                                         LensLoaderModule.LENS_PR+
//                                                         LensLoaderModule.ToWork+
//                                                         LensLoaderModule.PICK_LENS_FROM_TRAY)
                        logicManager.performHandling(lens_loader_parameter.moduleName,LensLoaderModule.LENS_TRAY2+
                                                     LensLoaderModule.LENS_PR+
                                                     LensLoaderModule.ToWork+
                                                     LensLoaderModule.PICK_LENS_FROM_TRAY)
                    }
                }
                Button{
                    text:qsTr("空位视觉")
                    width:40
                    height: 40
                    onClicked:{
                        material_tray.setTrayCurrent(t_ncol.text-1,t_nrow.text-1,1)
                        if(baseModuleManager.getServerMode()===0){
                            //logicManager.performLocation(lens_loader_parameter.vacancyLocationName)
                            logicManagerState.setCurrentLocationName(lens_loader_parameter.vacancyLocationName)
                            logicManagerState.setUseOriginPr(true)
                            logicManager.performHandling(LogicManager.PERFORM_LOCATION)
                        }else{
//                            sensorLoaderModule.performHandling(SensorLoaderModule.TO_PR_OFFSET
//                                                               +SensorLoaderModule.TRAY_EMPTY_PR
//                                                               +SensorLoaderModule.SENSOR_TRAY_2_POS)
                            logicManager.performHandling(sensorLoaderParameter.moduleName,SensorLoaderModule.TO_PR_OFFSET
                                                         +SensorLoaderModule.TRAY_EMPTY_PR
                                                         +SensorLoaderModule.SENSOR_TRAY_2_POS)
                        }
                    }
                }

                Button{
                    text:qsTr("移动吸头")
                    width:40
                    height: 40
                    onClicked: {
                        material_tray.setTrayCurrent(t_ncol.text-1,t_nrow.text-1,1)

                        if (baseModuleManager.getServerMode() === 0){
                            //logicManager.lensPickArmVacancyTrayPR()
//                            lensLoaderModule.performHandling(LensLoaderModule.VACANCY_PR+
//                                                             LensLoaderModule.ToWork)
                            logicManager.performHandling(lens_loader_parameter.moduleName,LensLoaderModule.VACANCY_PR+
                                                         LensLoaderModule.ToWork)
                        }else{
                            //232
//                            sensorLoaderModule.performHandling(SensorLoaderModule.TO_PLACE_PRODUCT_OFFSET
//                                                               +SensorLoaderModule.TRAY_EMPTY_PR
//                                                               +SensorLoaderModule.SENSOR_TRAY_2_POS)
                            logicManager.performHandling(sensorLoaderParameter.moduleName,SensorLoaderModule.TO_PLACE_PRODUCT_OFFSET
                                                         +SensorLoaderModule.TRAY_EMPTY_PR
                                                         +SensorLoaderModule.SENSOR_TRAY_2_POS)
                        }
                    }
                }

                Button{
                    text:baseModuleManager.getServerMode()===0?qsTr("放lens"):qsTr("放成品")
                    width: 40
                    height: 40
                    onClicked: {
                        material_tray.setTrayCurrent(t_ncol.text-1,t_nrow.text-1,1)

                        if (baseModuleManager.getServerMode() === 0){
                            //logicManager.lensPickArmMoveToPlaceLensToTray2()
//                            lensLoaderModule.performHandling(LensLoaderModule.LENS_TRAY2+
//                                                             LensLoaderModule.VACANCY_PR+
//                                                             LensLoaderModule.ToWork+
//                                                             LensLoaderModule.PLACE_NG_LENS_TO_TRAY)
                            logicManager.performHandling(lens_loader_parameter.moduleName,LensLoaderModule.LENS_TRAY2+
                                                         LensLoaderModule.VACANCY_PR+
                                                         LensLoaderModule.ToWork+
                                                         LensLoaderModule.PLACE_NG_LENS_TO_TRAY)
                        }else{
                            //9232
//                            sensorLoaderModule.performHandling(SensorLoaderModule.PLACE_PRODUCT_TO_TRAY
//                                                               +SensorLoaderModule.TO_PLACE_PRODUCT_OFFSET
//                                                               +SensorLoaderModule.TRAY_EMPTY_PR
//                                                               +SensorLoaderModule.SENSOR_TRAY_2_POS)
                            logicManager.performHandling(sensorLoaderParameter.moduleName,SensorLoaderModule.PLACE_PRODUCT_TO_TRAY
                                                         +SensorLoaderModule.TO_PLACE_PRODUCT_OFFSET
                                                         +SensorLoaderModule.TRAY_EMPTY_PR
                                                         +SensorLoaderModule.SENSOR_TRAY_2_POS)
                        }
                    }
                }
                Button{
                    visible: baseModuleManager.getServerMode()!==0
                    text:qsTr("放ng Sensor")
                    width:40
                    height: 40
                    onClicked: {
                        material_tray.setTrayCurrent(t_ncol.text-1,t_nrow.text-1,2)
                        //6232
//                        sensorLoaderModule.performHandling(SensorLoaderModule.PLACE_NG_SENSOR_TO_TRAY
//                                                           +SensorLoaderModule.TO_PLACE_PRODUCT_OFFSET
//                                                           +SensorLoaderModule.TRAY_EMPTY_PR
//                                                           +SensorLoaderModule.SENSOR_TRAY_2_POS)
                        logicManager.performHandling(sensorLoaderParameter.moduleName,SensorLoaderModule.PLACE_NG_SENSOR_TO_TRAY
                                                     +SensorLoaderModule.TO_PLACE_PRODUCT_OFFSET
                                                     +SensorLoaderModule.TRAY_EMPTY_PR
                                                     +SensorLoaderModule.SENSOR_TRAY_2_POS)
                    }
                }
            }
        }
    }
}
