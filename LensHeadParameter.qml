import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11
ColumnLayout {
    RowLayout{
        Label{
            text:qsTr("力限制")
        }
        TextField{
            text:lens_loader_parameter.vcmWorkForce
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator{
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                lens_loader_parameter.setVcmWorkForce(text)
            }
        }
        Label{
            text:qsTr("速度")
        }
        TextField{
            text:lens_loader_parameter.vcmWorkSpeed
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator{
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                lens_loader_parameter.setVcmWorkSpeed(text)
            }
        }
    }
    RowLayout{
        Label{
            text:qsTr("限力区间")
        }
        TextField{
            text:lens_loader_parameter.vcmMargin
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator{
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                lens_loader_parameter.setVcmMargin(text)
            }
        }

        Label{
            text:qsTr("视觉位置X")
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
