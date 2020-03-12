import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11

ColumnLayout {
    RowLayout{
        id:row1
        Label{
            text:qsTr("头1：力限制")
        }
        TextField{
            text:materialLoaderParameter.vcm1PickForce
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator{
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                materialLoaderParameter.setVcm1PickForce(text)
            }
        }
        Label{
            text:qsTr("速度")
        }
        TextField{
            text:materialLoaderParameter.vcm1Svel
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator{
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                materialLoaderParameter.setVcm1Svel(text)
            }
        }
        Label{
            text:qsTr("限力区间")
        }
        TextField{
            text:materialLoaderParameter.vcm1Margin
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator{
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                materialLoaderParameter.setVcm1Margin(text)
            }
        }
    }

    RowLayout{
        id:row2
        Label{
            text:qsTr("头2：力限制")
        }
        TextField{
            text:materialLoaderParameter.vcm2PickForce
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator{
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                materialLoaderParameter.setVcm2PickForce(text)
            }
        }
        Label{
            text:qsTr("速度")
        }
        TextField{
            text:materialLoaderParameter.vcm2Svel
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator{
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                materialLoaderParameter.setVcm2Svel(text)
            }
        }
        Label{
            text:qsTr("限力区间")
        }
        TextField{
            text:materialLoaderParameter.vcm2Margin
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator{
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                materialLoaderParameter.setVcm2Margin(text)
            }
        }
    }
        ColumnLayout{
            RowLayout{
                Label{
                    text:qsTr("相机到1号吸嘴偏移 X:")
                }
                TextField{
                    text:sh_camera_to_picker1_offset.X
                    horizontalAlignment: TextInput.AlignHCenter
                    validator: DoubleValidator{
                        decimals: 6
                        notation: DoubleValidator.StandardNotation
                    }
                    onEditingFinished: {
                        sh_camera_to_picker1_offset.setX(text)
                    }
                }
                Label{
                    text:qsTr("Y")
                }
                TextField{
                    text:sh_camera_to_picker1_offset.Y
                    horizontalAlignment: TextInput.AlignHCenter
                    validator: DoubleValidator{
                        decimals: 6
                        notation: DoubleValidator.StandardNotation
                    }
                    onEditingFinished: {
                        sh_camera_to_picker1_offset.setY(text)
                    }
                }
                Button{
                    text:qsTr("测量")
                    width: 40
                    height: 40
                    onClicked: {
                      sh_materialLoaderModule.cameraTipOffsetCalibration(0)
                    }
                    enabled: userManagement.currentAuthority >= 2 //At least engineer authority
                }
            }
        }
        ColumnLayout{
            RowLayout{
                Label{
                    text:qsTr("相机到2号吸嘴偏移 X:")
                }
                TextField{
                    text:sh_camera_to_picker2_offset.X
                    horizontalAlignment: TextInput.AlignHCenter
                    validator: DoubleValidator{
                        decimals: 6
                        notation: DoubleValidator.StandardNotation
                    }
                    onEditingFinished: {
                        sh_camera_to_picker2_offset.setX(text)
                    }
                }
                Label{
                    text:qsTr("Y")
                }
                TextField{
                    text:sh_camera_to_picker2_offset.Y
                    horizontalAlignment: TextInput.AlignHCenter
                    validator: DoubleValidator{
                        decimals: 6
                        notation: DoubleValidator.StandardNotation
                    }
                    onEditingFinished: {
                        sh_camera_to_picker2_offset.setY(text)
                    }
                }
                Button{
                    text:qsTr("测量")
                    width: 40
                    height: 40
                    onClicked: {
                        sh_materialLoaderModule.cameraTipOffsetCalibration(1)
                    }
                    enabled: userManagement.currentAuthority >= 2 //At least engineer authority
                }
            }
        }
    }
//}
