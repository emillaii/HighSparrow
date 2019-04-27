import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11
ColumnLayout {
    GroupBox{
        title:"放lens位置"
        ColumnLayout{
            RowLayout{
                Label{
                    text:qsTr("X")
                }
                TextField{
                    text:lut_pr_position1.X
                    horizontalAlignment: TextInput.AlignHCenter
                    validator: DoubleValidator{
                        decimals: 3
                        notation: DoubleValidator.StandardNotation
                    }
                    onEditingFinished: {
                        lut_pr_position1.setX(text)
                    }
                }
                Label{
                    text:qsTr("Y")
                }
                TextField{
                    text:lut_pr_position1.Y
                    horizontalAlignment: TextInput.AlignHCenter
                    validator: DoubleValidator{
                        decimals: 3
                        notation: DoubleValidator.StandardNotation
                    }
                    onEditingFinished: {
                        lut_pr_position1.setY(text)
                    }
                }
                Button{
                    text:title_move_to
                    width: 40
                    height: 40
                    onClicked: {
                        logicManager.lensPickArmMoveToLutPos1()
                    }
                }
                Button{
                    text:title_read_encoder
                    width: 40
                    height: 40
                    onClicked: {

                        var x = baseModuleManager.getMotorFeedbackPos(lensPickArmParams.motorTrayName)
                        var y = baseModuleManager.getMotorFeedbackPos(lensPickArmParams.motorYName)

                        lut_pr_position1.setX(x);
                        lut_pr_position1.setY(y);
                    }
                }
            }

            RowLayout{
                Label{
                    text:qsTr("物料高度")
                }
                TextField{
                    text:lensPickArmModuleParameter.placeLensZ
                    horizontalAlignment: TextInput.AlignHCenter
                    validator: DoubleValidator{
                        decimals: 3
                        notation: DoubleValidator.StandardNotation
                    }
                    onEditingFinished: {
                        lensPickArmModuleParameter.setPlaceLensZ(text)
                    }
                }
                Button{
                    text:qsTr("测高")
                    width: 40
                    height: 40
                    onClicked: {
                        logicManager.lensPickArmLUTMeasureHeight()
                    }
                }
                Button{
                    text:qsTr("放lens")
                    width: 40
                    height: 40
                    onClicked: {
                        logicManager.lensPickArmMoveToPlaceLensToLut()
                    }
                }
            }
        }
    }
    GroupBox{
        title:"取NGlens位置"
        ColumnLayout{
            RowLayout{
                Label{
                    text:qsTr("X")
                }
                TextField{
                    text:lut_pr_position2.X
                    horizontalAlignment: TextInput.AlignHCenter
                    validator: DoubleValidator{
                        decimals: 3
                        notation: DoubleValidator.StandardNotation
                    }
                    onEditingFinished: {
                        lut_pr_position2.setX(text)
                    }
                }
                Label{
                    text:qsTr("Y")
                }
                TextField{
                    text:lut_pr_position2.Y
                    horizontalAlignment: TextInput.AlignHCenter
                    validator: DoubleValidator{
                        decimals: 3
                        notation: DoubleValidator.StandardNotation
                    }
                    onEditingFinished: {
                        lut_pr_position2.setY(text)
                    }
                }
                Button{
                    text:title_move_to
                    width: 40
                    height: 40
                    onClicked: {
                        logicManager.lensPickArmMoveToLutPos2()
                    }
                }
                Button{
                    text:title_read_encoder
                    width: 40
                    height: 40
                    onClicked: {
                        var x = baseModuleManager.getMotorFeedbackPos(lensPickArmParams.motorTrayName)
                        var y = baseModuleManager.getMotorFeedbackPos(lensPickArmParams.motorYName)

                        lut_pr_position2.setX(x);
                        lut_pr_position2.setY(y);
                    }
                }
            }
            RowLayout{
                Layout.alignment: Qt.AlignVCenter|Qt.AlignRight
                Button{
                    text:qsTr("NGLens视觉")
                    width:40
                    height: 40
                    onClicked: {
                        logicManager.lensPickArmLUTPR()
                    }
                }
                Button{
                    text:qsTr("取NGlens")
                    width:40
                    height: 40
                    onClicked: {
                        logicManager.lensPickArmMoveToPickLensFromLut()
                    }
                }
            }
        }
    }
    GroupBox{
        title:"camera位置"
        ColumnLayout{
            RowLayout{
                Label{
                    text:qsTr("X")
                }
                TextField{
                    text:lut_camera_position.X
                    horizontalAlignment: TextInput.AlignHCenter
                    validator: DoubleValidator{
                        decimals: 3
                        notation: DoubleValidator.StandardNotation
                    }
                    onEditingFinished: {
                        lut_camera_position.setX(text)
                    }
                }
                Label{
                    text:qsTr("Y")
                }
                TextField{
                    text:lut_camera_position.Y
                    horizontalAlignment: TextInput.AlignHCenter
                    validator: DoubleValidator{
                        decimals: 3
                        notation: DoubleValidator.StandardNotation
                    }
                    onEditingFinished: {
                        lut_camera_position.setY(text)
                    }
                }
            }
            RowLayout{
                Layout.alignment: Qt.AlignVCenter|Qt.AlignRight
                Button{
                    text:title_move_to
                    onClicked: {
                        logicManager.lensPickArmMoveToUplookCameraPos()
                    }
                }
                Button{
                    text:title_read_encoder
                    onClicked: {
                        var x = baseModuleManager.getMotorFeedbackPos(lensPickArmParams.motorXName)-baseModuleManager.getPROffsetX(lensPickArmModuleParameter.uplookCameraLocaation)
                        var y = baseModuleManager.getMotorFeedbackPos(lensPickArmParams.motorYName)-baseModuleManager.getPROffsetY(lensPickArmModuleParameter.uplookCameraLocaation)
                        lut_picker_position.setX(x)
                        lut_picker_position.setX(y)

                    }
                }
            }
        }
    }
    GroupBox{
        title:"picker位置"
        ColumnLayout{
            RowLayout{
                Label{
                    text:qsTr("X")
                }
                TextField{
                    text:lut_picker_position.X
                    horizontalAlignment: TextInput.AlignHCenter
                    validator: DoubleValidator{
                        decimals: 3
                        notation: DoubleValidator.StandardNotation
                    }
                    onEditingFinished: {
                        lut_picker_position.setX(text)
                    }
                }
                Label{
                    text:qsTr("Y")
                }
                TextField{
                    text:lut_picker_position.Y
                    horizontalAlignment: TextInput.AlignHCenter
                    validator: DoubleValidator{
                        decimals: 3
                        notation: DoubleValidator.StandardNotation
                    }
                    onEditingFinished: {
                        lut_picker_position.setY(text)
                    }
                }
            }
            RowLayout{
                Layout.alignment: Qt.AlignVCenter|Qt.AlignRight
                Button{
                    text:title_move_to
                    onClicked: {
                        logicManager.lensPickArmMoveToUplookPickerPos()
                    }
                }
                Button{
                    text:title_read_encoder
                    onClicked: {
                        var x = baseModuleManager.getMotorFeedbackPos(lensPickArmParams.motorXName)-baseModuleManager.getPROffsetX(lensPickArmModuleParameter.uplookPickLocation)
                        var y = baseModuleManager.getMotorFeedbackPos(lensPickArmParams.motorYName)-baseModuleManager.getPROffsetY(lensPickArmModuleParameter.uplookPickLocation)
                        lut_picker_position.setX(x)
                        lut_picker_position.setX(y)
                    }
                }
            }
        }
    }
}
