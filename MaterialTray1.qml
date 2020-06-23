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
                            //logicManager.lensPickArmMoveToTray1StartPos()
                            //lensLoaderModule.performHandling(LensLoaderModule.LENS_TRAY1_START_POS)
                            logicManager.performHandling(lens_loader_parameter.moduleName,LensLoaderModule.LENS_TRAY1_START_POS)
                        }else{
                            //sensorLoaderModule.performHandling(SensorLoaderModule.SENSOR_TRAY_1_START_POS)
                            logicManager.performHandling(sensorLoaderParameter.moduleName,SensorLoaderModule.SENSOR_TRAY_1_START_POS)
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
                            tray_start_point1.setX(x + lens_loader_parameter.visonPositionX - x1);
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
                            //logicManager.lensPickArmMoveToTray1EndPos()
                            //lensLoaderModule.performHandling(LensLoaderModule.LENS_TRAY1_END_POS)
                            logicManager.performHandling(lens_loader_parameter.moduleName,LensLoaderModule.LENS_TRAY1_END_POS)
                        }else{
                            //sensorLoaderModule.performHandling(SensorLoaderModule.SENSOR_TRAY_1_END_POS)
                            logicManager.performHandling(sensorLoaderParameter.moduleName,SensorLoaderModule.SENSOR_TRAY_1_END_POS)
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
                            first_tray_end_position.setX(x + lens_loader_parameter.visonPositionX - x1);
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
                    text:qsTr("下次取料位置")
                    width: 40
                    height: 40
                    onClicked: {
                        material_tray.setTrayFirst(t_ncol.text-1,t_nrow.text-1,0)
                    }
                }
                Button{
                    text:title_move_to
                    width: 40
                    height: 40
                    onClicked: {
                        material_tray.setTrayCurrent(t_ncol.text-1,t_nrow.text-1,0)

                        if (baseModuleManager.getServerMode() === 0){
//                            lensLoaderModule.performHandling(LensLoaderModule.LENS_TRAY1)
                            logicManager.performHandling(lens_loader_parameter.moduleName,LensLoaderModule.LENS_TRAY1)
                        }else{
//                            sensorLoaderModule.performHandling(SensorLoaderModule.SENSOR_TRAY_1_POS)
                            logicManager.performHandling(sensorLoaderParameter.moduleName,SensorLoaderModule.SENSOR_TRAY_1_POS)
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
                    text: sensorTray1PickOffset.X
                    horizontalAlignment: TextInput.AlignHCenter
                    validator: DoubleValidator{
                        decimals: 6
                        notation: DoubleValidator.StandardNotation
                    }
                    onEditingFinished: {
                        sensorTray1PickOffset.setX(text)
                    }
                }

                Label{
                    text: qsTr("Y")
                }
                TextField{
                    text: sensorTray1PickOffset.Y
                    horizontalAlignment: TextInput.AlignHCenter
                    validator: DoubleValidator{
                        decimals: 6
                        notation: DoubleValidator.StandardNotation
                    }
                    onEditingFinished: {
                        sensorTray1PickOffset.setY(text)
                    }
                }
                Button{
                    text:qsTr("视觉校正")
                    width:40
                    height:40
                    onClicked: {
                        material_tray.setTrayCurrent(t_ncol.text-1,t_nrow.text-1,0)
//                        sensorLoaderModule.performHandling(SensorLoaderModule.SENSOR_TRAY_1_POS
//                                                           +SensorLoaderModule.TRAY_SENSOR_OFFSET)
                        logicManager.performHandling(sensorLoaderParameter.moduleName,SensorLoaderModule.SENSOR_TRAY_1_POS
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
                        material_tray.setTrayCurrent(t_ncol.text-1,t_nrow.text-1,0)
//                        sensorLoaderModule.performHandling(SensorLoaderModule.SENSOR_TRAY_1_POS
//                                                           +SensorLoaderModule.TRAY_SENSOR_PR
//                                                           +SensorLoaderModule.TO_PR_OFFSET)
                        logicManager.performHandling(sensorLoaderParameter.moduleName,SensorLoaderModule.SENSOR_TRAY_1_POS
                                                     +SensorLoaderModule.TRAY_SENSOR_PR
                                                     +SensorLoaderModule.TO_PR_OFFSET)
                    }
                }

                Label{
                    text: qsTr("sensor高度")
                }
                TextField{
                    text: sensorLoaderParameter.pickSensorZ
                    horizontalAlignment: TextInput.AlignHCenter
                    validator: DoubleValidator{
                        decimals: 6
                        notation: DoubleValidator.StandardNotation
                    }
                    onEditingFinished: {
                        sensorLoaderParameter.setPickSensorZ(text)
                    }
                }
                Button{
                    text:qsTr("移动吸头")
                    width: 40
                    height: 40
                    onClicked: {
                        material_tray.setTrayCurrent(t_ncol.text-1,t_nrow.text-1,0)
//                        sensorLoaderModule.performHandling(SensorLoaderModule.SENSOR_TRAY_1_POS
//                                                           +SensorLoaderModule.TRAY_SENSOR_PR
//                                                           +SensorLoaderModule.TO_PICK_SENSOR_POS1)
                        logicManager.performHandling(sensorLoaderParameter.moduleName,SensorLoaderModule.SENSOR_TRAY_1_POS
                                                     +SensorLoaderModule.TRAY_SENSOR_PR
                                                     +SensorLoaderModule.TO_PICK_SENSOR_POS1)
                    }
                }

                Button{
                    text:qsTr("测高")
                    width: 40
                    height: 40
                    onClicked: {
//                        sensorLoaderModule.performHandling(SensorLoaderModule.MEASURE_SENSOR_IN_TRAY1)
                        logicManager.performHandling(sensorLoaderParameter.moduleName,SensorLoaderModule.MEASURE_SENSOR_IN_TRAY1)
                    }
                }

                Button{
                    text:qsTr("取sensor")
                    width: 40
                    height: 40
                    onClicked: {
                        material_tray.setTrayCurrent(t_ncol.text-1,t_nrow.text-1,0)
//                        sensorLoaderModule.performHandling(SensorLoaderModule.SENSOR_TRAY_1_POS
//                                                           +SensorLoaderModule.TRAY_SENSOR_PR
//                                                           +SensorLoaderModule.TO_PICK_SENSOR_POS1
//                                                           +SensorLoaderModule.PICK_SENSOR_FROM_TRAY1)
                        logicManager.performHandling(sensorLoaderParameter.moduleName,SensorLoaderModule.SENSOR_TRAY_1_POS
                                                     +SensorLoaderModule.TRAY_SENSOR_PR
                                                     +SensorLoaderModule.TO_PICK_SENSOR_POS1
                                                     +SensorLoaderModule.PICK_SENSOR_FROM_TRAY1)
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
                        material_tray.setTrayCurrent(t_ncol.text-1,t_nrow.text-1,0)
//                        sensorLoaderModule.performHandling(SensorLoaderModule.SENSOR_TRAY_1_POS
//                                                           +SensorLoaderModule.TRAY_EMPTY_PR
//                                                           +SensorLoaderModule.TO_PR_OFFSET)
                        logicManager.performHandling(sensorLoaderParameter.moduleName,SensorLoaderModule.SENSOR_TRAY_1_POS
                                                     +SensorLoaderModule.TRAY_EMPTY_PR
                                                     +SensorLoaderModule.TO_PR_OFFSET)
                    }
                }

                Label{
                    text: qsTr("成品  高度")
                }
                TextField{
                    text: sensorLoaderParameter.placeProductZ
                    horizontalAlignment: TextInput.AlignHCenter
                    validator: DoubleValidator{
                        decimals: 6
                        notation: DoubleValidator.StandardNotation
                    }
                    onEditingFinished: {
                        sensorLoaderParameter.setPlaceProductZ(text)
                    }
                }

                Button{
                    text:qsTr("移动吸头")
                    width: 40
                    height: 40
                    onClicked: {
                        material_tray.setTrayCurrent(t_ncol.text-1,t_nrow.text-1,0)
//                        sensorLoaderModule.performHandling(SensorLoaderModule.SENSOR_TRAY_1_POS
//                                                           +SensorLoaderModule.TRAY_EMPTY_PR
//                                                           +SensorLoaderModule.TO_PLACE_PRODUCT_POS1)
                        logicManager.performHandling(sensorLoaderParameter.moduleName,SensorLoaderModule.SENSOR_TRAY_1_POS
                                                     +SensorLoaderModule.TRAY_EMPTY_PR
                                                     +SensorLoaderModule.TO_PLACE_PRODUCT_POS1)
                    }
                }

                Button{
                    text:qsTr("测高")
                    width: 40
                    height: 40
                    onClicked: {
//                        sensorLoaderModule.performHandling(SensorLoaderModule.MEASURE_PRODUCT_IN_TRAY1)
                        logicManager.performHandling(sensorLoaderParameter.moduleName,SensorLoaderModule.MEASURE_PRODUCT_IN_TRAY1)
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
                        material_tray.setTrayCurrent(t_ncol.text-1,t_nrow.text-1,0)
                        sensorLoaderModule.performHandling(SensorLoaderModule.SENSOR_TRAY_1_POS
                                                           +SensorLoaderModule.TRAY_EMPTY_PR
                                                           +SensorLoaderModule.TO_PICK_SENSOR_POS1
                                                           +SensorLoaderModule.PLACE_SENSOR_BACK_TO_TRAY1)
                    }
                }
                Button{
                    text:qsTr("退NG sensor")
                    width:40
                    height: 40
                    onClicked: {
                        material_tray.setTrayCurrent(t_ncol.text-1,t_nrow.text-1,0)
                        sensorLoaderModule.performHandling(SensorLoaderModule.SENSOR_TRAY_1_POS
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
                        material_tray.setTrayCurrent(t_ncol.text-1,t_nrow.text-1,0)
                        logicManager.performHandling(sensorLoaderParameter.moduleName,SensorLoaderModule.SENSOR_TRAY_1_POS
                                                     +SensorLoaderModule.TRAY_EMPTY_PR
                                                     +SensorLoaderModule.TO_PLACE_PRODUCT_POS1
                                                     +SensorLoaderModule.PLACE_PRODUCT_TO_TRAY1)
                    }
                }
            }

            RowLayout{
                visible: baseModuleManager.getServerMode()===1
                Label{
                    text:qsTr("放料offsetX")
                }
                TextField{
                    text:sensorTray1PlaceOffset.X
                    horizontalAlignment: TextInput.AlignHCenter
                    validator: DoubleValidator{
                        decimals: 6
                        notation: DoubleValidator.StandardNotation
                    }
                    onEditingFinished: {
                        sensorTray1PlaceOffset.setX(text)
                    }
                }
                Label{
                    text:qsTr("放料offsetY")
                }
                TextField{
                    text:sensorTray1PlaceOffset.Y
                    horizontalAlignment: TextInput.AlignHCenter
                    validator: DoubleValidator{
                        decimals: 6
                        notation: DoubleValidator.StandardNotation
                    }
                    onEditingFinished: {
                        sensorTray1PlaceOffset.setY(text)
                    }
                }
            }

            RowLayout{
                visible: baseModuleManager.getServerMode()===0
                Label{
                    text: baseModuleManager.getServerMode()===0?qsTr("物料高度"):qsTr("sensor高度")
                }
                TextField{
                    text: baseModuleManager.getServerMode()===0 ?lens_loader_parameter.pickLensZ:sensorLoaderParameter.pickSensorZ
                    horizontalAlignment: TextInput.AlignHCenter
                    validator: DoubleValidator{
                        decimals: 6
                        notation: DoubleValidator.StandardNotation
                    }
                    onEditingFinished: {
                        if (baseModuleManager.getServerMode()===0)
                            lens_loader_parameter.setPickLensZ(text)
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
                            //logicManager.lensPickArmLensMeasureHeight()
//                            lensLoaderModule.performHandling(LensLoaderModule.MeasureLensInTray)
                            logicManager.performHandling(lens_loader_parameter.moduleName,LensLoaderModule.MeasureLensInTray)
                        }else{
//                            sensorLoaderModule.performHandling(SensorLoaderModule.MEASURE_SENSOR_IN_TRAY1)
                            logicManager.performHandling(sensorLoaderParameter.moduleName,SensorLoaderModule.MEASURE_SENSOR_IN_TRAY1)
                        }
                    }
                }
                Button{
                    text:qsTr("执行视觉")
                    width:40
                    height:40
                    onClicked: {
                        if(baseModuleManager.getServerMode()===0){
                            //logicManager.performLocation(lens_loader_parameter.lensLocationName);
                            logicManagerState.setCurrentLocationName(lens_loader_parameter.lensLocationName)
                            logicManagerState.setUseOriginPr(true)
                            logicManager.performHandling(LogicManager.PERFORM_LOCATION)
                        }else{
//                            sensorLoaderModule.performHandling(SensorLoaderModule.TO_PR_OFFSET
//                                                               + SensorLoaderModule.TRAY_SENSOR_PR
//                                                               + SensorLoaderModule.SENSOR_TRAY_1_POS)
                            logicManager.performHandling(sensorLoaderParameter.moduleName,SensorLoaderModule.TO_PR_OFFSET
                                                         + SensorLoaderModule.TRAY_SENSOR_PR
                                                         + SensorLoaderModule.SENSOR_TRAY_1_POS)
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
                            //logicManager.lensPickArmLensPR()
//                            lensLoaderModule.performHandling(LensLoaderModule.LENS_PR+
//                                                             LensLoaderModule.ToWork)
                            logicManager.performHandling(lens_loader_parameter.moduleName,LensLoaderModule.LENS_PR+
                                                         LensLoaderModule.ToWork)
                        }else{
//                            sensorLoaderModule.performHandling(SensorLoaderModule.TO_PICK_SENSOR_OFFSET
//                                                               + SensorLoaderModule.TRAY_SENSOR_PR
//                                                               + SensorLoaderModule.SENSOR_TRAY_1_POS)
                            logicManager.performHandling(sensorLoaderParameter.moduleName,SensorLoaderModule.TO_PICK_SENSOR_OFFSET
                                                         + SensorLoaderModule.TRAY_SENSOR_PR
                                                         + SensorLoaderModule.SENSOR_TRAY_1_POS)
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
                            //logicManager.lensPickArmMoveToPickLensFromTray1()
//                            lensLoaderModule.performHandling(LensLoaderModule.LENS_TRAY1+
//                                                             LensLoaderModule.LENS_PR+
//                                                             LensLoaderModule.ToWork+
//                                                             LensLoaderModule.PICK_LENS_FROM_TRAY)
                            logicManager.performHandling(lens_loader_parameter.moduleName,LensLoaderModule.LENS_TRAY1+
                                                         LensLoaderModule.LENS_PR+
                                                         LensLoaderModule.ToWork+
                                                         LensLoaderModule.PICK_LENS_FROM_TRAY)
                        }else{
//                            sensorLoaderModule.performHandling(SensorLoaderModule.PICK_SENSOR_FROM_TRAY
//                                                               +SensorLoaderModule.TO_PICK_SENSOR_OFFSET
//                                                               +SensorLoaderModule.TRAY_SENSOR_PR
//                                                               +SensorLoaderModule.SENSOR_TRAY_1_POS)
                            logicManager.performHandling(sensorLoaderParameter.moduleName,SensorLoaderModule.PICK_SENSOR_FROM_TRAY
                                                         +SensorLoaderModule.TO_PICK_SENSOR_OFFSET
                                                         +SensorLoaderModule.TRAY_SENSOR_PR
                                                         +SensorLoaderModule.SENSOR_TRAY_1_POS)
                        }
                    }
                }
            }




            RowLayout{
                visible: baseModuleManager.getServerMode()===0
                Layout.alignment: Qt.AlignVCenter|Qt.AlignRight
                Button{
                    text:qsTr("空位视觉")
                    width:40
                    height:40
                    onClicked: {
                        //logicManager.performLocation(lens_loader_parameter.vacancyLocationName)
                        logicManagerState.setCurrentLocationName(lens_loader_parameter.vacancyLocationName)
                        logicManagerState.setUseOriginPr(true)
                        logicManager.performHandling(LogicManager.PERFORM_LOCATION)
                    }
                }

                Button{
                    text:qsTr("移动吸头")
                    width: 40
                    height: 40
                    onClicked: {
                        //logicManager.lensPickArmVacancyTrayPR()
//                        lensLoaderModule.performHandling(LensLoaderModule.VACANCY_PR+
//                                                         LensLoaderModule.ToWork)
                        logicManager.performHandling(lens_loader_parameter.moduleName,LensLoaderModule.VACANCY_PR+
                                                     LensLoaderModule.ToWork)
                    }
                }
                Button{
                    text:qsTr("放lens")
                    width: 40
                    height: 40
                    onClicked: {
                        material_tray.setTrayCurrent(t_ncol.text-1,t_nrow.text-1,0)
                        //logicManager.lensPickArmMoveToPlaceLensToTray1()
//                        lensLoaderModule.performHandling(LensLoaderModule.LENS_TRAY1+
//                                                         LensLoaderModule.VACANCY_PR+
//                                                         LensLoaderModule.ToWork+
//                                                         LensLoaderModule.PLACE_NG_LENS_TO_TRAY)
                        logicManager.performHandling(lens_loader_parameter.moduleName,LensLoaderModule.LENS_TRAY1+
                                                     LensLoaderModule.VACANCY_PR+
                                                     LensLoaderModule.ToWork+
                                                     LensLoaderModule.PLACE_NG_LENS_TO_TRAY)
                    }
                }
            }
        }
    }
}
