import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11
import SomeLib 1.1
ColumnLayout {
    RowLayout{
        Label{
            text:qsTr("sensor高度")
        }
        TextField{
            text:sensorLoaderParameter.placeSensorZ
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator{
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                sensorLoaderParameter.setPlaceSensorZ(text)
            }
        }
        Button{
            text:qsTr("测高")
            width: 40
            height: 40
            onClicked: {
                sensorLoaderModule.performHandling(SensorLoaderModule.MEASURE_SENSOR_IN_SUT1)
            }
        }
    }
    RowLayout{
        Label{
            text:qsTr("product高度")
        }
        TextField{
            text:sensorLoaderParameter.pickProductZ
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator{
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                sensorLoaderParameter.setPickProductZ(text)
            }
        }
        Button{
            text:qsTr("测高")
            width: 40
            height: 40
            onClicked: {
                sensorLoaderModule.performHandling(SensorLoaderModule.MEASURE_PRODUCT_IN_SUT1)
            }
        }
    }
    RowLayout{
        Label{
            text:qsTr("NG sensor高度")
        }
        TextField{
            text:sensorLoaderParameter.pickNgSensorZ
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator{
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                sensorLoaderParameter.setPickNgSensorZ(text)
            }
        }
        Button{
            text:qsTr("测高")
            width: 40
            height: 40
            onClicked: {
                sensorLoaderModule.performHandling(SensorLoaderModule.MEASURE_NG_SENSOR_IN_SUT1)
            }
        }
    }
    GroupBox{
        title:"SUT位置1"
        ColumnLayout{
            RowLayout{
                Label{
                    text:qsTr("X")
                }
                TextField{
                    text:sut_pr_position1.X
                    horizontalAlignment: TextInput.AlignHCenter
                    validator: DoubleValidator{
                        decimals: 6
                        notation: DoubleValidator.StandardNotation
                    }
                    onEditingFinished: {
                        sut_pr_position1.setX(text)
                    }
                }
                Label{
                    text:qsTr("Y")
                }
                TextField{
                    text:sut_pr_position1.Y
                    horizontalAlignment: TextInput.AlignHCenter
                    validator: DoubleValidator{
                        decimals: 6
                        notation: DoubleValidator.StandardNotation
                    }
                    onEditingFinished: {
                        sut_pr_position1.setY(text)
                    }
                }
                Button{
                    text:title_move_to
                    width: 40
                    height: 40
                    onClicked: {
                        //console.log(SensorLoaderModule.SUT_POS1)
                        sensorLoaderModule.performHandling(SensorLoaderModule.SUT_POS1)
                    }
                }
                Button{
                    text:title_read_encoder
                    width: 40
                    height: 40
                    onClicked: {

                        var x = baseModuleManager.getMotorFeedbackPos(sensorPickArmParams.motorXName)
                        var y = baseModuleManager.getMotorFeedbackPos(sensorPickArmParams.motorYName)

                        sut_pr_position1.setX(x);
                        sut_pr_position1.setY(y);
                    }
                }
            }

            RowLayout{
                Layout.alignment: Qt.AlignVCenter|Qt.AlignRight
                Button{
                    text:qsTr("放sensors")
                    width: 40
                    height: 40
                    onClicked: {
                        //2110
                        sensorLoaderModule.performHandling(SensorLoaderModule.PLACE_SENSOR_TO_SUT1
                                                           +SensorLoaderModule.TO_PLACE_SENSOR_OFFSET
                                                           +SensorLoaderModule.RESET_PR
                                                           +SensorLoaderModule.SUT_POS1)
                    }
                }
                Button{
                    text:qsTr("取成品视觉")
                    width:40
                    height: 40
                    onClicked: {
                        sensorLoaderModule.performHandling(263)
                    }
                }

                Button{
                    text:qsTr("取成品")
                    width:40
                    height: 40
                    onClicked: {
                        //7263
                        sensorLoaderModule.performHandling(SensorLoaderModule.PICK_PRODUCT_FROM_SUT1
                                                           +SensorLoaderModule.TO_PLACE_PRODUCT_OFFSET
                                                           +SensorLoaderModule.PRODUCT_PR
                                                           +SensorLoaderModule.SUT_POS1)
                    }
                }
                Button{
                    text:qsTr("取ng Sensor视觉")
                    width:40
                    height: 40
                    onClicked: {
                        //253
                        sensorLoaderModule.performHandling(SensorLoaderModule.TO_PICK_PRODUCT_OFFSET
                                                           +SensorLoaderModule.NG_SENSOR_PR
                                                           +SensorLoaderModule.SUT_POS1)
                    }
                }
                Button{
                    text:qsTr("取Ng Sensor")
                    width:40
                    height: 40
                    onClicked: {
                        //4253
                        sensorLoaderModule.performHandling(SensorLoaderModule.PICK_NG_SENSOR_FROM_SUT1
                                                           +SensorLoaderModule.TO_PICK_PRODUCT_OFFSET
                                                           +SensorLoaderModule.NG_SENSOR_PR
                                                           +SensorLoaderModule.SUT_POS1)
                    }
                }

            }
        }
    }
    GroupBox{
        title:"SUT 位置2"
        ColumnLayout{
            RowLayout{
                Label{
                    text:qsTr("X")
                }
                TextField{
                    text:sut_pr_position2.X
                    horizontalAlignment: TextInput.AlignHCenter
                    validator: DoubleValidator{
                        decimals: 6
                        notation: DoubleValidator.StandardNotation
                    }
                    onEditingFinished: {
                        sut_pr_position2.setX(text)
                    }
                }
                Label{
                    text:qsTr("Y")
                }
                TextField{
                    text:sut_pr_position2.Y
                    horizontalAlignment: TextInput.AlignHCenter
                    validator: DoubleValidator{
                        decimals: 6
                        notation: DoubleValidator.StandardNotation
                    }
                    onEditingFinished: {
                        sut_pr_position2.setY(text)
                    }
                }
                Button{
                    text:title_move_to
                    width: 40
                    height: 40
                    onClicked: {
                        sensorLoaderModule.performHandling(SensorLoaderModule.SUT_POS2)
                    }
                }
                Button{
                    text:title_read_encoder
                    width: 40
                    height: 40
                    onClicked: {
                        var x = baseModuleManager.getMotorFeedbackPos(sensorPickArmParams.motorXName)
                        var y = baseModuleManager.getMotorFeedbackPos(sensorPickArmParams.motorYName)

                        sut_pr_position2.setX(x);
                        sut_pr_position2.setY(y);
                    }
                }
            }
            RowLayout{
                Layout.alignment: Qt.AlignVCenter|Qt.AlignRight
                Button{
                    text:qsTr("放sensors")
                    width:40
                    height: 40
                    onClicked: {
                        //3114
                        sensorLoaderModule.performHandling(SensorLoaderModule.PLACE_SENSOR_TO_SUT2
                                                           +SensorLoaderModule.TO_PLACE_SENSOR_OFFSET
                                                           +SensorLoaderModule.RESET_PR
                                                           +SensorLoaderModule.SUT_POS2)
                    }
                }
                Button{
                    text:qsTr("取成品视觉")
                    width:40
                    height: 40
                    onClicked: {
                        //264
                        sensorLoaderModule.performHandling(SensorLoaderModule.TO_PR_OFFSET
                                                           +SensorLoaderModule.PRODUCT_PR
                                                           +SensorLoaderModule.SUT_POS2)
                    }
                }


                Button{
                    text:qsTr("取成品")
                    width:40
                    height: 40
                    onClicked: {
                        //7264
                        sensorLoaderModule.performHandling(SensorLoaderModule.PICK_PRODUCT_FROM_SUT2
                                                           +SensorLoaderModule.TO_PICK_PRODUCT_OFFSET
                                                           +SensorLoaderModule.PRODUCT_PR
                                                           +SensorLoaderModule.SUT_POS2)
                    }
                }
                Button{
                    text:qsTr("取Ng Sensor视觉")
                    width:40
                    height: 40
                    onClicked: {
                        //254
                        sensorLoaderModule.performHandling(SensorLoaderModule.TO_PR_OFFSET
                                                           +SensorLoaderModule.NG_SENSOR_PR
                                                           +SensorLoaderModule.SUT_POS2)
                    }
                }
                Button{
                    text:qsTr("取ng Sensor")
                    width:40
                    height: 40
                    onClicked: {
                        //4254
                        sensorLoaderModule.performHandling(SensorLoaderModule.PICK_NG_SENSOR_FROM_SUT2
                                                           +SensorLoaderModule.TO_PICK_PRODUCT_OFFSET
                                                           +SensorLoaderModule.NG_SENSOR_PR
                                                           +SensorLoaderModule.SUT_POS2)
                    }
                }
            }
        }
    }
}
