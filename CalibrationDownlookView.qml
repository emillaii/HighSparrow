import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11
import QtQuick.Dialogs 1.2

ColumnLayout {
    RowLayout {
        FileDialog {
            id: loadDownlookFileDialog
            title: "选择加载PR文件"
            selectExisting: true
            selectFolder: false
            selectMultiple: false

            nameFilters: ["avdata文件 (*.avdata)"]
            onAccepted: {
                prSUTDownlookParams.setPrFileName(fileUrl)
            }
        }

        Dial {
            width: 25
            from: 0
            value: prSUTDownlookParams.lightBrightness
            to: 255
            stepSize: 1

            Label {
                text: prSUTDownlookParams.lightBrightness
                color: "white"
                font.pixelSize: Qt.application.font.pixelSize * 3
                anchors.centerIn: parent
            }
            onValueChanged: {
                prSUTDownlookParams.setLightBrightness(value)
                lightingController.setDownlookLighting(value)
            }
        }

        Button {
            text: qsTr("Load PR")
            onClicked: {
                loadDownlookFileDialog.open()
            }
        }

        TextField {
            color: "#57f529"
            text: prSUTDownlookParams.prFileName
            font.pixelSize: 14
        }
    }
    RowLayout {
        Button {
            text: qsTr("Move To Downlook Position")
            onClicked: {
                sutModule.moveToDownlookPos()
            }
        }

        Button {
            text: qsTr("Start Downlook Cali")
            onClicked: {
                baseModuleManager.performDownlookCalibration()
            }
        }
        Button {
            text: qsTr("Perform PR")
            onClicked: {
                highSprrow.performSUTDownlookPR()
            }
        }
    }
}
