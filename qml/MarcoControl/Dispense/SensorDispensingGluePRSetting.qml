import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11
import QtQuick.Dialogs 1.2
import LogicManagerLib 1.1
import SomeLib 1.1
import SutModuleLib 1.1

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
                aa1_downLook_location_slider.enabled = !aa1_downLook_location_slider.enabled
                if (aa1_downLook_location_slider.enabled) {
                    aa1_downLook_location_lock.icon.color = "lightGreen"
                } else {
                    aa1_downLook_location_lock.icon.color = "red"
                }
            }
        }

        Slider {
            id: aa1_downLook_location_slider
            enabled: false
            width: 25
            from: 0
            value: tcp_vision_location_aa1_downlook.lightBrightness
            to: 255
            stepSize: 1
            onValueChanged: {
                tcp_vision_location_aa1_downlook.setLightBrightness(value)
                lightingController.setSensorPickarmLighting(value)
            }
        }
        Label {
            text: aa1_downLook_location_slider.value
        }
        Button{
            text:qsTr("SPA Sensor PR Draw")
            onClicked: {
                 highSprrow.callAVSPRSettingProcess("GenericNCCavproj.avexe", "aa1_downLook_location");
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
                aa1_downLook_location_image.source = ""
                aa1_downLook_location_image.source = "file:///" + dirPath + "//config//prConfig//aa1_downLook_location_resultImage.jpg"
            }
        }
        TextField{
            color: "#57f529"
            text: tcp_vision_location_aa1_downlook.locationName
            font.pixelSize: 14
            width: 200
            Layout.preferredWidth: 200
        }
    }
    RowLayout {
        Button{
            text:qsTr("执行PR")
            onClicked:{
                logicManager.performHandling("", LogicManager.HANDLING_ORIGIN_LOCATION, "aa1_downLook_location")
            }
        }
        Button{
            text:qsTr("执行PR偏移")
            onClicked:{
                logicManager.performHandling("", LogicManager.HANDLING_OFFSET_LOCATION, "aa1_downLook_location")
            }
        }
        Image{
            id: aa1_downLook_location_image
            cache: false
            sourceSize.width: 250
            sourceSize.height: 250
            fillMode: Image.PreserveAspectFit
            source: "file:///" + dirPath + "//config//prConfig//aa1_downLook_location_resultImage.jpg"
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
                aa2_downLook_location_slider.enabled = !aa2_downLook_location_slider.enabled
                if (aa2_downLook_location_slider.enabled) {
                    aa2_downLook_location_lock.icon.color = "lightGreen"
                } else {
                    aa2_downLook_location_lock.icon.color = "red"
                }
            }
        }

        Slider {
            id: aa2_downLook_location_slider
            enabled: false
            width: 25
            from: 0
            value: aa2_downLook_location.lightBrightness
            to: 255
            stepSize: 1
            onValueChanged: {
                aa2_downLook_location.setLightBrightness(value)
                lightingController.setSensorPickarmLighting(value)
            }
        }
        Label {
            text: aa2_downLook_location_slider.value
        }
        Button{
            text:qsTr("SPA Sensor PR Draw")
            onClicked: {
                 highSprrow.callAVSPRSettingProcess("GenericNCCavproj.avexe", "aa2_downLook_location");
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
                aa2_downLook_location_image.source = ""
                aa2_downLook_location_image.source = "file:///" + dirPath + "//config//prConfig//aa2_downLook_location_resultImage.jpg"
            }
        }
        TextField{
            color: "#57f529"
            text: aa2_downLook_location.locationName
            font.pixelSize: 14
            width: 200
            Layout.preferredWidth: 200
        }
    }
    RowLayout {
        Button{
            text:qsTr("执行PR")
            onClicked:{
                logicManager.performHandling("", LogicManager.HANDLING_ORIGIN_LOCATION, "aa2_downLook_location")
            }
        }
        Button{
            text:qsTr("执行PR偏移")
            onClicked:{
                logicManager.performHandling("", LogicManager.HANDLING_OFFSET_LOCATION, "aa2_downLook_location")
            }
        }
        Image{
            id: aa2_downLook_location_image
            cache: false
            sourceSize.width: 250
            sourceSize.height: 250
            fillMode: Image.PreserveAspectFit
            source: "file:///" + dirPath + "//config//prConfig//aa2_downLook_location_resultImage.jpg"
        }
    }

    RowLayout {
        Button {
            text: qsTr("SUT 1 PR Position");
            onClicked: {
                logicManager.performHandling(tcpSUTParams.moduleName,SutModule.DOWNLOOK_PR_POS)
            }
        }

        Button {
            text: qsTr("SUT 2 PR Position");
            onClicked: {
                logicManager.performHandling(sutParams.moduleName,SutModule.DOWNLOOK_PR_POS)
            }
        }
    }
}
