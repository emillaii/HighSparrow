import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11
import QtQuick.Dialogs 1.2
import LogicManagerLib 1.1
import SomeLib 1.1

ColumnLayout {
    Label { text: qsTr("PR Draw") }
    RowLayout {
        RoundButton{
            id: lut1_pocket_location_lock
            transformOrigin: Item.Center
            display: Button.TextBesideIcon
            icon.width: 30
            icon.height: 30
            icon.source: "../../../icons/unlock.png"
            icon.color: "red"
            onClicked: {
                lut1_pocket_location_dial.enabled = !lut1_pocket_location_dial.enabled
                if (lut1_pocket_location_dial.enabled) {
                    lut1_pocket_location_lock.icon.color = "lightGreen"
                } else {
                    lut1_pocket_location_lock.icon.color = "red"
                }
            }
        }
        Dial{
            id: lut1_pocket_location_dial
            enabled: false
            width: 25
            from: 0
            value: lut1_pocket_location.lightBrightness
            to: 255
            stepSize: 1
            Label {
                text: lut1_pocket_location.lightBrightness
                color: "white"
                font.pixelSize: Qt.application.font.pixelSize * 3
                anchors.centerIn: parent
            }
            onValueChanged: {
            }
        }
        Button{
            text:qsTr("LUT1 Pocket PR Draw")
            onClicked: {
            }
        }
        TextField{
            color: "#57f529"
            text: lut1_pocket_location.prFileName
            font.pixelSize: 14
            width: 600
            Layout.preferredWidth: 600
        }
    }
    RowLayout {
        FileDialog{
            id:lut1_pocket_location_file_dialog
            title:"选择加载PR文件"
            selectExisting: true
            selectFolder: false
            selectMultiple: false
            nameFilters: ["avdata文件 (*.avdata)"]
            onAccepted:{
                lut1_pocket_location.setPrFileName(fileUrl)
            }
        }
        Button{
            text:qsTr("读取PR文件")
            onClicked: {
                lut1_pocket_location_file_dialog.open()
            }
        }
        Button{
            text:qsTr("执行PR")
            onClicked:{
            }
        }
        Button{
            text:qsTr("执行PR偏移")
            onClicked:{
            }
        }
    }

    RowLayout {
        RoundButton{
            id: lut2_pocket_location_lock
            transformOrigin: Item.Center
            display: Button.TextBesideIcon
            icon.width: 30
            icon.height: 30
            icon.source: "../../../icons/unlock.png"
            icon.color: "red"
            onClicked: {
                lut2_pocket_location_dial.enabled = !lut2_pocket_location_dial.enabled
                if (lut2_pocket_location_dial.enabled) {
                    lut2_pocket_location_lock.icon.color = "lightGreen"
                } else {
                    lut2_pocket_location_lock.icon.color = "red"
                }
            }
        }
        Dial{
            id: lut2_pocket_location_dial
            enabled: false
            width: 25
            from: 0
            value: lut2_pocket_location.lightBrightness
            to: 255
            stepSize: 1
            Label {
                text: lut2_pocket_location.lightBrightness
                color: "white"
                font.pixelSize: Qt.application.font.pixelSize * 3
                anchors.centerIn: parent
            }
            onValueChanged: {
            }
        }
        Button{
            text:qsTr("LUT2 Pocket PR Draw")
            onClicked: {
            }
        }
        TextField{
            color: "#57f529"
            text: lut2_pocket_location.prFileName
            font.pixelSize: 14
            width: 600
            Layout.preferredWidth: 600
        }
    }
    RowLayout {
        FileDialog{
            id:lut2_pocket_location_file_dialog
            title:"选择加载PR文件"
            selectExisting: true
            selectFolder: false
            selectMultiple: false
            nameFilters: ["avdata文件 (*.avdata)"]
            onAccepted:{
                lut2_pocket_location.setPrFileName(fileUrl)
            }
        }
        Button{
            text:qsTr("读取PR文件")
            onClicked: {
                lut2_pocket_location_file_dialog.open()
            }
        }
        Button{
            text:qsTr("执行PR")
            onClicked:{
            }
        }
        Button{
            text:qsTr("执行PR偏移")
            onClicked:{
            }
        }
    }

    RowLayout {
        RoundButton{
            id: lut_uplook_location_lock
            transformOrigin: Item.Center
            display: Button.TextBesideIcon
            icon.width: 30
            icon.height: 30
            icon.source: "../../../icons/unlock.png"
            icon.color: "red"
            onClicked: {
                lut_uplook_location_dial.enabled = !lut_uplook_location_dial.enabled
                if (lut_uplook_location_dial.enabled) {
                    lut_uplook_location_lock.icon.color = "lightGreen"
                } else {
                    lut_uplook_location_lock.icon.color = "red"
                }
            }
        }
        Dial{
            id: lut_uplook_location_dial
            enabled: false
            width: 25
            from: 0
            value: lut_uplook_location.lightBrightness
            to: 255
            stepSize: 1
            Label {
                text: lut_uplook_location.lightBrightness
                color: "white"
                font.pixelSize: Qt.application.font.pixelSize * 3
                anchors.centerIn: parent
            }
            onValueChanged: {
            }
        }
        Button{
            text:qsTr("LUT Up Look PR Draw")
            onClicked: {
            }
        }
        TextField{
            color: "#57f529"
            text: lut_uplook_location.prFileName
            font.pixelSize: 14
            width: 600
            Layout.preferredWidth: 600
        }
    }
    RowLayout {
        FileDialog{
            id:lut_uplook_location_file_dialog
            title:"选择加载PR文件"
            selectExisting: true
            selectFolder: false
            selectMultiple: false
            nameFilters: ["avdata文件 (*.avdata)"]
            onAccepted:{
                lut_uplook_location.setPrFileName(fileUrl)
            }
        }
        Button{
            text:qsTr("读取PR文件")
            onClicked: {
                lut_uplook_location_file_dialog.open()
            }
        }
        Button{
            text:qsTr("执行PR")
            onClicked:{
            }
        }
        Button{
            text:qsTr("执行PR偏移")
            onClicked:{
            }
        }
    }

    RowLayout { Label { text: qsTr("测试") } }
    RowLayout {
        Button {
            text: qsTr("LUT移动到Load位置")
        }
        Button {
            text: qsTr("LPA移动到LUT1 Pocket PR Position")
        }
        Button {
            text: qsTr("LPA移动到LUT2 Pocket PR Position")
        }
        Button {
            text: qsTr("LPA移动到Up Look PR Position")
        }
    }
}
