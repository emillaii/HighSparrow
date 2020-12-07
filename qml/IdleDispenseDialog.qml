import QtQuick.Controls 2.4
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.11
import QtQuick 2.11
import QtQuick.Window 2.0

Window {

    property int count: 0
    property bool isStarted: false
    id: myWindow

    width: 800
    height: 200

    flags:  Qt.Window | Qt.WindowSystemMenuHint
            | Qt.WindowTitleHint | Qt.WindowMinimizeButtonHint
            | Qt.WindowMaximizeButtonHint//| Qt.WindowStaysOnTopHint

    modality: Qt.NonModal // no need for this as it is the default value

    Timer {
        id: idleDispensingTimer
        interval: 60000; running: false; repeat: true
        onTriggered: {
            dispenseModule.purge()
        }
    }

    Rectangle {
        color: "black"
        anchors.fill: parent
        RowLayout {
            Label {
                text: qsTr("Idle Dispensing.....")
            }
            TextField{
                id: idleDispensingPeriod
                text: qsTr("60")
                horizontalAlignment: TextInput.AlignHCenter
                validator: IntValidator {}
            }
            Label {
                text: qsTr("seconds")
            }

            BusyIndicator {
                running: isStarted
            }

            Button {
                text: qsTr("Set Period")
                onClicked: {
                    idleDispensingTimer.interval = idleDispensingPeriod.text*1000
                }
            }

            Button {
                text: qsTr("Start")
                onClicked: {
                    isStarted = true
                    idleDispensingTimer.start()
                }
            }

            Button {
                text: qsTr("Close")
                onClicked: {
                    isStarted = false
                    idleDispensingTimer.stop()
                    myWindow.visible = false
                }
            }
        }
    }
}
