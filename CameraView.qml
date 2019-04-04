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
                text: qsTr("Uplook Camera")
                wrapMode: Text.WordWrap
                elide: Text.ElideLeft
                fontSizeMode: Text.Fit
                lineHeight: 3.1
                anchors.fill: parent
                font.pixelSize: 12
            }

            Slider {
                id: slider
                y: 249
                stepSize: 1
                to: 255
                from: 0

                anchors.left: parent.left
                anchors.leftMargin: 0
                anchors.bottom: parent.bottom
                anchors.bottomMargin: -21
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
                y: 249
                stepSize: 1
                to: 255
                from: 0
                anchors.left: parent.left
                anchors.leftMargin: 0
                anchors.bottom: parent.bottom
                anchors.bottomMargin: -21
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
            text: qsTr("Downlook Camera")
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








