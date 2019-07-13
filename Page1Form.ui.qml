import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Window 2.0

Page {
    id: page1
    width: 1580
    height: 720
    property alias featureButton: featureButton
    property alias navigationFeatureButton: navigationFeatureButton
    property alias drawPathButton: drawPathButton

    ColumnLayout {
        id: mainRowLayout
        width: 120
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        anchors.top: parent.top
        anchors.topMargin: 10
        anchors.bottomMargin: 10

        Button {
            id: navigationFeatureButton
            text: qsTr("边缘视觉")
            icon.name: "navigation"
            icon.source: "icons/select.png"
            Layout.fillWidth: true
            Layout.fillHeight: true
        }

        Button {
            id: featureButton
            text: qsTr("机械视觉")
            icon.source: "icons/select.png"
            Layout.fillWidth: true
            Layout.fillHeight: true
        }

        Button {
            id: drawPathButton
            text: qsTr("画胶")
            icon.source: "icons/select.png"
            Layout.fillWidth: true
            Layout.fillHeight: true
        }
    }

    MachineConfigListView {
        id: machineConfigListView
        anchors.left: mainRowLayout.right
        anchors.bottom: parent.bottom
        anchors.top: parent.top
        anchors.topMargin: 10
        anchors.bottomMargin: 10
        anchors.leftMargin: 10
    }

    CameraView {
        id: cameraView
        clip: false
        anchors.left: machineConfigListView.right
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.topMargin: 10
        anchors.bottomMargin: 10
        anchors.leftMargin: 10
        anchors.rightMargin: 10
    }
}
