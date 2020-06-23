import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11
import SomeLib 1.1
ColumnLayout {
    GroupBox{
        title:"放lens位置"
        ColumnLayout{
            RowLayout{
                Label{
                    text:qsTr("X")
                }
                TextField{
                    text:lut_pr_position1.X
                    horizontalAlignment: TextInput.AlignHCenter
                    validator: DoubleValidator{
                        decimals: 6
                        notation: DoubleValidator.StandardNotation
                    }
                    onEditingFinished: {
                        lut_pr_position1.setX(text)
                    }
                }
                Label{
                    text:qsTr("Y")
                }
                TextField{
                    text:lut_pr_position1.Y
                    horizontalAlignment: TextInput.AlignHCenter
                    validator: DoubleValidator{
                        decimals: 6
                        notation: DoubleValidator.StandardNotation
                    }
                    onEditingFinished: {
                        lut_pr_position1.setY(text)
                    }
                }
                Button{
                    text:title_move_to
                    width: 40
                    height: 40
                    onClicked: {
                        //logicManager.lensPickArmMoveToLutPos1()
//                        lensLoaderModule.performHandling(LensLoaderModule.LUT_POS1)
                        logicManager.performHandling(lens_loader_parameter.moduleName,LensLoaderModule.LUT_POS1)
                    }
                }
                Button{
                    text:title_read_encoder
                    width: 40
                    height: 40
                    onClicked: {

                        var x = baseModuleManager.getMotorFeedbackPos(lensPickArmParams.motorXName)
                        var y = baseModuleManager.getMotorFeedbackPos(lensPickArmParams.motorYName)

                        lut_pr_position1.setX(x);
                        lut_pr_position1.setY(y);
                    }
                }
            }

            RowLayout{
                Label{
                    text:qsTr("物料高度")
                }
                TextField{
                    text:lens_loader_parameter.placeLensZ
                    horizontalAlignment: TextInput.AlignHCenter
                    validator: DoubleValidator{
                        decimals: 6
                        notation: DoubleValidator.StandardNotation
                    }
                    onEditingFinished: {
                        lens_loader_parameter.setPlaceLensZ(text)
                    }
                }
                Label{
                    text:qsTr("Theta")
                }
                TextField{
                    text:lens_loader_parameter.placeTheta
                    horizontalAlignment: TextInput.AlignHCenter
                    validator: DoubleValidator{
                        decimals: 6
                        notation: DoubleValidator.StandardNotation
                    }
                    onEditingFinished: {
                        lens_loader_parameter.setPlaceTheta(text)
                    }
                }
                Button{
                    text:qsTr("测高")
                    width: 40
                    height: 40
                    onClicked: {
                        //logicManager.lensPickArmLUTMeasureHeight()
                        //lensLoaderModule.performHandling(LensLoaderModule.MeasureLensInLUT)
                        logicManager.performHandling(lens_loader_parameter.moduleName,LensLoaderModule.MeasureLensInLUT)
                    }
                }
                Button{
                    text:qsTr("放lens")
                    width: 40
                    height: 40
                    onClicked: {
                        //logicManager.lensPickArmMoveToPlaceLensToLut()
//                        lensLoaderModule.performHandling(LensLoaderModule.LUT_POS1+
//                                                         LensLoaderModule.RESET_PR+
//                                                         LensLoaderModule.ToWork+
//                                                         LensLoaderModule.PLACE_LENS_TO_LUT)
                        logicManager.performHandling(lens_loader_parameter.moduleName,LensLoaderModule.LUT_POS1+
                                                     LensLoaderModule.RESET_PR+
                                                     LensLoaderModule.ToWork+
                                                     LensLoaderModule.PLACE_LENS_TO_LUT)
                    }
                }
            }
        }
    }
    GroupBox{
        title:"取NGlens位置"
        ColumnLayout{
            RowLayout{
                Label{
                    text:qsTr("X")
                }
                TextField{
                    text:lut_pr_position2.X
                    horizontalAlignment: TextInput.AlignHCenter
                    validator: DoubleValidator{
                        decimals: 6
                        notation: DoubleValidator.StandardNotation
                    }
                    onEditingFinished: {
                        lut_pr_position2.setX(text)
                    }
                }
                Label{
                    text:qsTr("Y")
                }
                TextField{
                    text:lut_pr_position2.Y
                    horizontalAlignment: TextInput.AlignHCenter
                    validator: DoubleValidator{
                        decimals: 6
                        notation: DoubleValidator.StandardNotation
                    }
                    onEditingFinished: {
                        lut_pr_position2.setY(text)
                    }
                }
                Button{
                    text:title_move_to
                    width: 40
                    height: 40
                    onClicked: {
                        //logicManager.lensPickArmMoveToLutPos2()
//                        lensLoaderModule.performHandling(LensLoaderModule.LUT_POS2)
                        logicManager.performHandling(lens_loader_parameter.moduleName,LensLoaderModule.LUT_POS2)
                    }
                }
                Button{
                    text:title_read_encoder
                    width: 40
                    height: 40
                    onClicked: {
                        var x = baseModuleManager.getMotorFeedbackPos(lensPickArmParams.motorXName)
                        var y = baseModuleManager.getMotorFeedbackPos(lensPickArmParams.motorYName)

                        lut_pr_position2.setX(x);
                        lut_pr_position2.setY(y);
                    }
                }
            }
            RowLayout{
                Layout.alignment: Qt.AlignVCenter|Qt.AlignRight
                Button{
                    text:qsTr("NGLens视觉")
                    width:40
                    height: 40
                    onClicked: {
                        //logicManager.lensPickArmLUTPR()
//                        lensLoaderModule.performHandling(LensLoaderModule.LUT_PR+
//                                                         LensLoaderModule.ToWork)
                        logicManager.performHandling(lens_loader_parameter.moduleName,LensLoaderModule.LUT_LENS_PR+
                                                     LensLoaderModule.ToWork)
                    }
                }
                Button{
                    text:qsTr("取NGlens")
                    width:40
                    height: 40
                    onClicked: {
                        //logicManager.lensPickArmMoveToPickLensFromLut()
//                        lensLoaderModule.performHandling(LensLoaderModule.LUT_POS2+
//                                                         LensLoaderModule.LUT_LENS_PR+
//                                                         LensLoaderModule.ToWork+
//                                                         LensLoaderModule.PICK_NG_LENS_FROM_LUT2)
                        logicManager.performHandling(lens_loader_parameter.moduleName,LensLoaderModule.LUT_POS2+
                                                     LensLoaderModule.LUT_LENS_PR+
                                                     LensLoaderModule.ToWork+
                                                     LensLoaderModule.PICK_NG_LENS_FROM_LUT2)
                    }
                }
            }
        }
    }
    GroupBox{
        title:"download camera位置"
        ColumnLayout{
            RowLayout{
                Label{
                    text:qsTr("X")
                }
                TextField{
                    text:lut_camera_position.X
                    horizontalAlignment: TextInput.AlignHCenter
                    validator: DoubleValidator{
                        decimals: 6
                        notation: DoubleValidator.StandardNotation
                    }
                    onEditingFinished: {
                        lut_camera_position.setX(text)
                    }
                }
                Label{
                    text:qsTr("Y")
                }
                TextField{
                    text:lut_camera_position.Y
                    horizontalAlignment: TextInput.AlignHCenter
                    validator: DoubleValidator{
                        decimals: 6
                        notation: DoubleValidator.StandardNotation
                    }
                    onEditingFinished: {
                        lut_camera_position.setY(text)
                    }
                }
            }
            RowLayout{
                Layout.alignment: Qt.AlignVCenter|Qt.AlignRight
                Button{
                    text:title_move_to
                    onClicked: {
                        //logicManager.lensPickArmMoveToUpdownlookDownPos()
                        //lensLoaderModule.performHandling(LensLoaderModule.UPDOWNLOOK_DOWN_POS)
                        logicManager.performHandling(lens_loader_parameter.moduleName,LensLoaderModule.UPDOWNLOOK_DOWN_POS)
                    }
                }
                Button{
                    text:title_read_encoder
                    onClicked: {
                        var x = baseModuleManager.getMotorFeedbackPos(lensPickArmParams.motorXName)-baseModuleManager.getPROffsetX(lensPickArmModuleParameter.lpaUpdownlookDownLocationName)
                        var y = baseModuleManager.getMotorFeedbackPos(lensPickArmParams.motorYName)-baseModuleManager.getPROffsetY(lensPickArmModuleParameter.lpaUpdownlookDownLocationName)
                        lut_camera_position.setX(x)
                        lut_camera_position.setY(y)

                    }
                }
            }
        }
    }
    GroupBox{
        title:"Updownlook Lens Calibration"
        ColumnLayout{
            RowLayout{
                Layout.alignment: Qt.AlignVCenter|Qt.AlignRight
                Button{
                    text: qsTr("Start Calibratio")
                    onClicked: {
                        logicManager.performLensUpDnLookCalibration()
                    }
                }
            }
        }
    }
    GroupBox{
        title:"upload picker位置"
        ColumnLayout{
            RowLayout{
                Label{
                    text:qsTr("X")
                }
                TextField{
                    text:lut_picker_position.X
                    horizontalAlignment: TextInput.AlignHCenter
                    validator: DoubleValidator{
                        decimals: 6
                        notation: DoubleValidator.StandardNotation
                    }
                    onEditingFinished: {
                        lut_picker_position.setX(text)
                    }
                }
                Label{
                    text:qsTr("Y")
                }
                TextField{
                    text:lut_picker_position.Y
                    horizontalAlignment: TextInput.AlignHCenter
                    validator: DoubleValidator{
                        decimals: 6
                        notation: DoubleValidator.StandardNotation
                    }
                    onEditingFinished: {
                        lut_picker_position.setY(text)
                    }
                }
            }
            RowLayout{
                Layout.alignment: Qt.AlignVCenter|Qt.AlignRight
                Button{
                    text:title_move_to
                    onClicked: {
                        //logicManager.lensPickArmMoveToUpdownlookUpPos()
//                        lensLoaderModule.performHandling(LensLoaderModule.UPDOWNLOOK_UP_POS)
                        logicManager.performHandling(lens_loader_parameter.moduleName,LensLoaderModule.UPDOWNLOOK_UP_POS)
                    }
                }
                Button{
                    text:title_read_encoder
                    onClicked: {
                        var x = baseModuleManager.getMotorFeedbackPos(lensPickArmParams.motorXName)
                        var y = baseModuleManager.getMotorFeedbackPos(lensPickArmParams.motorYName)
                        lut_picker_position.setX(x)
                        lut_picker_position.setY(y)
                    }
                }
            }
        }
    }
}
