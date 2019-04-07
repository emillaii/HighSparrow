import QtQuick 2.9
import QtQuick.Controls 2.2

Grid {
    id: grid
    x: 850
    width: 700
    height: 700
    spacing: 10
    rows: 2
    columns: 2
    anchors.bottom: parent.bottom
    anchors.bottomMargin: 106
    anchors.top: parent.top
    anchors.topMargin: 6
    anchors.right: parent.right
    anchors.rightMargin: 30

    Frame {
        id: frame
        width: 300
        height: 300

        Image {
            id: image
            anchors.fill: parent
            source: "icons/home.png"
            fillMode: Image.PreserveAspectFit
            cache: false

            Text {
                id: element
                x: -12
                y: -12
                color: "#9ef678"
                text: qsTr("LUT Uplook Camera")
                wrapMode: Text.WordWrap
                elide: Text.ElideLeft
                fontSizeMode: Text.Fit
                lineHeight: 3.1
                anchors.fill: parent
                font.pixelSize: 12
            }

            Slider {
                id: slider
                y: 256
                stepSize: 1
                to: 255
                from: 0

                anchors.left: parent.left
                anchors.leftMargin: -3
                anchors.bottom: parent.bottom
                anchors.bottomMargin: -28
                value: baseModuleManager.uplookLighting
                onValueChanged: {
                    baseModuleManager.setUplookLighting(value)
                }
                Label {
                    id: label
                    y: 6
                    color: "#46eb46"
                    text: baseModuleManager.uplookLighting
                    font.pointSize: 20
                    font.family: "Times New Roman"
                    anchors.bottom: parent.bottom
                    anchors.bottomMargin: 12
                    anchors.left: parent.left
                    anchors.leftMargin: 202
                }
            }

            Connections {
                target: uplookCamera
                onCallQmlRefeshImg: {
                    image.source = ""
                    image.source = "image://uplookCameraImage"
                }
            }



        }

    }

    Frame {
        id: frame1
        width: 300
        height: 300

        Image {
            id: image1
            anchors.fill: parent
            source: "icons/home.png"
            fillMode: Image.PreserveAspectFit
            cache: false

            Slider {
                y: 255
                stepSize: 1
                to: 255
                from: 0
                anchors.left: parent.left
                anchors.leftMargin: 0
                anchors.bottom: parent.bottom
                anchors.bottomMargin: -27
                value: baseModuleManager.downlookLighting
                onValueChanged: {
                    baseModuleManager.setDownlookLighting(value)
                }
                Label {
                    y: 6
                    color: "#46eb46"
                    text: baseModuleManager.downlookLighting
                    font.pointSize: 20
                    font.family: "Times New Roman"
                    anchors.bottom: parent.bottom
                    anchors.bottomMargin: 12
                    anchors.left: parent.left
                    anchors.leftMargin: 202
                }
            }

            Connections {
                target: downlookCamera
                onCallQmlRefeshImg: {
                    image1.source = ""
                    image1.source = "image://downlookCameraImage"
                }
            }
        }

        Text {
            id: element1
            color: "#9ef678"
            text: qsTr("AA Downlook Camera")
            fontSizeMode: Text.Fit
            lineHeight: 3.1
            anchors.fill: parent
            font.pixelSize: 12
        }
    }


    Frame {
        id: frame2
        width: 300
        height: 300
        Image {
            id: image2
            anchors.fill: parent
            source: "icons/home.png"
            fillMode: Image.PreserveAspectFit

            Slider {
                id: slider1
                y: 250
                anchors.bottom: parent.bottom
                anchors.bottomMargin: -22
                anchors.left: parent.left
                anchors.leftMargin: -5
                value: 0.5

                Label {
                    y: 6
                    color: "#46eb46"
                    text: baseModuleManager.downlookLighting
                    font.pointSize: 20
                    font.family: "Times New Roman"
                    anchors.bottom: parent.bottom
                    anchors.bottomMargin: 12
                    anchors.left: parent.left
                    anchors.leftMargin: 202
                }
            }
        }
    }

    Frame {
        id: frame3
        width: 300
        height: 300
        Image {
            id: image3
            anchors.fill: parent
            source: "icons/home.png"
            fillMode: Image.PreserveAspectFit
        }
    }
}









/*##^## Designer {
    D{i:17;anchors_x:0}
}
 ##^##*/
