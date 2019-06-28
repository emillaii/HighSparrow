import QtQuick 2.9
import QtQuick.Controls 2.5

Item {
    width: 400
    height: 400

    property alias listView: listView

    Rectangle {
        id: rectangle
        anchors.fill: parent
        border.color: "#000000"
        color: "#ffffff"

        ListView {
            id: listView
            clip: true
            anchors.fill: parent
            anchors.margins: 5
            contentWidth: 100

            ScrollBar.vertical: ScrollBar{
                contentItem: Rectangle {
                    implicitWidth: 6
                    radius: width / 2
                    color: "gray"
                }
            }

            model: logModel

            delegate: Text {
                text: logString
                anchors.left: parent.left
                anchors.right: parent.right
            }

            onCountChanged: {
                currentIndex = count-1
            }

        }
    }
}
