import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11
import FileContentItem 1.0
import QtQuick.Dialogs 1.2
import AACoreNew 1.1

ItemDelegate {
    width: parent.width

    contentItem: RowLayout  {
        spacing: 10

        ColumnLayout{
            Layout.alignment: Qt.AlignTop

            GroupBox{
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
                            text: "图像处理1"
                            transformOrigin: Item.Center
                            display: Button.TextBesideIcon
                            icon.width: 30
                            icon.height: 30
                            icon.source: "../../icons/camera.png"
                            icon.color: "yellow"
                            onClicked: {
                                var json = {
                                    method: 1
                                }
                                aaNewCore.performHandling(AACoreNew.TOF, JSON.stringify(json))
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
                            text: qsTr("X = ")
                        }
                        TextField {
                            Layout.preferredWidth: 300
                            id: input_X
                            text: aaCoreParams.x1
                            onEditingFinished: {
                                aaCoreParams.setX1(text)
                            }
                        }
                    }

                    RowLayout {
                        Label {
                            text: qsTr("Y = ")
                        }
                        TextField {
                            Layout.preferredWidth: 300
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
                            Layout.preferredWidth: 300
                            id: input_Z
                            text: aaCoreParams.z1
                            onEditingFinished: {
                                aaCoreParams.setZ1(text)
                            }
                        }
                    }

                    RowLayout {
                        Label {
                            text: qsTr("RX = ")
                        }
                        TextField {
                            Layout.preferredWidth: 300
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
                            Layout.preferredWidth: 300
                            id: input_RY
                            text: aaCoreParams.ry1
                            onEditingFinished: {
                                aaCoreParams.setRY1(text)
                            }
                        }
                    }

                    RowLayout {
                        Label {
                            text: qsTr("RZ = ")
                        }
                        TextField {
                            Layout.preferredWidth: 300
                            id: input_RZ
                            text: aaCoreParams.rz1
                            onEditingFinished: {
                                aaCoreParams.setRZ1(text)
                            }
                        }
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
                                method: 2
                            }
                            aaNewCore.performHandling(AACoreNew.TOF, JSON.stringify(json))
                        }
                    }

                    RowLayout {
                        Label {
                            text: qsTr("X = ")
                        }
                        TextField {
                            Layout.preferredWidth: 300
                            id: input_X2
                            text: aaCoreParams.x2
                            onEditingFinished: {
                                aaCoreParams.setX2(text)
                            }
                        }
                    }

                    RowLayout {
                        Label {
                            text: qsTr("Y = ")
                        }
                        TextField {
                            Layout.preferredWidth: 300
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
                            Layout.preferredWidth: 300
                            id: input_Z2
                            text: aaCoreParams.z2
                            onEditingFinished: {
                                aaCoreParams.setZ2(text)
                            }
                        }
                    }

                    RowLayout {
                        Label {
                            text: qsTr("RX = ")
                        }
                        TextField {
                            Layout.preferredWidth: 300
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
                            Layout.preferredWidth: 300
                            id: input_RY2
                            text: aaCoreParams.ry2
                            onEditingFinished: {
                                aaCoreParams.setRY2(text)
                            }
                        }
                    }

                    RowLayout {
                        Label {
                            text: qsTr("RZ = ")
                        }
                        TextField {
                            Layout.preferredWidth: 300
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

        Frame {
            id: frame1
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
        Frame {
            id: frame2
            Layout.fillWidth: true
            contentHeight: 720

            Image {
                id: image2
                anchors.fill: parent
                source: "../../icons/sparrow.png"
                fillMode: Image.PreserveAspectFit
                cache: false
                Connections {
                    target: highSprrow
                    onDisplayAACoreTuningImageInUI: {
                        image2.source = ""
                        image2.source = "image://aaDebugImage"
                    }
                }
            }
        }
    }
}

