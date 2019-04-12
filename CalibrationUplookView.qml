import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11
import QtQuick.Dialogs 1.2

ColumnLayout {
    RowLayout {
        FileDialog {
            id: loadUplookFileDialog
            title: "选择加载PR文件"
            selectExisting: true
            selectFolder: false
            selectMultiple: false

            nameFilters: ["avdata文件 (*.avdata)"]
            onAccepted: {
                prAA1LUTUplookParams.setPrFileName(fileUrl)
            }
        }

        Dial {
            width: 25
            from: 0
            value: prAA1LUTUplookParams.lightBrightness
            to: 255
            stepSize: 1

            Label {
                text: prAA1LUTUplookParams.lightBrightness
                color: "white"
                font.pixelSize: Qt.application.font.pixelSize * 3
                anchors.centerIn: parent
            }
            onValueChanged: {
                prAA1LUTUplookParams.setLightBrightness(value)
                lightingController.setUplookLighting(value)
            }
        }

        Button {
            text: qsTr("Load PR")
            onClicked: {
                loadUplookFileDialog.open()
            }
        }

        TextField {
            color: "#57f529"
            text: prAA1LUTUplookParams.prFileName
            font.pixelSize: 14
        }
    }
    RowLayout {
        Button {
            text: qsTr("Move To Uplook Position")
            onClicked: {
                lutModule.moveToAA1UplookPos()
            }
        }

        Button {
            text: qsTr("Start Downlook Cali")
            onClicked: {
                baseModuleManager.performUplookCalibration()
            }
        }
        Button {
            text: qsTr("Perform PR")
            onClicked: {
                highSprrow.performLUTUplookPR()
            }
        }
    }
}
