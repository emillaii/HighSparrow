import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11
import SomeLib 1.1
ColumnLayout {
    GroupBox{
        title:qsTr("料盘起点")
        ColumnLayout{
            RowLayout{
                Label{
                    text: qsTr("X")
                }
                TextField{
                    text: tray_start_point3.X
                    horizontalAlignment: TextInput.AlignHCenter
                    validator: DoubleValidator {
                        decimals: 6
                        notation: DoubleValidator.StandardNotation
                    }
                    onEditingFinished: {
                        tray_start_point3.setX(text)
                    }
                }
                Label{
                    text: qsTr("Y")
                }
                TextField{
                    text: tray_start_point3.Y
                    horizontalAlignment: TextInput.AlignHCenter
                    validator: DoubleValidator {
                        decimals: 6
                        notation: DoubleValidator.StandardNotation
                    }
                    onEditingFinished: {
                        tray_start_point3.setY(text)
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
//                            sensorLoaderModule.performHandling(SensorLoaderModule.NG_SENSOR_TRAY_START_POS)
                            logicManager.performHandling(sensorLoaderParameter.moduleName,SensorLoaderModule.NG_SENSOR_TRAY_START_POS)
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
                            tray_start_point3.setX(x + lens_loader_parameter.visonPositionX - x1);
                            tray_start_point3.setY(y);
                        }else{

                            x = baseModuleManager.getMotorFeedbackPos(sensorPickArmParams.motorXName)
                            y = baseModuleManager.getMotorFeedbackPos(sensorPickArmParams.motorYName)
                            tray_start_point3.setX(x);
                            tray_start_point3.setY(y);
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
                Button{
                    text: qsTr("换Ng盘")
                    width: 40
                    height: 40
                    onClicked:{
                        sensorLoaderModule.changeNgTray()
                    }
                }
            }
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
                        material_tray.setTrayCurrent(t_ncol.text-1,t_nrow.text-1,2)
//                        sensorLoaderModule.performHandling(SensorLoaderModule.NG_SENSOR_TRAY_POS)
                        logicManager.performHandling(sensorLoaderParameter.moduleName,SensorLoaderModule.NG_SENSOR_TRAY_POS)
                    }
                }
            }


            RowLayout{
                CheckBox{
                    text: qsTr("不限力")
                    checked: sensorLoaderParameter.disablePlaceToNgTrayForceLimit
                    onClicked: {
                        sensorLoaderParameter.setDisablePlaceToNgTrayForceLimit(checked)
                    }
                }

                Label{
                    text:qsTr("放料区间")
                }
                TextField{
                    text:sensorLoaderParameter.placeToNgTrayMargin
                    horizontalAlignment: TextInput.AlignHCenter
                    validator: DoubleValidator{
                        decimals: 6
                        notation: DoubleValidator.StandardNotation
                    }
                    onEditingFinished: {
                        sensorLoaderParameter.setPlaceToNgTrayMargin(text)
                    }
                }
            }
            RowLayout{
                visible: baseModuleManager.getServerMode() !==0

                Label{
                    text:qsTr("Ng成品高度")
                }
                TextField{
                    text: sensorLoaderParameter.placeNgProductZ
                    horizontalAlignment: TextInput.AlignHCenter
                    validator: DoubleValidator {
                        decimals: 6
                        notation: DoubleValidator.StandardNotation
                    }
                    onEditingFinished: {
                        sensorLoaderParameter.setPlaceNgProductZ(text)
                    }
                }
                Button{
                    text:qsTr("测高")
                    onClicked: {
//                        sensorLoaderModule.performHandling(SensorLoaderModule.MEASURE_NG_PRODUCT_IN_TRAY)
                        logicManager.performHandling(sensorLoaderParameter.moduleName,SensorLoaderModule.MEASURE_NG_PRODUCT_IN_TRAY)
                    }
                }
            }
            RowLayout{
                visible: baseModuleManager.getServerMode() !==0
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
//                        sensorLoaderModule.performHandling(SensorLoaderModule.MEASURE_NG_SENSOR_IN_TRAY)
                        logicManager.performHandling(sensorLoaderParameter.moduleName,SensorLoaderModule.MEASURE_NG_SENSOR_IN_TRAY)
                    }
                }
            }
            RowLayout{
                Layout.alignment: Qt.AlignVCenter|Qt.AlignRight

                Button{
                    text:qsTr("空位视觉")
                    width:40
                    height: 40
                    onClicked:{
                        material_tray.setTrayCurrent(t_ncol.text-1,t_nrow.text-1,2)
//                        sensorLoaderModule.performHandling(SensorLoaderModule.NG_SENSOR_TRAY_POS
//                                                           +SensorLoaderModule.TRAY_EMPTY_PR
//                                                           +SensorLoaderModule.TO_PR_OFFSET)
                        logicManager.performHandling(sensorLoaderParameter.moduleName,SensorLoaderModule.NG_SENSOR_TRAY_POS
                                                     +SensorLoaderModule.TRAY_EMPTY_PR
                                                     +SensorLoaderModule.TO_PR_OFFSET)
                    }
                }

                Button{
                    text:qsTr("移动吸头")
                    width:40
                    height: 40
                    onClicked: {
                        material_tray.setTrayCurrent(t_ncol.text-1,t_nrow.text-1,2)
//                        sensorLoaderModule.performHandling(SensorLoaderModule.NG_SENSOR_TRAY_POS
//                                                           +SensorLoaderModule.TRAY_EMPTY_PR
//                                                           +SensorLoaderModule.TO_PLACE_NG_POS)
                        logicManager.performHandling(sensorLoaderParameter.moduleName,SensorLoaderModule.NG_SENSOR_TRAY_POS
                                                     +SensorLoaderModule.TRAY_EMPTY_PR
                                                     +SensorLoaderModule.TO_PLACE_NG_POS)
                    }
                }

                CheckBox{
                    text: qsTr("使能视觉")
                    checked: sensorLoaderParameter.enablePlaceNgSensorPr
                    onCheckedChanged:
                        sensorLoaderParameter.setEnablePlaceNgSensorPr(checked)
                }
                Button{
                    text:qsTr("放ng")
                    width:40
                    height: 40
                    onClicked: {
                        material_tray.setTrayCurrent(t_ncol.text-1,t_nrow.text-1,2)
//                        sensorLoaderModule.performHandling(SensorLoaderModule.NG_SENSOR_TRAY_POS
//                                                           +SensorLoaderModule.TRAY_EMPTY_PR
//                                                           +SensorLoaderModule.TO_PLACE_NG_POS
//                                                           +SensorLoaderModule.PLACE_NG_SENSOR_TO_TRAY)
                        logicManager.performHandling(sensorLoaderParameter.moduleName,SensorLoaderModule.NG_SENSOR_TRAY_POS
                                                     +SensorLoaderModule.TRAY_EMPTY_PR
                                                     +SensorLoaderModule.TO_PLACE_NG_POS
                                                     +SensorLoaderModule.PLACE_NG_SENSOR_TO_TRAY)
                    }
                }

                CheckBox{
                    text: qsTr("使能视觉")
                    checked: sensorLoaderParameter.enablePlaceNgProductPr
                    onCheckedChanged:
                        sensorLoaderParameter.setEnableNgProductPr(checked)
                }
                Button{
                    text:qsTr("放Ng成品")
                    width: 40
                    height: 40
                    onClicked: {
                        material_tray.setTrayCurrent(t_ncol.text-1,t_nrow.text-1,2)
//                        sensorLoaderModule.performHandling(SensorLoaderModule.NG_SENSOR_TRAY_POS
//                                                           +SensorLoaderModule.TRAY_EMPTY_PR
//                                                           +SensorLoaderModule.TO_PLACE_NG_POS
//                                                           +SensorLoaderModule.PLACE_NG_PRODUCT_TO_TRAY)
                        logicManager.performHandling(sensorLoaderParameter.moduleName,SensorLoaderModule.NG_SENSOR_TRAY_POS
                                                     +SensorLoaderModule.TRAY_EMPTY_PR
                                                     +SensorLoaderModule.TO_PLACE_NG_POS
                                                     +SensorLoaderModule.PLACE_NG_PRODUCT_TO_TRAY)
                    }
                }
            }

            RowLayout{
                Label{
                    text:qsTr("放料offsetX")
                }
                TextField{
                    text:ngTrayPlaceOffset.X
                    horizontalAlignment: TextInput.AlignHCenter
                    validator: DoubleValidator{
                        decimals: 6
                        notation: DoubleValidator.StandardNotation
                    }
                    onEditingFinished: {
                        ngTrayPlaceOffset.setX(text)
                    }
                }
                Label{
                    text:qsTr("放料offsetY")
                }
                TextField{
                    text:ngTrayPlaceOffset.Y
                    horizontalAlignment: TextInput.AlignHCenter
                    validator: DoubleValidator{
                        decimals: 6
                        notation: DoubleValidator.StandardNotation
                    }
                    onEditingFinished: {
                        ngTrayPlaceOffset.setY(text)
                    }
                }
            }
        }
    }
}
