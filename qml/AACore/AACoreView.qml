﻿import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11
import FileContentItem 1.0
import QtQuick.Dialogs 1.2
import AACoreNew 1.1

ScrollView {
    width: parent.width
    contentWidth: 1920

    RowLayout  {
        anchors.fill: parent
        spacing: 10

        ColumnLayout{
            Layout.alignment: Qt.AlignTop

            GroupBox{
                id: coreParamGroupBox
                title: qsTr("AA Core Parameters")
                ColumnLayout {
                    RowLayout{
                        Button{
                            text: qsTr("加载")
                            onClicked: {
                                baseModuleManager.loadAACoreParameter()
                            }
                        }
                    }

                    RowLayout {
                        Label {
                            text: qsTr("Selected Peak Profile: ")
                        }
                        ComboBox {
                            id: aaCoreProfile
                            Layout.preferredWidth: 300
                            model: [ "Best CC", "Best Layer1", "Best Layer2", "Best Layer3" ]
                            currentIndex: aaCoreParams.peakProfile
                            onCurrentIndexChanged: {
                                aaCoreParams.setPeakProfile(currentIndex)
                            }
                        }
                    }
                    RowLayout {
                        CheckBox {
                            text: qsTr("使能zpeak系数:")
                            checked: aaCoreParams.enableZpeakCoefficient
                            onClicked: {
                                aaCoreParams.setEnableZpeakCoefficient(checked)
                            }
                        }
                        Label {
                            text: qsTr("CC")
                        }
                        TextField {
                            text: aaCoreParams.zpeakccCoefficient
                            horizontalAlignment: TextInput.AlignHCenter

                            validator: DoubleValidator {
                                decimals: 2
                                notation: DoubleValidator.StandardNotation

                            }
                            onEditingFinished: {
                                aaCoreParams.setZpeakccCoefficient(text)
                            }
                        }
                        Label {
                            text: qsTr("Layer1")
                        }
                        TextField {
                            text: aaCoreParams.zpeakL1Coefficient
                            horizontalAlignment: TextInput.AlignHCenter
                            validator: DoubleValidator {
                                decimals: 2
                                notation: DoubleValidator.StandardNotation
                            }
                            onEditingFinished: {
                                aaCoreParams.setZpeakL1Coefficient(text)
                            }
                        }
                        Label {
                            text: qsTr("Layer2")
                        }
                        TextField {
                            text: aaCoreParams.zpeakL2Coefficient
                            horizontalAlignment: TextInput.AlignHCenter
                            validator: DoubleValidator {
                                decimals: 2
                                notation: DoubleValidator.StandardNotation
                            }
                            onEditingFinished: {
                                aaCoreParams.setZpeakL2Coefficient(text)
                            }
                        }
                        Label {
                            text: qsTr("Layer3")
                        }
                        TextField {
                            text: aaCoreParams.zpeakL3Coefficient
                            horizontalAlignment: TextInput.AlignHCenter
                            validator: DoubleValidator {
                                decimals: 2
                                notation: DoubleValidator.StandardNotation
                            }
                            onEditingFinished: {
                                aaCoreParams.setZpeakL3Coefficient(text)
                            }
                        }
                    }
                    RowLayout {

                        Label {
                            text: qsTr("MinDev")
                        }
                        TextField {
                            text: aaCoreParams.minDev
                            horizontalAlignment: TextInput.AlignHCenter
                            validator: DoubleValidator {
                                decimals: 2
                                notation: DoubleValidator.StandardNotation
                            }
                            onEditingFinished: {
                                aaCoreParams.setMinDev(text)
                            }
                        }
                        Label {
                            text: qsTr("MaxDev")
                        }
                        TextField {
                            text: aaCoreParams.maxDev
                            horizontalAlignment: TextInput.AlignHCenter
                            validator: DoubleValidator {
                                decimals: 2
                                notation: DoubleValidator.StandardNotation
                            }
                            onEditingFinished: {
                                aaCoreParams.setMaxDev(text)
                            }
                        }
                        Label {
                            text: qsTr("L1 ZPeak Diff")
                        }
                        TextField {
                            text: aaCoreParams.zPeakDiffL1Max
                            horizontalAlignment: TextInput.AlignHCenter
                            validator: DoubleValidator {
                                decimals: 2
                                notation: DoubleValidator.StandardNotation
                            }
                            onEditingFinished: {
                                aaCoreParams.setZPeakDiffL1Max(text)
                            }
                        }
                        Label {
                            text: qsTr("L2 ZPeak Diff")
                        }
                        TextField {
                            text: aaCoreParams.zPeakDiffL2Max
                            horizontalAlignment: TextInput.AlignHCenter
                            validator: DoubleValidator {
                                decimals: 2
                                notation: DoubleValidator.StandardNotation
                            }
                            onEditingFinished: {
                                aaCoreParams.setZPeakDiffL2Max(text)
                            }
                        }
                        Label {
                            text: qsTr("L3 ZPeak Diff")
                        }
                        TextField {
                            text: aaCoreParams.zPeakDiffL3Max
                            horizontalAlignment: TextInput.AlignHCenter
                            validator: DoubleValidator {
                                decimals: 2
                                notation: DoubleValidator.StandardNotation
                            }
                            onEditingFinished: {
                                aaCoreParams.setZPeakDiffL3Max(text)
                            }
                        }
                    }
                    RowLayout {
                        Label {
                            text: qsTr("CC-L1MinDev")
                        }
                        TextField {
                            text: aaCoreParams.CCL1MinDev
                            horizontalAlignment: TextInput.AlignHCenter
                            validator: DoubleValidator {
                                decimals: 2
                                notation: DoubleValidator.StandardNotation
                            }
                            onEditingFinished: {
                                aaCoreParams.setCCL1MinDev(text)
                            }
                        }
                        Label {
                            text: qsTr("CC-L1MaxDev")
                        }
                        TextField {
                            text: aaCoreParams.CCL1MaxDev
                            horizontalAlignment: TextInput.AlignHCenter
                            validator: DoubleValidator {
                                decimals: 2
                                notation: DoubleValidator.StandardNotation
                            }
                            onEditingFinished: {
                                aaCoreParams.setCCL1MaxDev(text)
                            }
                        }
                        Label {
                            text: qsTr("CC-L2MinDev")
                        }
                        TextField {
                            text: aaCoreParams.CCL2MinDev
                            horizontalAlignment: TextInput.AlignHCenter
                            validator: DoubleValidator {
                                decimals: 2
                                notation: DoubleValidator.StandardNotation
                            }
                            onEditingFinished: {
                                aaCoreParams.setCCL2MinDev(text)
                            }
                        }
                        Label {
                            text: qsTr("CC-L2MaxDev")
                        }
                        TextField {
                            text: aaCoreParams.CCL2MaxDev
                            horizontalAlignment: TextInput.AlignHCenter
                            validator: DoubleValidator {
                                decimals: 2
                                notation: DoubleValidator.StandardNotation
                            }
                            onEditingFinished: {
                                aaCoreParams.setCCL2MaxDev(text)
                            }
                        }
                        Label {
                            text: qsTr("CC-L3MinDev")
                        }
                        TextField {
                            text: aaCoreParams.CCL3MinDev
                            horizontalAlignment: TextInput.AlignHCenter
                            validator: DoubleValidator {
                                decimals: 2
                                notation: DoubleValidator.StandardNotation
                            }
                            onEditingFinished: {
                                aaCoreParams.setCCL3MinDev(text)
                            }
                        }
                        Label {
                            text: qsTr("CC-L3MaxDev")
                        }
                        TextField {
                            text: aaCoreParams.CCL3MaxDev
                            horizontalAlignment: TextInput.AlignHCenter
                            validator: DoubleValidator {
                                decimals: 2
                                notation: DoubleValidator.StandardNotation
                            }
                            onEditingFinished: {
                                aaCoreParams.setCCL3MaxDev(text)
                            }
                        }
                    }
                    RowLayout {
                        CheckBox{
                            text: qsTr("启用Lens VCM")
                            checked: aaCoreParams.enableLensVcm
                            onClicked:
                            {
                                aaCoreParams.setEnableLensVcm(checked)
                            }
                        }
                        Button{
                            text:qsTr("初始化")
                            onClicked: {
                                //                                aaNewCore.performHandling(AACoreNew.INIT_VCM,0)
                                logicManager.performHandling(aaCoreParams.moduleName,AACoreNew.INIT_VCM)
                            }
                        }
                        Label {
                            text: qsTr("Lens AA位置")
                        }
                        TextField {
                            text: aaCoreParams.lensVcmWorkPosition
                            horizontalAlignment: TextInput.AlignHCenter
                            validator: DoubleValidator {
                                decimals: 2
                                notation: DoubleValidator.StandardNotation
                            }
                            onEditingFinished: {
                                aaCoreParams.setLensVcmWorkPosition(text)
                            }
                        }
                        Button{
                            text:qsTr("移动")
                            onClicked: {
                                //                                aaNewCore.performHandling(AACoreNew.LENS_VCM_POS,0)
                                logicManager.performHandling(aaCoreParams.moduleName,AACoreNew.LENS_VCM_POS)
                            }
                        }
                    }
                    RowLayout {
                        Label {
                            text: qsTr("EFL")
                        }
                        TextField {
                            text: aaCoreParams.EFL
                            horizontalAlignment: TextInput.AlignHCenter
                            validator: DoubleValidator {
                                decimals: 6
                                notation: DoubleValidator.StandardNotation
                            }
                            onEditingFinished: {
                                aaCoreParams.setEFL(text)
                            }
                        }
                        Label {
                            text: qsTr("ROI Ratio")
                        }
                        TextField {
                            text: aaCoreParams.ROIRatio
                            horizontalAlignment: TextInput.AlignHCenter
                            validator: DoubleValidator {
                                decimals: 6
                                notation: DoubleValidator.StandardNotation
                            }
                            onEditingFinished: {
                                aaCoreParams.setROIRatio(text)
                            }
                        }
                        Label {
                            text: qsTr("Max Intensity")
                        }
                        TextField {
                            text: aaCoreParams.MaxIntensity
                            horizontalAlignment: TextInput.AlignHCenter
                            validator: DoubleValidator {
                                decimals: 6
                                notation: DoubleValidator.StandardNotation
                            }
                            onEditingFinished: {
                                aaCoreParams.setMaxIntensity(text)
                            }
                        }
                    }
                    RowLayout {
                        Label {
                            text: qsTr("Min Pattern Area")
                        }
                        TextField {
                            text: aaCoreParams.MinArea
                            horizontalAlignment: TextInput.AlignHCenter
                            validator: DoubleValidator {
                                decimals: 6
                                notation: DoubleValidator.StandardNotation
                            }
                            onEditingFinished: {
                                aaCoreParams.setMinArea(text)
                            }
                        }
                        Label {
                            text: qsTr("Max Pattern Area")
                        }
                        TextField {
                            text: aaCoreParams.MaxArea
                            horizontalAlignment: TextInput.AlignHCenter
                            validator: DoubleValidator {
                                decimals: 6
                                notation: DoubleValidator.StandardNotation
                            }
                            onEditingFinished: {
                                aaCoreParams.setMaxArea(text)
                            }
                        }
                    }
                    RowLayout {
                        Label {
                            text: qsTr("Sensor X Ratio")
                        }
                        TextField {
                            text: aaCoreParams.SensorXRatio
                            horizontalAlignment: TextInput.AlignHCenter
                            validator: DoubleValidator {
                                decimals: 6
                                notation: DoubleValidator.StandardNotation
                            }
                            onEditingFinished: {
                                aaCoreParams.setSensorXRatio(text)
                            }
                        }
                        Label {
                            text: qsTr("Sensor Y Ratio")
                        }
                        TextField {
                            text: aaCoreParams.SensorYRatio
                            horizontalAlignment: TextInput.AlignHCenter
                            validator: DoubleValidator {
                                decimals: 6
                                notation: DoubleValidator.StandardNotation
                            }
                            onEditingFinished: {
                                aaCoreParams.setSensorYRatio(text)
                            }
                        }
                    }
                    RowLayout {
                        Label {
                            text: qsTr("Estimated FOV Slope")
                        }
                        TextField {
                            text: aaCoreParams.EstimatedFOVSlope
                            horizontalAlignment: TextInput.AlignHCenter
                            validator: DoubleValidator {
                                decimals: 6
                                notation: DoubleValidator.StandardNotation
                            }
                            onEditingFinished: {
                                aaCoreParams.setEstimatedFOVSlope(text)
                            }
                        }
                        Label {
                            text: qsTr("Estimated AA FOV")
                        }
                        TextField {
                            text: aaCoreParams.EstimatedAAFOV
                            horizontalAlignment: TextInput.AlignHCenter
                            validator: DoubleValidator {
                                decimals: 6
                                notation: DoubleValidator.StandardNotation
                            }
                            onEditingFinished: {
                                aaCoreParams.setEstimatedAAFOV(text)
                            }
                        }
                    }
                    RowLayout {
                        Label {
                            text: qsTr("AA Tilt Compensation ( AA-A, AA-B ) : ")
                        }
                        ComboBox {
                            Layout.preferredWidth: 300
                            model: [ "(xTilt, yTilt)", "(xTilt, -yTilt)", "(-xTilt, yTilt)", "(-xTilt, -yTilt)","(yTilt, xTilt)", "(yTilt, -xTilt)", "(-yTilt, xTilt)", "(-yTilt, -xTilt)" ]
                            currentIndex: aaCoreParams.tiltRelationship
                            onCurrentIndexChanged: {
                                aaCoreParams.setTiltRelationship(currentIndex)
                            }
                        }
                    }
                    RowLayout {
                        Label {
                            text: qsTr("UL w_top")
                        }
                        TextField {
                            text: aaCoreParams.WeightList[0]
                            horizontalAlignment: TextInput.AlignHCenter
                            validator: DoubleValidator {
                                decimals: 6
                                notation: DoubleValidator.StandardNotation
                            }
                            onEditingFinished: {
                                aaCoreParams.setWeightValue(0, text);
                            }
                        }
                        Label {
                            text: qsTr("w_right")
                        }
                        TextField {
                            text: aaCoreParams.WeightList[1]
                            horizontalAlignment: TextInput.AlignHCenter
                            validator: DoubleValidator {
                                decimals: 6
                                notation: DoubleValidator.StandardNotation
                            }
                            onEditingFinished: {
                                aaCoreParams.setWeightValue(1, text);
                            }
                        }
                        Label {
                            text: qsTr("w_bottom")
                        }
                        TextField {
                            text: aaCoreParams.WeightList[2]
                            horizontalAlignment: TextInput.AlignHCenter
                            validator: DoubleValidator {
                                decimals: 6
                                notation: DoubleValidator.StandardNotation
                            }
                            onEditingFinished: {
                                aaCoreParams.setWeightValue(2, text);
                            }
                        }
                        Label {
                            text: qsTr("w_left")
                        }
                        TextField {
                            text: aaCoreParams.WeightList[3]
                            width: 1000
                            horizontalAlignment: TextInput.AlignHCenter
                            validator: DoubleValidator {
                                decimals: 6
                                notation: DoubleValidator.StandardNotation
                            }
                            onEditingFinished: {
                                aaCoreParams.setWeightValue(3, text);
                            }
                        }
                    }

                    RowLayout {
                        Label {
                            text: qsTr("LL w_top")
                        }
                        TextField {
                            text: aaCoreParams.WeightList[4]
                            horizontalAlignment: TextInput.AlignHCenter
                            validator: DoubleValidator {
                                decimals: 6
                                notation: DoubleValidator.StandardNotation
                            }
                            onEditingFinished: {
                                aaCoreParams.setWeightValue(4, text);
                            }
                        }
                        Label {
                            text: qsTr("w_right")
                        }
                        TextField {
                            text: aaCoreParams.WeightList[5]
                            horizontalAlignment: TextInput.AlignHCenter
                            validator: DoubleValidator {
                                decimals: 6
                                notation: DoubleValidator.StandardNotation
                            }
                            onEditingFinished: {
                                aaCoreParams.setWeightValue(5, text);
                            }
                        }
                        Label {
                            text: qsTr("w_bottom")
                        }
                        TextField {
                            text: aaCoreParams.WeightList[6]
                            horizontalAlignment: TextInput.AlignHCenter
                            validator: DoubleValidator {
                                decimals: 6
                                notation: DoubleValidator.StandardNotation
                            }
                            onEditingFinished: {
                                aaCoreParams.setWeightValue(6, text);
                            }
                        }
                        Label {
                            text: qsTr("w_left")
                        }
                        TextField {
                            text: aaCoreParams.WeightList[7]
                            width: 1000
                            horizontalAlignment: TextInput.AlignHCenter
                            validator: DoubleValidator {
                                decimals: 6
                                notation: DoubleValidator.StandardNotation
                            }
                            onEditingFinished: {
                                aaCoreParams.setWeightValue(7, text);
                            }
                        }
                    }

                    RowLayout {
                        Label {
                            text: qsTr("LR w_top")
                        }
                        TextField {
                            text: aaCoreParams.WeightList[8]
                            horizontalAlignment: TextInput.AlignHCenter
                            validator: DoubleValidator {
                                decimals: 6
                                notation: DoubleValidator.StandardNotation
                            }
                            onEditingFinished: {
                                aaCoreParams.setWeightValue(8, text);
                            }
                        }
                        Label {
                            text: qsTr("w_right")
                        }
                        TextField {
                            text: aaCoreParams.WeightList[9]
                            horizontalAlignment: TextInput.AlignHCenter
                            validator: DoubleValidator {
                                decimals: 6
                                notation: DoubleValidator.StandardNotation
                            }
                            onEditingFinished: {
                                aaCoreParams.setWeightValue(9, text);
                            }
                        }
                        Label {
                            text: qsTr("w_bottom")
                        }
                        TextField {
                            text: aaCoreParams.WeightList[10]
                            horizontalAlignment: TextInput.AlignHCenter
                            validator: DoubleValidator {
                                decimals: 6
                                notation: DoubleValidator.StandardNotation
                            }
                            onEditingFinished: {
                                aaCoreParams.setWeightValue(10, text);
                            }
                        }
                        Label {
                            text: qsTr("w_left")
                        }
                        TextField {
                            text: aaCoreParams.WeightList[11]
                            horizontalAlignment: TextInput.AlignHCenter
                            validator: DoubleValidator {
                                decimals: 6
                                notation: DoubleValidator.StandardNotation
                            }
                            onEditingFinished: {
                                aaCoreParams.setWeightValue(11, text);
                            }
                        }
                    }

                    RowLayout {
                        Label {
                            text: qsTr("UR w_top")
                        }
                        TextField {
                            text: aaCoreParams.WeightList[12]
                            horizontalAlignment: TextInput.AlignHCenter
                            validator: DoubleValidator {
                                decimals: 6
                                notation: DoubleValidator.StandardNotation
                            }
                            onEditingFinished: {
                                aaCoreParams.setWeightValue(12, text);
                            }
                        }
                        Label {
                            text: qsTr("w_right")
                        }
                        TextField {
                            text: aaCoreParams.WeightList[13]
                            horizontalAlignment: TextInput.AlignHCenter
                            validator: DoubleValidator {
                                decimals: 6
                                notation: DoubleValidator.StandardNotation
                            }
                            onEditingFinished: {
                                aaCoreParams.setWeightValue(13, text);
                            }
                        }
                        Label {
                            text: qsTr("w_bottom")
                        }
                        TextField {
                            text: aaCoreParams.WeightList[14]
                            horizontalAlignment: TextInput.AlignHCenter
                            validator: DoubleValidator {
                                decimals: 6
                                notation: DoubleValidator.StandardNotation
                            }
                            onEditingFinished: {
                                aaCoreParams.setWeightValue(14, text);
                            }
                        }
                        Label {
                            text: qsTr("w_left")
                        }
                        TextField {
                            text: aaCoreParams.WeightList[15]
                            horizontalAlignment: TextInput.AlignHCenter
                            validator: DoubleValidator {
                                decimals: 6
                                notation: DoubleValidator.StandardNotation
                            }
                            onEditingFinished: {
                                aaCoreParams.setWeightValue(15, text);
                            }
                        }
                    }
                }
            }
        }
        ColumnLayout{
            Frame {
                id: frame
                Layout.fillWidth: true
                contentHeight: 720

                Image {
                    id: image1
                    anchors.fill: parent
                    source: "../../icons/sparrow.png"
                    fillMode: Image.PreserveAspectFit
                    cache: false
                    Connections {
                        target: highSprrow
                        onDisplayAACoreTuningImageInUI: {
                            image1.source = ""
                            image1.source = "image://aaDebugImage"
                        }
                    }
                }
            }
        }
    }
}

