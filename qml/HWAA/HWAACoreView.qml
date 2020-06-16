import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11
import FileContentItem 1.0
import QtQuick.Dialogs 1.2
import AACoreNew 1.1
import "../Log"

ItemDelegate {

    property int selectedChannel: 1

    width: parent.width

    contentItem: RowLayout  {
        spacing: 10

        ColumnLayout{
            Layout.alignment: Qt.AlignTop

            FileDialog{
                id:file_dialog
                title:"选择加载PR文件"
                selectExisting: true
                selectFolder: false
                selectMultiple: false

                onAccepted:{
                    //setPrFileName(fileUrl)
                    if (selectedChannel === 1)
                        imageProcess1Filename.text = fileUrl
                    else if (selectedChannel === 2)
                        imageProcess2Filename.text = fileUrl
                }
            }

            GroupBox{
                title: qsTr("AA Core Parameters")
                //enabled: userManagement.currentAuthority >= 2 //At least engineer authority

                ColumnLayout {

                    RowLayout {
                        RoundButton {
                            text: "Load"
                            transformOrigin: Item.Center
                            display: Button.TextBesideIcon
                            icon.width: 30
                            icon.height: 30
                            icon.source: "../../icons/camera.png"
                            icon.color: "yellow"
                            onClicked: {
                                selectedChannel = 1
                                file_dialog.open()
                            }
                        }
                        TextField {
                            id: imageProcess1Filename
                        }
                        RoundButton {
                            text: "图像处理1"
                            transformOrigin: Item.Center
                            display: Button.TextBesideIcon
                            icon.width: 30
                            icon.height: 30
                            icon.source: "../../icons/camera.png"
                            icon.color: "yellow"
                            onClicked: {
                                var json = {
                                    method: 1,
                                    enable_motion: enable_motion1.currentIndex,
                                    filename: imageProcess1Filename.text
                                }
                                aaNewCore.performHandling(AACoreNew.TOF, JSON.stringify(json))
                            }
                        }
                    }

                    RowLayout {
                        Label {
                            text: qsTr("Enable Motion: ")
                        }
                        ComboBox {
                            width: 200
                            id: enable_motion1
                            model: [ "Disable", "Enable"]
                        }
                        Label {
                            text: qsTr("Alpha: ")
                        }
                        TextField {
                            Layout.preferredWidth: 100
                            text: aaCoreParams.intensityCorrectionFactor1
                            onEditingFinished: {
                                aaCoreParams.setIntensityCorrectionFactor1(text)
                            }
                        }
                    }

                    RowLayout {
                        Label {
                            text: qsTr("Y1 = ")
                        }
                        TextField {
                            Layout.preferredWidth: 100
                            text: aaCoreParams.scanY1PixelLocation1
                            onEditingFinished: {
                                aaCoreParams.setScanY1PixelLocation1(text)
                            }
                        }
                        Label {
                            text: qsTr("Y2 = ")
                        }
                        TextField {
                            Layout.preferredWidth: 100
                            text: aaCoreParams.scanY2PixelLocation1
                            onEditingFinished: {
                                aaCoreParams.setScanY2PixelLocation1(text)
                            }
                        }
                    }
                    RowLayout {
                        Label {
                            text: qsTr("Y4 = ")
                        }
                        TextField {
                            Layout.preferredWidth: 100
                            text: aaCoreParams.scanY4PixelLocation1
                            onEditingFinished: {
                                aaCoreParams.setScanY4PixelLocation1(text)
                            }
                        }
                        Label {
                            text: qsTr("Y5 = ")
                        }
                        TextField {
                            Layout.preferredWidth: 100
                            text: aaCoreParams.scanY5PixelLocation1
                            onEditingFinished: {
                                aaCoreParams.setScanY5PixelLocation1(text)
                            }
                        }
                    }

                    RowLayout {
                        Label {
                            text: qsTr("Intensity = ")
                        }
                        TextField {
                            Layout.preferredWidth: 100
                            text: aaCoreParams.halfWidth1
                            onEditingFinished: {
                                aaCoreParams.setHalfWidth1(text)
                            }
                        }
                    }

                    RowLayout {
                        Label {
                            text: qsTr("X = ")
                        }
                        TextField {
                            Layout.preferredWidth: 150
                            id: input_X
                            text: aaCoreParams.x1
                            onEditingFinished: {
                                aaCoreParams.setX1(text)
                            }
                        }
                        Label {
                            text: qsTr("Y = ")
                        }
                        TextField {
                            Layout.preferredWidth: 150
                            id: input_Y
                            text: aaCoreParams.y1
                            onEditingFinished: {
                                aaCoreParams.setY1(text)
                            }
                        }
                    }

                    RowLayout {
                        Label {
                            text: qsTr("Z = ")
                        }
                        TextField {
                            Layout.preferredWidth: 150
                            id: input_Z
                            text: aaCoreParams.z1
                            onEditingFinished: {
                                aaCoreParams.setZ1(text)
                            }
                        }
                        Label {
                            text: qsTr("RX = ")
                        }
                        TextField {
                            Layout.preferredWidth: 150
                            id: input_RX
                            text: aaCoreParams.rx1
                            onEditingFinished: {
                                aaCoreParams.setRX1(text)
                            }
                        }
                    }

                    RowLayout {
                        Label {
                            text: qsTr("RY = ")
                        }
                        TextField {
                            Layout.preferredWidth: 150
                            id: input_RY
                            text: aaCoreParams.ry1
                            onEditingFinished: {
                                aaCoreParams.setRY1(text)
                            }
                        }
                        Label {
                            text: qsTr("RZ = ")
                        }
                        TextField {
                            Layout.preferredWidth: 150
                            id: input_RZ
                            text: aaCoreParams.rz1
                            onEditingFinished: {
                                aaCoreParams.setRZ1(text)
                            }
                        }
                    }
                    RowLayout {
                        RoundButton {
                            text: "Load"
                            transformOrigin: Item.Center
                            display: Button.TextBesideIcon
                            icon.width: 30
                            icon.height: 30
                            icon.source: "../../icons/camera.png"
                            icon.color: "cyan"
                            onClicked: {
                                selectedChannel = 2
                                file_dialog.open()
                            }
                        }
                        TextField {
                            id: imageProcess2Filename
                        }
                        RoundButton {
                            text: "图像处理2"
                            transformOrigin: Item.Center
                            display: Button.TextBesideIcon
                            icon.width: 30
                            icon.height: 30
                            icon.source: "../../icons/camera.png"
                            icon.color: "cyan"
                            onClicked: {
                                var json = {
                                    method: 2,
                                    enable_motion: enable_motion2.currentIndex,
                                    filename: imageProcess2Filename.text
                                }
                                aaNewCore.performHandling(AACoreNew.TOF, JSON.stringify(json))
                            }
                        }
                    }
                    RowLayout {
                        Label {
                            text: qsTr("Enable Motion: ")
                        }
                        ComboBox {
                            width: 200
                            id: enable_motion2
                            model: [ "Disable", "Enable"]
                        }
                    }

                    RowLayout {
                        Label {
                            text: qsTr("Y1 = ")
                        }
                        TextField {
                            Layout.preferredWidth: 100
                            text: aaCoreParams.scanY1PixelLocation2
                            onEditingFinished: {
                                aaCoreParams.setScanY1PixelLocation2(text)
                            }
                        }
                        Label {
                            text: qsTr("Y2 = ")
                        }
                        TextField {
                            Layout.preferredWidth: 100
                            text: aaCoreParams.scanY2PixelLocation2
                            onEditingFinished: {
                                aaCoreParams.setScanY2PixelLocation2(text)
                            }
                        }
                    }

                    RowLayout {
                        Label {
                            text: qsTr("Y3 = ")
                        }
                        TextField {
                            Layout.preferredWidth: 100
                            text: aaCoreParams.scanY3PixelLocation2
                            onEditingFinished: {
                                aaCoreParams.setScanY3PixelLocation2(text)
                            }
                        }
                        Label {
                            text: qsTr("Intensity = ")
                        }
                        TextField {
                            Layout.preferredWidth: 100
                            text: aaCoreParams.halfWidth2
                            onEditingFinished: {
                                aaCoreParams.setHalfWidth2(text)
                            }
                        }
                    }


                    RowLayout {
                        Label {
                            text: qsTr("X = ")
                        }
                        TextField {
                            Layout.preferredWidth: 150
                            id: input_X2
                            text: aaCoreParams.x2
                            onEditingFinished: {
                                aaCoreParams.setX2(text)
                            }
                        }
                        Label {
                            text: qsTr("Y = ")
                        }
                        TextField {
                            Layout.preferredWidth: 150
                            id: input_Y2
                            text: aaCoreParams.y2
                            onEditingFinished: {
                                aaCoreParams.setY2(text)
                            }
                        }
                    }

                    RowLayout {
                        Label {
                            text: qsTr("Z = ")
                        }
                        TextField {
                            Layout.preferredWidth: 150
                            id: input_Z2
                            text: aaCoreParams.z2
                            onEditingFinished: {
                                aaCoreParams.setZ2(text)
                            }
                        }
                        Label {
                            text: qsTr("RX = ")
                        }
                        TextField {
                            Layout.preferredWidth: 150
                            id: input_RX2
                            text: aaCoreParams.rx2
                            onEditingFinished: {
                                aaCoreParams.setRX2(text)
                            }
                        }
                    }

                    RowLayout {
                        Label {
                            text: qsTr("RY = ")
                        }
                        TextField {
                            Layout.preferredWidth: 150
                            id: input_RY2
                            text: aaCoreParams.ry2
                            onEditingFinished: {
                                aaCoreParams.setRY2(text)
                            }
                        }
                        Label {
                            text: qsTr("RZ = ")
                        }
                        TextField {
                            Layout.preferredWidth: 150
                            id: input_RZ2
                            text: aaCoreParams.rz2
                            onEditingFinished: {
                                aaCoreParams.setRZ2(text)
                            }
                        }
                    }
                }
            }
        }

        LogView{
            id: logView
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.margins: 3

            height: parent.height * 0.6
        }

    }
}

