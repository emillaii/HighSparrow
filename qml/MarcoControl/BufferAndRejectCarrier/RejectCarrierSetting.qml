import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11

Column {
    RowLayout {
        Label { text: qsTr("料盘信息") }
    }
    RowLayout {
        Label { text: qsTr("料盘起点") }
        Label { text: qsTr("X") }
        TextField{
            text: tray_start_point3.X
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator{
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                tray_start_point3.setX(text)
            }
        }
        Label { text: qsTr("Y") }
        TextField{
            text: tray_start_point3.Y
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator{
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                tray_start_point3.setY(text)
            }
        }
        Button {
            text: title_read_encoder
            onClicked: {
                var x =0;
                var y =0;
                x = baseModuleManager.getMotorFeedbackPos(sensorPickArmParams.motorXName)
                y = baseModuleManager.getMotorFeedbackPos(sensorPickArmParams.motorYName)
                tray_start_point3.setX(x);
                tray_start_point3.setY(y);
            }
        }
    }
    RowLayout {
        Label { text: qsTr("参数设置") }
    }
    RowLayout {
        Label { text: qsTr("取料压力") }
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
        Label { text: qsTr("Reject 放料区间") }
        TextField{
            text:sensorLoaderParameter.placeToNgTrayMargin
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator{
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                sensorLoaderParameter.setPlaceToNgTrayMargin(text)
            }
        }
        CheckBox{
            text: qsTr("不限力")
            checked: sensorLoaderParameter.disablePlaceToNgTrayForceLimit
            onClicked: {
                sensorLoaderParameter.setDisablePlaceToNgTrayForceLimit(checked)
            }
        }
    }

    RowLayout {
        Label { text: qsTr("Reject Place Offset") }
        Label { text: qsTr("X") }
        TextField{
            text:ngTrayPlaceOffset.X
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator{
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                ngTrayPlaceOffset.setX(text)
            }
        }
        Label { text: qsTr("Y") }
        TextField{
            text:ngTrayPlaceOffset.Y
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator{
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                ngTrayPlaceOffset.setY(text)
            }
        }
//        Label { text: qsTr("Theta") }
//        TextField{
//            horizontalAlignment: TextInput.AlignHCenter
//            validator: DoubleValidator{
//                decimals: 6
//                notation: DoubleValidator.StandardNotation
//            }
//            onEditingFinished: {
//            }
//        }
//        Button {
//            text: title_read_encoder
//        }
//        Button {
//            text: title_move_to
//        }
    }

    RowLayout {
        Label { text: qsTr("Reject Place高度") }
        TextField{
            text: sensorLoaderParameter.placeNgProductZ
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                sensorLoaderParameter.setPlaceNgProductZ(text)
            }
        }
        Button {
            text: qsTr("视觉")
            onClicked:{
                material_tray.setTrayCurrent(t_ncol.text-1,t_nrow.text-1,2)
                logicManager.performHandling(sensorLoaderParameter.moduleName,SensorLoaderModule.NG_SENSOR_TRAY_POS
                                             +SensorLoaderModule.TRAY_EMPTY_PR
                                             +SensorLoaderModule.TO_PR_OFFSET)
            }
        }
        Button {
            text: qsTr("移动到取放位置")
            onClicked: {
                material_tray.setTrayCurrent(t_ncol.text-1,t_nrow.text-1,2)
                logicManager.performHandling(sensorLoaderParameter.moduleName,SensorLoaderModule.NG_SENSOR_TRAY_POS
                                             +SensorLoaderModule.TRAY_EMPTY_PR
                                             +SensorLoaderModule.TO_PLACE_NG_POS)
            }
        }
        Button {
            text: qsTr("测高")
            onClicked: {
                logicManager.performHandling(sensorLoaderParameter.moduleName,SensorLoaderModule.MEASURE_NG_SENSOR_IN_TRAY)
            }
        }
    }

    RowLayout {
        RejectCarrierTestOperation {}
    }
}
