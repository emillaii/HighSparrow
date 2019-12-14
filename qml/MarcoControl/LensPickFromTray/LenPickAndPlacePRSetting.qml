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
                   lpa_lens_location_slider.enabled = !lpa_lens_location_slider.enabled
                   if (lpa_lens_location_slider.enabled) {
                       lpa_lens_location_lock.icon.color = "lightGreen"
                   } else {
                       lpa_lens_location_lock.icon.color = "red"
                   }
                   lpa_lens_location_slider.value = tcp_vision_location_lpa_lens.lightBrightness
               }
           }
           Slider {
               id: lpa_lens_location_slider
               enabled: false
               width: 25
               from: 0
               value: tcp_vision_location_lpa_lens.lightBrightness
               to: 255
               stepSize: 1
               onValueChanged: {
                   lightingController.setPickarmLighting(value)
                   tcp_vision_location_lpa_lens.setLightBrightness(value)
               }
           }

           Label {
               text: lpa_lens_location_slider.value
           }

           Button{
               text:qsTr("LPA Lens PR Draw")
               onClicked: {
                    highSprrow.callAVSPRSettingProcess("GenericNCCavproj.avexe", "lpa_lens_location");
               }
           }
           RoundButton{
               transformOrigin: Item.Center
               display: Button.TextBesideIcon
               icon.width: 30
               icon.height: 30
               icon.source: "../../../icons/refresh.png"
               icon.color: "cyan"
               onClicked: {
                   lpa_lens_location_image.source = ""
                   lpa_lens_location_image.source = "file:///" + dirPath + "//config//prConfig//lpa_lens_location_resultImage.jpg"
               }
           }
           TextField{
               color: "#57f529"
               text: tcp_vision_location_lpa_lens.locationName
               enabled: false
               font.pixelSize: 14
               width: 600
               Layout.preferredWidth: 600
           }
       }
       RowLayout {
           Button{
               text:qsTr("执行calibration")
               onClicked:{
                   logicManager.performHandling("", LogicManager.HANDLING_CALIBRATION, "lpa_lens_location")
               }
           }
           Button{
               text:qsTr("执行PR")
               onClicked:{
                   logicManager.performHandling("", LogicManager.HANDLING_ORIGIN_LOCATION, "lpa_lens_location")
               }
           }
           Button{
               text:qsTr("执行PR偏移")
               onClicked:{
                   logicManager.performHandling("", LogicManager.HANDLING_OFFSET_LOCATION, "lpa_lens_location")
               }
           }
           Image{
               id: lpa_lens_location_image
               cache: false
               sourceSize.width: 250
               sourceSize.height: 250
               fillMode: Image.PreserveAspectFit
               source: "file:///" + dirPath + "//config//prConfig//lpa_lens_location_resultImage.jpg"
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
                   lpa_vacancy_location_slider.enabled = !lpa_vacancy_location_slider.enabled
                   if (lpa_vacancy_location_slider.enabled) {
                       lpa_vacancy_location_lock.icon.color = "lightGreen"
                   } else {
                       lpa_vacancy_location_lock.icon.color = "red"
                   }
                   lpa_vacancy_location_slider.value = tcp_vision_location_lpa_vacancy.lightBrightness
               }
           }
           Slider {
               id: lpa_vacancy_location_slider
               enabled: false
               width: 25
               from: 0
               value: tcp_vision_location_lpa_vacancy.lightBrightness
               to: 255
               stepSize: 1
               onValueChanged: {
                   lightingController.setPickarmLighting(value)
                   tcp_vision_location_lpa_vacancy.setLightBrightness(value)
               }
           }

           Label {
               text: lpa_vacancy_location_slider.value
           }
           Button{
               text:qsTr("LPA Lens Tray PR Draw")
               onClicked: {
                    highSprrow.callAVSPRSettingProcess("GenericNCCavproj.avexe", "lpa_vacancy_location");
               }
           }
           RoundButton{
               transformOrigin: Item.Center
               display: Button.TextBesideIcon
               icon.width: 30
               icon.height: 30
               icon.source: "../../../icons/refresh.png"
               icon.color: "cyan"
               onClicked: {
                   lpa_vacancy_location_image.source = ""
                   lpa_vacancy_location_image.source = "file:///" + dirPath + "//config//prConfig//lpa_vacancy_location_resultImage.jpg"
               }
           }
           TextField{
               color: "#57f529"
               text: tcp_vision_location_lpa_vacancy.locationName
               enabled: false
               font.pixelSize: 14
               width: 400
               Layout.preferredWidth: 600
           }
       }
       RowLayout {
           Button{
               text:qsTr("执行PR")
               onClicked:{
                   logicManager.performHandling("", LogicManager.HANDLING_ORIGIN_LOCATION, "lpa_vacancy_location")
               }
           }
           Button{
               text:qsTr("执行PR偏移")
               onClicked:{
                   logicManager.performHandling("", LogicManager.HANDLING_OFFSET_LOCATION, "lpa_vacancy_location")
               }
           }
           Image{
               id: lpa_vacancy_location_image
               cache: false
               sourceSize.width: 250
               sourceSize.height: 250
               fillMode: Image.PreserveAspectFit
               source: "file:///" + dirPath + "//config//prConfig//lpa_vacancy_location_resultImage.jpg"
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
                   var param = {}
                   param["col"] = t_ncol.text-1
                   param["row"] = t_nrow.text-1
                   param["tray_index"] = 0
                   logicManager.performHandling(tcp_lens_loader_parameter.moduleName,LensLoaderModule.LENS_TRAY1, JSON.stringify(param))
               }
           }
       }
   }
}

