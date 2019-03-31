import QtQuick 2.9
import QtQuick.Controls 2.2

Page {

    readonly property string downlookCameraName: "DownlookCamera"
    readonly property string uplookCameraName: "UplookCamera"
    readonly property string pickarmCameraName: "PickarmCamera"

    id: page2
    width: 1280
    height: 720
    property alias frame: frame
    header: Label {
        text: qsTr("Page 2")
        font.pixelSize: Qt.application.font.pixelSize * 2
        padding: 10
    }

    Row {
        id: row
        width: 900
        height: 300

        Frame {
            id: frame
            width: 300
            height: 300

            Image {
                id: prResultPreview1
                anchors.fill: parent
                source: ""
                fillMode: Image.PreserveAspectFit
                cache: false
                Connections {
                    target: visionModule
                    onCallQmlRefeshImg: {
                        element.text =   Qt.formatTime(new Date(), "hh:mm:ss")
                        prResultPreview1.source = ""
                        prResultPreview1.source = "image://preview1/" + uplookCameraName
                    }
                }

                Text {
                    id: element
                    x: 126
                    y: 0
                    color: "#f2d03d"
                    text: qsTr("No Data")
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.bottom: parent.bottom
                    anchors.bottomMargin: 0
                    font.pixelSize: 20
                }
            }
        }

        Frame {
            id: frame1
            width: 300
            height: 300
        }

        Frame {
            id: frame2
            width: 300
            height: 300
        }
    }
}


















/*##^## Designer {
    D{i:14;anchors_x:126}D{i:11;anchors_height:100;anchors_width:100}D{i:5;anchors_height:200;anchors_width:200}
}
 ##^##*/
