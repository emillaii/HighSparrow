import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11
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
           text: qsTr("Camera视觉")
           onClicked: {
           }
        }
        Button{
           text: qsTr("移动到取放位置")
           onClicked: {
           }
        }
        Button{
           text: qsTr("测高")
           onClicked: {
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
           text: qsTr("Camera视觉")
           onClicked: {
           }
        }
        Button{
           text: qsTr("移动到取放位置")
           onClicked: {
           }
        }
        Button{
           text: qsTr("测高")
           onClicked: {
           }
        }
    }

    RowLayout {
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
                logicManager.performHandling(sensorLoaderParameter.moduleName, SensorLoaderModule.MEASURE_SENSOR_IN_SUT)
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
