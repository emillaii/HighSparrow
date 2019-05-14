import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11

    ColumnLayout{
        RowLayout{
            Label{
                text:qsTr("lut_pr_position1_X")
            }
            TextField{
                text:lut_pr_position1.X
                validator: DoubleValidator{
                    decimals: 6
                    notation: DoubleValidator.StandardNotation
                }
                onEditingFinished: {
                    lut_pr_position1.setX(text)
                }
            }
            Label{
                text:qsTr("lut_pr_position1_Y")
            }
            TextField{
                text:lut_pr_position1.Y
                validator: DoubleValidator{
                    decimals: 6
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
                text:qsTr("lut_pr_position2_X")
            }
            TextField{
                text:lut_pr_position2.X
                validator: DoubleValidator{
                    decimals: 6
                    notation: DoubleValidator.StandardNotation
                }
                onEditingFinished: {
                    lut_pr_position2.setX(text)
                }
            }
            Label{
                text:qsTr("lut_pr_position2_Y")
            }
            TextField{
                text:lut_pr_position2.Y
                validator: DoubleValidator{
                    decimals: 6
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
                text:qsTr("lut_camera_position_X")
            }
            TextField{
                text:lut_camera_position.X
                validator: DoubleValidator{
                    decimals: 6
                    notation: DoubleValidator.StandardNotation
                }
                onEditingFinished: {
                    lut_camera_position.setX(text)
                }
            }
            Label{
                text:qsTr("lut_camera_position_Y")
            }
            TextField{
                text:lut_camera_position.Y
                validator: DoubleValidator{
                    decimals: 6
                    notation: DoubleValidator.StandardNotation
                }
                onEditingFinished: {
                    lut_camera_position.setY(text)
                }
            }
            Button{
                text:title_move_to
                width: 40
                height: 40
                onClicked: {

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
                text:qsTr("lut_picker_position_X")
            }
            TextField{
                text:lut_picker_position.X
                validator: DoubleValidator{
                    decimals: 6
                    notation: DoubleValidator.StandardNotation
                }
                onEditingFinished: {
                    lut_picker_position.setX(text)
                }
            }
            Label{
                text:qsTr("lut_picker_position_Y")
            }
            TextField{
                text:lut_picker_position.Y
                validator: DoubleValidator{
                    decimals: 6
                    notation: DoubleValidator.StandardNotation
                }
                onEditingFinished: {
                    lut_picker_position.setY(text)
                }
            }
            Button{
                text:title_move_to
                width: 40
                height: 40
                onClicked: {

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
    }

