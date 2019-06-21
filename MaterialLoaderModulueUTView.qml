import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11
import SingleHeadMaterialLoaderLib 1.0
ColumnLayout {
    GroupBox{
        title:qsTr("sut_pr_position")
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
//                        sensorLoaderModule.performHandling(SensorLoaderModule.SUT_POS1)
                        sh_materialLoaderModule.performHandling(MaterialLoaderModule.SUT_POS);
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
                        sh_materialLoaderModule.performHandling(MaterialLoaderModule.MEASURE_NG_SENSOR_IN_SUT)
                    }
                }
                Button{
                    text:qsTr("移动吸头")
                    width:40
                    height:40
                    onClicked: {
                        sh_materialLoaderModule.performHandling(MaterialLoaderModule.SENSOR_TO_PICK)
                    }
                }
                Button{
                    text:qsTr("取NG Sensor")
                    width:40
                    height: 40
                    onClicked: {
                        sh_materialLoaderModule.performHandling(MaterialLoaderModule.SUT_POS|
                                                                MaterialLoaderModule.NG_SENSOR_PR|
                                                                MaterialLoaderModule.SENSOR_TO_PICK|
                                                                MaterialLoaderModule.PICK_NG_SENSOR_FROM_SUT)
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
                        //sensorLoaderModule.performHandling(SensorLoaderModule.MEASURE_PRODUCT_IN_SUT1)
                        MaterialLoaderModule.performHandling(MaterialLoaderModule.MEASURE_PRODUCT_IN_SUT)
                    }
                }
                Button{
                    text:qsTr("移动吸头")
                    width:40
                    height:40
                    onClicked: {
                        MaterialLoaderModule.performHandling(MaterialLoaderModule.LPA_TO_WORK)
                    }
                }
                Button{
                    text:qsTr("取Product")
                    width:40
                    height: 40
                    onClicked: {
                        sh_materialLoaderModule.performHandling(MaterialLoaderModule.SUT_POS|
                                                                MaterialLoaderModule.PRODUCT_PR|
                                                                MaterialLoaderModule.LPA_TO_WORK|
                                                                MaterialLoaderModule.PICK_PRODUCT_FROM_SUT)
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
                        sh_materialLoaderModule.performHandling(MaterialLoaderModule.SUT_PR)
                    }
                }
            }
            RowLayout{
                Button{
                    text:qsTr("测高")
                    width: 40
                    height: 40
                    onClicked: {
                        //sensorLoaderModule.performHandling(SensorLoaderModule.MEASURE_NG_SENSOR_IN_SUT1)
                        sh_materialLoaderModule.performHandling(MaterialLoaderModule.MEASURE_SENSOR_IN_SUT)
                    }
                }
                Button{
                    text:qsTr("移动吸头")
                    width:40
                    height: 40
                    onClicked: {
                        sh_materialLoaderModule.performHandling(MaterialLoaderModule.SENSOR_TO_PICK)
                    }
                }
                Button{
                    text:qsTr("放Sensor")
                    width:40
                    height: 40
                    onClicked: {
                        sh_materialLoaderModule.performHandling(MaterialLoaderModule.SUT_POS|
                                                                MaterialLoaderModule.SUT_PR|
                                                                MaterialLoaderModule.SENSOR_TO_PICK|
                                                                MaterialLoaderModule.PLACE_SENSOR_TO_SUT)
                    }
                }
            }
        }
    }
    GroupBox{
        title:qsTr("lut_pr_position")
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
//                        sensorLoaderModule.performHandling(SensorLoaderModule.SUT_POS1)
                        sh_materialLoaderModule.performHandling(MaterialLoaderModule.LUT_POS);
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
                        //sensorLoaderModule.performHandling(SensorLoaderModule.MEASURE_NG_SENSOR_IN_SUT1)
                        sh_materialLoaderModule.performHandling(MaterialLoaderModule.MEASURE_LENS_IN_LUT)
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
                    text:qsTr("取NG Lens")
                    width:40
                    height: 40
                    onClicked: {
                        sh_materialLoaderModule.performHandling(MaterialLoaderModule.LUT_POS|
                                                                MaterialLoaderModule.NG_LENS_PR|
                                                                MaterialLoaderModule.LPA_TO_WORK|
                                                                MaterialLoaderModule.PICK_NG_LENS_FROM_LUT)
                    }
                }
                Button{
                    text:qsTr("放Lens")
                    width:40
                    height: 40
                    onClicked: {
                        sh_materialLoaderModule.performHandling(MaterialLoaderModule.LUT_POS|
                                                                MaterialLoaderModule.NG_LENS_PR|
                                                                MaterialLoaderModule.LPA_TO_WORK|
                                                                MaterialLoaderModule.PLACE_LENS_TO_LUT)
                    }
                }
            }
        }
    }
    GroupBox{
        title:qsTr("updownlook_up_position")
        ColumnLayout{
            RowLayout{
                Label{
                    text:qsTr("X")
                }
                TextField{
                    text:sh_updownlook_up_position.X
                    horizontalAlignment: TextInput.AlignHCenter
                    validator: DoubleValidator{
                        decimals: 6
                        notation: DoubleValidator.StandardNotation
                    }
                    onEditingFinished: {
                        sh_updownlook_up_position.setX(text)
                    }
                }
                Label{
                    text:qsTr("Y")
                }
                TextField{
                    text:sh_updownlook_up_position.Y
                    horizontalAlignment: TextInput.AlignHCenter
                    validator: DoubleValidator{
                        decimals: 6
                        notation: DoubleValidator.StandardNotation
                    }
                    onEditingFinished: {
                        sh_updownlook_up_position.setY(text)
                    }
                }
                Button{
                    text:title_move_to
                    width: 40
                    height: 40
                    onClicked: {
//                        sensorLoaderModule.performHandling(SensorLoaderModule.SUT_POS1)
                    }
                }
                Button{
                    text:title_read_encoder
                    width: 40
                    height: 40
                    onClicked: {
                        var x = baseModuleManager.getMotorFeedbackPos(materialPickArmParams.motorVcmXName)
                        var y = baseModuleManager.getMotorFeedbackPos(materialPickArmParams.motorYName)

                        sh_updownlook_up_position.setX(x);
                        sh_updownlook_up_position.setY(y);
                    }
                }
            }
        }
    }
    GroupBox{
        title:qsTr("lens_suction_offset")
        ColumnLayout{
            RowLayout{
                Label{
                    text:qsTr("X")
                }
                TextField{
                    text:sh_lens_suction_offset.X
                    horizontalAlignment: TextInput.AlignHCenter
                    validator: DoubleValidator{
                        decimals: 6
                        notation: DoubleValidator.StandardNotation
                    }
                    onEditingFinished: {
                        sh_lens_suction_offset.setX(text)
                    }
                }
                Label{
                    text:qsTr("Y")
                }
                TextField{
                    text:sh_lens_suction_offset.Y
                    horizontalAlignment: TextInput.AlignHCenter
                    validator: DoubleValidator{
                        decimals: 6
                        notation: DoubleValidator.StandardNotation
                    }
                    onEditingFinished: {
                        sh_lens_suction_offset.setY(text)
                    }
                }
                Button{
                    text:qsTr("测量")
                    width: 40
                    height: 40
                    onClicked: {
//                        sensorLoaderModule.performHandling(SensorLoaderModule.SUT_POS1)
                    }
                }
            }
        }
    }
    GroupBox{
        title:qsTr("sensor_suction_offset")
        ColumnLayout{
            RowLayout{
                Label{
                    text:qsTr("X")
                }
                TextField{
                    text:sh_sensor_suction_offset.X
                    horizontalAlignment: TextInput.AlignHCenter
                    validator: DoubleValidator{
                        decimals: 6
                        notation: DoubleValidator.StandardNotation
                    }
                    onEditingFinished: {
                        sh_sensor_suction_offset.setX(text)
                    }
                }
                Label{
                    text:qsTr("Y")
                }
                TextField{
                    text:sh_sensor_suction_offset.Y
                    horizontalAlignment: TextInput.AlignHCenter
                    validator: DoubleValidator{
                        decimals: 6
                        notation: DoubleValidator.StandardNotation
                    }
                    onEditingFinished: {
                        sh_sensor_suction_offset.setY(text)
                    }
                }
                Button{
                    text:qsTr("测量")
                    width: 40
                    height: 40
                    onClicked: {
//                        sensorLoaderModule.performHandling(SensorLoaderModule.SUT_POS1)
                    }
                }
            }
        }
    }
}
