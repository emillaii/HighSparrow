import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11
import QtQuick.Dialogs 1.2
import LogicManagerLib 1.1
import SomeLib 1.1
import LutModuleLib 1.1

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
                lut_load_loaction_slider.enabled = !lut_load_loaction_slider.enabled
                if (lut_load_loaction_slider.enabled) {
                    lut1_pocket_location_lock.icon.color = "lightGreen"
                } else {
                    lut1_pocket_location_lock.icon.color = "red"
                }
                lut_load_loaction_slider.value = tcp_vision_location_lut_load.lightBrightness
            }
        }
        Slider {
            id: lut_load_loaction_slider
            enabled: false
            width: 25
            from: 0
            value: tcp_vision_location_lut_load.lightBrightness
            to: 255
            stepSize: 1
            onValueChanged: {
                tcp_vision_location_lut_load.setLightBrightness(value)
                lightingController.setPickarmLighting(value)
            }
        }

        Label {
            text: lut_load_loaction_slider.value
        }
        Button{
            text:qsTr("LUT1 Pocket PR Draw")
            onClicked: {
                 highSprrow.callAVSPRSettingProcess("GenericNCCavproj.avexe", "lut_load_loaction");
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
                lut_load_loaction_image.source = ""
                lut_load_loaction_image.source = "file:///" + dirPath + "//config//prConfig//lut_load_loaction_resultImage.jpg"
            }
        }
        TextField{
            color: "#57f529"
            text: tcp_vision_location_lut_load.locationName
            font.pixelSize: 14
            width: 200
            Layout.preferredWidth: 200
        }
    }
    RowLayout {
        Button{
            text:qsTr("执行Calibration")
            onClicked:{
                logicManager.performHandling("", logicManager.HANDLING_CALIBRATION, "lut_load_loaction")
            }
        }
        Button{
            text:qsTr("执行PR")
            onClicked:{
                logicManager.performHandling("", LogicManager.HANDLING_ORIGIN_LOCATION, "lut_load_loaction")
            }
        }
        Button{
            text:qsTr("执行PR偏移")
            onClicked:{
                logicManager.performHandling("", LogicManager.HANDLING_OFFSET_LOCATION, "lut_load_loaction")
            }
        }
        Image{
            id: lut_load_loaction_image
            cache: false
            sourceSize.width: 250
            sourceSize.height: 250
            fillMode: Image.PreserveAspectFit
            source: "file:///" + dirPath + "//config//prConfig//lut_load_loaction_resultImage.jpg"
        }
    }

    RowLayout {
        RoundButton{
            id: lpa_lut_ng_location_lock
            transformOrigin: Item.Center
            display: Button.TextBesideIcon
            icon.width: 30
            icon.height: 30
            icon.source: "../../../icons/unlock.png"
            icon.color: "red"
            onClicked: {
                lpa_lut_ng_location_slider.enabled = !lpa_lut_ng_location_slider.enabled
                if (lpa_lut_ng_location_slider.enabled) {
                    lpa_lut_ng_location_lock.icon.color = "lightGreen"
                } else {
                    lpa_lut_ng_location_lock.icon.color = "red"
                }
                lpa_lut_ng_location_slider.value = tcp_vision_location_lpa_lut_ng.lightBrightness
            }
        }
        Slider {
            id: lpa_lut_ng_location_slider
            enabled: false
            width: 25
            from: 0
            value: tcp_vision_location_lpa_lut_ng.lightBrightness
            to: 255
            stepSize: 1
            onValueChanged: {
                tcp_vision_location_lpa_lut_ng.setLightBrightness(value)
                lightingController.setPickarmLighting(value)
            }
        }

        Label {
            text: lpa_lut_ng_location_slider.value
        }
        Button{
            text:qsTr("LUT NG Lens PR Draw")
            onClicked: {
                 highSprrow.callAVSPRSettingProcess("GenericNCCavproj.avexe", "lpa_lut_ng_location");
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
                lpa_lut_ng_location_image.source = ""
                lpa_lut_ng_location_image.source = "file:///" + dirPath + "//config//prConfig//lpa_lut_ng_location_resultImage.jpg"
            }
        }
        TextField{
            color: "#57f529"
            text: tcp_vision_location_lpa_lut_ng.locationName
            font.pixelSize: 14
            width: 200
            Layout.preferredWidth: 200
        }
    }

    RowLayout {
        Button{
            text:qsTr("执行PR")
            onClicked:{
                 logicManager.performHandling("", LogicManager.HANDLING_ORIGIN_LOCATION, "lpa_lut_ng_location")
            }
        }
        Button{
            text:qsTr("执行PR偏移")
            onClicked:{
                logicManager.performHandling("", LogicManager.HANDLING_OFFSET_LOCATION, "lpa_lut_ng_location")
            }
        }
        Image{
            id: lpa_lut_ng_location_image
            cache: false
            sourceSize.width: 250
            sourceSize.height: 250
            fillMode: Image.PreserveAspectFit
            source: "file:///" + dirPath + "//config//prConfig//lpa_lut_ng_location_resultImage.jpg"
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
                lut_uplook_location_slider.enabled = !lut_uplook_location_slider.enabled
                if (lut_uplook_location_slider.enabled) {
                    lut_uplook_location_lock.icon.color = "lightGreen"
                } else {
                    lut_uplook_location_lock.icon.color = "red"
                }
                lut_uplook_location_slider.value = tcp_vision_location_lut_uplook_picker.lightBrightness
            }
        }
        Slider {
            id: lut_uplook_location_slider
            enabled: false
            width: 25
            from: 0
            value: tcp_vision_location_lut_uplook_picker.lightBrightness
            to: 255
            stepSize: 1
            onValueChanged: {
                tcp_vision_location_lut_uplook_picker.setLightBrightness(value)
                lightingController.setUplookLighting(value)
            }
        }
        Label {
            text: lut_uplook_location_slider.value
        }
        Button{
            text:qsTr("LUT Up Look PR Draw")
            onClicked: {
                 highSprrow.callAVSPRSettingProcess("GenericNCCavproj.avexe", "lut_uplook_picker_location");
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
                lut_uplook_picker_location_image.source = ""
                lut_uplook_picker_location_image.source = "file:///" + dirPath + "//config//prConfig//lut_uplook_picker_location_resultImage.jpg"
            }
        }
        TextField{
            color: "#57f529"
            text: tcp_vision_location_lut_uplook_picker.locationName
            font.pixelSize: 14
            width: 200
            Layout.preferredWidth: 200
        }
    }
    RowLayout {
        Button{
            text:qsTr("执行PR")
            onClicked:{
                 logicManager.performHandling("", LogicManager.HANDLING_ORIGIN_LOCATION, "lut_uplook_picker_location")
            }
        }
        Button{
            text:qsTr("执行PR偏移")
            onClicked:{
                logicManager.performHandling("", LogicManager.HANDLING_OFFSET_LOCATION, "lut_uplook_picker_location")
            }
        }
        Image{
            id: lut_uplook_picker_location_image
            cache: false
            sourceSize.width: 250
            sourceSize.height: 250
            fillMode: Image.PreserveAspectFit
            source: "file:///" + dirPath + "//config//prConfig//lut_uplook_picker_location_resultImage.jpg"
        }
    }

    RowLayout { Label { text: qsTr("测试") } }
    RowLayout {
        Button {
            text: qsTr("LUT移动到Load位置")
            onClicked: {
                logicManager.performHandling(tcpLUTParams.moduleName, LutModule.LOAD_POS)
            }
        }
        Button {
            text: qsTr("LPA移动到LUT1 Pocket PR Position")
            onClicked: {
                logicManager.performHandling(tcp_lens_loader_parameter.moduleName,LensLoaderModule.LUT_POS1)
            }
        }
        Button {
            text: qsTr("LPA移动到LUT2 Pocket PR Position")
            onClicked: {
                logicManager.performHandling(tcp_lens_loader_parameter.moduleName,LensLoaderModule.LUT_POS2)
            }
        }
        Button {
            text: qsTr("LPA移动到Up Look PR Position")
            onClicked: {
                 logicManager.performHandling(tcp_lens_loader_parameter.moduleName,LensLoaderModule.UPDOWNLOOK_DOWN_POS)
            }
        }
    }
}
