import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11
import SomeLib 1.1

ColumnLayout {
    GroupBox{
        //title:"SPA standby position"
        ColumnLayout{
            RowLayout{
                Label{
                    text:qsTr("SPA standby： X")
                }
                TextField{
                    text:spa_standby_position.X
                    horizontalAlignment: TextInput.AlignHCenter
                    validator: DoubleValidator{
                        decimals: 6
                        notation: DoubleValidator.StandardNotation
                    }
                    onEditingFinished: {
                        spa_standby_position.setX(text)
                    }
                }
                Label{
                    text:qsTr("Y")
                }
                TextField{
                    text:spa_standby_position.Y
                    horizontalAlignment: TextInput.AlignHCenter
                    validator: DoubleValidator{
                        decimals: 6
                        notation: DoubleValidator.StandardNotation
                    }
                    onEditingFinished: {
                        spa_standby_position.setY(text)
                    }
                }
                Button{
                    text:title_move_to
                    width: 40
                    height: 40
                    onClicked: {
//                        sensorLoaderModule.performHandling(SensorLoaderModule.SPA_STANDBY_POS)
                        logicManager.performHandling(sensorLoaderParameter.moduleName,SensorLoaderModule.SPA_STANDBY_POS)
                    }
                }
                Button{
                    text:title_read_encoder
                    width: 40
                    height: 40
                    onClicked: {

                        var x = baseModuleManager.getMotorFeedbackPos(sensorPickArmParams.motorXName)
                        var y = baseModuleManager.getMotorFeedbackPos(sensorPickArmParams.motorYName)

                        spa_standby_position.setX(x);
                        spa_standby_position.setY(y);
                    }
                }
            }
        }
    }

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
            text:qsTr("成品力限制")
        }
        TextField{
            text:sensorLoaderParameter.pickProductForce
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator{
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                sensorLoaderParameter.setPickProductForce(text)
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
                logicManager.performHandling(sensorLoaderParameter.moduleName,SensorLoaderModule.MEASURE_Z_OFFSET)
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
    RowLayout{
        Label{
            text:qsTr("吸头1吸料角度")
        }
        TextField{
            text:sensorLoaderParameter.picker1PickTheta
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator{
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                sensorLoaderParameter.setPicker1PickTheta(text)
            }
        }
        Label{
            text:qsTr("吸头1放料角度")
        }
        TextField{
            text:sensorLoaderParameter.picker1PlaceTheta
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator{
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                sensorLoaderParameter.setPicker1PlaceTheta(text)
            }
        }
    }
    RowLayout{
        Label{
            text:qsTr("吸头2吸料角度")
        }
        TextField{
            text:sensorLoaderParameter.picker2PickTheta
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator{
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                sensorLoaderParameter.setPicker2PickTheta(text)
            }
        }
        Label{
            text:qsTr("吸头2放料角度")
        }
        TextField{
            text:sensorLoaderParameter.picker2PlaceTheta
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator{
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                sensorLoaderParameter.setPicker2PlaceTheta(text)
            }
        }
    }
}
