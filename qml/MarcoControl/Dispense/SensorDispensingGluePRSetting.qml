import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11
import QtQuick.Dialogs 1.2
import LogicManagerLib 1.1
import SomeLib 1.1

ColumnLayout{
    RowLayout {
        RoundButton{
            id: aa1_downLook_location_lock
            transformOrigin: Item.Center
            display: Button.TextBesideIcon
            icon.width: 30
            icon.height: 30
            icon.source: "../../../icons/unlock.png"
            icon.color: "red"
            onClicked: {
                aa1_downLook_location_dial.enabled = !aa1_downLook_location_dial.enabled
                if (aa1_downLook_location_dial.enabled) {
                    aa1_downLook_location_lock.icon.color = "lightGreen"
                } else {
                    aa1_downLook_location_lock.icon.color = "red"
                }
            }
        }
        Dial{
            id: aa1_downLook_location_dial
            enabled: false
            width: 25
            from: 0
            value: aa1_downLook_location.lightBrightness
            to: 255
            stepSize: 1
            Label {
                text: aa1_downLook_location.lightBrightness
                color: "white"
                font.pixelSize: Qt.application.font.pixelSize * 3
                anchors.centerIn: parent
            }
            onValueChanged: {
            }
        }
        Button{
            text:qsTr("SUT 1 Dispensing PR Draw")
            onClicked: {
            }
        }
        TextField{
            color: "#57f529"
            text: aa1_downLook_location.prFileName
            font.pixelSize: 14
            width: 600
            Layout.preferredWidth: 600
        }
    }
    RowLayout {
        FileDialog{
            id:aa1_downLook_location_file_dialog
            title:"选择加载PR文件"
            selectExisting: true
            selectFolder: false
            selectMultiple: false

            nameFilters: ["avdata文件 (*.avdata)"]
            onAccepted:{
                aa1_downLook_location.setPrFileName(fileUrl)
            }
        }
        Button{
            text:qsTr("读取PR文件")
            onClicked: {
                aa1_downLook_location_file_dialog.open()
            }
        }
        Button{
            text:qsTr("执行PR")
            onClicked:{
                logicManagerState.setCurrentLocationName(aa1_downLook_location.locationName)
                logicManagerState.setUseOriginPr(true)
                logicManager.performHandling(LogicManager.PERFORM_LOCATION)
            }
        }
        Button{
            text:qsTr("执行PR偏移")
            onClicked:{
                logicManagerState.setCurrentLocationName(aa1_downLook_location.locationName)
                logicManagerState.setUseOriginPr(false)
                logicManager.performHandling(LogicManager.PERFORM_LOCATION)
            }
        }
    }

    //---------------------------------------
    RowLayout {
        RoundButton{
            id: aa2_downLook_location_lock
            transformOrigin: Item.Center
            display: Button.TextBesideIcon
            icon.width: 30
            icon.height: 30
            icon.source: "../../../icons/unlock.png"
            icon.color: "red"
            onClicked: {
                aa2_downLook_location_dial.enabled = !aa2_downLook_location_dial.enabled
                if (aa2_downLook_location_dial.enabled) {
                    aa2_downLook_location_lock.icon.color = "lightGreen"
                } else {
                    aa2_downLook_location_lock.icon.color = "red"
                }
            }
        }
        Dial{
            id: aa2_downLook_location_dial
            enabled: false
            width: 25
            from: 0
            value: aa2_downLook_location.lightBrightness
            to: 255
            stepSize: 1
            Label {
                text: aa2_downLook_location.lightBrightness
                color: "white"
                font.pixelSize: Qt.application.font.pixelSize * 3
                anchors.centerIn: parent
            }
            onValueChanged: {
            }
        }
        Button{
            text:qsTr("SUT 2 Dispensing PR Draw")
            onClicked: {
            }
        }
        TextField{
            color: "#57f529"
            text: aa2_downLook_location.prFileName
            font.pixelSize: 14
            width: 600
            Layout.preferredWidth: 600
        }
    }
    RowLayout {
        FileDialog{
            id:aa2_downLook_location_file_dialog
            title:"选择加载PR文件"
            selectExisting: true
            selectFolder: false
            selectMultiple: false

            nameFilters: ["avdata文件 (*.avdata)"]
            onAccepted:{
                aa2_downLook_location.setPrFileName(fileUrl)
            }
        }
        Button{
            text:qsTr("读取PR文件")
            onClicked: {
                aa2_downLook_location_file_dialog.open()
            }
        }
        Button{
            text:qsTr("执行PR")
            onClicked:{
                logicManagerState.setCurrentLocationName(aa2_downLook_location.locationName)
                logicManagerState.setUseOriginPr(true)
                logicManager.performHandling(LogicManager.PERFORM_LOCATION)
            }
        }
        Button{
            text:qsTr("执行PR偏移")
            onClicked:{
                logicManagerState.setCurrentLocationName(aa2_downLook_location.locationName)
                logicManagerState.setUseOriginPr(false)
                logicManager.performHandling(LogicManager.PERFORM_LOCATION)
            }
        }
    }

    RowLayout {
        Button {
            text: qsTr("SUT 1 PR Position");
        }

        Button {
            text: qsTr("SUT 2 PR Position");
        }
    }
}
