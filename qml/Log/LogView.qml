import QtQuick 2.0
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0

Rectangle {
    Layout.fillHeight: true
    Layout.fillWidth: true

    color: "#303030"

    Rectangle{
        id: loglevelSelector
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.margins: 3

        border.width: 1
        width: 80
        color: "#303030"

        ColumnLayout{
            anchors.fill: parent

            RadioButton{
                text: qsTr("Debug")
                checked: logConfig.logLevel == 0

                onClicked: {
                    logConfig.setLogLevel(0)
                }
            }
            RadioButton{
                text: qsTr("Info")
                checked: logConfig.logLevel== 1
                onClicked: {
                    logConfig.setLogLevel(1)
                }
            }
            RadioButton{
                text: qsTr("Warn")
                checked: logConfig.logLevel == 2
                onClicked: {
                    logConfig.setLogLevel(2)
                }
            }
        }
    }

    Rectangle{
        anchors.left: loglevelSelector.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        anchors.margins: 3

        border.width: 1
        color: "#303030"

        ListView{
            id: logView
            anchors.fill: parent
            model: logModel

            clip: true

            delegate: Text {
                width: parent.width - 3
                color: logColor
                text: logStr
                wrapMode: Text.Wrap
                lineHeight: 1.2
                lineHeightMode: Text.ProportionalHeight
            }

            MouseArea{
                z: 1
                anchors.fill: parent
                onDoubleClicked: {
                    logModel.onClearLog()
                }
            }

            ScrollBar.vertical: ScrollBar {
                id: scrollBar
                z: 2
                width: 15
            }

            MouseArea{
                id: logViewMouseArea
                z: 3
                anchors.fill: parent
                hoverEnabled: true
                onPressed: {
                    mouse.accepted = false
                }
            }
        }
    }

    Timer {
        id: scrollTimer;
        repeat: true;
        interval: 200;
        onTriggered: {
            if(!logViewMouseArea.containsMouse)
            {
                logView.positionViewAtEnd()
            }
        }
    }

    Component.onCompleted: {
        scrollTimer.start()
    }
}

