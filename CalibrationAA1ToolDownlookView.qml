import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11
import QtQuick.Dialogs 1.2

ColumnLayout {
    RowLayout {
        FileDialog {
            id: loadAA1ToolDownlookFileDialog
            title: "选择加载PR文件"
            selectExisting: true
            selectFolder: false
            selectMultiple: false

            nameFilters: ["avdata文件 (*.avdata)"]
            onAccepted: {
                prAA1ToolDownlookParams.setPrFileName(fileUrl)
            }
        }

        Dial {
            width: 25
            from: 0
            value: prAA1ToolDownlookParams.lightBrightness
            to: 255
            stepSize: 1

            Label {
                text: prAA1ToolDownlookParams.lightBrightness
                color: "white"
                font.pixelSize: Qt.application.font.pixelSize * 3
                anchors.centerIn: parent
            }
            onValueChanged: {
                if (calibrationViewSwitch.checked) {
                    prAA1ToolDownlookParams.setLightBrightness(value)
                    lightingController.setDownlookLighting(value)
                }
            }
        }

        Button {
            text: qsTr("Load PR")
            onClicked: {
                loadAA1ToolDownlookFileDialog.open()
            }
        }

        TextField {
            color: "#57f529"
            text: prAA1ToolDownlookParams.prFileName
            font.pixelSize: 14
        }
    }
    RowLayout {
        Button {
            text: qsTr("Move To Tool Downlook Position")
            onClicked: {
                sutModule.moveToToolDownlookPos(true)
            }
        }

        Button {
            text: qsTr("Start Mushroom Cali")
            onClicked: {
                baseModuleManager.performUpDownlookCalibration()
            }
        }
        Button {
            text: qsTr("Perform PR")
            onClicked: {
                sutModule.toolDownlookPR(false,true)
            }
        }
    }
}
