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
                decimals: 3
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
                decimals: 3
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                lensPickArmModuleParameter.setVcmWorkSpeed(text)
            }
        }
        Label{
            text:qsTr("限力空间")
        }
        TextField{
            text:lensPickArmModuleParameter.vcmMargin
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator{
                decimals: 3
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                lensPickArmModuleParameter.setVcmMargin(text)
            }
        }
    }
}
