import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11
import LogicManagerLib 1.1
import SomeLib 1.1
import AACoreNew 1.1
import SutModuleLib 1.1
Column {
    RowLayout { Label { text: qsTr("取放参数") } }
    RowLayout {
        Label { text: qsTr("取料压力") }
        TextField{
           text: sensorLoaderParameter.pickProductForce
           horizontalAlignment: TextInput.AlignHCenter
           validator: DoubleValidator{
               decimals: 6
               notation: DoubleValidator.StandardNotation
           }
           onEditingFinished: {
               sensorLoaderParameter.setPickProductForce(text)
           }
        }
        Label { text: qsTr("速度") }
        TextField{
           text: sensorLoaderParameter.vcmWorkSpeed
           horizontalAlignment: TextInput.AlignHCenter
           validator: DoubleValidator{
               decimals: 6
               notation: DoubleValidator.StandardNotation
           }
           onEditingFinished: {
               sensorLoaderParameter.setVcmWorkSpeed(text)
           }
        }
        Label { text: qsTr("限力区间") }
        TextField{            
           text:sensorLoaderParameter.vcmMargin
           horizontalAlignment: TextInput.AlignHCenter
           validator: DoubleValidator{
               decimals: 6
               notation: DoubleValidator.StandardNotation
           }
           onEditingFinished: {
               sensorLoaderParameter.setVcmMargin(text)
           }
        }
    }
    RowLayout {
        Label { text: qsTr("SUT 1 放料区间") }
        TextField{
           text: sensorLoaderParameter.placeToSutMargin
           horizontalAlignment: TextInput.AlignHCenter
           validator: DoubleValidator{
               decimals: 6
               notation: DoubleValidator.StandardNotation
           }
           onEditingFinished: {
               sensorLoaderParameter.setPlaceToSutMargin(text)
           }
        }
        CheckBox{
            text: qsTr("不限力")
            checked: sensorLoaderParameter.disablePlaceToSutForceLimmit
            onCheckedChanged: {
                sensorLoaderParameter.setDisablePlaceToSutForceLimmit(checked)
            }
        }
        Label { text: qsTr("SUT1 取料区间") }
        TextField{
           text: sensorLoaderParameter.pickFromSutMargin
           horizontalAlignment: TextInput.AlignHCenter
           validator: DoubleValidator{
               decimals: 6
               notation: DoubleValidator.StandardNotation
           }
           onEditingFinished: {
               sensorLoaderParameter.setPickFromSutMargin(text)
           }
        }
        CheckBox{
            text: qsTr("不限力")
            checked: sensorLoaderParameter.disablePickFromSutForceLimit
            onCheckedChanged: {
                sensorLoaderParameter.setDisablePickFromSutForceLimit(checked)
            }
        }
    }
    RowLayout {
        Label { text: qsTr("SUT2 放料区间") }
        TextField{
           text: sensorLoaderParameter.placeToSutMargin
           horizontalAlignment: TextInput.AlignHCenter
           validator: DoubleValidator{
               decimals: 6
               notation: DoubleValidator.StandardNotation
           }
           onEditingFinished: {
               sensorLoaderParameter.setPlaceToSutMargin(text)
           }
        }
        CheckBox{
            text: qsTr("不限力")
            checked: sensorLoaderParameter.disablePlaceToSutForceLimmit
            onCheckedChanged: {
                sensorLoaderParameter.setDisablePlaceToSutForceLimmit(checked)
            }
        }
        Label { text: qsTr("SUT2 取料区间") }
        TextField{
           text: sensorLoaderParameter.pickFromSutMargin
           horizontalAlignment: TextInput.AlignHCenter
           validator: DoubleValidator{
               decimals: 6
               notation: DoubleValidator.StandardNotation
           }
           onEditingFinished: {
               text: sensorLoaderParameter.setPickFromSutMargin
           }
        }
        CheckBox{
            text: qsTr("不限力")
            checked: sensorLoaderParameter.disablePickFromSutForceLimit
            onCheckedChanged: {
                sensorLoaderParameter.setDisablePickFromSutForceLimit(checked)
            }
        }
    }

    RowLayout {
        Label { text: qsTr("CPA Pick And Place SUT1 Position") }
        Label { text: qsTr("X") }
        TextField{
           text: sut_pr_position1.X
           horizontalAlignment: TextInput.AlignHCenter
           validator: DoubleValidator{
               decimals: 6
               notation: DoubleValidator.StandardNotation
           }
           onEditingFinished: {
               sut_pr_position1.setX(text)
           }
        }
        Label { text: qsTr("Y") }
        TextField{
            text: sut_pr_position1.Y
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
           text: title_read_encoder
           onClicked: {          
               var x = baseModuleManager.getMotorFeedbackPos(sensorPickArmParams.motorXName)
               var y = baseModuleManager.getMotorFeedbackPos(sensorPickArmParams.motorYName)
               sut_pr_position1.setX(x);
               sut_pr_position1.setY(y);
           }
        }
        Button{
           text: title_move_to
           onClicked: {
               logicManager.performHandling(sensorLoaderParameter.moduleName,SensorLoaderModule.SUT_POS1)
           }
        }
        Button{
           text: qsTr("成品视觉")
           onClicked: {
               logicManager.performHandling(sensorLoaderParameter.moduleName,SensorLoaderModule.SUT_POS1
                                            +SensorLoaderModule.SUT_PRODUCT_PR
                                            +SensorLoaderModule.TO_PR_OFFSET)
           }
        }
        Button{
           text: qsTr("移动到取放成品位置")
           onClicked: {
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
                logicManager.performHandling(sensorLoaderParameter.moduleName,SensorLoaderModule.SUT_POS1
                                             +SensorLoaderModule.SUT_PRODUCT_PR
                                             +SensorLoaderModule.TO_PICK_PRODUCT_POS1
                                             +SensorLoaderModule.PICK_PRODUCT_FROM_SUT1)
            }
        }
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
             text: qsTr("移动到取放NG SENSOR位置")
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

    RowLayout {
        Label { text: qsTr("CPA Pick And Place SUT2 Position") }
        Label { text: qsTr("X") }
        TextField{
           text: sut_pr_position2.X
           horizontalAlignment: TextInput.AlignHCenter
           validator: DoubleValidator{
               decimals: 6
               notation: DoubleValidator.StandardNotation
           }
           onEditingFinished: {
               sut_pr_position2.setX(text)
           }
        }
        Label { text: qsTr("Y") }
        TextField{
           text: sut_pr_position2.Y
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
           text: title_read_encoder
           onClicked: {
               var x = baseModuleManager.getMotorFeedbackPos(sensorPickArmParams.motorXName)
               var y = baseModuleManager.getMotorFeedbackPos(sensorPickArmParams.motorYName)
               sut_pr_position2.setX(x);
               sut_pr_position2.setY(y);
           }
        }
        Button{
           text: title_move_to
           onClicked: {
               logicManager.performHandling(sensorLoaderParameter.moduleName,SensorLoaderModule.SUT_POS2)
           }
        }
        Button{
           text: qsTr("成品视觉")
           onClicked: {
               logicManager.performHandling(sensorLoaderParameter.moduleName,SensorLoaderModule.SUT_POS2
                                            +SensorLoaderModule.SUT_PRODUCT_PR
                                            +SensorLoaderModule.TO_PR_OFFSET)
           }
        }
        Button{
           text: qsTr("移动到取放成品位置")
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
                logicManager.performHandling(sensorLoaderParameter.moduleName,SensorLoaderModule.SUT_POS2
                                             +SensorLoaderModule.SUT_PRODUCT_PR
                                             +SensorLoaderModule.TO_PICK_PRODUCT_POS2
                                             +SensorLoaderModule.PICK_PRODUCT_FROM_SUT2)
            }
        }
        Button{
            text:qsTr("NG sesor视觉")
            width:40
            height: 40
            onClicked: {
                logicManager.performHandling(sensorLoaderParameter.moduleName,SensorLoaderModule.SUT_POS2
                                             +SensorLoaderModule.SUT_SENSOR_PR
                                             +SensorLoaderModule.TO_PR_OFFSET)
            }
        }
        Button{
             text: qsTr("移动到取放NG SENSOR位置")
            width:40
            height: 40
            onClicked: {
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
                logicManager.performHandling(sensorLoaderParameter.moduleName,SensorLoaderModule.SUT_POS2
                                             +SensorLoaderModule.SUT_SENSOR_PR
                                             +SensorLoaderModule.TO_PICK_NG_SENSOR_POS2
                                             +SensorLoaderModule.PICK_NG_SENSOR_FROM_SUT2)
            }
        }
    }

    RowLayout {
        Label{
            text:qsTr("picker1 放料高度")
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
                logicManager.performHandling(sensorLoaderParameter.moduleName, SensorLoaderModule.MEASURE_SENSOR_IN_SUT)
            }
        }
    }

    RowLayout{
        Label{
            text:qsTr("picker2 取成品高度")
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
                logicManager.performHandling(sensorLoaderParameter.moduleName, SensorLoaderModule.MEASURE_PRODUCT_IN_SUT)
            }
        }
        CheckBox{
            text: qsTr("使能视觉")
            checked: sensorLoaderParameter.enableProductPr
            onCheckedChanged:
                sensorLoaderParameter.setEnableProductPr(checked)
        }
    }
    RowLayout{
        Label{
            text:qsTr("picker2 取NG sensor高度")
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
                logicManager.performHandling(sensorLoaderParameter.moduleName, SensorLoaderModule.MEASURE_NG_SENSOR_IN_SUT)
            }
        }
        CheckBox{
            text: qsTr("使能视觉")
            checked: sensorLoaderParameter.enableNgSensorPr
            onCheckedChanged:
                sensorLoaderParameter.setEnableNgSensorPr(checked)
        }
    }

    RowLayout {
        Label { text: qsTr("SUT Escape") }
        Label { text: qsTr("X") }
        TextField{
           text: sensorLoaderParameter.escapeX
           horizontalAlignment: TextInput.AlignHCenter
           validator: DoubleValidator{
               decimals: 6
               notation: DoubleValidator.StandardNotation
           }
           onEditingFinished: {
               sensorLoaderParameter.setEscapeX(text)
           }
        }
        Label { text: qsTr("Y") }
        TextField{
           text: sensorLoaderParameter.escapeY
           horizontalAlignment: TextInput.AlignHCenter
           validator: DoubleValidator{
               decimals: 6
               notation: DoubleValidator.StandardNotation
           }
           onEditingFinished: {
               sensorLoaderParameter.setEscapeY(text)
           }
        }
        Label { text: qsTr("Z") }
        TextField{
           text: sensorLoaderParameter.escapeHeight
           horizontalAlignment: TextInput.AlignHCenter
           validator: DoubleValidator{
               decimals: 6
               notation: DoubleValidator.StandardNotation
           }
           onEditingFinished: {
               sensorLoaderParameter.setEscapeHeight(text)
           }
        }
    }

    CameraPickAndPlaceSettingTestOperation {}
}
