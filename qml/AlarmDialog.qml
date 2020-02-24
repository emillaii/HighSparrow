import QtQuick.Controls 2.4
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.11
import QtQuick 2.11
import QtQuick.Window 2.0

Window {
    property int count: 0
    property bool lightState: true
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
            lightState = !lightState
            if (workersManager.ShowAlarmDialog) {
                alarmDialog.visible = true
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
                    id: moduleRect
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
                                var alarmType = workersManager.getAlarmState(modelData)
                                errorMessage.text = workersManager.getAlarmMessage(modelData)
                                if (alarmType === 0) {
                                    moduleRect.color = "lightGreen"
                                    button1.text = "Ok"
                                    button2.text = "Cancel"
                                } else if (alarmType === 1) {
                                    moduleRect.color = "orange"
                                    button1.text = "Ok"
                                    button2.text = "Cancel"
                                } else if (alarmType === 2) {
                                    moduleRect.color = "orange"
                                    button1.text = "Continue"
                                    button2.text = "Give Up"
                                } else if (alarmType === 3) {
                                    moduleRect.color = "orange"
                                    button1.text = "Continue"
                                    button2.text = "Retry"
                                } else if (alarmType === 4) {
                                    lightState ? moduleRect.color = "pink" : moduleRect.color = "red"
                                    button1.text = "Retry"
                                    button2.text = "Stop"
                                }
                                else {
                                    moduleRect.color = "red"
                                }
                            }
                        }
                    }
                    Text {
                        id: errorMessage
                        anchors.centerIn: parent
                        font.pixelSize: 14
                        text: "modelData"
                        color: "white"
                    }
                }
                Button {
                    id: button1
                    text: qsTr("OK")
                    onClicked: {
                        workersManager.performAlarmOperation(modelData, 0);
                    }
                }
                Button {
                    id: button2
                    text: qsTr("Cancel")
                    onClicked: {
                        workersManager.performAlarmOperation(modelData, 1);
                    }
                }
            }
        }
    }
}

