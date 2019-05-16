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
            text:sensorLoaderParameter.vcmWorkForce
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator{
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                sensorLoaderParameter.setVcmWorkForce(text)
            }
        }
        Label{
            text:qsTr("速度")
        }
        TextField{
            text:sensorLoaderParameter.vcmWorkSpeed
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator{
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                sensorLoaderParameter.setVcmWorkSpeed(text)
            }
        }
        Label{
            text:qsTr("限力区间")
        }
        TextField{
            text:sensorLoaderParameter.vcmMargin
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator{
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                sensorLoaderParameter.setVcmMargin(text)
            }
        }
    }
    RowLayout{
        Label{
            text:qsTr("z_offset")
        }
        TextField{
            text:sensorLoaderParameter.zOffset
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator{
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                sensorLoaderParameter.setZOffset(text)
            }
        }
        Button{
            text: qsTr("测量")
            onClicked: {
                sensorLoaderModule.performHandling(SensorLoaderModule.MEASURE_Z_OFFSET);
            }
        }
    }

    RowLayout{
        Label{
            text:qsTr("pick1_offset")
        }
    }
    RowLayout{
        Label{
            text:qsTr("X")
        }
        TextField{
            text:pick1_offset.X
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator{
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                pick1_offset.setX(text)
            }
        }
        Label{
            text:qsTr("Y")
        }
        TextField{
            text:pick1_offset.Y
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator{
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                pick1_offset.setY(text)
            }
        }
        Button{
            text: qsTr("测量")
            onClicked: {
                 sensorLoaderModule.cameraTipOffsetCalibration(1)
            }
        }
    }
    RowLayout{
        Label{
            text:qsTr("pick2_offset")
        }
    }
    RowLayout{
        Label{
            text:qsTr("X")
        }
        TextField{
            text:pick2_offset.X
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator{
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                pick2_offset.setX(text)
            }
        }
        Label{
            text:qsTr("Y")
        }
        TextField{
            text:pick2_offset.Y
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator{
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                pick2_offset.setY(text)
            }
        }
        Button{
            text: qsTr("测量")
            onClicked: {
                 sensorLoaderModule.cameraTipOffsetCalibration(2)
            }
        }
    }
}
