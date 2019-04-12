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
                prAA1MushroomParams.setPrFileName(fileUrl)
            }
        }

        Dial {
            width: 25
            from: 0
            value: prAA1MushroomParams.lightBrightness
            to: 255
            stepSize: 1

            Label {
                text: prAA1MushroomParams.lightBrightness
                color: "white"
                font.pixelSize: Qt.application.font.pixelSize * 3
                anchors.centerIn: parent
            }
            onValueChanged: {
                prAA1MushroomParams.setLightBrightness(value)
                lightingController.setUplookLighting(value)
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
            text: prAA1MushroomParams.prFileName
            font.pixelSize: 14
        }
    }
    RowLayout {
        Button {
            text: qsTr("Move To Mushroom Position")
            onClicked: {
                lutModule.moveToAA1MushroomLens(true)
            }
        }

        Button {
            text: qsTr("Start Mushroom Cali")
            onClicked: {
                baseModuleManager.performAA1MushroomHeadCalibration()
            }
        }
        Button {
            text: qsTr("Perform PR")
            onClicked: {
                highSprrow.performLUTMushroomPR()
            }
        }
    }
}
