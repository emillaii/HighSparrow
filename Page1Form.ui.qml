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
    x: -1
    width: 1280
    height: 720
    property alias featureButton: featureButton
    property alias navigationFeatureButton: navigationFeatureButton

    header: Label {
        text: qsTr("")
        font.pixelSize: Qt.application.font.pixelSize * 2
        padding: 10
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
                id: featureButton
                text: qsTr("机械视觉")
                icon.source: "icons/select.png"
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

    CameraView {
        id: cameraView
        x: 659
        width: 624
        anchors.bottomMargin: 150
        anchors.topMargin: -38
        anchors.rightMargin: -3
    }

    //    ListView {
    //        id: machineConfigListView
    //        anchors.right: parent.right
    //        anchors.rightMargin: 627
    //        anchors.bottom: parent.bottom
    //        anchors.left: parent.left
    //        anchors.top: parent.top
    //        anchors.bottomMargin: 49
    //        anchors.leftMargin: 112
    //        anchors.topMargin: 35
    //        model: MachineSettingModel {
    //        }
    //        delegate: MachineSettingDelegate {
    //        }
    //    }
    TextArea {
        id: textArea
        x: 659
        width: 608
        text: qsTr("Text Area")
        anchors.right: parent.right
        anchors.rightMargin: 13
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 8
        anchors.top: parent.top
        anchors.topMargin: 582
    }
}
