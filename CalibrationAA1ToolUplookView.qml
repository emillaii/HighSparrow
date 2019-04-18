import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11
import QtQuick.Dialogs 1.2

ColumnLayout {
    RowLayout {
        FileDialog {
            id: loadAA1ToolUplookFileDialog
            title: "选择加载PR文件"
            selectExisting: true
            selectFolder: false
            selectMultiple: false

            nameFilters: ["avdata文件 (*.avdata)"]
            onAccepted: {
                prAA1ToolUplookParams.setPrFileName(fileUrl)
            }
        }

        Dial {
            width: 25
            from: 0
            value: prAA1ToolUplookParams.lightBrightness
            to: 255
            stepSize: 1

            Label {
                text: prAA1ToolUplookParams.lightBrightness
                color: "white"
                font.pixelSize: Qt.application.font.pixelSize * 3
                anchors.centerIn: parent
            }
            onValueChanged: {
                if (calibrationViewSwitch.checked) {
                    prAA1ToolUplookParams.setLightBrightness(value)
                    lightingController.setUplookLighting(value)
                }
            }
        }

        Button {
            text: qsTr("Load PR")
            onClicked: {
                loadAA1ToolUplookFileDialog.open()
            }
        }

        TextField {
            color: "#57f529"
            text: prAA1ToolUplookParams.prFileName
            font.pixelSize: 14
        }
    }
    RowLayout {
        Button {
            text: qsTr("Move To Tool Uplook Position")
            onClicked: {
                lutModule.moveToToolUplookPos(true)
            }
        }

        Button {
            text: qsTr("Start Mushroom Cali")
            onClicked: {
                baseModuleManager.performUpDownlookUpCalibration()
            }
        }
        Button {
            text: qsTr("Perform PR")
            onClicked: {
                lutModule.toolUplookPR(false,true)
            }
        }
    }
}
