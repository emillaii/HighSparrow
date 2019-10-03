import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11
import QtQuick.Dialogs 1.2
import LogicManagerLib 1.1
import SomeLib 1.1
GroupBox{
   title: lpa_lens_location.locationName
   ColumnLayout {
       RowLayout {
           RoundButton{
               id: lpa_lens_location_lock
               transformOrigin: Item.Center
               display: Button.TextBesideIcon
               icon.width: 30
               icon.height: 30
               icon.source: "../../../icons/unlock.png"
               icon.color: "red"
               onClicked: {
                   lpa_lens_location_dial.enabled = !lpa_lens_location_dial.enabled
                   if (lpa_lens_location_dial.enabled) {
                       lpa_lens_location_lock.icon.color = "lightGreen"
                   } else {
                       lpa_lens_location_lock.icon.color = "red"
                   }
               }
           }

           Dial{
               id: lpa_lens_location_dial
               enabled: false
               width: 25
               from: 0
               value: lpa_lens_location.lightBrightness
               to: 255
               stepSize: 1
               Label {
                   text: lpa_lens_location.lightBrightness
                   color: "white"
                   font.pixelSize: Qt.application.font.pixelSize * 3
                   anchors.centerIn: parent
               }
               onValueChanged: {
                   lpa_lens_location.setLightBrightness(value)
                   baseModuleManager.setLightingBrightness(lpa_lens_location.locationName)
               }
           }
           Button{
               text:qsTr("LPA Lens PR Draw")
               onClicked: {

               }
           }
           TextField{
               color: "#57f529"
               text: lpa_lens_location.prFileName
               font.pixelSize: 14
               width: 600
               Layout.preferredWidth: 600
           }
       }
       RowLayout {
           FileDialog{
               id:lpa_lens_location_file_dialog
               title:"选择加载PR文件"
               selectExisting: true
               selectFolder: false
               selectMultiple: false

               nameFilters: ["avdata文件 (*.avdata)"]
               onAccepted:{
                   lpa_lens_location.setPrFileName(fileUrl)
               }
           }
           Button{
               text:qsTr("读取PR文件")
               onClicked: {
                   lpa_lens_location_file_dialog.open()
               }
           }
           Button{
               text:qsTr("执行PR")
               onClicked:{
                   logicManagerState.setCurrentLocationName(lpa_lens_location.locationName)
                   logicManagerState.setUseOriginPr(true)
                   logicManager.performHandling(LogicManager.PERFORM_LOCATION)
               }
           }
           Button{
               text:qsTr("执行PR偏移")
               onClicked:{
                   logicManagerState.setCurrentLocationName(lpa_lens_location.locationName)
                   logicManagerState.setUseOriginPr(false)
                   logicManager.performHandling(LogicManager.PERFORM_LOCATION)
               }
           }
       }
       RowLayout {
           RoundButton{
               id: lpa_vacancy_location_lock
               transformOrigin: Item.Center
               display: Button.TextBesideIcon
               icon.width: 30
               icon.height: 30
               icon.source: "../../../icons/unlock.png"
               icon.color: "red"
               onClicked: {
                   lpa_vacancy_location_dial.enabled = !lpa_vacancy_location_dial.enabled
                   if (lpa_vacancy_location_dial.enabled) {
                       lpa_vacancy_location_lock.icon.color = "lightGreen"
                   } else {
                       lpa_vacancy_location_lock.icon.color = "red"
                   }
               }
           }
           Dial{
               id: lpa_vacancy_location_dial
               enabled: false
               width: 25
               from: 0
               value: lpa_vacancy_location.lightBrightness
               to: 255
               stepSize: 1
               Label {
                   text: lpa_vacancy_location.lightBrightness
                   color: "white"
                   font.pixelSize: Qt.application.font.pixelSize * 3
                   anchors.centerIn: parent
               }
               onValueChanged: {
                   lpa_vacancy_location.setLightBrightness(value)
                   baseModuleManager.setLightingBrightness(lpa_vacancy_location.locationName)
               }
           }
           Button{
               text:qsTr("LPA Lens Tray PR Draw")
               onClicked: {

               }
           }
           TextField{
               color: "#57f529"
               text: lpa_vacancy_location.prFileName
               font.pixelSize: 14
               width: 600
               Layout.preferredWidth: 600
           }
       }
       RowLayout {
           FileDialog{
               id:lpa_vacancy_location_file_dialog
               title:"选择加载PR文件"
               selectExisting: true
               selectFolder: false
               selectMultiple: false

               nameFilters: ["avdata文件 (*.avdata)"]
               onAccepted:{
                   lpa_vacancy_location.setPrFileName(fileUrl)
               }
           }
           Button{
               text:qsTr("读取PR文件")
               onClicked: {
                   lpa_vacancy_location_file_dialog.open()
               }
           }
           Button{
               text:qsTr("执行PR")
               onClicked:{
                   logicManagerState.setCurrentLocationName(lpa_vacancy_location.locationName)
                   logicManagerState.setUseOriginPr(true)
                   logicManager.performHandling(LogicManager.PERFORM_LOCATION)
               }
           }
           Button{
               text:qsTr("执行PR偏移")
               onClicked:{
                   logicManagerState.setCurrentLocationName(lpa_vacancy_location.locationName)
                   logicManagerState.setUseOriginPr(false)
                   logicManager.performHandling(LogicManager.PERFORM_LOCATION)
               }
           }
       }
       RowLayout {
           Label { text: qsTr("目标位置") }
           Label{
               text: qsTr("目标行")
           }
           TextField{
               id:t_nrow
               text: "1"
               horizontalAlignment: TextInput.AlignHCenter
               validator: IntValidator{
                   bottom: 1
               }
           }
           Label{
               text: qsTr("目标列")
           }
           TextField{
               id:t_ncol
               text: "1"
               horizontalAlignment: TextInput.AlignHCenter
               validator: IntValidator{
                   bottom: 1
               }
           }
           Button{
               text:title_move_to
               width: 40
               height: 40
               onClicked: {
                   material_tray.setTrayCurrent(t_ncol.text-1,t_nrow.text-1,0)
                   if (baseModuleManager.getServerMode() === 0){
                       logicManager.performHandling(lens_loader_parameter.moduleName,LensLoaderModule.LENS_TRAY1)
                   }else{
                       logicManager.performHandling(sensorLoaderParameter.moduleName,SensorLoaderModule.SENSOR_TRAY_1_POS)
                   }
               }
           }
       }
   }
}

