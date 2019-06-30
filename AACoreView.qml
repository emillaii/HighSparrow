import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11
import FileContentItem 1.0
import QtQuick.Dialogs 1.2

ItemDelegate {
    width: parent.width
    contentItem: ColumnLayout {
        spacing: 0

        RowLayout {
            GroupBox{
                title: qsTr("AA Core Parameters")
                ColumnLayout {
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
                    }
                    RowLayout {
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
            Frame {
                id: frame
                Layout.fillWidth: true
                contentHeight: 720

                Image {
                    id: image1
                    anchors.fill: parent
                    source: "icons/sparrow.png"
                    fillMode: Image.PreserveAspectFit
                    cache: false
                    Connections {
                        target: highSprrow
                        onDisplayAACoreTuningImageInUI: {
                            image1.source = ""
                            image1.source = "image://aaCoreTuningImage"
                        }
                    }
                }
            }
        }
    }
}

