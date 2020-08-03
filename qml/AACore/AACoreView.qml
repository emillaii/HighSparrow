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
                //enabled: userManagement.currentAuthority >= 2 //At least engineer authority

                ColumnLayout {
                    RowLayout {
                        RoundButton {
                            text: "从图像采集卡捕获图像"
                            transformOrigin: Item.Center
                            display: Button.TextBesideIcon
                            icon.width: 30
                            icon.height: 30
                            icon.source: "../../icons/camera.png"
                            icon.color: "lightGreen"
                            onClicked: {
                                 aaNewCore.captureLiveImage()
                            }
                        }
                        RoundButton {
                            text: "测试 Setting 1"
                            transformOrigin: Item.Center
                            display: Button.TextBesideIcon
                            icon.width: 30
                            icon.height: 30
                            icon.source: "../../icons/camera.png"
                            icon.color: "yellow"
                            onClicked: {
                                visionModule.aaDebugImage("livePhoto.bmp", aaCoreParams.MaxIntensity, aaCoreParams.MinArea, aaCoreParams.MaxArea)
                            }
                        }
                        RoundButton {
                            text: "测试 Setting 2"
                            transformOrigin: Item.Center
                            display: Button.TextBesideIcon
                            icon.width: 30
                            icon.height: 30
                            icon.source: "../../icons/camera.png"
                            icon.color: "cyan"
                            onClicked: {
                                visionModule.aaDebugImage("livePhoto.bmp", aaCoreParams.MaxIntensity2, aaCoreParams.MinArea, aaCoreParams.MaxArea)
                            }
                        }
                    }

                    RowLayout {
                        Label {
                            text: qsTr("Selected Profile: ")
                        }
                        ComboBox {
                            width: 200
                            model: [ "Default", "Profile 1", "Profile 2" ]
                        }
                    }

                    RowLayout {
                        Label {
                            text: qsTr("MTF Check Nyquist Frequency: ")
                        }
                        ComboBox {
                            Layout.preferredWidth: 300
                            model: [ "1/8", "1/4" ]
                            currentIndex: aaCoreParams.mtfFrequency
                            onCurrentIndexChanged: {
                                aaCoreParams.setMTFFrequency(currentIndex)
                            }
                        }
                    }
                    RowLayout {
                        Label {
                            text: qsTr("Oversampling:")
                        }
                        ComboBox {
                            Layout.preferredWidth: 300
                            model: [ "1", "2"]
                            currentIndex: aaCoreParams.aaScanOversampling
                            onCurrentIndexChanged: {
                                aaCoreParams.setAAScanOversampling(currentIndex)
                            }
                        }
                        Label {
                            text: qsTr("AA Scan MTF Nyquist Frequency:")
                        }
                        ComboBox {
                            Layout.preferredWidth: 300
                            model: [ "1/8", "1/4" ]
                            currentIndex: aaCoreParams.aaScanMTFFrequency
                            onCurrentIndexChanged: {
                                aaCoreParams.setAAScanMTFFrequency(currentIndex)
                            }
                        }
                    }

                    RowLayout {
                        Label {
                            text: qsTr("Fit Order:")
                        }
                        TextField {
                            text: aaCoreParams.aaScanCurveFitOrder
                            horizontalAlignment: TextInput.AlignHCenter
                            validator: IntValidator {
                                bottom: 4
                                top: 6
                            }
                            onEditingFinished: {
                                aaCoreParams.setAAScanCurveFitOrder(text)
                            }
                        }
                        Label {
                            text: qsTr("Text Size:")
                        }
                        TextField {
                            text: aaCoreParams.drawTextSize
                            horizontalAlignment: TextInput.AlignHCenter
                            validator: IntValidator {
                                bottom: 1
                                top: 1000
                            }
                            onEditingFinished: {
                                aaCoreParams.setDrawTextSize(text)
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
                            text: qsTr("Max Intensity Setting 1")
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
                        Label {
                            text: qsTr("Max Intensity Setting 2")
                        }
                        TextField {
                            text: aaCoreParams.MaxIntensity2
                            horizontalAlignment: TextInput.AlignHCenter
                            validator: DoubleValidator {
                                decimals: 6
                                notation: DoubleValidator.StandardNotation
                            }
                            onEditingFinished: {
                                aaCoreParams.setMaxIntensity2(text)
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
                            text: qsTr("Layer 0 w_t")
                            color: "red"
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
                            text: qsTr("w_r")
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
                            text: qsTr("w_b")
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
                            text: qsTr("w_l")
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
                            text: qsTr("Layer 1 w_t")
                            color: "purple"
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
                            text: qsTr("w_r")
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
                            text: qsTr("w_b")
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
                            text: qsTr("w_l")
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
                            text: qsTr("Layer 2 w_t")
                            color: "blue"
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
                            text: qsTr("w_r")
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
                            text: qsTr("w_b")
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
                            text: qsTr("w_l")
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
                            text: qsTr("Layer 3 w_t")
                            color: "yellow"
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
                            text: qsTr("w_r")
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
                            text: qsTr("w_b")
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
                            text: qsTr("w_l")
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
                            id: chartCalibrationTheta
                            text: qsTr("")
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
                        image1.source = "image://aaDebugImage"
                    }
                }
            }
        }
    }
}

