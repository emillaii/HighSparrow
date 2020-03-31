import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11
import SomeLib 1.1

Column {
    RowLayout { Label { text: qsTr("取放参数") } }
    RowLayout {
        Label { text: qsTr("SUT1 放料区间") }
        TextField{
           horizontalAlignment: TextInput.AlignHCenter
           validator: DoubleValidator{
               decimals: 6
               notation: DoubleValidator.StandardNotation
           }
           onEditingFinished: {
               sensorLoaderParameter.setPlaceToSut1Margin(text)
           }
        }
        CheckBox{
            text: qsTr("不限力")
            checked: sensorLoaderParameter.disablePlaceToSutForceLimmit
            onCheckedChanged:
                sensorLoaderParameter.setDisablePlaceToSutForceLimmit(checked)
        }
        Label { text: qsTr("SUT1 取料区间") }
        TextField{
           horizontalAlignment: TextInput.AlignHCenter
           validator: DoubleValidator{
               decimals: 6
               notation: DoubleValidator.StandardNotation
           }
           onEditingFinished: {
               sensorLoaderParameter.setPickFromSut1Margin(text)
           }
        }
        CheckBox{
            text: qsTr("不限力")
            checked: sensorLoaderParameter.disablePickFromSutForceLimmit
            onCheckedChanged:
                sensorLoaderParameter.setDisablePickFromSutForceLimit(checked)
        }
    }

    RowLayout {
        Label { text: qsTr("Carrier1 放料区间") }
        TextField{
           horizontalAlignment: TextInput.AlignHCenter
           validator: DoubleValidator{
               decimals: 6
               notation: DoubleValidator.StandardNotation
           }
           onEditingFinished: {
               sensorLoaderParameter.setPlaceToGoodTrayMargin(text)
           }
        }
        CheckBox{
            text: qsTr("不限力")
            checked: sensorLoaderParameter.disablePlaceToGoodTrayForceLimit
            onCheckedChanged: {
                sensorLoaderParameter.setDisablePlaceToGoodTrayForceLimit(checked)
            }
        }
        Label { text: qsTr("Carrier1 取料区间") }
        TextField{
           horizontalAlignment: TextInput.AlignHCenter
           validator: DoubleValidator{
               decimals: 6
               notation: DoubleValidator.StandardNotation
           }
           onEditingFinished: {
           }
        }
        CheckBox{
            text: qsTr("不限力")
            onCheckedChanged: {
            }
        }
    }

    RowLayout {
        Label { text: qsTr("Carrier2 放料区间") }
        TextField{
           horizontalAlignment: TextInput.AlignHCenter
           validator: DoubleValidator{
               decimals: 6
               notation: DoubleValidator.StandardNotation
           }
           onEditingFinished: {
               sensorLoaderParameter.setPlaceToGoodTrayMargin(text)
           }
        }
        CheckBox{
            text: qsTr("不限力")
            checked: sensorLoaderParameter.disablePlaceToGoodTrayForceLimit
            onCheckedChanged: {
                sensorLoaderParameter.setDisablePlaceToGoodTrayForceLimit(checked)
            }
        }
        Label { text: qsTr("Carrier2 取料区间") }
        TextField{
           horizontalAlignment: TextInput.AlignHCenter
           validator: DoubleValidator{
               decimals: 6
               notation: DoubleValidator.StandardNotation
           }
           onEditingFinished: {
           }
        }
        CheckBox{
            text: qsTr("不限力")
            onCheckedChanged: {
            }
        }
    }

    RowLayout {
        Label { text: qsTr("Carrier1 Pick And Place 高度") }
        TextField{
           horizontalAlignment: TextInput.AlignHCenter
           validator: DoubleValidator{
               decimals: 6
               notation: DoubleValidator.StandardNotation
           }
           onEditingFinished: {
               sensorLoaderParameter.setPlaceProductZ(text)
           }
        }
        Button {
            text: qsTr("Camera视觉")
            onClicked: {
                logicManager.performHandling(sensorLoaderParameter.moduleName,SensorLoaderModule.SENSOR_TRAY_1_POS
                                             +SensorLoaderModule.TRAY_EMPTY_PR
                                             +SensorLoaderModule.TO_PR_OFFSET)
            }
        }
        Button {
            text: qsTr("移动到取放位置")
            onClicked: {
                logicManager.performHandling(sensorLoaderParameter.moduleName,SensorLoaderModule.SENSOR_TRAY_1_POS
                                             +SensorLoaderModule.TRAY_EMPTY_PR
                                             +SensorLoaderModule.TO_PLACE_PRODUCT_POS1)
            }
        }
        Button {
            text: qsTr("测高")
            onClicked: {
            logicManager.performHandling(sensorLoaderParameter.moduleName,SensorLoaderModule.MEASURE_PRODUCT_IN_TRAY1)
            }
        }
    }

    RowLayout {
        Label { text: qsTr("Carrier2 Pick And Place 高度") }
        TextField{
           horizontalAlignment: TextInput.AlignHCenter
           validator: DoubleValidator{
               decimals: 6
               notation: DoubleValidator.StandardNotation
           }
           onEditingFinished: {
               sensorLoaderParameter.setPlaceProductZ2(text)
           }
        }
        Button {
            text: qsTr("Camera视觉")
            onClicked: {
                logicManager.performHandling(sensorLoaderParameter.moduleName,SensorLoaderModule.SENSOR_TRAY_2_POS
                                             +SensorLoaderModule.TRAY_EMPTY_PR
                                             +SensorLoaderModule.TO_PR_OFFSET)
            }
        }
        Button {
            text: qsTr("移动到取放位置")
            onClicked: {
                logicManager.performHandling(sensorLoaderParameter.moduleName,SensorLoaderModule.SENSOR_TRAY_2_POS
                                             +SensorLoaderModule.TRAY_EMPTY_PR
                                             +SensorLoaderModule.TO_PLACE_PRODUCT_POS2)
            }
        }
        Button {
            text: qsTr("测高")
            onClicked: {
                logicManager.performHandling(sensorLoaderParameter.moduleName,SensorLoaderModule.MEASURE_PRODUCT_IN_TRAY2)
            }
        }
    }
    CameraPlaceSettingTestOperation {}

}
