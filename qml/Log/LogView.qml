import QtQuick 2.12
import QtQuick.Controls 2.5


Rectangle {
    id: rectangle
    border.color: "#000000"
    color: "#ffffff"

    TableView {
        id: tableView
        clip: true
        anchors.fill: parent
        anchors.margins: 5

        ScrollBar.vertical: ScrollBar{
            contentItem: Rectangle {
                implicitWidth: 6
                radius: width / 2
                color: "gray"
            }
        }

        ScrollBar.horizontal: ScrollBar{
            contentItem: Rectangle {
                implicitWidth: 6
                radius: width / 2
                color: "gray"
            }
        }

        model: logModel

        delegate: Text {
            text: logString
        }
    }
}
