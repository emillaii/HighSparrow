import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11
import SomeLib 1.1
ColumnLayout {
    GroupBox{
        title:qsTr("料盘起点")
        ColumnLayout{
            RowLayout{
                Label{
                    text: qsTr("X")
                }
                TextField{
                    text: tray_start_point4.X
                    horizontalAlignment: TextInput.AlignHCenter
                    validator: DoubleValidator {
                        decimals: 6
                        notation: DoubleValidator.StandardNotation
                    }
                    onEditingFinished: {
                        tray_start_point4.setX(text)
                    }
                }
                Label{
                    text: qsTr("Y")
                }
                TextField{
                    text: tray_start_point4.Y
                    horizontalAlignment: TextInput.AlignHCenter
                    validator: DoubleValidator {
                        decimals: 6
                        notation: DoubleValidator.StandardNotation
                    }
                    onEditingFinished: {
                        tray_start_point4.setY(text)
                    }
                }
                Button{
                    text:title_move_to
                    width: 40
                    height: 40
                    onClicked: {
//                        sensorLoaderModule.performHandling(SensorLoaderModule.BUFFER_TRAY_START_POS)
                        logicManager.performHandling(sensorLoaderParameter.moduleName,SensorLoaderModule.BUFFER_TRAY_START_POS)
                    }
                }
                Button{
                    text:title_read_encoder
                    width: 40
                    height: 40
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
        }
    }

    GroupBox{
        title:qsTr("料盘操作")
        ColumnLayout{
            RowLayout{
                Label{
                    text: qsTr("目标行")
                }
                TextField{
                    id:t_nrow
                    text: "1"
                    horizontalAlignment: TextInput.AlignHCenter
                    validator: IntValidator{
                        bottom: 1
                    }
                }
                Label{
                    text: qsTr("目标列")
                }
                TextField{
                    id:t_ncol
                    text: "1"
                    horizontalAlignment: TextInput.AlignHCenter
                    validator: IntValidator{
                        bottom: 1
                    }
                }
                Button{
                    text:title_move_to
                    width: 40
                    height: 40
                    onClicked: {
                        material_tray.setTrayCurrent(t_ncol.text-1,t_nrow.text-1,3)
//                        sensorLoaderModule.performHandling(SensorLoaderModule.BUFFER_TRAY_POS)
                        logicManager.performHandling(sensorLoaderParameter.moduleName,SensorLoaderModule.BUFFER_TRAY_POS)
                    }
                }
            }
            RowLayout{
                visible: baseModuleManager.getServerMode() !==0
                CheckBox{
                    text: qsTr("不限力")
                    checked: sensorLoaderParameter.disablePlaceToGoodTrayForceLimit
                    onClicked: {
                        sensorLoaderParameter.setDisablePlaceToGoodTrayForceLimit(checked)
                    }
                }

                Label{
                    text:qsTr("放料区间")
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
            }
            RowLayout{
                visible: baseModuleManager.getServerMode() !==0

                Label{
                    text:qsTr("成品高度")
                }
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
                    text:qsTr("测高")
                    onClicked: {
//                        sensorLoaderModule.performHandling(SensorLoaderModule.MEASURE_PRODUCT_IN_TRAY2)
                        logicManager.performHandling(sensorLoaderParameter.moduleName,SensorLoaderModule.MEASURE_PRODUCT_IN_TRAY2)
                    }
                }
            }
            RowLayout{
                Layout.alignment: Qt.AlignVCenter|Qt.AlignRight

                Button{
                    text:qsTr("空位视觉")
                    width:40
                    height: 40
                    onClicked:{
                        material_tray.setTrayCurrent(t_ncol.text-1,t_nrow.text-1,3)
//                        sensorLoaderModule.performHandling(SensorLoaderModule.BUFFER_TRAY_POS
//                                                           +SensorLoaderModule.TRAY_EMPTY_PR
//                                                           +SensorLoaderModule.TO_PR_OFFSET)
                        logicManager.performHandling(sensorLoaderParameter.moduleName,SensorLoaderModule.BUFFER_TRAY_POS
                                                     +SensorLoaderModule.TRAY_EMPTY_PR
                                                     +SensorLoaderModule.TO_PR_OFFSET)
                    }
                }

                Button{
                    text:qsTr("移动吸头")
                    width:40
                    height: 40
                    onClicked: {
                        material_tray.setTrayCurrent(t_ncol.text-1,t_nrow.text-1,3)
//                        sensorLoaderModule.performHandling(SensorLoaderModule.BUFFER_TRAY_POS
//                                                           +SensorLoaderModule.TRAY_EMPTY_PR
//                                                           +SensorLoaderModule.TO_PLACE_BUFFER_POS)
                        logicManager.performHandling(sensorLoaderParameter.moduleName,SensorLoaderModule.BUFFER_TRAY_POS
                                                     +SensorLoaderModule.TRAY_EMPTY_PR
                                                     +SensorLoaderModule.TO_PLACE_BUFFER_POS)
                    }
                }

                CheckBox{
                    text: qsTr("使能视觉")
                    checked: sensorLoaderParameter.enableBufferProductPr
                    onCheckedChanged:
                        sensorLoaderParameter.setEnableBufferProductPr(checked)
                }
                Button{
                    text:qsTr("放成品")
                    width:40
                    height: 40
                    onClicked: {
                        material_tray.setTrayCurrent(t_ncol.text-1,t_nrow.text-1,3)
//                        sensorLoaderModule.performHandling(SensorLoaderModule.BUFFER_TRAY_POS
//                                                           +SensorLoaderModule.TRAY_EMPTY_PR
//                                                           +SensorLoaderModule.TO_PLACE_BUFFER_POS
//                                                           +SensorLoaderModule.PLACE_PRODUCT_TO_BUFFER_TRAY)
                        logicManager.performHandling(sensorLoaderParameter.moduleName,SensorLoaderModule.BUFFER_TRAY_POS
                                                     +SensorLoaderModule.TRAY_EMPTY_PR
                                                     +SensorLoaderModule.TO_PLACE_BUFFER_POS
                                                     +SensorLoaderModule.PLACE_PRODUCT_TO_BUFFER_TRAY)
                    }
                }
            }

        }
    }
}
