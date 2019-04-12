import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11
import QtQuick.Dialogs 1.2

ItemDelegate {
    width: parent.width
    contentItem: ColumnLayout {
        spacing: 0
        RowLayout {
            ColumnLayout {
                RowLayout {
                    Label {
                        id: itemLabel
                        text: model.label
                        font.pixelSize: 20
                    }
                }
            }
            Switch {
                id: calibrationViewSwitch
            }
        }
        ColumnLayout {
            visible: calibrationViewSwitch.checked

            GroupBox {
                title: "Downlook calibration"

                RowLayout {
                    FileDialog {
                        id: loadfileDialog
                        title: "选择加载PR文件"
                        selectExisting: true
                        selectFolder: false
                        selectMultiple: false

                        nameFilters: ["avdata文件 (*.avdata)"]
                        onAccepted: {
                            downlookPRFileName.text = loadfileDialog.fileUrl
                            sutParams.setPRName(loadfileDialog.fileUrl)
                        }
                    }


                    Dial {
                        id: downlookLighingDial
                        width: 25
                        from: 0
                        value: sutParams.Lighting
                        to: 255
                        stepSize: 1

                        Label {
                            text: downlookLighingDial.value.toFixed(0)
                            color: "white"
                            font.pixelSize: Qt.application.font.pixelSize * 3
                            anchors.centerIn: parent
                        }
                        onValueChanged: {
                            sutParams.setLighting(value)
                            lightingController.setDownlookLighting(value)
                        }
                    }

                    Button {
                        text: qsTr("Load PR")
                        onClicked: {
                            loadfileDialog.open()
                        }
                    }

                    Button {
                        id: button
                        text: qsTr("Start Downlook Cali")
                        onClicked: {
                            baseModuleManager.performDownlookCalibration()
                        }
                    }

                    TextField {
                        id: downlookPRFileName
                        color: "#57f529"
                        text: sutParams.prName
                        font.pixelSize: 14
                    }


                }
            }

            GroupBox {
                ColumnLayout {
                    RowLayout {
                        FileDialog {
                            id: loadUplookFileDialog
                            title: "选择加载PR文件"
                            selectExisting: true
                            selectFolder: false
                            selectMultiple: false

                            nameFilters: ["avdata文件 (*.avdata)"]
                            onAccepted: {
                                uplookPRFileName.text = loadUplookFileDialog.fileUrl
                                lutParams.setPRName(loadUplookFileDialog.fileUrl)
                            }
                        }

                        Dial {
                            id: uplookLighingDial
                            width: 25
                            from: 0
                            value: lutParams.Lighting
                            to: 255
                            stepSize: 1

                            Label {
                                text: uplookLighingDial.value.toFixed(0)
                                color: "white"
                                font.pixelSize: Qt.application.font.pixelSize * 3
                                anchors.centerIn: parent
                            }
                            onValueChanged: {
                                lutParams.setLighting(value)
                                lightingController.setUplookLighting(value)
                            }
                        }

                        Button {
                            id: button2
                            text: qsTr("Load PR")
                            onClicked: {
                                loadUplookFileDialog.open()
                            }
                        }
                        Button {
                            text: qsTr("Move To Uplook Cali")
                            onClicked: {
                                lutModule.moveToAA1UplookPos()
                            }
                        }

                        Button {
                            text: qsTr("Perform PR")
                            onClicked: {

                            }
                        }

                        TextField {
                            id: uplookPRFileName
                            color: "#57f529"
                            text: lutParams.prName
                            font.pixelSize: 14
                        }
                    }
                    RowLayout {
                        Button {
                            text: qsTr("Start Uplook Cali")
                            onClicked: {
                                baseModuleManager.performUplookCalibration()
                            }
                        }
                    }
                }
            }

            RowLayout {
                FileDialog {
                    id: loadUpDnlookUpCaliFileDialog
                    title: "选择加载PR文件"
                    selectExisting: true
                    selectFolder: false
                    selectMultiple: false

                    nameFilters: ["avdata文件 (*.avdata)"]
                    onAccepted: {
                        loadUpDnlookUpPRFileName.text = loadUplookFileDialog.fileUrl
                        lutParams.setUpDownLookPrName(fileUrl)
                    }
                }

                Dial {
                    id: upDownlookUpCalibLighingDial
                    width: 25
                    from: 0
                    value: lutParams.UpDnLookLighting
                    to: 255
                    stepSize: 1

                    Label {
                        text: upDownlookUpCalibLighingDial.value.toFixed(0)
                        color: "white"
                        font.pixelSize: Qt.application.font.pixelSize * 3
                        anchors.centerIn: parent
                    }
                    onValueChanged: {
                        lutParams.setUpDnLookLighting(value)
                        lightingController.setUplookLighting(value)
                    }
                }
                Button {
                    text: qsTr("Load UpDnlook Up Cali PR")
                    onClicked: {
                        loadUpDnlookUpCaliFileDialog.open()
                    }
                }
                Button {
                    text: qsTr("Start UpDnLook Up Cali")
                    onClicked: {
                        baseModuleManager.performUpDownlookUpCalibration();
                    }
                }
                TextField {
                    id: loadUpDnlookUpPRFileName
                    color: "#57f529"
                    text: lutParams.upDownLookPrName
                    font.pixelSize: 14
                }
            }

            RowLayout {
                FileDialog {
                    id: loadUpDnlookDownCaliFileDialog
                    title: "选择加载PR文件"
                    selectExisting: true
                    selectFolder: false
                    selectMultiple: false

                    nameFilters: ["avdata文件 (*.avdata)"]
                    onAccepted: {
                        loadUpDnlookDownPRFileName.text = loadUplookFileDialog.fileUrl
                        sutParams.setUpDownLookPrName(fileUrl)
                    }
                }
                Dial {
                    id: upDownlookDownCalibLighingDial
                    width: 26
                    from: 0
                    value: sutParams.UpDnLookLighting
                    to: 255
                    stepSize: 1

                    Label {
                        text: upDownlookDownCalibLighingDial.value.toFixed(0)
                        color: "white"
                        font.pixelSize: Qt.application.font.pixelSize * 3
                        anchors.centerIn: parent
                    }
                    onValueChanged: {
                        sutParams.setUpDnLookLighting(value)
                        lightingController.setDownlookLighting(value)
                    }
                }
                Button {
                    text: qsTr("Load UpDnlook Down Cali PR")
                    onClicked: {
                        loadUpDnlookDownCaliFileDialog.open()
                    }
                }
                Button {
                    text: qsTr("Start UpDnLook Down Cali")
                    onClicked: {
                        baseModuleManager.performUpDownlookCalibration()
                    }
                }
                TextField {
                    id: loadUpDnlookDownPRFileName
                    color: "#57f529"
                    text: sutParams.upDownLookPrName
                    font.pixelSize: 14
                }
            }
        }
    }
}
