import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11
    ColumnLayout{
        RowLayout{
            Label{
                text:qsTr("runMode")
            }
            TextField{
                text:lensPickArmModuleParameter.runMode
                horizontalAlignment: TextInput.AlignHCenter
                validator: IntValidator
                onEditingFinished: {
                    lensPickArmModuleParameter.setColumnCount(text)
                }
            }
        }
        RowLayout{
            Label{
                text:qsTr("vcmWorkForce")
            }
            TextField{
                text:lensPickArmModuleParameter.vcmWorkForce
                horizontalAlignment: TextInput.AlignHCenter
                validator: DoubleValidator{
                    decimals: 6
                    notation: DoubleValidator.StandardNotation
                }
                onEditingFinished: {
                    lensPickArmModuleParameter.setVcmWorkForce(text)
                }
            }
            Label{
                text:qsTr("vcmWorkSpeed")
            }
            TextField{
                text:lensPickArmModuleParameter.vcmWorkSpeed
                horizontalAlignment: TextInput.AlignHCenter
                validator: DoubleValidator{
                    decimals: 6
                    notation: DoubleValidator.StandardNotation
                }
                onEditingFinished: {
                    lensPickArmModuleParameter.setVcmWorkSpeed(text)
                }
            }
            Label{
                text:qsTr("vcmMargin")
            }
            TextField{
                text:lensPickArmModuleParameter.vcmMargin
                horizontalAlignment: TextInput.AlignHCenter
                validator: DoubleValidator{
                    decimals: 6
                    notation: DoubleValidator.StandardNotation
                }
                onEditingFinished: {
                    lensPickArmModuleParameter.setVcmMargin(text)
                }
            }
        }
        RowLayout{
            Label{
                text:qsTr("pickLensZ")
            }
            TextField{
                text:lensPickArmModuleParameter.pickLensZ
                horizontalAlignment: TextInput.AlignHCenter
                validator: DoubleValidator{
                    decimals: 6
                    notation:DoubleValidator.StandardNotation
                }
                onEditingFinished: {
                    lensPickArmModuleParameter.setPickLensZ(text)
                }
            }

            Button{
                text:qsTr("取")
                onClicked: {

                }
            }
            Button{
                text:qsTr("放")
                onClicked: {

                }
            }
        }
        RowLayout{
            Label{
                text:qsTr("placeLensZ")
            }
            TextField{
                text:lensPickArmModuleParameter.placeLensZ
                horizontalAlignment: TextInput.AlignHCenter
                validator: DoubleValidator{
                    decimals: 6
                    notation: DoubleValidator.StandardNotation
                }
                onEditingFinished: {
                    lensPickArmModuleParameter.setPlaceLensZ(text)
                }
            }
            Button{
                text:qsTr("取")
                onClicked: {

                }
            }
            Button{
                text:qsTr("放")
                onClicked: {

                }
            }
        }
        RowLayout{
            Label{
                text:qsTr("visonPositionX")
            }
            TextField{
                text:lens_loader_parameter.visonPositionX
                horizontalAlignment: TextInput.AlignHCenter
                validator: DoubleValidator{
                    decimals: 6
                    notation: DoubleValidator.StandardNotation
                }
                onEditingFinished: {
                    lens_loader_parameter.setVisonPositionX(text)
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
