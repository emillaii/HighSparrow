import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11
import QtQuick.Dialogs 1.2
ColumnLayout {
    RowLayout{
        FileDialog{
            id:loadPrLensLutLookFileDialog
            title: "选择加载PR文件"
            selectExisting: true
            selectFolder: false
            selectMultiple: false

            nameFilters: ["avdata文件 (*.avdata)"]
            onAccepted: {
                prLENSLutLook.setPrFileName(fileUrl)
            }
        }
        Dial{
            width: 25
            from: 0
            value: prLENSLutLook.lightBrightness
            to: 255
            stepSize: 1

            Label {
                text: prLENSLutLook.lightBrightness
                color: "white"
                font.pixelSize: Qt.application.font.pixelSize * 3
                anchors.centerIn: parent
            }
            onValueChanged: {
                if (calibrationViewSwitch.checked) {
                    prLENSLutLook.setLightBrightness(value)
                    lightingController.setUplookLighting(value)
                }
            }
        }
        Button{
            text:qsTr("读取PR文件")
            onClicked: {
                loadPrLensLutLookFileDialog.open()
            }
        }
        TextField{
            color: "#57f529"
            text: prLENSLutLook.prFileName
            font.pixelSize: 14
        }

    }
    RowLayout{
        Button{
            text:qsTr("移动")
            onClicked: {
                logicManager.lensPickArmMoveToLutPos2()
            }
        }
        Button{
            text:qsTr("执行PR")
            onClicked: {
                highSprrow.performLUTLensPR()
            }
        }
    }
}
