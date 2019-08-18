import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Window 2.2
import "qml"

Page {
    property alias webView: webView
    id: page3
    width: 1280
    height: 720
    header: Label {
        text: qsTr("Page 3")
        font.pixelSize: Qt.application.font.pixelSize * 2
        padding: 10
    }

    Frame {
        x: 0
        y: 0
        anchors.fill: parent
        visible: true
        FlowchartWeb {
            id: webView
        }
    }
}
