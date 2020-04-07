import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11
import SomeLib 1.1
ColumnLayout {
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
                        //sensorLoaderModule.performHandling(SensorLoaderModule.SUT_POS1)
                        logicManager.performHandling(sensorLoaderParameter.moduleName,SensorLoaderModule.SUT_POS1)
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
                Label{
                    text:qsTr("sensor高度")
                }
                TextField{
                    text:sensorLoaderParameter.placeSUT1SensorZ
                    horizontalAlignment: TextInput.AlignHCenter
                    validator: DoubleValidator{
                        decimals: 6
                        notation: DoubleValidator.StandardNotation
                    }
                    onEditingFinished: {
                        sensorLoaderParameter.setPlaceSUT1SensorZ(text)
                    }
                }
                Button{
                    text:qsTr("测高")
                    width: 40
                    height: 40
                    onClicked: {
                        logicManager.performHandling(sensorLoaderParameter.moduleName, SensorLoaderModule.MEASURE_SENSOR_IN_SUT1)
                    }
                }
            }

            RowLayout{
                CheckBox{
                    text: qsTr("使能视觉")
                    checked: sensorLoaderParameter.enableProductPr
                    onCheckedChanged:
                        sensorLoaderParameter.setEnableProductPr(checked)
                }
                Label{
                    text:qsTr("product高度")
                }
                TextField{
                    text:sensorLoaderParameter.pickSUT1ProductZ
                    horizontalAlignment: TextInput.AlignHCenter
                    validator: DoubleValidator{
                        decimals: 6
                        notation: DoubleValidator.StandardNotation
                    }
                    onEditingFinished: {
                        sensorLoaderParameter.setPickSUT1ProductZ(text)
                    }
                }
                Button{
                    text:qsTr("测高")
                    width: 40
                    height: 40
                    onClicked: {
                        logicManager.performHandling(sensorLoaderParameter.moduleName, SensorLoaderModule.MEASURE_PRODUCT_IN_SUT1)
                    }
                }
            }
            RowLayout{
                CheckBox{
                    text: qsTr("使能视觉")
                    checked: sensorLoaderParameter.enableNgSensorPr
                    onCheckedChanged:
                        sensorLoaderParameter.setEnableNgSensorPr(checked)
                }
                Label{
                    text:qsTr("NG sensor高度")
                }
                TextField{
                    text:sensorLoaderParameter.pickSUT1NgSensorZ
                    horizontalAlignment: TextInput.AlignHCenter
                    validator: DoubleValidator{
                        decimals: 6
                        notation: DoubleValidator.StandardNotation
                    }
                    onEditingFinished: {
                        sensorLoaderParameter.setPickSUT1NgSensorZ(text)
                    }
                }
                Button{
                    text:qsTr("测高")
                    width: 40
                    height: 40
                    onClicked: {
                        logicManager.performHandling(sensorLoaderParameter.moduleName, SensorLoaderModule.MEASURE_NG_SENSOR_IN_SUT1)
                    }
                }
            }

            RowLayout{
                CheckBox{
                    text: qsTr("不限力")
                    checked: sensorLoaderParameter.disablePlaceToSutForceLimmit
                    onCheckedChanged:
                        sensorLoaderParameter.setDisablePlaceToSutForceLimmit(checked)
                }
                Label{
                    text:qsTr("放料区间")
                }
                TextField{
                    text:sensorLoaderParameter.placeToSut1Margin
                    horizontalAlignment: TextInput.AlignHCenter
                    validator: DoubleValidator{
                        decimals: 6
                        notation: DoubleValidator.StandardNotation
                    }
                    onEditingFinished: {
                        sensorLoaderParameter.setPlaceToSut1Margin(text)
                    }
                }
                Button{
                    text:qsTr("移动吸头")
                    width:40
                    height: 40
                    onClicked: {
//                        sensorLoaderModule.performHandling(SensorLoaderModule.SUT_POS1
//                                                           +SensorLoaderModule.TO_PLACE_SENSOR_POS1)
                        logicManager.performHandling(sensorLoaderParameter.moduleName,SensorLoaderModule.SUT_POS1
                                                     +SensorLoaderModule.TO_PLACE_SENSOR_POS1)
                    }
                }
                Button{
                    text:qsTr("放sensor")
                    width: 40
                    height: 40
                    onClicked: {
//                        sensorLoaderModule.performHandling(SensorLoaderModule.SUT_POS1
//                                                           +SensorLoaderModule.TO_PLACE_SENSOR_POS1
//                                                           +SensorLoaderModule.PLACE_SENSOR_TO_SUT1)
                        logicManager.performHandling(sensorLoaderParameter.moduleName,SensorLoaderModule.SUT_POS1
                                                     +SensorLoaderModule.TO_PLACE_SENSOR_POS1
                                                     +SensorLoaderModule.PLACE_SENSOR_TO_SUT1)
                    }
                }
            }
            RowLayout{
                CheckBox{
                    text: qsTr("不限力")
                    checked: sensorLoaderParameter.disablePlaceToSutForceLimmit
                    onCheckedChanged:
                        sensorLoaderParameter.setDisablePlaceToSutForceLimmit(checked)
                }
                Label{
                    text:qsTr("取料区间")
                }
                TextField{
                    text:sensorLoaderParameter.pickFromSut1Margin
                    horizontalAlignment: TextInput.AlignHCenter
                    validator: DoubleValidator{
                        decimals: 6
                        notation: DoubleValidator.StandardNotation
                    }
                    onEditingFinished: {
                        sensorLoaderParameter.setPickFromSut1Margin(text)
                    }
                }
                Button{
                    text:qsTr("成品视觉")
                    width:40
                    height: 40
                    onClicked: {
//                        sensorLoaderModule.performHandling(SensorLoaderModule.SUT_POS1
//                                                           +SensorLoaderModule.SUT_PRODUCT_PR
//                                                           +SensorLoaderModule.TO_PR_OFFSET)
                        logicManager.performHandling(sensorLoaderParameter.moduleName,SensorLoaderModule.SUT_POS1
                                                     +SensorLoaderModule.SUT_PRODUCT_PR
                                                     +SensorLoaderModule.TO_PR_OFFSET)
                    }
                }
                Button{
                    text:qsTr("移动吸头")
                    width:40
                    height: 40
                    onClicked: {
//                        sensorLoaderModule.performHandling(SensorLoaderModule.SUT_POS1
//                                                           +SensorLoaderModule.SUT_PRODUCT_PR
//                                                           +SensorLoaderModule.TO_PICK_PRODUCT_POS1)
                        logicManager.performHandling(sensorLoaderParameter.moduleName,SensorLoaderModule.SUT_POS1
                                                     +SensorLoaderModule.SUT_PRODUCT_PR
                                                     +SensorLoaderModule.TO_PICK_PRODUCT_POS1)
                    }
                }
                Button{
                    text:qsTr("取成品")
                    width:40
                    height: 40
                    onClicked: {
//                        sensorLoaderModule.performHandling(SensorLoaderModule.SUT_POS1
//                                                           +SensorLoaderModule.SUT_PRODUCT_PR
//                                                           +SensorLoaderModule.TO_PICK_PRODUCT_POS1
//                                                           +SensorLoaderModule.PICK_PRODUCT_FROM_SUT1)
                        logicManager.performHandling(sensorLoaderParameter.moduleName,SensorLoaderModule.SUT_POS1
                                                     +SensorLoaderModule.SUT_PRODUCT_PR
                                                     +SensorLoaderModule.TO_PICK_PRODUCT_POS1
                                                     +SensorLoaderModule.PICK_PRODUCT_FROM_SUT1)
                    }
                }
            }
            RowLayout{
                Layout.alignment: Qt.AlignVCenter|Qt.AlignRight
                Button{
                    text:qsTr("NG sesor视觉")
                    width:40
                    height: 40
                    onClicked: {
                        logicManager.performHandling(sensorLoaderParameter.moduleName,SensorLoaderModule.SUT_POS1
                                                     +SensorLoaderModule.SUT_SENSOR_PR
                                                     +SensorLoaderModule.TO_PR_OFFSET)
                    }
                }
                Button{
                    text:qsTr("移动吸头")
                    width:40
                    height: 40
                    onClicked: {
                        logicManager.performHandling(sensorLoaderParameter.moduleName,SensorLoaderModule.SUT_POS1
                                                     +SensorLoaderModule.SUT_SENSOR_PR
                                                     +SensorLoaderModule.TO_PICK_NG_SENSOR_POS1)
                    }
                }
                Button{
                    text:qsTr("取NG sensor")
                    width:40
                    height: 40
                    onClicked: {
                        logicManager.performHandling(sensorLoaderParameter.moduleName,SensorLoaderModule.SUT_POS1
                                                     +SensorLoaderModule.SUT_SENSOR_PR
                                                     +SensorLoaderModule.TO_PICK_NG_SENSOR_POS1
                                                     +SensorLoaderModule.PICK_NG_SENSOR_FROM_SUT1)
                    }
                }
            }
            RowLayout{
                Label{
                    text:qsTr("取料offsetX")
                }
                TextField{
                    text:sut1PickOffset.X
                    horizontalAlignment: TextInput.AlignHCenter
                    validator: DoubleValidator{
                        decimals: 6
                        notation: DoubleValidator.StandardNotation
                    }
                    onEditingFinished: {
                        sut1PickOffset.setX(text)
                    }
                }
                Label{
                    text:qsTr("取料offsetY")
                }
                TextField{
                    text:sut1PickOffset.Y
                    horizontalAlignment: TextInput.AlignHCenter
                    validator: DoubleValidator{
                        decimals: 6
                        notation: DoubleValidator.StandardNotation
                    }
                    onEditingFinished: {
                        sut1PickOffset.setY(text)
                    }
                }
            }
            RowLayout{
                Label{
                    text:qsTr("放料offsetX")
                }
                TextField{
                    text:sut1PlaceOffset.X
                    horizontalAlignment: TextInput.AlignHCenter
                    validator: DoubleValidator{
                        decimals: 6
                        notation: DoubleValidator.StandardNotation
                    }
                    onEditingFinished: {
                        sut1PlaceOffset.setX(text)
                    }
                }
                Label{
                    text:qsTr("放料offsetY")
                }
                TextField{
                    text:sut1PlaceOffset.Y
                    horizontalAlignment: TextInput.AlignHCenter
                    validator: DoubleValidator{
                        decimals: 6
                        notation: DoubleValidator.StandardNotation
                    }
                    onEditingFinished: {
                        sut1PlaceOffset.setY(text)
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
                        //sensorLoaderModule.performHandling(SensorLoaderModule.SUT_POS2)
                        logicManager.performHandling(sensorLoaderParameter.moduleName,SensorLoaderModule.SUT_POS2)
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
                Label{
                    text:qsTr("sensor高度")
                }
                TextField{
                    text:sensorLoaderParameter.placeSUT2SensorZ
                    horizontalAlignment: TextInput.AlignHCenter
                    validator: DoubleValidator{
                        decimals: 6
                        notation: DoubleValidator.StandardNotation
                    }
                    onEditingFinished: {
                        sensorLoaderParameter.setPlaceSUT2SensorZ(text)
                    }
                }
                Button{
                    text:qsTr("测高")
                    width: 40
                    height: 40
                    onClicked: {
                        logicManager.performHandling(sensorLoaderParameter.moduleName, SensorLoaderModule.MEASURE_SENSOR_IN_SUT2)
                    }
                }
            }

            RowLayout{
                CheckBox{
                    text: qsTr("使能视觉")
                    checked: sensorLoaderParameter.enableProductPr
                    onCheckedChanged:
                        sensorLoaderParameter.setEnableProductPr(checked)
                }
                Label{
                    text:qsTr("product高度")
                }
                TextField{
                    text:sensorLoaderParameter.pickSUT2ProductZ
                    horizontalAlignment: TextInput.AlignHCenter
                    validator: DoubleValidator{
                        decimals: 6
                        notation: DoubleValidator.StandardNotation
                    }
                    onEditingFinished: {
                        sensorLoaderParameter.setPickSUT2ProductZ(text)
                    }
                }
                Button{
                    text:qsTr("测高")
                    width: 40
                    height: 40
                    onClicked: {
                        logicManager.performHandling(sensorLoaderParameter.moduleName, SensorLoaderModule.MEASURE_PRODUCT_IN_SUT2)
                    }
                }
            }
            RowLayout{
                CheckBox{
                    text: qsTr("使能视觉")
                    checked: sensorLoaderParameter.enableNgSensorPr
                    onCheckedChanged:
                        sensorLoaderParameter.setEnableNgSensorPr(checked)
                }
                Label{
                    text:qsTr("NG sensor高度")
                }
                TextField{
                    text:sensorLoaderParameter.pickSUT2NgSensorZ
                    horizontalAlignment: TextInput.AlignHCenter
                    validator: DoubleValidator{
                        decimals: 6
                        notation: DoubleValidator.StandardNotation
                    }
                    onEditingFinished: {
                        sensorLoaderParameter.setPickSUT2NgSensorZ(text)
                    }
                }
                Button{
                    text:qsTr("测高")
                    width: 40
                    height: 40
                    onClicked: {
                        logicManager.performHandling(sensorLoaderParameter.moduleName, SensorLoaderModule.MEASURE_NG_SENSOR_IN_SUT2)
                    }
                }
            }

            RowLayout{
                CheckBox{
                    text: qsTr("不限力")
                    checked: sensorLoaderParameter.disablePlaceToSutForceLimmit
                    onCheckedChanged:
                        sensorLoaderParameter.setDisablePlaceToSutForceLimmit(checked)
                }
                Label{
                    text:qsTr("放料区间")
                }
                TextField{
                    text:sensorLoaderParameter.placeToSut2Margin
                    horizontalAlignment: TextInput.AlignHCenter
                    validator: DoubleValidator{
                        decimals: 6
                        notation: DoubleValidator.StandardNotation
                    }
                    onEditingFinished: {
                        sensorLoaderParameter.setPlaceToSut2Margin(text)
                    }
                }
                Button{
                    text:qsTr("移动吸头")
                    width:40
                    height: 40
                    onClicked: {
//                        sensorLoaderModule.performHandling(SensorLoaderModule.SUT_POS2
//                                                           +SensorLoaderModule.TO_PLACE_SENSOR_POS2)
                        logicManager.performHandling(sensorLoaderParameter.moduleName,SensorLoaderModule.SUT_POS2
                                                     +SensorLoaderModule.TO_PLACE_SENSOR_POS2)
                    }
                }
                Button{
                    text:qsTr("放sensor")
                    width:40
                    height: 40
                    onClicked: {
                        logicManager.performHandling(sensorLoaderParameter.moduleName,SensorLoaderModule.SUT_POS2
                                                     +SensorLoaderModule.TO_PLACE_SENSOR_POS2
                                                     +SensorLoaderModule.PLACE_SENSOR_TO_SUT2)
                    }
                }
            }
            RowLayout{
                CheckBox{
                    text: qsTr("不限力")
                    checked: sensorLoaderParameter.disablePickFromSutForceLimit
                    onCheckedChanged:
                        sensorLoaderParameter.setDisablePickFromSutForceLimit(checked)
                }
                Label{
                    text:qsTr("取料区间")
                }
                TextField{
                    text:sensorLoaderParameter.pickFromSut2Margin
                    horizontalAlignment: TextInput.AlignHCenter
                    validator: DoubleValidator{
                        decimals: 6
                        notation: DoubleValidator.StandardNotation
                    }
                    onEditingFinished: {
                        sensorLoaderParameter.setPickFromSut2Margin(text)
                    }
                }

                Button{
                    text:qsTr("成品视觉")
                    width:40
                    height: 40
                    onClicked: {
                        logicManager.performHandling(sensorLoaderParameter.moduleName,SensorLoaderModule.SUT_POS2
                                                     +SensorLoaderModule.SUT_PRODUCT_PR
                                                     +SensorLoaderModule.TO_PR_OFFSET)
                    }
                }
                Button{
                    text:qsTr("移动吸头")
                    width:40
                    height: 40
                    onClicked: {
                        logicManager.performHandling(sensorLoaderParameter.moduleName,SensorLoaderModule.SUT_POS2
                                                     +SensorLoaderModule.SUT_PRODUCT_PR
                                                     +SensorLoaderModule.TO_PICK_PRODUCT_POS2)
                    }
                }
                Button{
                    text:qsTr("取成品")
                    width:40
                    height: 40
                    onClicked: {
//                        sensorLoaderModule.performHandling(SensorLoaderModule.SUT_POS2
//                                                           +SensorLoaderModule.SUT_PRODUCT_PR
//                                                           +SensorLoaderModule.TO_PICK_PRODUCT_POS2
//                                                           +SensorLoaderModule.PICK_PRODUCT_FROM_SUT2)
                        logicManager.performHandling(sensorLoaderParameter.moduleName,SensorLoaderModule.SUT_POS2
                                                     +SensorLoaderModule.SUT_PRODUCT_PR
                                                     +SensorLoaderModule.TO_PICK_PRODUCT_POS2
                                                     +SensorLoaderModule.PICK_PRODUCT_FROM_SUT2)
                    }
                }
            }
            RowLayout{
                Layout.alignment: Qt.AlignVCenter|Qt.AlignRight
                Button{
                    text:qsTr("NG sesor视觉")
                    width:40
                    height: 40
                    onClicked: {
//                        sensorLoaderModule.performHandling(SensorLoaderModule.SUT_POS2
//                                                           +SensorLoaderModule.SUT_SENSOR_PR
//                                                           +SensorLoaderModule.TO_PR_OFFSET)
                        logicManager.performHandling(sensorLoaderParameter.moduleName,SensorLoaderModule.SUT_POS2
                                                     +SensorLoaderModule.SUT_SENSOR_PR
                                                     +SensorLoaderModule.TO_PR_OFFSET)
                    }
                }
                Button{
                    text:qsTr("移动吸头")
                    width:40
                    height: 40
                    onClicked: {
//                        sensorLoaderModule.performHandling(SensorLoaderModule.SUT_POS2
//                                                           +SensorLoaderModule.SUT_SENSOR_PR
//                                                           +SensorLoaderModule.TO_PICK_NG_SENSOR_POS2)
                        logicManager.performHandling(sensorLoaderParameter.moduleName,SensorLoaderModule.SUT_POS2
                                                     +SensorLoaderModule.SUT_SENSOR_PR
                                                     +SensorLoaderModule.TO_PICK_NG_SENSOR_POS2)
                    }
                }
                Button{
                    text:qsTr("取NG sensor")
                    width:40
                    height: 40
                    onClicked: {
//                        sensorLoaderModule.performHandling(SensorLoaderModule.SUT_POS2
//                                                           +SensorLoaderModule.SUT_SENSOR_PR
//                                                           +SensorLoaderModule.TO_PICK_NG_SENSOR_POS2
//                                                           +SensorLoaderModule.PICK_NG_SENSOR_FROM_SUT2)
                        logicManager.performHandling(sensorLoaderParameter.moduleName,SensorLoaderModule.SUT_POS2
                                                     +SensorLoaderModule.SUT_SENSOR_PR
                                                     +SensorLoaderModule.TO_PICK_NG_SENSOR_POS2
                                                     +SensorLoaderModule.PICK_NG_SENSOR_FROM_SUT2)
                    }
                }
            }
            RowLayout{
                Label{
                    text:qsTr("取料offsetX")
                }
                TextField{
                    text:sut2PickOffset.X
                    horizontalAlignment: TextInput.AlignHCenter
                    validator: DoubleValidator{
                        decimals: 6
                        notation: DoubleValidator.StandardNotation
                    }
                    onEditingFinished: {
                        sut2PickOffset.setX(text)
                    }
                }
                Label{
                    text:qsTr("取料offsetY")
                }
                TextField{
                    text:sut2PickOffset.Y
                    horizontalAlignment: TextInput.AlignHCenter
                    validator: DoubleValidator{
                        decimals: 6
                        notation: DoubleValidator.StandardNotation
                    }
                    onEditingFinished: {
                        sut2PickOffset.setY(text)
                    }
                }
            }
            RowLayout{
                Label{
                    text:qsTr("放料offsetX")
                }
                TextField{
                    text:sut2PlaceOffset.X
                    horizontalAlignment: TextInput.AlignHCenter
                    validator: DoubleValidator{
                        decimals: 6
                        notation: DoubleValidator.StandardNotation
                    }
                    onEditingFinished: {
                        sut2PlaceOffset.setX(text)
                    }
                }
                Label{
                    text:qsTr("放料offsetY")
                }
                TextField{
                    text:sut2PlaceOffset.Y
                    horizontalAlignment: TextInput.AlignHCenter
                    validator: DoubleValidator{
                        decimals: 6
                        notation: DoubleValidator.StandardNotation
                    }
                    onEditingFinished: {
                        sut2PlaceOffset.setY(text)
                    }
                }
            }
        }
    }
}
