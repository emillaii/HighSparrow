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
            ColumnLayout {
                RowLayout {
                    Label {
                        text: model.label
                        font.pixelSize: 20
                    }
                }
            }
            Switch {
                id: aaCoreViewSwitch
            }
        }

        ColumnLayout {
            visible: aaCoreViewSwitch.checked
            RowLayout {
                Label {
                    text: qsTr("")
                }
            }
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
                                decimals: 3
                                notation: DoubleValidator.StandardNotation
                            }
                            onEditingFinished: {
                                aaCoreParams.setEFL(text)
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
                                decimals: 3
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
                                decimals: 3
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
                                decimals: 3
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
                                decimals: 3
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
                                decimals: 3
                                notation: DoubleValidator.StandardNotation
                            }
                            onEditingFinished: {
                                aaCoreParams.setSensorYRatio(text)
                            }
                        }
                    }
                }
            }
        }
    }
}
