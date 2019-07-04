import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11

import SingleHeadMaterialLoaderLib 1.0

ColumnLayout {
    GroupBox{
        title:qsTr("Lens盘1起点")
        ColumnLayout{
            RowLayout{
                Label{
                    text: qsTr("X")
                }
                TextField{
                    text: sh_lens_tray_start_point1.X
                    horizontalAlignment: TextInput.AlignHCenter
                    validator: DoubleValidator {
                        decimals: 6
                        notation: DoubleValidator.StandardNotation
                    }
                    onEditingFinished: {
                        sh_lens_tray_start_point1.setX(text)
                    }
                }
                Label{
                    text: qsTr("Y")
                }
                TextField{
                    text: sh_lens_tray_start_point1.Y
                    horizontalAlignment: TextInput.AlignHCenter
                    validator: DoubleValidator {
                        decimals: 6
                        notation: DoubleValidator.StandardNotation
                    }
                    onEditingFinished: {
                        sh_lens_tray_start_point1.setY(text)
                    }
                }
                Button{
                    text:title_move_to
                    width: 40
                    height: 40
                    onClicked: {
                        sh_materialLoaderModule.performHandling(MaterialLoaderModule.LENS_TRAY1_START_POS)
                    }
                }
                Button{
                    text:title_read_encoder
                    width: 40
                    height: 40
                    onClicked: {
                        var x = baseModuleManager.getMotorFeedbackPos(materialPickArmParams.motorXName)
                        var y = baseModuleManager.getMotorFeedbackPos(materialPickArmParams.motorYName)
                        sh_lens_tray_start_point1.setX(x);
                        sh_lens_tray_start_point1.setY(y);
                    }
                }
            }
        }
    }

    GroupBox{
        title:qsTr("Lens盘1终点")
        ColumnLayout{
            RowLayout{
                Label{
                    text: qsTr("X")
                }
                TextField{
                    text: sh_lens_first_tray_end_position.X
                    horizontalAlignment: TextInput.AlignHCenter
                    validator: DoubleValidator {
                        decimals: 6
                        notation: DoubleValidator.StandardNotation
                    }
                    onEditingFinished: {
                        sh_lens_first_tray_end_position.setX(text)
                    }
                }
                Label{
                    text: qsTr("Y")
                }
                TextField{
                    text: sh_lens_first_tray_end_position.Y
                    horizontalAlignment: TextInput.AlignHCenter
                    validator: DoubleValidator {
                        decimals: 6
                        notation: DoubleValidator.StandardNotation
                    }
                    onEditingFinished: {
                        sh_lens_first_tray_end_position.setY(text)
                    }
                }
                Button{
                    text:title_move_to
                    width: 40
                    height: 40
                    onClicked: {
                        sh_materialLoaderModule.performHandling(MaterialLoaderModule.LENS_TRAY1_END_POS)
                    }
                }
                Button{
                    text:title_read_encoder
                    width: 40
                    height: 40

                    onClicked: {
                        var x = baseModuleManager.getMotorFeedbackPos(materialPickArmParams.motorXName)
                        var y = baseModuleManager.getMotorFeedbackPos(materialPickArmParams.motorYName)
                        sh_lens_first_tray_end_position.setX(x);
                        sh_lens_first_tray_end_position.setY(y);
                    }
                }
            }
        }
    }

    GroupBox{
        title:qsTr("Lens盘1操作")
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
                        sh_lens_tray.setTrayCurrent(t_ncol.text-1,t_nrow.text-1,0)
                        //todo moveto
                        sh_materialLoaderModule.performHandling(MaterialLoaderModule.LENS_TRAY1)
                    }
                }
            }

            RowLayout{
                Label{
                    text: qsTr("lens高度")
                }
                TextField{
                    text: materialPickArmParams.pickLensZ
                    horizontalAlignment: TextInput.AlignHCenter
                    validator: DoubleValidator{
                        decimals: 6
                        notation: DoubleValidator.StandardNotation
                    }
                    onEditingFinished: {
                        materialPickArmParams.pickLensZ
                    }
                }

                Button{
                    text:qsTr("测高")
                    width: 40
                    height: 40
                    onClicked: {
                        sh_materialLoaderModule.performHandling(MaterialLoaderModule.MEASURE_LENS_IN_TRAY)
                    }
                }
                Button{
                    text:qsTr("执行视觉")
                    width:40
                    height:40
                    onClicked: {
                        sh_materialLoaderModule.performHandling(MaterialLoaderModule.LENS_PR)
                    }
                }

                Button{
                    text:qsTr("移动吸头")
                    width: 40
                    height: 40
                    onClicked: {
                        sh_materialLoaderModule.performHandling(MaterialLoaderModule.LPA_TO_WORK|
                                                                MaterialLoaderModule.LENS_PR)
                    }
                }

                Button{
                    text:qsTr("取lens")
                    width: 40
                    height: 40
                    onClicked: {
                        sh_lens_tray.setTrayCurrent(t_ncol.text-1,t_nrow.text-1,0)
                        sh_materialLoaderModule.performHandling(MaterialLoaderModule.LENS_TRAY1|
                                                                MaterialLoaderModule.LENS_PR|
                                                                MaterialLoaderModule.LPA_TO_WORK|
                                                                MaterialLoaderModule.PICK_LENS_FROM_TRAY)
                    }
                }
            }
            RowLayout{
                Layout.alignment: Qt.AlignVCenter|Qt.AlignRight
                Button{
                    text:qsTr("执行空位视觉")
                    width:40
                    height:40
                    onClicked: {
                        sh_materialLoaderModule.performHandling(MaterialLoaderModule.LENS_VACANCY_PR)
                    }
                }

                Button{
                    text:qsTr("移动吸头")
                    width: 40
                    height: 40
                    onClicked: {
                        sh_materialLoaderModule.performHandling(MaterialLoaderModule.LPA_TO_WORK|
                                                                MaterialLoaderModule.LENS_VACANCY_PR)
                    }
                }
                Button{
                    text:qsTr("放NGlens")
                    width: 40
                    height: 40
                    onClicked: {
                        sh_lens_tray.setTrayCurrent(t_ncol.text-1,t_nrow.text-1,0)
                        sh_materialLoaderModule.performHandling(MaterialLoaderModule.LENS_TRAY1|
                                                                MaterialLoaderModule.LENS_VACANCY_PR|
                                                                MaterialLoaderModule.LPA_TO_WORK|
                                                                MaterialLoaderModule.PLACE_NG_LENS_TO_TRAY)
                    }
                }
            }
        }
    }

    GroupBox{
        title:qsTr("Lens盘2起点")
        ColumnLayout{
            RowLayout{
                Label{
                    text: qsTr("X")
                }
                TextField{
                    text: sh_lens_tray_start_point2.X
                    horizontalAlignment: TextInput.AlignHCenter
                    validator: DoubleValidator {
                        decimals: 6
                        notation: DoubleValidator.StandardNotation
                    }
                    onEditingFinished: {
                        sh_lens_tray_start_point2.setX(text)
                    }
                }
                Label{
                    text: qsTr("Y")
                }
                TextField{
                    text: sh_lens_tray_start_point2.Y
                    horizontalAlignment: TextInput.AlignHCenter
                    validator: DoubleValidator {
                        decimals: 6
                        notation: DoubleValidator.StandardNotation
                    }
                    onEditingFinished: {
                        sh_lens_tray_start_point2.setY(text)
                    }
                }
                Button{
                    text:title_move_to
                    width: 40
                    height: 40
                    onClicked: {
                        sh_materialLoaderModule.performHandling(MaterialLoaderModule.LENS_TRAY2_START_POS)
                    }
                }
                Button{
                    text:title_read_encoder
                    width: 40
                    height: 40
                    onClicked: {
                        var x = baseModuleManager.getMotorFeedbackPos(materialPickArmParams.motorXName)
                        var y = baseModuleManager.getMotorFeedbackPos(materialPickArmParams.motorYName)
                        sh_lens_tray_start_point2.setX(x);
                        sh_lens_tray_start_point2.setY(y);
                    }
                }
            }
        }
    }

    GroupBox{
        title:qsTr("Lens盘2操作")
        ColumnLayout{
            RowLayout{
                Label{
                    text: qsTr("目标行")
                }
                TextField{
                    id:t_nrow2
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
                    id:t_ncol2
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
                        sh_lens_tray.setTrayCurrent(t_ncol2.text-1,t_nrow2.text-1,1)
                        sh_materialLoaderModule.performHandling(MaterialLoaderModule.LENS_TRAY2)
                    }
                }
            }
            RowLayout{
                Layout.alignment: Qt.AlignVCenter|Qt.AlignRight
                Button{
                    text:qsTr("lens视觉")
                    width: 40
                    height: 40
                    onClicked: {
                        sh_materialLoaderModule.performHandling(MaterialLoaderModule.LENS_PR)
                    }
                }
                Button{
                    text:qsTr("取lens")
                    width: 40
                    height: 40
                    onClicked: {
                        sh_materialLoaderModule.performHandling(MaterialLoaderModule.LENS_TRAY2|
                                                                MaterialLoaderModule.LENS_PR|
                                                                MaterialLoaderModule.LPA_TO_WORK|
                                                                MaterialLoaderModule.PICK_LENS_FROM_TRAY)
                    }
                }
                Button{
                    text:qsTr("空位视觉")
                    width:40
                    height: 40
                    onClicked:{
                        sh_materialLoaderModule.performHandling(MaterialLoaderModule.LENS_VACANCY_PR)
                    }
                }

                Button{
                    text:qsTr("移动吸头")
                    width:40
                    height: 40
                    onClicked: {
                        sh_lens_tray.setTrayCurrent(t_ncol2.text-1,t_nrow2.text-1,1)
                        sh_materialLoaderModule.performHandling(MaterialLoaderModule.LENS_VACANCY_PR|
                                                                MaterialLoaderModule.LPA_TO_WORK)
                    }
                }

                Button{
                    text:qsTr("放NGlens")
                    width: 40
                    height: 40
                    onClicked: {
                        sh_lens_tray.setTrayCurrent(t_ncol2.text-1,t_nrow2.text-1,1)
                        sh_materialLoaderModule.performHandling(MaterialLoaderModule.LENS_TRAY2|
                                                                MaterialLoaderModule.LENS_VACANCY_PR|
                                                                MaterialLoaderModule.LPA_TO_WORK|
                                                                MaterialLoaderModule.PLACE_NG_LENS_TO_TRAY)
                    }
                }
            }
        }
    }

}
