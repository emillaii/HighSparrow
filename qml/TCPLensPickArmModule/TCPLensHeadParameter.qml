import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11
ColumnLayout {
    RowLayout{
        Label{
            text:qsTr("力限制")
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
            text:qsTr("速度")
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
    }
    RowLayout{
        Label{
            text:qsTr("限力区间")
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

        Label{
            text:qsTr("视觉位置X")
        }
        TextField{
            text:lensPickArmModuleParameter.visonPositionX
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator{
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                lensPickArmModuleParameter.setVisonPositionX(text)
            }
        }
    }
    RowLayout{
        Label{
            text:qsTr("picker_offset")
        }
    }
    RowLayout{
        Label{
            text:qsTr("X")
        }
        TextField{
            text:picker_offset.X
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator{
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                picker_offset.setX(text)
                lpa_picker_offset.setX(text)
            }
        }
        Label{
            text:qsTr("Y")
        }
        TextField{
            text:picker_offset.Y
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator{
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                picker_offset.setY(text)
                lpa_picker_offset.setY(text)
            }
        }
        Button{
            text: qsTr("测量")
            onClicked: {
//                lensLoaderModule.performLensUpDnLookCalibration
            }
        }
    }
}
