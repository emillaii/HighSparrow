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
            }
        }
        TextField{
            color: "#57f529"
            text: tcp_vision_location_lut_load.prFileName
            font.pixelSize: 14
            width: 600
            Layout.preferredWidth: 600
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
            }
        }
        TextField{
            color: "#57f529"
            text: tcp_vision_location_lut_uplook_picker.prFileName
            font.pixelSize: 14
            width: 600
            Layout.preferredWidth: 600
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
