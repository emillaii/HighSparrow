import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11
import QtQuick.Dialogs 1.2

ColumnLayout {
    id: columnLayout
    anchors.fill: parent


    RowLayout {
        FileDialog {
            id: loadfileDialog
            title: "选择加载PR文件"
            selectExisting: true
            selectFolder: false
            selectMultiple: false

            nameFilters: ["avdata文件 (*.avdata)"]
            onAccepted: {
                downlookPRFileName.text = loadfileDialog.fileUrl
                sutParams.setPRName(loadfileDialog.fileUrl)
            }
        }
        Button {
            text: qsTr("Load PR")
            onClicked: {
                loadfileDialog.open()
            }
        }
        Slider {
            id: slider
            value: 0.5
        }

        Button {
            id: button
            text: qsTr("Start Uplook Cali")
            onClicked: {
                baseModuleManager.performUplookCalibration()
            }
        }

        Text {
            id: downlookPRFileName
            color: "#57f529"
            text: sutParams.prName
            Layout.fillHeight: false
            Layout.fillWidth: true
            font.pixelSize: 27
        }


    }

    RowLayout {
        FileDialog {
            id: loadUplookFileDialog
            title: "选择加载PR文件"
            selectExisting: true
            selectFolder: false
            selectMultiple: false

            nameFilters: ["avdata文件 (*.avdata)"]
            onAccepted: {
                uplookPRFileName.text = loadUplookFileDialog.fileUrl
                lutParams.setPRName(loadUplookFileDialog.fileUrl)
            }
        }
        Button {
            id: button2
            text: qsTr("Load PR")
            onClicked: {
                loadUplookFileDialog.open()
            }
        }

        Slider {
            id: slider1
            value: 0.5
        }

        Button {
            id: button3
            text: qsTr("Start Downlook Cali")
            onClicked: {
                baseModuleManager.performUplookCalibration()
            }
        }

        Text {
            color: "#57f529"
            text: lutParams.prName
            Layout.fillHeight: false
            Layout.fillWidth: true
            font.pixelSize: 27
        }


    }

    RowLayout {
        FileDialog {
            id: loadUpDnlookUpCaliFileDialog
            title: "选择加载PR文件"
            selectExisting: true
            selectFolder: false
            selectMultiple: false

            nameFilters: ["avdata文件 (*.avdata)"]
            onAccepted: {

            }
        }
        Button {
            text: qsTr("Load UpDnlook Up Cali PR")
            onClicked: {
                loadUpDnlookUpCaliFileDialog.open()
            }
        }

        Text {
            color: "#57f529"
            text: qsTr("Testing")
            Layout.fillHeight: false
            Layout.fillWidth: true
            font.pixelSize: 27
        }
    }

    RowLayout {
        FileDialog {
            id: loadUpDnlookDownCaliFileDialog
            title: "选择加载PR文件"
            selectExisting: true
            selectFolder: false
            selectMultiple: false

            nameFilters: ["avdata文件 (*.avdata)"]
            onAccepted: {

            }
        }
        Button {
            text: qsTr("Load UpDnlook Down Cali PR")
            onClicked: {
                loadUpDnlookUpCaliFileDialog.open()
            }
        }

        Button {
            text: qsTr("Start UpDnLook Cali")
            onClicked: {
                baseModuleManager.performUpDownlookCalibration()
            }
        }
        Text {
            color: "#57f529"
            text: qsTr("Testing")
            Layout.fillHeight: false
            Layout.fillWidth: true
            font.pixelSize: 27
        }
    }
    RowLayout {
        Button {
            text: qsTr("Save Calibration Params")
            onClicked: {
            }
        }
    }
}




















/*##^## Designer {
    D{i:0;autoSize:true;height:480;width:640}D{i:1;anchors_height:100;anchors_width:100}
}
 ##^##*/
