import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11
ColumnLayout {
    RowLayout{
        Label{
            text:qsTr("力限制")
        }
        TextField{
            text:tcpLensPickArmModuleParameter.vcmWorkForce
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator{
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                tcpLensPickArmModuleParameter.setVcmWorkForce(text)
            }
        }
        Label{
            text:qsTr("速度")
        }
        TextField{
            text:tcpLensPickArmModuleParameter.vcmWorkSpeed
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator{
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                tcpLensPickArmModuleParameter.setVcmWorkSpeed(text)
            }
        }
    }
    RowLayout{
        Label{
            text:qsTr("限力区间")
        }
        TextField{
            text:tcpLensPickArmModuleParameter.vcmMargin
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator{
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                tcpLensPickArmModuleParameter.setVcmMargin(text)
            }
        }

        Label{
            text:qsTr("视觉位置X")
        }
        TextField{
            text:tcpLensPickArmModuleParameter.visonPositionX
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator{
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                tcpLensPickArmModuleParameter.setVisonPositionX(text)
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
                tcp_picker_offset.setX(text)
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
                tcp_picker_offset.setY(text)
                lpa_picker_offset.setY(text)
            }
        }
        Button{
            text: qsTr("测量")
            onClicked: {
            }
        }
    }
}
