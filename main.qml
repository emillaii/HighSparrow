import QtQuick 2.9
import QtQuick.Controls 2.2

ApplicationWindow {
    visible: true
    width: 1320
    height: 720
    title: qsTr("High Sparrow")

    header: ToolBar {
        id: toolBar
        height: 80
        anchors.right: parent.right
        anchors.rightMargin: 8
        anchors.left: parent.left
        anchors.leftMargin: 8
        anchors.top: parent.top
        anchors.topMargin: 8

        ToolButton {
            id: homeButton
            objectName: "HomeButtonObject"
            text: qsTr("Home")
            transformOrigin: Item.Center
            display: Button.TextUnderIcon
            anchors.top: parent.top
            anchors.topMargin: 0
            anchors.left: parent.left
            anchors.leftMargin: 8
            icon.width: 50
            icon.height: 50
            icon.source: "icons/home.png"
            onClicked: {
                highSprrow.home();
            }
        }
    }

    SwipeView {
        id: swipeView
        anchors.fill: parent
        currentIndex: tabBar.currentIndex

        Page1Form {
        }

        Page2Form {
        }
    }

    footer: TabBar {
        id: tabBar
        currentIndex: swipeView.currentIndex

        TabButton {
            text: qsTr("Page 1")
        }
        TabButton {
            text: qsTr("Page 2")
        }
    }
}
