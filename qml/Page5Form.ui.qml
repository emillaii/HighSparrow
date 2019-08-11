import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Window 2.2
import QtQuick.Layouts 1.11

Page {
    width: 1280
    height: 720
    header: Label {
        text: qsTr("Page 5")
        font.pixelSize: Qt.application.font.pixelSize * 2
        padding: 10
    }
    ColumnLayout {
        anchors.fill: parent
        DiagnosticsView {
            Layout.fillHeight: true
            Layout.fillWidth: true
        }
    }
}
