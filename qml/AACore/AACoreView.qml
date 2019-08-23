import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11
import FileContentItem 1.0
import QtQuick.Dialogs 1.2

ItemDelegate {
    width: parent.width

    contentItem: RowLayout  {
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
                            model: [ "Best CC", "Best 0.3F", "Best 0.5F", "Best 0.8F" ]
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
                            text: qsTr("03")
                        }
                        TextField {
                            text: aaCoreParams.zpeak03Coefficient
                            horizontalAlignment: TextInput.AlignHCenter
                            validator: DoubleValidator {
                                decimals: 2
                                notation: DoubleValidator.StandardNotation
                            }
                            onEditingFinished: {
                                aaCoreParams.setZpeak03Coefficient(text)
                            }
                        }
                        Label {
                            text: qsTr("05")
                        }
                        TextField {
                            text: aaCoreParams.zpeak05Coefficient
                            horizontalAlignment: TextInput.AlignHCenter
                            validator: DoubleValidator {
                                decimals: 2
                                notation: DoubleValidator.StandardNotation
                            }
                            onEditingFinished: {
                                aaCoreParams.setZpeak05Coefficient(text)
                            }
                        }
                        Label {
                            text: qsTr("08")
                        }
                        TextField {
                            text: aaCoreParams.zpeak08Coefficient
                            horizontalAlignment: TextInput.AlignHCenter
                            validator: DoubleValidator {
                                decimals: 2
                                notation: DoubleValidator.StandardNotation
                            }
                            onEditingFinished: {
                                aaCoreParams.setZpeak08Coefficient(text)
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
                            text: qsTr("05F ZPeak Diff")
                        }
                        TextField {
                            text: aaCoreParams.zPeakDiff05Max
                            horizontalAlignment: TextInput.AlignHCenter
                            validator: DoubleValidator {
                                decimals: 2
                                notation: DoubleValidator.StandardNotation
                            }
                            onEditingFinished: {
                                aaCoreParams.setZPeakDiff05Max(text)
                            }
                        }
                        Label {
                            text: qsTr("08F ZPeak Diff")
                        }
                        TextField {
                            text: aaCoreParams.zPeakDiff08Max
                            horizontalAlignment: TextInput.AlignHCenter
                            validator: DoubleValidator {
                                decimals: 2
                                notation: DoubleValidator.StandardNotation
                            }
                            onEditingFinished: {
                                aaCoreParams.setZPeakDiff08Max(text)
                            }
                        }
                    }
                    RowLayout {
                        Label {
                            text: qsTr("CC_08MinDev")
                        }
                        TextField {
                            text: aaCoreParams.CC08MinDev
                            horizontalAlignment: TextInput.AlignHCenter
                            validator: DoubleValidator {
                                decimals: 2
                                notation: DoubleValidator.StandardNotation
                            }
                            onEditingFinished: {
                                aaCoreParams.setCC08MinDev(text)
                            }
                        }
                        Label {
                            text: qsTr("CC_08MaxDev")
                        }
                        TextField {
                            text: aaCoreParams.CC08MaxDev
                            horizontalAlignment: TextInput.AlignHCenter
                            validator: DoubleValidator {
                                decimals: 2
                                notation: DoubleValidator.StandardNotation
                            }
                            onEditingFinished: {
                                aaCoreParams.setCC08MaxDev(text)
                            }
                        }
                        Label {
                            text: qsTr("CC_05MinDev")
                        }
                        TextField {
                            text: aaCoreParams.CC05MinDev
                            horizontalAlignment: TextInput.AlignHCenter
                            validator: DoubleValidator {
                                decimals: 2
                                notation: DoubleValidator.StandardNotation
                            }
                            onEditingFinished: {
                                aaCoreParams.setCC05MinDev(text)
                            }
                        }
                        Label {
                            text: qsTr("CC_05MaxDev")
                        }
                        TextField {
                            text: aaCoreParams.CC05MaxDev
                            horizontalAlignment: TextInput.AlignHCenter
                            validator: DoubleValidator {
                                decimals: 2
                                notation: DoubleValidator.StandardNotation
                            }
                            onEditingFinished: {
                                aaCoreParams.setCC05MaxDev(text)
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
                            text: qsTr("Min Pattern Area")
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

                    RowLayout {
                        Label {
                            id: chartCalibrationTheta
                            text: qsTr("")
                        }
                        RoundButton{
                            text: qsTr("Take Photo")
                            onClicked: {
                                aaNewCore.captureLiveImage()
                            }
                        }

                        Button {
                            text: qsTr("Show Chart Theta")
                            onClicked: {
                                chartCalibrationTheta.text = baseModuleManager.showChartCalibrationRotation()
                            }
                        }
                    }
                }
            }
        }

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
                        image1.source = "image://dispenseImage"
                    }
                }
            }
        }
    }
}

