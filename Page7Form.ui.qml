import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Window 2.0
import "qml"

Page {
    id: page7
    width: 1580
    height: 720
    property alias featureButton: featureButton
    property alias navigationFeatureButton: navigationFeatureButton
    property alias drawPathButton: drawPathButton
    property alias yLevelDrawPathButton: yLevelDrawPathButton

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
            display: AbstractButton.TextOnly
            icon.name: "navigation"
            icon.source: "icons/select.png"
            Layout.fillWidth: true
            Layout.fillHeight: true
        }

        Button {
            id: featureButton
            text: qsTr("机械视觉")
            display: AbstractButton.TextOnly
            icon.source: "icons/select.png"
            Layout.fillWidth: true
            Layout.fillHeight: true
        }

        Button {
            id: drawPathButton
            text: qsTr("画胶")
            display: AbstractButton.TextOnly
            icon.source: "icons/select.png"
            Layout.fillWidth: true
            Layout.fillHeight: true
        }

        Button {
            id: yLevelDrawPathButton
            text: qsTr("Y Level")
            display: AbstractButton.TextOnly
            icon.source: "icons/select.png"
            Layout.fillWidth: true
            Layout.fillHeight: true
        }
    }

    TCPMachineConfigListView {
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
