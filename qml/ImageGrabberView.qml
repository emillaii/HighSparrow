import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11
import QtQuick.Dialogs 1.2
import FileContentItem 1.0

ItemDelegate {
    width: parent.width
    contentItem: ColumnLayout {
        spacing: 0
        ColumnLayout {
            FileDialog {
                id: loadfileDialog
                title: qsTr("选择加载文件")
                selectExisting: true
                selectFolder: false
                selectMultiple: false

                nameFilters: ["ini文件 (*.ini)"]
                onAccepted: {
                    file.setSource(loadfileDialog.fileUrl)
                    imageGrabber.setIniFilename(loadfileDialog.fileUrl)
                }
            }

            RowLayout {
                id: rowLayout
                width: 100
                height: 100

                RoundButton {
                    id: roundButton
                    text: "+"
                    onClicked: {
                        loadfileDialog.open()
                    }
                }

                Text {
                    id: element1
                    color: "#3ef340"
                    text: qsTr("Ini: ")
                    font.pixelSize: 14
                }

                Text {
                    id: element
                    width: 20
                    height: 35
                    color: "#3ef340"
                    text: imageGrabber.IniFilename
                    Layout.fillWidth: true
                    font.pixelSize: 14
                }

            }

            RowLayout {
                id: rowLayout1
                width: 100
                height: 100

                Button {
                    id: button
                    text: qsTr("Open Camera")
                    onClicked: {
                        baseModuleManager.initSensor()
                    }
                }

                Button {
                    id: button1
                    text: qsTr("Close Camera")
                    onClicked: {
                        baseModuleManager.closeSensor()
                    }
                }
            }
        }
    }
}
