import QtQuick 2.9
import QtQuick.Controls 2.2
import QtCharts 2.0
import QtQuick.Layouts 1.12
import QtCharts 2.0
import Qt.labs.calendar 1.0

Page {

    readonly property string downlookCameraName: "AA_DLCamera1"
    readonly property string uplookCameraName: "LUT_ULCamera"
    readonly property string pickarmCameraName: "PickarmCamera"

    id: page2
    width: 1280
    height: 720
    font.family: "Courier"
    spacing: -1
    wheelEnabled: true
    //property alias frame: frame
    header: Label {
        text: qsTr("Page 2")
        font.pixelSize: Qt.application.font.pixelSize * 2
        padding: 10
    }

    Row {
        id: row
        width: 450
        height: 300

        Frame {
            id: frame1
            width: 300
            height: 300
        }

        LogLevelSelectionView{}
    }

    LogView {
        id: logView
        anchors.top: row.top
        anchors.left: row.right
        anchors.bottom: row.bottom
        anchors.right: parent.right
    }

    TrayMapView {
        id: trayMapView
        anchors.top: row.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
    }
}
