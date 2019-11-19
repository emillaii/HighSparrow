import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11
import SingleHeadMaterialLoaderLib 1.0

ColumnLayout {
    CheckBox{
        id: ckbPerformPrAsPlacingNgDutToTray
        text: qsTr("PerformPrAsPlacingNgDutToTray")
        checked: materialLoaderParameter.performPrAsPlacingNgDut
        onCheckedChanged: {
            materialLoaderParameter.setPerformPrAsPlacingNgDut(ckbPerformPrAsPlacingNgDutToTray.checked)
        }
    }
    GroupBox{
        title:qsTr("reject盘起点")
        ColumnLayout{
            RowLayout{
                Label{
                    text: qsTr("X")
                }
                TextField{
                    text: sh_reject_tray_start_point1.X
                    horizontalAlignment: TextInput.AlignHCenter
                    validator: DoubleValidator {
                        decimals: 6
                        notation: DoubleValidator.StandardNotation
                    }
                    onEditingFinished: {
                        sh_reject_tray_start_point1.setX(text)
                    }
                }
                Label{
                    text: qsTr("Y")
                }
                TextField{
                    text: sh_reject_tray_start_point1.Y
                    horizontalAlignment: TextInput.AlignHCenter
                    validator: DoubleValidator {
                        decimals: 6
                        notation: DoubleValidator.StandardNotation
                    }
                    onEditingFinished: {
                        sh_reject_tray_start_point1.setY(text)
                    }
                }
                Button{
                    text:title_move_to
                    width: 40
                    height: 40
                    onClicked: {
                        sh_materialLoaderModule.performHandling(MaterialLoaderModule.REJECT_TRAY_START_POS)
                    }
                }
                Button{
                    text:title_read_encoder
                    width: 40
                    height: 40
                    onClicked: {
                        var x = baseModuleManager.getMotorFeedbackPos(materialPickArmParams.motorXName)
                        var y = baseModuleManager.getMotorFeedbackPos(materialPickArmParams.motorYName)
                        sh_reject_tray_start_point1.setX(x);
                        sh_reject_tray_start_point1.setY(y);
                    }
                }
            }
        }
    }
    GroupBox{
        title:qsTr("reject盘终点")
        ColumnLayout{
            RowLayout{
                Label{
                    text: qsTr("X")
                }
                TextField{
                    text: sh_reject_first_tray_end_position.X
                    horizontalAlignment: TextInput.AlignHCenter
                    validator: DoubleValidator {
                        decimals: 6
                        notation: DoubleValidator.StandardNotation
                    }
                    onEditingFinished: {
                        sh_reject_first_tray_end_position.setX(text)
                    }
                }
                Label{
                    text: qsTr("Y")
                }
                TextField{
                    text: sh_reject_first_tray_end_position.Y
                    horizontalAlignment: TextInput.AlignHCenter
                    validator: DoubleValidator {
                        decimals: 6
                        notation: DoubleValidator.StandardNotation
                    }
                    onEditingFinished: {
                        sh_reject_first_tray_end_position.setY(text)
                    }
                }
                Button{
                    text:title_move_to
                    width: 40
                    height: 40
                    onClicked: {
                        sh_materialLoaderModule.performHandling(MaterialLoaderModule.REJECT_TRAY_END_POS)
                    }
                }
                Button{
                    text:title_read_encoder
                    width: 40
                    height: 40

                    onClicked: {
                        var x = baseModuleManager.getMotorFeedbackPos(materialPickArmParams.motorXName)
                        var y = baseModuleManager.getMotorFeedbackPos(materialPickArmParams.motorYName)
                        sh_reject_first_tray_end_position.setX(x);
                        sh_reject_first_tray_end_position.setY(y);
                    }
                }
            }
        }

    }
    GroupBox{
        title: qsTr("PlaceNgSensorOffset")
        RowLayout{
            Label{
                text: qsTr("X")
            }
            TextField{
                text: sh_place_ng_sensor_to_tray_offset.X
                horizontalAlignment: TextInput.AlignHCenter
                validator: DoubleValidator {
                    decimals: 6
                    notation: DoubleValidator.StandardNotation
                }
                onEditingFinished: {
                    sh_place_ng_sensor_to_tray_offset.setX(text)
                }
            }
            Label{
                text: qsTr("Y")
            }
            TextField{
                text: sh_place_ng_sensor_to_tray_offset.Y
                horizontalAlignment: TextInput.AlignHCenter
                validator: DoubleValidator {
                    decimals: 6
                    notation: DoubleValidator.StandardNotation
                }
                onEditingFinished: {
                    sh_place_ng_sensor_to_tray_offset.setY(text)
                }
            }
        }
    }
    GroupBox{
        title: qsTr("PlaceNgProductOffset")
        RowLayout{
            Label{
                text: qsTr("X")
            }
            TextField{
                text: sh_place_ng_product_to_tray_offset.X
                horizontalAlignment: TextInput.AlignHCenter
                validator: DoubleValidator {
                    decimals: 6
                    notation: DoubleValidator.StandardNotation
                }
                onEditingFinished: {
                    sh_place_ng_product_to_tray_offset.setX(text)
                }
            }
            Label{
                text: qsTr("Y")
            }
            TextField{
                text: sh_place_ng_product_to_tray_offset.Y
                horizontalAlignment: TextInput.AlignHCenter
                validator: DoubleValidator {
                    decimals: 6
                    notation: DoubleValidator.StandardNotation
                }
                onEditingFinished: {
                    sh_place_ng_product_to_tray_offset.setY(text)
                }
            }
        }
    }
    GroupBox{
        title:qsTr("reject盘操作")
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
                            sh_reject_tray.setTrayCurrent(t_ncol.text-1,t_nrow.text-1,0)
                            sh_materialLoaderModule.performHandling(MaterialLoaderModule.REJECT_TRAY)
                        }
                    }
                }
                RowLayout{

                    Button{
                        text:qsTr("PICKER1放ng Sensor")
                        width:40
                        height: 40
                        onClicked: {
                            sh_reject_tray.setTrayCurrent(t_ncol.text-1,t_nrow.text-1,1)
                            if(materialLoaderParameter.performPrAsPlacingNgDut)
                            {
                                sh_materialLoaderModule.performHandling(MaterialLoaderModule.REJECT_TRAY|
                                                                        MaterialLoaderModule.SENSOR_VACANCY_PR|
                                                                        MaterialLoaderModule.APPLY_PLACE_NG_SENSOR_TO_TRAY_OFFSET|
                                                                        MaterialLoaderModule.PICKER1_TO_WORKPOS|
                                                                        MaterialLoaderModule.PICKER1_PLACE_NG_SENSOR_TO_TRAY)
                            }
                            else{
                                sh_materialLoaderModule.performHandling(MaterialLoaderModule.REJECT_TRAY|
                                                                        MaterialLoaderModule.RESET_PR|
                                                                        MaterialLoaderModule.APPLY_PLACE_NG_SENSOR_TO_TRAY_OFFSET|
                                                                        MaterialLoaderModule.PICKER1_TO_WORKPOS|
                                                                        MaterialLoaderModule.PICKER1_PLACE_NG_SENSOR_TO_TRAY)
                            }


                        }
                    }
                    Button{
                        text:qsTr("PICKER2放ng Sensor")
                        width:40
                        height: 40
                        onClicked: {
                            sh_reject_tray.setTrayCurrent(t_ncol.text-1,t_nrow.text-1,1)

                            if(materialLoaderParameter.performPrAsPlacingNgDut)
                            {
                                sh_materialLoaderModule.performHandling(MaterialLoaderModule.REJECT_TRAY|
                                                                        MaterialLoaderModule.SENSOR_VACANCY_PR|
                                                                        MaterialLoaderModule.APPLY_PLACE_NG_SENSOR_TO_TRAY_OFFSET|
                                                                        MaterialLoaderModule.PICKER2_TO_WORKPOS|
                                                                        MaterialLoaderModule.PICKER2_PLACE_NG_SENSOR_TO_TRAY)
                            }
                            else{
                                sh_materialLoaderModule.performHandling(MaterialLoaderModule.REJECT_TRAY|
                                                                        MaterialLoaderModule.RESET_PR|
                                                                        MaterialLoaderModule.APPLY_PLACE_NG_SENSOR_TO_TRAY_OFFSET|
                                                                        MaterialLoaderModule.PICKER2_TO_WORKPOS|
                                                                        MaterialLoaderModule.PICKER2_PLACE_NG_SENSOR_TO_TRAY)
                            }
                        }
                    }
                    Button{
                        text:qsTr("PICKER1放ng product")
                        width: 40
                        height: 40
                        onClicked: {
                            sh_reject_tray.setTrayCurrent(t_ncol.text-1,t_nrow.text-1,1)
                            if(materialLoaderParameter.performPrAsPlacingNgDut)
                            {
                                sh_materialLoaderModule.performHandling(MaterialLoaderModule.REJECT_TRAY|
                                                                    MaterialLoaderModule.SENSOR_VACANCY_PR|
                                                                    MaterialLoaderModule.APPLY_PLACE_NG_PRODUCT_TO_TRAY_OFFSET|
                                                                    MaterialLoaderModule.PICKER1_TO_WORKPOS|
                                                                    MaterialLoaderModule.PICKER1_PLACE_NG_PRODUCT_TO_TRAY)
                            }
                            else{
                                sh_materialLoaderModule.performHandling(MaterialLoaderModule.REJECT_TRAY|
                                                                    MaterialLoaderModule.RESET_PR|
                                                                    MaterialLoaderModule.APPLY_PLACE_NG_PRODUCT_TO_TRAY_OFFSET|
                                                                    MaterialLoaderModule.PICKER1_TO_WORKPOS|
                                                                    MaterialLoaderModule.PICKER1_PLACE_NG_PRODUCT_TO_TRAY)
                            }
                        }
                    }
                }
            }
        }
    }
}

