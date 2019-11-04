import QtQuick 2.9
import QtQuick.Controls 2.2
import QtCharts 2.0
import QtQuick.Layouts 1.12
import QtCharts 2.0
import Qt.labs.calendar 1.0
import "qml/Log"
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

    LogView{
        id: logView
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.margins: 3

        height: parent.height * 0.6
    }
    TrayMapView {
        id: trayMapView
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: logView.bottom
        anchors.bottom: parent.bottom
        anchors.margins: 3
    }

}
