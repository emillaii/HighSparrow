import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11
import QtQuick.Dialogs 1.2
import LogicManagerLib 1.1
import SomeLib 1.1

Column {
    RowLayout {
        RoundButton{
            id: cpa_carrier_camera_location_lock
            transformOrigin: Item.Center
            display: Button.TextBesideIcon
            icon.width: 30
            icon.height: 30
            icon.source: "../../../icons/unlock.png"
            icon.color: "red"
            onClicked: {
                cpa_carrier_camera_location_dial.enabled = !cpa_carrier_camera_location_dial.enabled
                if (cpa_carrier_camera_location_dial.enabled) {
                    cpa_carrier_camera_location_lock.icon.color = "lightGreen"
                } else {
                    cpa_carrier_camera_location_lock.icon.color = "red"
                }
            }
        }
        Dial{
            id: cpa_carrier_camera_location_dial
            enabled: false
            width: 25
            from: 0
            value: cpa_carrier_camera_location.lightBrightness
            to: 255
            stepSize: 1
            Label {
                text: cpa_carrier_camera_location.lightBrightness
                color: "white"
                font.pixelSize: Qt.application.font.pixelSize * 3
                anchors.centerIn: parent
            }
            onValueChanged: {
            }
        }
        Button{
            text:qsTr("CPA Carrier Camera PR Draw")
            onClicked: {
            }
        }
        TextField{
            color: "#57f529"
            text: cpa_carrier_camera_location.prFileName
            font.pixelSize: 14
            width: 600
            Layout.preferredWidth: 600
        }
    }
    RowLayout {
        FileDialog{
            id: cpa_carrier_camera_location_file_dialog
            title:"选择加载PR文件"
            selectExisting: true
            selectFolder: false
            selectMultiple: false

            nameFilters: ["avdata文件 (*.avdata)"]
            onAccepted:{
                cpa_carrier_camera_location.setPrFileName(fileUrl)
            }
        }
        Button{
            text:qsTr("读取PR文件")
            onClicked: {
                cpa_carrier_camera_location_file_dialog.open()
            }
        }
        Button{
            text:qsTr("执行PR")
            onClicked:{
                logicManagerState.setCurrentLocationName(cpa_carrier_camera_location.locationName)
                logicManagerState.setUseOriginPr(true)
                logicManager.performHandling(LogicManager.PERFORM_LOCATION)
            }
        }
        Button{
            text:qsTr("执行PR偏移")
            onClicked:{
                logicManagerState.setCurrentLocationName(cpa_carrier_camera_location.locationName)
                logicManagerState.setUseOriginPr(false)
                logicManager.performHandling(LogicManager.PERFORM_LOCATION)
            }
        }
    }
}
