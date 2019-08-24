import QtQuick.Controls 2.4
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.11
import QtQuick 2.11
import QtQuick.Window 2.0

Window {

    property int count: 0
    id: myWindow

    width: 800
    height: 200

    flags:  Qt.Window | Qt.WindowSystemMenuHint
            | Qt.WindowTitleHint | Qt.WindowMinimizeButtonHint
            | Qt.WindowMaximizeButtonHint//| Qt.WindowStaysOnTopHint

    modality: Qt.NonModal // no need for this as it is the default value

    function updateBusyDialogStatus() {
        if (logicManagerState.isHandling) {
            count++;
            stateLabel.text = logicManagerState.handlingMessage
            timerLabel.text = Math.floor(count*0.5) + " s"
            busyDialog.show()
        } else {
            count = -1;
            busyDialog.close()
        }
    }

    Rectangle {
        color: "black"
        anchors.fill: parent
        RowLayout {
            Label {
                id: stateLabel
                text: logicManagerState.handlingMessage
                font.pointSize: 20
                color: "white"
            }
            BusyIndicator {}
            Label {
                id: timerLabel
                font.pointSize: 20
                color: "skyblue"
            }
        }
    }
}
