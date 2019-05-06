import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11
ColumnLayout {
    RowLayout{
        Label{
            text:qsTr("sensor高度")
        }
        TextField{
            text:sensorLoaderParameter.placeSensorZ
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator{
                decimals: 3
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                sensorLoaderParameter.setPlaceSensorZ(text)
            }
        }
        Button{
            text:qsTr("测高")
            width: 40
            height: 40
            onClicked: {
                //logicManager.lensPickArmLUTMeasureHeight()
            }
        }
    }
    RowLayout{
        Label{
            text:qsTr("product高度")
        }
        TextField{
            text:sensorLoaderParameter.pickProductZ
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator{
                decimals: 3
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                sensorLoaderParameter.setPickProductZ(text)
            }
        }
        Button{
            text:qsTr("测高")
            width: 40
            height: 40
            onClicked: {
                //logicManager.lensPickArmLUTMeasureHeight()
            }
        }
    }
    RowLayout{
        Label{
            text:qsTr("NG sensor高度")
        }
        TextField{
            text:sensorLoaderParameter.pickNgSensorZ
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator{
                decimals: 3
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                sensorLoaderParameter.setPickNgSensorZ(text)
            }
        }
        Button{
            text:qsTr("测高")
            width: 40
            height: 40
            onClicked: {
                //logicManager.lensPickArmLUTMeasureHeight()
            }
        }
    }
    GroupBox{
        title:"SUT位置1"
        ColumnLayout{
            RowLayout{
                Label{
                    text:qsTr("X")
                }
                TextField{
                    text:sut_pr_position1.X
                    horizontalAlignment: TextInput.AlignHCenter
                    validator: DoubleValidator{
                        decimals: 3
                        notation: DoubleValidator.StandardNotation
                    }
                    onEditingFinished: {
                        sut_pr_position1.setX(text)
                    }
                }
                Label{
                    text:qsTr("Y")
                }
                TextField{
                    text:sut_pr_position1.Y
                    horizontalAlignment: TextInput.AlignHCenter
                    validator: DoubleValidator{
                        decimals: 3
                        notation: DoubleValidator.StandardNotation
                    }
                    onEditingFinished: {
                        sut_pr_position1.setY(text)
                    }
                }
                Button{
                    text:title_move_to
                    width: 40
                    height: 40
                    onClicked: {
                        //logicManager.lensPickArmMoveToLutPos1()
                    }
                }
                Button{
                    text:title_read_encoder
                    width: 40
                    height: 40
                    onClicked: {

                        var x = baseModuleManager.getMotorFeedbackPos(sensorPickArmParams.motorXName)
                        var y = baseModuleManager.getMotorFeedbackPos(sensorPickArmParams.motorYName)

                        sut_pr_position1.setX(x);
                        sut_pr_position1.setY(y);
                    }
                }
            }

            RowLayout{
                Layout.alignment: Qt.AlignVCenter|Qt.AlignRight
                Button{
                    text:qsTr("放sensors")
                    width: 40
                    height: 40
                    onClicked: {
                        //logicManager.lensPickArmMoveToPlaceLensToLut()
                    }
                }
                Button{
                    text:qsTr("取成品")
                    width:40
                    height: 40
                    onClicked: {

                    }
                }
                Button{
                    text:qsTr("取Ng Sensor")
                    width:40
                    height: 40
                    onClicked: {

                    }
                }
            }
        }
    }
    GroupBox{
        title:"SUT 位置2"
        ColumnLayout{
            RowLayout{
                Label{
                    text:qsTr("X")
                }
                TextField{
                    text:sut_pr_position2.X
                    horizontalAlignment: TextInput.AlignHCenter
                    validator: DoubleValidator{
                        decimals: 3
                        notation: DoubleValidator.StandardNotation
                    }
                    onEditingFinished: {
                        sut_pr_position2.setX(text)
                    }
                }
                Label{
                    text:qsTr("Y")
                }
                TextField{
                    text:sut_pr_position2.Y
                    horizontalAlignment: TextInput.AlignHCenter
                    validator: DoubleValidator{
                        decimals: 3
                        notation: DoubleValidator.StandardNotation
                    }
                    onEditingFinished: {
                        sut_pr_position2.setY(text)
                    }
                }
                Button{
                    text:title_move_to
                    width: 40
                    height: 40
                    onClicked: {
                        //logicManager.lensPickArmMoveToLutPos2()
                    }
                }
                Button{
                    text:title_read_encoder
                    width: 40
                    height: 40
                    onClicked: {
                        var x = baseModuleManager.getMotorFeedbackPos(sensorPickArmParams.motorXName)
                        var y = baseModuleManager.getMotorFeedbackPos(sensorPickArmParams.motorYName)

                        sut_pr_position2.setX(x);
                        sut_pr_position2.setY(y);
                    }
                }
            }
            RowLayout{
                Layout.alignment: Qt.AlignVCenter|Qt.AlignRight
                Button{
                    text:qsTr("放sensors")
                    width:40
                    height: 40
                    onClicked: {
                        //logicManager.lensPickArmMoveToPickLensFromLut()
                    }
                }
                Button{
                    text:qsTr("取成品")
                    width:40
                    height: 40
                    onClicked: {

                    }
                }
                Button{
                    text:qsTr("取Ng Sensor")
                    width:40
                    height: 40
                    onClicked: {

                    }
                }
            }
        }
    }
}
