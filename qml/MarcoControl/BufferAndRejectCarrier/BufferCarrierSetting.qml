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
            text: tray_start_point4.X
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator{
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                tray_start_point4.setX(text)
            }
        }
        Label { text: qsTr("Y") }
        TextField{
            text: tray_start_point4.Y
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator{
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                tray_start_point4.setY(text)
            }
        }
        Button {
            text: title_read_encoder
            onClicked: {
                var x =0;
                var y =0;
                x = baseModuleManager.getMotorFeedbackPos(sensorPickArmParams.motorXName)
                y = baseModuleManager.getMotorFeedbackPos(sensorPickArmParams.motorYName)
                tray_start_point4.setX(x);
                tray_start_point4.setY(y);
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
        Label { text: qsTr("Buffer 放料区间") }
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
            onClicked: {
                sensorLoaderParameter.setDisablePlaceToGoodTrayForceLimit(checked)
            }
        }
    }

    RowLayout {
        Label { text: qsTr("Buffer Place Offset") }
        Label { text: qsTr("X") }
        TextField{
            text:bufferTrayPlaceOffset.X
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator{
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                bufferTrayPlaceOffset.setX(text)
            }
        }
        Label { text: qsTr("Y") }
        TextField{
            text:bufferTrayPlaceOffset.Y
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator{
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                bufferTrayPlaceOffset.setY(text)
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
        Label { text: qsTr("Buffer Place高度") }
        TextField{
            text: sensorLoaderParameter.placeBufferProductZ
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                sensorLoaderParameter.setPlaceBufferProductZ(text)
            }
        }
        Button{
            text:qsTr("空位视觉")
            width:40
            height: 40
            onClicked:{
                material_tray.setTrayCurrent(t_ncol.text-1,t_nrow.text-1,3)
                logicManager.performHandling(sensorLoaderParameter.moduleName,SensorLoaderModule.BUFFER_TRAY_POS
                                             +SensorLoaderModule.TRAY_EMPTY_PR
                                             +SensorLoaderModule.TO_PR_OFFSET)
            }
        }
        Button {
            text: qsTr("移动到取放位置")
            onClicked: {
                material_tray.setTrayCurrent(t_ncol.text-1,t_nrow.text-1,3)
                logicManager.performHandling(sensorLoaderParameter.moduleName,SensorLoaderModule.BUFFER_TRAY_POS
                                             +SensorLoaderModule.TRAY_EMPTY_PR
                                             +SensorLoaderModule.TO_PLACE_BUFFER_POS)
            }
        }
        Button {
            text: qsTr("测高")
            onClicked: {
                logicManager.performHandling(sensorLoaderParameter.moduleName,SensorLoaderModule.MEASURE_PRODUCT_IN_TRAY2)
            }
        }
    }

    RowLayout {
        BufferCarrierTestOperation {}
    }
}
