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

                    }
                }
            }
            RowLayout{
                Button{
                    text:qsTr("视觉")
                    width:40
                    height: 40
                    onClicked: {

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
}
