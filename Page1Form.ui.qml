import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Imagine 2.12
import QtQuick.Window 2.0
import QtQuick.Controls.Material 2.12

Page {
    id: page1
    width: 1280
    height: 720

    header: Label {
        id: label1
        text: qsTr("Page 1")
        font.pixelSize: Qt.application.font.pixelSize * 2
        padding: 10
    }

    Label {
        id: labels
        text: qsTr("You are on Page 1.")
        anchors.verticalCenterOffset: 296
        anchors.horizontalCenterOffset: 450
        anchors.centerIn: parent
    }

    RowLayout {
        id: mainRowLayout
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.margins: 24
        spacing: 36

        Container {
            id: leftTabBar

            currentIndex: 1

            Layout.fillWidth: false
            Layout.fillHeight: true

            ButtonGroup {
                buttons: columnLayout.children
            }

            contentItem: ColumnLayout {
                id: columnLayout
                spacing: 3

                Repeater {
                    model: leftTabBar.contentModel
                }
            }

            FeatureButton {
                id: navigationFeatureButton
                text: qsTr("Navigation")
                icon.name: "navigation"
                Layout.fillHeight: true
            }

            FeatureButton {
                text: qsTr("Navigation")
                icon.name: "music"
                checked: true
                Layout.fillHeight: true
            }

            FeatureButton {
                text: qsTr("Navigation")
                icon.name: "message"
                Layout.fillHeight: true
            }

            FeatureButton {
                text: qsTr("Navigation")
                icon.name: "command"
                Layout.fillHeight: true
            }

            FeatureButton {
                text: qsTr("Settings")
                icon.name: "settings"
                Layout.fillHeight: true
            }
        }
    }

    ListView {
        id: machineConfigListView
        anchors.rightMargin: 668
        anchors.bottomMargin: 49
        anchors.leftMargin: 112
        anchors.topMargin: 35
        anchors.fill: parent
        model: MachineConfigModel {
        }
        delegate: MachineConfigDelegate {
        }
    }

    CameraView {
        id: cameraView
        x: 659
        width: 624
        anchors.bottomMargin: 150
        anchors.topMargin: -38
        anchors.rightMargin: -3
    }

    Rectangle {
        id: rectangle
        x: 659
        width: 611
        color: "#ffffff"
        anchors.right: parent.right
        anchors.rightMargin: 10
        anchors.top: parent.top
        anchors.topMargin: 589
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 8
    }
}
