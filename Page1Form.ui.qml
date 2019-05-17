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

    header: Label {
        text: qsTr("")
        font.pixelSize: Qt.application.font.pixelSize * 2
        padding: 10
    }

    RowLayout {
        id: rowLayout
        anchors.fill: parent

        RowLayout {
            id: mainRowLayout

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

                Button {
                    id: navigationFeatureButton
                    text: qsTr("边缘视觉")
                    icon.name: "navigation"
                    icon.source: "icons/select.png"
                    Layout.fillHeight: true
                }

                Button {
                    id: featureButton
                    text: qsTr("机械视觉")
                    icon.source: "icons/select.png"
                    Layout.fillHeight: true
                }
            }
        }

        MachineConfigListView {
            id: machineConfigListView
            anchors.right: parent.right
            anchors.rightMargin: 627
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.bottomMargin: 122
            anchors.leftMargin: 112
            anchors.topMargin: -38
        }

        CameraView {
            id: cameraView
            height: 661
            clip: false
            visible: true
            Layout.fillHeight: true
            Layout.fillWidth: true
            anchors.left: mainRowLayout.right
            anchors.leftMargin: 592
            anchors.right: parent.right
            anchors.bottomMargin: 150
            anchors.topMargin: -38
            anchors.rightMargin: 25
        }
    }
}

/*##^## Designer {
    D{i:11;anchors_width:240;anchors_x:400}
}
 ##^##*/
