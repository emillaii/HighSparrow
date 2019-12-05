import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11
import QtQuick.Dialogs 1.2
import LogicManagerLib 1.1
import SomeLib 1.1

ColumnLayout {
    RowLayout {
        Label { text: qsTr("取放参数") }
    }
    RowLayout {
        Label { text: qsTr("取料力度") }
        TextField{
            text: sensorLoaderParameter.vcmWorkForce
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator{
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                sensorLoaderParameter.setVcmWorkForce(text)
            }
        }
        Label{
            text:qsTr("速度")
        }
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
        Label{
            text:qsTr("限力区间")
        }
        TextField{
            text: sensorLoaderParameter.vcmMargin
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
        Label {
            text: qsTr("Carrier 取放料区间")
        }
        TextField{
            text:sensorLoaderParameter.placeToGoodTrayMargin
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
    }
    RowLayout {
        Label {
            text: qsTr("Sensor Collet Picker 1 Offset")
        }
        Label{
            text:qsTr("X")
        }
        TextField{
            text:pick1_offset.X
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator{
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                pick1_offset.setX(text)
            }
        }
        Label{
            text:qsTr("Y")
        }
        TextField{
            text:pick1_offset.Y
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator{
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                pick1_offset.setY(text)
            }
        }
    }
    RowLayout {
        Label {
            text: qsTr("Sensor Collet Picker 2 Offset")
        }
        Label{
            text:qsTr("X")
        }
        TextField{
            text:pick2_offset.X
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator{
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                pick2_offset.setX(text)
            }
        }
        Label{
            text:qsTr("Y")
        }
        TextField{
            text:pick2_offset.Y
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator{
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                pick2_offset.setY(text)
            }
        }
    }

    RowLayout {
        Label{
            text: qsTr("Carrier1 Pick And Place高度")
        }
        TextField{
            text: sensorLoaderParameter.pickSensorZ
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator{
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                sensorLoaderParameter.setPickSensorZ(text)
            }
        }
        Button{
            text:qsTr("sensor视觉")
            width:40
            height:40
            onClicked: {
                logicManager.performHandling(sensorLoaderParameter.moduleName,SensorLoaderModule.SENSOR_TRAY_1_POS
                                             +SensorLoaderModule.TRAY_SENSOR_PR)

            }
        }
        Button{
            text:qsTr("移动到取放位置")
            width: 40
            height: 40
            onClicked: {
                logicManager.performHandling(sensorLoaderParameter.moduleName,SensorLoaderModule.SENSOR_TRAY_1_POS
                                             +SensorLoaderModule.TRAY_SENSOR_PR
                                             +SensorLoaderModule.TO_PR_OFFSET)

            }
        }
        Button{
            text:qsTr("测高")
            width: 40
            height: 40
            onClicked: {
                logicManager.performHandling(sensorLoaderParameter.moduleName,SensorLoaderModule.MEASURE_SENSOR_IN_TRAY1)
            }
        }
    }

    RowLayout {
        Label{
            text: qsTr("Carrier2 Pick And Place高度")
        }
        TextField{
            text: sensorLoaderParameter.pickSensorZ2
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator{
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                sensorLoaderParameter.setPickSensorZ2(text)
            }
        }
        Button{
            text:qsTr("sensor视觉")
            width:40
            height:40
            onClicked: {
                material_tray.setTrayCurrent(0,0,0)
                logicManager.performHandling(sensorLoaderParameter.moduleName,SensorLoaderModule.SENSOR_TRAY_1_POS
                                             +SensorLoaderModule.TRAY_SENSOR_PR
                                             +SensorLoaderModule.TO_PR_OFFSET)

            }
        }
        Button{
            text:qsTr("移动到取放位置")
            width: 40
            height: 40
            onClicked: {
                logicManager.performHandling(sensorLoaderParameter.moduleName,SensorLoaderModule.SENSOR_TRAY_1_POS
                                             + SensorLoaderModule.TRAY_SENSOR_PR
                                             + SensorLoaderModule.SENSOR_TRAY_1_POS)

            }
        }
        Button{
            text:qsTr("测高")
            width: 40
            height: 40
            onClicked: {
                logicManager.performHandling(sensorLoaderParameter.moduleName,SensorLoaderModule.MEASURE_SENSOR_IN_TRAY2)
            }
        }
    }
}
