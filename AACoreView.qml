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

