import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11
ColumnLayout {
    RowLayout{
        id:row1
        Label{
            text:qsTr("力限制")
        }
        TextField{
            text:materialLoaderParameter.vcm1PickForce//sensorLoaderParameter.vcmWorkForce
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator{
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                materialLoaderParameter.setVcm1PickForce(text)
                materialLoaderParameter.setVcm2PickForce(text)
            }
        }
        Label{
            text:qsTr("速度")
        }
        TextField{
            text:materialLoaderParameter.vcm1Svel//sensorLoaderParameter.vcmWorkSpeed
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator{
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                //sensorLoaderParameter.setVcmWorkSpeed(text)
                materialLoaderParameter.setVcm1Svel(text)
                materialLoaderParameter.setVcm2Svel(text)
            }
        }
        Label{
            text:qsTr("限力区间")
        }
        TextField{
            text:materialLoaderParameter.vcm1Margin//sensorLoaderParameter.vcmMargin
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator{
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                //sensorLoaderParameter.setVcmMargin(text)
                materialLoaderParameter.setVcm1Margin(text)
                materialLoaderParameter.setVcm2Margin(text)
            }
        }
    }

    RowLayout{
        Label{
            text:qsTr("sensor_picker_offset")
        }
    }
    RowLayout{
        Label{
            text:qsTr("X")
        }
        TextField{
            text:sensor_picker_offset.X
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator{
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                sensor_picker_offset.setX(text)
            }
        }
        Label{
            text:qsTr("Y")
        }
        TextField{
            text:sensor_picker_offset.Y
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator{
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                sensor_picker_offset.setY(text)
            }
        }
        Button{
            text: qsTr("测量")
            onClicked: {
                 //sensorLoaderModule.cameraTipOffsetCalibration(1)
            }
        }
    }
    RowLayout{
        Label{
            text:qsTr("lens_picker_offset")
        }
    }
    RowLayout{
        Label{
            text:qsTr("X")
        }
        TextField{
            text:lens_picker_offset.X
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator{
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                lens_picker_offset.setX(text)
            }
        }
        Label{
            text:qsTr("Y")
        }
        TextField{
            text:lens_picker_offset.Y
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator{
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                lens_picker_offset.setY(text)
            }
        }
        Button{
            text: qsTr("测量")
            onClicked: {
                 //sensorLoaderModule.cameraTipOffsetCalibration(2)
            }
        }
    }
}
