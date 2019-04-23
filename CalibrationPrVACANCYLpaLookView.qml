import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11
import QtQuick.Dialogs 1.2
ColumnLayout {
    RowLayout{
        FileDialog{
            id:loadPrVacancyLpaLookFileDialog
            title: "选择加载PR文件"
            selectExisting: true
            selectFolder: false
            selectMultiple: false

            nameFilters: ["avdata文件 (*.avdata)"]
            onAccepted: {
                prVACANCYLpaLook.setPrFileName(fileUrl)
            }
        }
        Dial{
            width: 25
            from: 0
            value: prVACANCYLpaLook.lightBrightness
            to: 255
            stepSize: 1

            Label {
                text: prVACANCYLpaLook.lightBrightness
                color: "white"
                font.pixelSize: Qt.application.font.pixelSize * 3
                anchors.centerIn: parent
            }
            onValueChanged: {
                if (calibrationViewSwitch.checked) {
                    prVACANCYLpaLook.setLightBrightness(value)
                    lightingController.setUplookLighting(value)
                }
            }
        }
        Button{
            text:qsTr("读取PR文件")
            onClicked: {
                loadPrVacancyLpaLookFileDialog.open()
            }
        }
        TextField{
            color: "#57f529"
            text: prVACANCYLpaLook.prFileName
            font.pixelSize: 14
        }

    }
    RowLayout{
        Button{
            text:qsTr("移动")
            onClicked: {
                logicManager.lensPickArmMoveToTray1StartPos()
            }
        }
        Button{
            text:qsTr("执行PR")
            onClicked: {
                highSprrow.performVacancyLensPR()
            }
        }
    }
}
