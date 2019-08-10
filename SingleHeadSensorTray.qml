﻿import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11
import SingleHeadMaterialLoaderLib 1.0

ColumnLayout {
    GroupBox{
        title:qsTr("sensor盘起点")
        ColumnLayout{
            RowLayout{
                Label{
                    text: qsTr("X")
                }
                TextField{
                    text: sh_sensor_tray_start_point1.X
                    horizontalAlignment: TextInput.AlignHCenter
                    validator: DoubleValidator {
                        decimals: 6
                        notation: DoubleValidator.StandardNotation
                    }
                    onEditingFinished: {
                        sh_sensor_tray_start_point1.setX(text)
                    }
                }
                Label{
                    text: qsTr("Y")
                }
                TextField{
                    text: sh_sensor_tray_start_point1.Y
                    horizontalAlignment: TextInput.AlignHCenter
                    validator: DoubleValidator {
                        decimals: 6
                        notation: DoubleValidator.StandardNotation
                    }
                    onEditingFinished: {
                        sh_sensor_tray_start_point1.setY(text)
                    }
                }
                Button{
                    text:title_move_to
                    width: 40
                    height: 40
                    onClicked: {
                        sh_materialLoaderModule.performHandling(MaterialLoaderModule.SENSOR_TRAY_START_POS)
                    }
                }
                Button{
                    text:title_read_encoder
                    width: 40
                    height: 40
                    onClicked: {
                        var x = baseModuleManager.getMotorFeedbackPos(materialPickArmParams.motorXName)
                        var y = baseModuleManager.getMotorFeedbackPos(materialPickArmParams.motorYName)
                        sh_sensor_tray_start_point1.setX(x);
                        sh_sensor_tray_start_point1.setY(y);
                    }
                }
            }
        }
    }
    GroupBox{
        title:qsTr("sensor盘终点")
        ColumnLayout{
            RowLayout{
                Label{
                    text: qsTr("X")
                }
                TextField{
                    text: sh_sensor_first_tray_end_position.X
                    horizontalAlignment: TextInput.AlignHCenter
                    validator: DoubleValidator {
                        decimals: 6
                        notation: DoubleValidator.StandardNotation
                    }
                    onEditingFinished: {
                        sh_sensor_first_tray_end_position.setX(text)
                    }
                }
                Label{
                    text: qsTr("Y")
                }
                TextField{
                    text: sh_sensor_first_tray_end_position.Y
                    horizontalAlignment: TextInput.AlignHCenter
                    validator: DoubleValidator {
                        decimals: 6
                        notation: DoubleValidator.StandardNotation
                    }
                    onEditingFinished: {
                        sh_sensor_first_tray_end_position.setY(text)
                    }
                }
                Button{
                    text:title_move_to
                    width: 40
                    height: 40
                    onClicked: {
                        sh_materialLoaderModule.performHandling(MaterialLoaderModule.SENSOR_TRAY_END_POS)
                    }
                }
                Button{
                    text:title_read_encoder
                    width: 40
                    height: 40

                    onClicked: {
                        var x = baseModuleManager.getMotorFeedbackPos(materialPickArmParams.motorXName)
                        var y = baseModuleManager.getMotorFeedbackPos(materialPickArmParams.motorYName)
                        sh_sensor_first_tray_end_position.setX(x);
                        sh_sensor_first_tray_end_position.setY(y);
                    }
                }
            }
        }

    }
    GroupBox{
        title:qsTr("sensor盘操作picker2")
        ColumnLayout{
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
                            sh_sensor_tray.setTrayCurrent(t_ncol.text-1,t_nrow.text-1,0)
                            sh_materialLoaderModule.performHandling(MaterialLoaderModule.SENSOR_TRAY)
                        }
                    }
                }

                RowLayout{
                    Label{
                        text: qsTr("sensor高度")
                    }
                    TextField{
                        text: materialPickArmParams.pickSensorZ
                        horizontalAlignment: TextInput.AlignHCenter
                        validator: DoubleValidator{
                            decimals: 6
                            notation: DoubleValidator.StandardNotation
                        }
                        onEditingFinished: {
                            materialPickArmParams.setPickSensorZ(text)
                        }
                    }

                    Button{
                        text:qsTr("测高")
                        width: 40
                        height: 40
                        onClicked: {
                            sh_materialLoaderModule.performHandling(MaterialLoaderModule.PICKER2_MEASURE_SENSOR_IN_TRAY)
                        }
                    }
//                    Button{
//                        text:qsTr("执行视觉")
//                        width:40
//                        height:40
//                        onClicked: {
//                            sh_materialLoaderModule.performHandling(MaterialLoaderModule.SENSOR_PR)
//                        }
//                    }

//                    Button{
//                        text:qsTr("移动吸头")
//                        width: 40
//                        height: 40
//                        onClicked: {
//                            sh_materialLoaderModule.performHandling(
//                                                                    MaterialLoaderModule.PICKER2_TO_WORKPOS)
//                        }
//                    }

                    Button{
                         text:qsTr("picker2取Sensor")
                        width: 40
                        height: 40
                        onClicked: {
                            sh_sensor_tray.setTrayCurrent(t_ncol.text-1,t_nrow.text-1,0)
                            sh_materialLoaderModule.performHandling(MaterialLoaderModule.SENSOR_TRAY|
                                                                    MaterialLoaderModule.SENSOR_PR|
                                                                    MaterialLoaderModule.PICKER2_TO_WORKPOS|
                                                                    MaterialLoaderModule.PICKER2_PICK_SENSOR_FROM_TRAY)
                        }
                    }
                }
                RowLayout{
                    Label{
                        text:qsTr("Ng Sensor高度")
                    }
                    TextField{
                        text: materialPickArmParams.placeNgSensorZ
                        horizontalAlignment: TextInput.AlignHCenter
                        validator: DoubleValidator {
                            decimals: 6
                            notation: DoubleValidator.StandardNotation
                        }
                        onEditingFinished: {
                            materialPickArmParams.setPlaceNgSensorZ(text)
                        }
                    }
                    Button{
                        text:qsTr("测高")
                        onClicked: {
                            sh_materialLoaderModule.performHandling(MaterialLoaderModule.PICKER2_MEASURE_NG_SENSOR_IN_TRAY)
                        }
                    }
                }
                RowLayout{
                    Layout.alignment: Qt.AlignVCenter|Qt.AlignRight
//                    Button{
//                        text:qsTr("空位视觉")
//                        width:40
//                        height: 40
//                        onClicked:{
//                            sh_materialLoaderModule.performHandling(MaterialLoaderModule.SENSOR_VACANCY_PR)
//                        }
//                    }

//                    Button{
//                        text:qsTr("移动吸头")
//                        width:40
//                        height: 40
//                        onClicked: {
//                            sh_sensor_tray.setTrayCurrent(t_ncol.text-1,t_nrow.text-1,1)
//                            sh_materialLoaderModule.performHandling(MaterialLoaderModule.PICKER2_TO_WORKPOS)

//                        }
//                    }

                    Button{
                        text:qsTr("放成品")
                        width: 40
                        height: 40
                        onClicked: {
                            sh_sensor_tray.setTrayCurrent(t_ncol.text-1,t_nrow.text-1,1)
                            sh_materialLoaderModule.performHandling(MaterialLoaderModule.SENSOR_TRAY|
                                                                    MaterialLoaderModule.SENSOR_VACANCY_PR|
                                                                    MaterialLoaderModule.PICKER2_TO_WORKPOS|
                                                                    MaterialLoaderModule.PLACE_PRODUCT_TO_TRAY)
                        }
                    }
//                    Button{
//                        text:qsTr("picker2放ng Sensor")
//                        width:40
//                        height: 40
//                        onClicked: {
//                            sh_sensor_tray.setTrayCurrent(t_ncol.text-1,t_nrow.text-1,1)
//                            sh_materialLoaderModule.performHandling(MaterialLoaderModule.SENSOR_TRAY|
//                                                                    MaterialLoaderModule.SENSOR_VACANCY_PR|
//                                                                    MaterialLoaderModule.PICKER2_TO_WORKPOS|
//                                                                    MaterialLoaderModule.PICKER2_PLACE_NG_SENSOR_TO_TRAY)
//                        }
//                    }
                }
            }
        }
    }
    GroupBox{
        title:qsTr("sensor盘终点")
        ColumnLayout{
            RowLayout{
                Label{
                    text: qsTr("X")
                }
                TextField{
                    text: sh_sensor_first_tray_end_position.X
                    horizontalAlignment: TextInput.AlignHCenter
                    validator: DoubleValidator {
                        decimals: 6
                        notation: DoubleValidator.StandardNotation
                    }
                    onEditingFinished: {
                        sh_sensor_first_tray_end_position.setX(text)
                    }
                }
                Label{
                    text: qsTr("Y")
                }
                TextField{
                    text: sh_sensor_first_tray_end_position.Y
                    horizontalAlignment: TextInput.AlignHCenter
                    validator: DoubleValidator {
                        decimals: 6
                        notation: DoubleValidator.StandardNotation
                    }
                    onEditingFinished: {
                        sh_sensor_first_tray_end_position.setY(text)
                    }
                }
                Button{
                    text:title_move_to
                    width: 40
                    height: 40
                    onClicked: {
                        sh_materialLoaderModule.performHandling(MaterialLoaderModule.SENSOR_TRAY_END_POS)
                    }
                }
                Button{
                    text:title_read_encoder
                    width: 40
                    height: 40

                    onClicked: {
                        var x = baseModuleManager.getMotorFeedbackPos(materialPickArmParams.motorXName)
                        var y = baseModuleManager.getMotorFeedbackPos(materialPickArmParams.motorYName)
                        sh_sensor_first_tray_end_position.setX(x);
                        sh_sensor_first_tray_end_position.setY(y);
                    }
                }
            }
        }

    }
    GroupBox{
        title:qsTr("sensor盘操作picker1")
        ColumnLayout{
            ColumnLayout{
                RowLayout{
                    Label{
                        text: qsTr("目标行")
                    }
                    TextField{
                        id:t_nrow1
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
                        id:t_ncol1
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
                            sh_sensor_tray.setTrayCurrent(t_ncol1.text-1,t_nrow1.text-1,0)
                            sh_materialLoaderModule.performHandling(MaterialLoaderModule.SENSOR_TRAY)
                        }
                    }
                }

                RowLayout{
                    Label{
                        text: qsTr("sensor高度")
                    }
                    TextField{
                        text: materialPickArmParams.pickSensorZ
                        horizontalAlignment: TextInput.AlignHCenter
                        validator: DoubleValidator{
                            decimals: 6
                            notation: DoubleValidator.StandardNotation
                        }
                        onEditingFinished: {
                            materialPickArmParams.setPickSensorZ(text)
                        }
                    }

                    Button{
                        text:qsTr("测高")
                        width: 40
                        height: 40
                        onClicked: {
                            sh_materialLoaderModule.performHandling(MaterialLoaderModule.PICKER2_MEASURE_SENSOR_IN_TRAY)
                        }
                    }
//                    Button{
//                        text:qsTr("执行视觉")
//                        width:40
//                        height:40
//                        onClicked: {
//                            sh_materialLoaderModule.performHandling(
//                                        MaterialLoaderModule.SENSOR_TRAY|
//                                        MaterialLoaderModule.SENSOR_PR)
//                        }
//                    }

//                    Button{
//                        text:qsTr("移动吸头")
//                        width: 40
//                        height: 40
//                        onClicked: {
//                            sh_materialLoaderModule.performHandling(
//                                                                    MaterialLoaderModule.PICKER1_TO_WORKPOS)
//                        }
//                    }

                    Button{
                        text:qsTr("picker1取Sensor")
                        width: 40
                        height: 40
                        onClicked: {
                            sh_sensor_tray.setTrayCurrent(t_ncol1.text-1,t_nrow1.text-1,0)
                            sh_materialLoaderModule.performHandling(MaterialLoaderModule.SENSOR_TRAY|
                                                                    MaterialLoaderModule.SENSOR_PR|
                                                                    MaterialLoaderModule.PICKER1_TO_WORKPOS|
                                                                    MaterialLoaderModule.PICKER1_PICK_SENSOR_FROM_TRAY)
                        }
                    }
                }
                RowLayout{
                    Label{
                        text:qsTr("Ng Sensor高度")
                    }
                    TextField{
                        text: materialPickArmParams.placeNgSensorZ
                        horizontalAlignment: TextInput.AlignHCenter
                        validator: DoubleValidator {
                            decimals: 6
                            notation: DoubleValidator.StandardNotation
                        }
                        onEditingFinished: {
                            materialPickArmParams.setPlaceNgSensorZ(text)
                        }
                    }
                    Button{
                        text:qsTr("测高")
                        onClicked: {
                            sh_materialLoaderModule.performHandling(MaterialLoaderModule.PICKER2_MEASURE_NG_SENSOR_IN_TRAY)
                        }
                    }
                }
                RowLayout{
                    Layout.alignment: Qt.AlignVCenter|Qt.AlignRight
//                    Button{
//                        text:qsTr("空位视觉")
//                        width:40
//                        height: 40
//                        onClicked:{
//                            sh_materialLoaderModule.performHandling(MaterialLoaderModule.SENSOR_VACANCY_PR)
//                        }
//                    }

//                    Button{
//                        text:qsTr("移动吸头")
//                        width:40
//                        height: 40
//                        onClicked: {
//                            sh_sensor_tray.setTrayCurrent(t_ncol1.text-1,t_nrow1.text-1,1)
//                            sh_materialLoaderModule.performHandling(
//                                                                    MaterialLoaderModule.PICKER1_TO_WORKPOS)
//                        }
//                    }

                    Button{
                        text:qsTr("picker1放成品")
                        width: 40
                        height: 40
                        onClicked: {
                            sh_sensor_tray.setTrayCurrent(t_ncol.text-1,t_nrow.text-1,1)
                            sh_materialLoaderModule.performHandling(MaterialLoaderModule.SENSOR_TRAY|
                                                                    MaterialLoaderModule.SENSOR_VACANCY_PR|
                                                                    MaterialLoaderModule.PICKER1_TO_WORKPOS|
                                                                    MaterialLoaderModule.PICKER1_PLACE_PRODUCT_TO_TRAY)
                        }
                    }
//                    Button{

//                        text:qsTr("picker1放ng Sensor")
//                        width:40
//                        height: 40
//                        onClicked: {
//                            sh_sensor_tray.setTrayCurrent(t_ncol1.text-1,t_nrow1.text-1,1)
//                            sh_materialLoaderModule.performHandling(MaterialLoaderModule.SENSOR_TRAY|
//                                                                    MaterialLoaderModule.SENSOR_VACANCY_PR|
//                                                                    MaterialLoaderModule.PICKER1_TO_WORKPOS|
//                                                                    MaterialLoaderModule.PICKER1_PLACE_NG_SENSOR_TO_TRAY)
//                        }
//                    }
                }
            }
        }
    }
}
