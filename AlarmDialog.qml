import QtQuick.Controls 2.4
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.11
import QtQuick 2.11
import QtQuick.Window 2.0

Window {
    property int count: 0
    id: alarmWindow
    title: "Alarm Dialog"
    width: 800
    height: 400

    flags:  Qt.Window | Qt.WindowSystemMenuHint
            | Qt.WindowTitleHint | Qt.WindowMinimizeButtonHint
            | Qt.WindowMaximizeButtonHint

    modality: Qt.NonModal // no need for this as it is the default value
    Timer {
        id: timer
        interval: 500; running: true; repeat: true
        onTriggered: {
            if (workerManager.ShowAlarmDialog) {
                console.log("Need to show dialog. Type: " + workerManager.AlarmType)
                alarmDialog.visible = true
               // errorMessage.text = workerManager.AlarmMessage
            } else {
                alarmDialog.visible = false
            }
        }
    }
    Rectangle {
        color: "black"
        anchors.fill: parent
        ListView {
            anchors.fill: parent
            anchors.margins: 20
            clip: true
            model: workerNameList
            delegate: numberDelegate
            spacing: 5
        }
        Component {
            id: numberDelegate
            RowLayout {
                Rectangle {
                    width:  100
                    height: 40
                    color: "lightGreen"
                    Text {
                        anchors.centerIn: parent
                        font.pixelSize: 10
                        text: modelData
                    }
                }
                Rectangle {
                    width:  500
                    height: 40
                    color: "black"
                    Connections{
                        target: timer
                        onTriggered: {
                            if (alarmDialog.visible) {
                                errorMessage.text = workerManager.getAlarmMessage(modelData)
                            }
                        }
                    }
                    Text {
                        id: errorMessage
                        anchors.centerIn: parent
                        font.pixelSize: 10
                        text: "modelData"
                        color: "white"
                    }
                }
                Button {
                    text: qsTr("OK")
                    onClicked: {
                        console.log("OK..." + modelData)
                    }
                }
                Button {
                    text: qsTr("Cancel")
                    onClicked: {
                        console.log("Cancel...")
                    }
                }
            }
        }
    }
}









/*##^## Designer {
    D{i:21;anchors_height:300;anchors_width:80}
}
 ##^##*/
