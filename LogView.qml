import QtQuick 2.4
import QtQuick.Controls 1.2

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
