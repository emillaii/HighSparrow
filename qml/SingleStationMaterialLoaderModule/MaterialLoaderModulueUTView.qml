import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11
import SingleHeadMaterialLoaderLib 1.0
ColumnLayout {
    GroupBox{
        title:qsTr("sut_pr_position_picker2")
        ColumnLayout{
            RowLayout{
                Label{
                    text:qsTr("X")
                }
                TextField{
                    text:sh_sut_pr_position.X
                    horizontalAlignment: TextInput.AlignHCenter
                    validator: DoubleValidator{
                        decimals: 6
                        notation: DoubleValidator.StandardNotation
                    }
                    onEditingFinished: {
                        sh_sut_pr_position.setX(text)
                    }
                }
                Label{
                    text:qsTr("Y")
                }
                TextField{
                    text:sh_sut_pr_position.Y
                    horizontalAlignment: TextInput.AlignHCenter
                    validator: DoubleValidator{
                        decimals: 6
                        notation: DoubleValidator.StandardNotation
                    }
                    onEditingFinished: {
                        sh_sut_pr_position.setY(text)
                    }
                }
                Button{
                    text:title_move_to
                    width: 40
                    height: 40
                    onClicked: {
                        sh_materialLoaderModule.performHandling(MaterialLoaderModule.SUT_PR_POS);
                    }
                }
                Button{
                    text:title_read_encoder
                    width: 40
                    height: 40
                    onClicked: {
                        var x = baseModuleManager.getMotorFeedbackPos(materialPickArmParams.motorVcmXName)
                        var y = baseModuleManager.getMotorFeedbackPos(materialPickArmParams.motorYName)
                        sh_sut_pr_position.setX(x);
                        sh_sut_pr_position.setY(y);
                    }
                }
            }
            RowLayout{
                Label{
                    text:qsTr("ng sensor高度")
                }
                TextField{
                    text:materialPickArmParams.pickNgSensorZ//sensorLoaderParameter.placeSensorZ
                    horizontalAlignment: TextInput.AlignHCenter
                    validator: DoubleValidator{
                        decimals: 6
                        notation: DoubleValidator.StandardNotation
                    }
                    onEditingFinished: {
                        //sensorLoaderParameter.setPlaceSensorZ(text)
                        materialPickArmParams.setPickNgSensorZ(text)
                    }
                }
                Button{
                    text:qsTr("NG Sensor视觉")
                    width: 40
                    height: 40
                    onClicked: {
                        sh_materialLoaderModule.performHandling(MaterialLoaderModule.NG_SENSOR_PR)

                    }
                }
            }
            RowLayout{
                Button{
                    text:qsTr("测高")
                    width: 40
                    height: 40
                    onClicked: {
                        sh_materialLoaderModule.performHandling(MaterialLoaderModule.PICKER2_MEASURE_NG_SENSOR_IN_SUT)
                    }
                }
                Button{
                    text:qsTr("移动吸头")
                    width:40
                    height:40
                    onClicked: {
                        sh_materialLoaderModule.performHandling(MaterialLoaderModule.PICKER2_TO_WORKPOS)
                    }
                }
                Button{
                    text:qsTr("吸頭動作")
                    width:40
                    height:40
                    onClicked: {
                        sh_materialLoaderModule.performHandling(MaterialLoaderModule.PICKER2_PICK_NG_SENSOR_FROM_SUT)
                    }
                }
                Button{
                    text:qsTr("取NG Sensor")
                    width:40
                    height: 40
                    onClicked: {
                        sh_materialLoaderModule.performHandling(MaterialLoaderModule.SUT_PR_POS|
                                                                MaterialLoaderModule.NG_SENSOR_PR|
                                                                MaterialLoaderModule.PICKER2_TO_WORKPOS|
                                                                MaterialLoaderModule.PICKER2_PICK_NG_SENSOR_FROM_SUT)
                    }
                }
            }
            RowLayout{
                Label{
                    text:qsTr("product高度")
                }
                TextField{
                    text:materialPickArmParams.pickProductZ
                    horizontalAlignment: TextInput.AlignHCenter
                    validator: DoubleValidator{
                        decimals: 6
                        notation: DoubleValidator.StandardNotation
                    }
                    onEditingFinished: {
                        materialPickArmParams.setPickProductZ(text)
                    }
                }
                Button{
                    text:qsTr("Product视觉")
                    width: 40
                    height: 40
                    onClicked: {
                        sh_materialLoaderModule.performHandling(MaterialLoaderModule.PRODUCT_PR)

                    }
                }
            }
            RowLayout{
                Button{
                    text:qsTr("测高")
                    width: 40
                    height: 40
                    onClicked: {
                        sh_materialLoaderModule.performHandling(MaterialLoaderModule.PICKER2_MEASURE_PRODUCT_IN_SUT)
                    }
                }
                Button{
                    text:qsTr("移动吸头")
                    width:40
                    height:40
                    onClicked: {
                        sh_materialLoaderModule.performHandling(MaterialLoaderModule.PICKER1_TO_WORKPOS)
                    }
                }
                Button{
                    text:qsTr("吸頭動作")
                    width:40
                    height: 40
                    onClicked: {
                        sh_materialLoaderModule.performHandling(MaterialLoaderModule.PICKER2_PICK_PRODUCT_FROM_SUT)
                    }
                }
                Button{
                    text:qsTr("取Product")
                    width:40
                    height: 40
                    onClicked: {
                        sh_materialLoaderModule.performHandling(
                                                                MaterialLoaderModule.SUT_PR_POS|
//                                                                MaterialLoaderModule.PRODUCT_PR|
                                                                MaterialLoaderModule.PICKER2_TO_WORKPOS|
                                                                MaterialLoaderModule.PICKER2_PICK_PRODUCT_FROM_SUT)
                    }
                }
            }
            RowLayout{
                Label{
                    text:qsTr("sut高度")
                }
                TextField{
                    text:materialPickArmParams.placeSensorZ
                    horizontalAlignment: TextInput.AlignHCenter
                    validator: DoubleValidator{
                        decimals: 6
                        notation: DoubleValidator.StandardNotation
                    }
                    onEditingFinished: {
                        materialPickArmParams.setPlaceSensorZ(text)
                    }
                }
                Button{
                    text:qsTr("SUT视觉")
                    width:40
                    height: 40
                    onClicked: {
                        sh_materialLoaderModule.performHandling(MaterialLoaderModule.SUT_PR_POS|
                                                                MaterialLoaderModule.SUT_PR)
                    }
                }
            }
            RowLayout{
                Button{
                    text:qsTr("测高")
                    width: 40
                    height: 40
                    onClicked: {
                        sh_materialLoaderModule.performHandling(MaterialLoaderModule.PICKER2_MEASURE_SENSOR_IN_SUT)
                    }
                }
                Button{
                    text:qsTr("移动吸头")
                    width:40
                    height: 40
                    onClicked: {
                        sh_materialLoaderModule.performHandling(MaterialLoaderModule.PICKER2_TO_WORKPOS)
                    }
                }
                Button{
                    text:qsTr("吸頭動作")
                    width:40
                    height: 40
                    onClicked: {
                        sh_materialLoaderModule.performHandling(MaterialLoaderModule.PICKER2_PLACE_SENSOR_TO_SUT)
                    }
                }
                Button{
                    text:qsTr("picker2放Sensor")
                    width:40
                    height: 40
                    onClicked: {
                        sh_materialLoaderModule.performHandling(MaterialLoaderModule.SUT_PR_POS|
                                                                MaterialLoaderModule.SUT_PR|
                                                                MaterialLoaderModule.PICKER2_TO_WORKPOS|
                                                                MaterialLoaderModule.PICKER2_PLACE_SENSOR_TO_SUT)
                    }
                }
            }
        }
    }


    GroupBox{
        title:qsTr("lut_pr_position_Picker1")
        ColumnLayout{
            RowLayout{
                Label{
                    text:qsTr("X")
                }
                TextField{
                    text:sh_lut_pr_position.X
                    horizontalAlignment: TextInput.AlignHCenter
                    validator: DoubleValidator{
                        decimals: 6
                        notation: DoubleValidator.StandardNotation
                    }
                    onEditingFinished: {
                        sh_lut_pr_position.setX(text)
                    }
                }
                Label{
                    text:qsTr("Y")
                }
                TextField{
                    text:sh_lut_pr_position.Y
                    horizontalAlignment: TextInput.AlignHCenter
                    validator: DoubleValidator{
                        decimals: 6
                        notation: DoubleValidator.StandardNotation
                    }
                    onEditingFinished: {
                        sh_lut_pr_position.setY(text)
                    }
                }
                Button{
                    text:title_move_to
                    width: 40
                    height: 40
                    onClicked: {
                        sh_materialLoaderModule.performHandling(MaterialLoaderModule.LUT_PR_POS);
                    }
                }
                Button{
                    text:title_read_encoder
                    width: 40
                    height: 40
                    onClicked: {
                        var x = baseModuleManager.getMotorFeedbackPos(materialPickArmParams.motorVcmXName)
                        var y = baseModuleManager.getMotorFeedbackPos(materialPickArmParams.motorYName)

                        sh_lut_pr_position.setX(x);
                        sh_lut_pr_position.setY(y);
                    }
                }
            }
            RowLayout{
                Label{
                    text:qsTr("放Lens高度")
                }
                TextField{
                    text:materialPickArmParams.placeLensZ
                    horizontalAlignment: TextInput.AlignHCenter
                    validator: DoubleValidator{
                        decimals: 6
                        notation: DoubleValidator.StandardNotation
                    }
                    onEditingFinished: {
                        materialPickArmParams.setPlaceLensZ()
                    }
                }
                Button{
                    text:qsTr("LUT 视觉")
                    width:40
                    height: 40
                    onClicked: {
                        sh_materialLoaderModule.performHandling(MaterialLoaderModule.LUT_PR)
                    }
                }
                Button{
                    text:qsTr("NG LENS 视觉")
                    width:40
                    height:40
                    onClicked: {
                        sh_materialLoaderModule.performHandling(MaterialLoaderModule.NG_LENS_PR)
                    }
                }

                Button{
                    text:qsTr("测高")
                    width: 40
                    height: 40
                    onClicked: {
                        sh_materialLoaderModule.performHandling(MaterialLoaderModule.PICKER1_MEASURE_LENS_IN_LUT)
                    }
                }
            }
            RowLayout{
                Label{
                    text:qsTr("Lens 角度")
                }
                TextField{
                    text:materialPickArmParams.placeLensTheta
                    horizontalAlignment: TextInput.AlignHCenter
                    validator: DoubleValidator{
                        decimals: 6
                        notation: DoubleValidator.StandardNotation
                    }
                    onEditingFinished: {
                        materialPickArmParams.setPlaceLensTheta(text)
                    }
                }
                Button{
                    text:qsTr("移動吸嘴")
                    width:40
                    height: 40
                    onClicked: {
                        sh_materialLoaderModule.performHandling(MaterialLoaderModule.PICKER1_TO_WORKPOS
                                                                )


                    }
                }
                Button{
                    text:qsTr("吸嘴動作")
                    width:40
                    height: 40
                    onClicked: {
                        sh_materialLoaderModule.performHandling(MaterialLoaderModule.PICKER1_PLACE_LENS_TO_LUT
                                                                )


                    }
                }
                Button{
                    text:qsTr("取NG Lens")
                    width:40
                    height: 40
                    onClicked: {
                        sh_materialLoaderModule.performHandling(MaterialLoaderModule.LUT_PR_POS|
                                                                MaterialLoaderModule.NG_LENS_PR|
                                                                MaterialLoaderModule.PICKER1_TO_WORKPOS|
                                                                MaterialLoaderModule.PICKER1_PICK_NG_LENS_FROM_LUT)
                    }
                }

                Button{
                    text:qsTr("放Lens")
                    width:40
                    height: 40
                    onClicked: {
                        sh_materialLoaderModule.performHandling(MaterialLoaderModule.LUT_PR_POS|
                                                                MaterialLoaderModule.NG_LENS_PR|
                                                                MaterialLoaderModule.PICKER1_TO_WORKPOS|
                                                                MaterialLoaderModule.PICKER1_PLACE_LENS_TO_LUT)
                    }
                }
            }
        }
    }


//    GroupBox{
//        title:qsTr("updownlook_up_position")
//        ColumnLayout{
//            RowLayout{
//                Label{
//                    text:qsTr("X")
//                }
//                TextField{
//                    text:sh_updownlook_up_position.X
//                    horizontalAlignment: TextInput.AlignHCenter
//                    validator: DoubleValidator{
//                        decimals: 6
//                        notation: DoubleValidator.StandardNotation
//                    }
//                    onEditingFinished: {
//                        sh_updownlook_up_position.setX(text)
//                    }
//                }
//                Label{
//                    text:qsTr("Y")
//                }
//                TextField{
//                    text:sh_updownlook_up_position.Y
//                    horizontalAlignment: TextInput.AlignHCenter
//                    validator: DoubleValidator{
//                        decimals: 6
//                        notation: DoubleValidator.StandardNotation
//                    }
//                    onEditingFinished: {
//                        sh_updownlook_up_position.setY(text)
//                    }
//                }
//                Button{
//                    text:title_move_to
//                    width: 40
//                    height: 40
//                    onClicked: {
////                        sensorLoaderModule.performHandling(SensorLoaderModule.SUT_PR_POS1)
//                    }
//                }
//                Button{
//                    text:title_read_encoder
//                    width: 40
//                    height: 40
//                    onClicked: {
//                        var x = baseModuleManager.getMotorFeedbackPos(materialPickArmParams.motorVcmXName)
//                        var y = baseModuleManager.getMotorFeedbackPos(materialPickArmParams.motorYName)

//                        sh_updownlook_up_position.setX(x);
//                        sh_updownlook_up_position.setY(y);
//                    }
//                }
//            }
//        }
//    }

}
