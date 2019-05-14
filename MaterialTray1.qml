import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11

ColumnLayout {
    GroupBox{
        title:qsTr("料盘起点")
        ColumnLayout{
            RowLayout{
                Label{
                    text: qsTr("X")
                }
                TextField{
                    text: tray_start_point1.X
                    horizontalAlignment: TextInput.AlignHCenter
                    validator: DoubleValidator {
                        decimals: 6
                        notation: DoubleValidator.StandardNotation
                    }
                    onEditingFinished: {
                        tray_start_point1.setX(text)
                    }
                }
                Label{
                    text: qsTr("Y")
                }
                TextField{
                    text: tray_start_point1.Y
                    horizontalAlignment: TextInput.AlignHCenter
                    validator: DoubleValidator {
                        decimals: 6
                        notation: DoubleValidator.StandardNotation
                    }
                    onEditingFinished: {
                        tray_start_point1.setY(text)
                    }
                }
                Button{
                    text:title_move_to
                    width: 40
                    height: 40
                    onClicked: {
                        if (baseModuleManager.ServerMode == 0){
                        logicManager.lensPickArmMoveToTray1StartPos()
                        }else{
                            sensorLoaderModule.performHandling(5)
                        }
                    }
                }
                Button{
                    text:title_read_encoder
                    width: 40
                    height: 40
                    onClicked: {
                        var x =0;
                        var x1 =0;
                        var y=0;
                        if (baseModuleManager.ServerMode == 0){
                            x = baseModuleManager.getMotorFeedbackPos(lensPickArmParams.motorTrayName)
                            x1 = baseModuleManager.getMotorFeedbackPos(lensPickArmParams.motorXName)
                            y = baseModuleManager.getMotorFeedbackPos(lensPickArmParams.motorYName)
                            tray_start_point1.setX(x + lensPickArmModuleParameter.visonPositionX - x1);
                            tray_start_point1.setY(y);
                        }else{

                            x = baseModuleManager.getMotorFeedbackPos(sensorPickArmParams.motorXName)
                            y = baseModuleManager.getMotorFeedbackPos(sensorPickArmParams.motorYName)
                            tray_start_point1.setX(x);
                            tray_start_point1.setY(y);
                        }
                    }
                }
            }
        }
    }

    GroupBox{
        title:qsTr("料盘终点")
        ColumnLayout{
            RowLayout{
                Label{
                    text: qsTr("X")
                }
                TextField{
                    text: first_tray_end_position.X
                    horizontalAlignment: TextInput.AlignHCenter
                    validator: DoubleValidator {
                        decimals: 6
                        notation: DoubleValidator.StandardNotation
                    }
                    onEditingFinished: {
                        first_tray_end_position.setX(text)
                    }
                }
                Label{
                    text: qsTr("Y")
                }
                TextField{
                    text: first_tray_end_position.Y
                    horizontalAlignment: TextInput.AlignHCenter
                    validator: DoubleValidator {
                        decimals: 6
                        notation: DoubleValidator.StandardNotation
                    }
                    onEditingFinished: {
                        first_tray_end_position.setY(text)
                    }
                }
                Button{
                    text:title_move_to
                    width: 40
                    height: 40
                    onClicked: {
                        if (baseModuleManager.ServerMode == 0){
                            logicManager.lensPickArmMoveToTray1EndPos()
                        }else{
                            sensorLoaderModule.performHandling(7)
                        }
                    }
                }
                Button{
                    text:title_read_encoder
                    width: 40
                    height: 40

                    onClicked: {
                        var x =0;
                        var x1 =0;
                        var y=0;
                        if (baseModuleManager.ServerMode == 0){
                            x = baseModuleManager.getMotorFeedbackPos(lensPickArmParams.motorTrayName)
                            x1 = baseModuleManager.getMotorFeedbackPos(lensPickArmParams.motorXName)
                            y = baseModuleManager.getMotorFeedbackPos(lensPickArmParams.motorYName)
                            first_tray_end_position.setX(x + lensPickArmModuleParameter.visonPositionX - x1);
                            first_tray_end_position.setY(y);
                        }else{

                            x = baseModuleManager.getMotorFeedbackPos(sensorPickArmParams.motorXName)
                            y = baseModuleManager.getMotorFeedbackPos(sensorPickArmParams.motorYName)
                            first_tray_end_position.setX(x);
                            first_tray_end_position.setY(y);
                        }
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
                        material_tray.setTrayCurrent(t_ncol.text-1,t_nrow.text-1,0)

                        if (baseModuleManager.ServerMode == 0){
                            logicManager.lensPickArmMoveToTray1Pos()
                        }else{
                            sensorLoaderModule.performHandling(1)
                        }
                    }
                }
            }

            RowLayout{
                Label{
                    text: baseModuleManager.ServerMode==0?qsTr("物料高度"):qsTr("sensor高度")
                }
                TextField{
                    text:lensPickArmModuleParameter.pickLensZ
                    horizontalAlignment: TextInput.AlignHCenter
                    validator: DoubleValidator{
                        decimals: 6
                        notation: DoubleValidator.StandardNotation
                    }
                    onEditingFinished: {
                        lensPickArmModuleParameter.setPickLensZ(text)
                    }
                }

                Button{
                    text:qsTr("测高")
                    width: 40
                    height: 40
                    onClicked: {

                        if (baseModuleManager.ServerMode == 0){
                            logicManager.lensPickArmLensMeasureHeight()
                        }else{
                            sensorLoaderModule.performHandling(10000)
                        }
                    }
                }

                Button{
                    text:baseModuleManager.ServerMode==0?qsTr("lens视觉"):qsTr("sensor视觉")
                    width: 40
                    height: 40
                    onClicked: {
                        if (baseModuleManager.ServerMode == 0){
                            logicManager.lensPickArmLensPR()
                        }else{
                            sensorLoaderModule.performHandling(120)
                        }
                    }
                }

                Button{
                    text:baseModuleManager.ServerMode==0?qsTr("取lens"):qsTr("取sensor")
                    width: 40
                    height: 40
                    onClicked: {
                        material_tray.setTrayCurrent(t_ncol.text-1,t_nrow.text-1,0)
                        if (baseModuleManager.ServerMode == 0){
                            logicManager.lensPickArmMoveToPickLensFromTray1()
                        }else{
                            sensorLoaderModule.performHandling(1121)
                        }
                    }
                }
            }
            RowLayout{
                visible: baseModuleManager.ServerMode==0
                Layout.alignment: Qt.AlignVCenter|Qt.AlignRight
                Button{
                    text:qsTr("空位视觉")
                    width: 40
                    height: 40
                    onClicked: {
                        logicManager.lensPickArmVacancyTrayPR()
                    }
                }
                Button{
                    text:qsTr("放lens")
                    width: 40
                    height: 40
                    onClicked: {
                        material_tray.setTrayCurrent(t_ncol.text-1,t_nrow.text-1,0)
                        logicManager.lensPickArmMoveToPlaceLensToTray1()
                    }
                }
            }
        }
    }
}
