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
                        decimals: 3
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
                        decimals: 3
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
                        logicManager.lensPickArmMoveToTray1StartPos()
                    }
                }
                Button{
                    text:title_read_encoder
                    width: 40
                    height: 40
                    onClicked: {
                        var x = baseModuleManager.getMotorFeedbackPos(m_LTL_X)
                        var y = baseModuleManager.getMotorFeedbackPos(m_LPA_Y)

                        tray_start_point1.setX(x);
                        tray_start_point1.setY(y);
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
                        decimals: 3
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
                        decimals: 3
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

                        logicManager.lensPickArmMoveToTray1EndPos()
                    }
                }
                Button{
                    text:title_read_encoder
                    width: 40
                    height: 40

                    onClicked: {
                        var x = baseModuleManager.getMotorFeedbackPos(m_LTL_X)
                        var y = baseModuleManager.getMotorFeedbackPos(m_LPA_Y)

                        first_tray_end_position.setX(x);
                        first_tray_end_position.setY(y);
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
                        material_tray.setTrayCurrent(t_ncol-1,t_nrow-1,0)
                        logicManager.lensPickArmMoveToTray1Pos()
                    }
                }
            }

            RowLayout{
                Label{
                    text: qsTr("物料高度")
                }
                TextField{
                    text:lensPickArmModuleParameter.pickLensZ
                    validator: DoubleValidator{
                        decimals: 3
                        notation: DoubleValidator.StandardNotation
                    }
                    onEditingFinished: {
                        lensPickArmModuleParameter.setPickLensZ(text)
                    }
                }

                Button{
                    text:qsTr("lensPR")
                    width: 40
                    height: 40
                    onClicked: {

                    }
                }
                Button{
                    text:qsTr("测高")
                    width: 40
                    height: 40
                    onClicked: {

                    }
                }
                Button{
                    text:qsTr("取lens")
                    width: 40
                    height: 40
                    onClicked: {
                        logicManager.lensPickArmMoveToPickLensFromTray1()
                    }
                }
                Button{
                    text:qsTr("放lens")
                    width: 40
                    height: 40
                    onClicked: {
                        logicManager.lensPickArmMoveToPlaceLensToTray1()
                    }
                }
            }
        }
    }
}
