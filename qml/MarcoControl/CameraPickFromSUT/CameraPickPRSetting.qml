import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11
import QtQuick.Dialogs 1.2
import LogicManagerLib 1.1
import SomeLib 1.1

Column {
    RowLayout { Label { text: qsTr("CPA Pick And Place PR Position") } }
    RowLayout {
        Label {
            text: qsTr("CPA Pick And Place SUT 1 PR Position")
        }
        Label{
           text:qsTr("X")
        }
        TextField{
           text: sut_pr_position1.X
           horizontalAlignment: TextInput.AlignHCenter
           validator: DoubleValidator{
               decimals: 6
               notation: DoubleValidator.StandardNotation
           }
           onEditingFinished: {
               sut_pr_position1.setX(text)
           }
        }
        Label{
           text:qsTr("Y")
        }
        TextField{
           text: sut_pr_position1.Y
           horizontalAlignment: TextInput.AlignHCenter
           validator: DoubleValidator{
               decimals: 6
               notation: DoubleValidator.StandardNotation
           }
           onEditingFinished: {
               sut_pr_position1.setY(text)
           }
        }

        Button{
           text:title_read_encoder
           onClicked: {
               var x = baseModuleManager.getMotorFeedbackPos(sensorPickArmParams.motorXName)
               var y = baseModuleManager.getMotorFeedbackPos(sensorPickArmParams.motorYName)
               sut_pr_position1.setX(x);
               sut_pr_position1.setY(y);
           }
        }
        Button{
           text:title_move_to
           onClicked: {
               logicManager.performHandling(sensorLoaderParameter.moduleName,SensorLoaderModule.SUT_POS1)
           }
        }
    }
    RowLayout {
        Label {
            text: qsTr("CPA Pick And Place SUT 2 PR Position")
        }
        Label{
           text:qsTr("X")
        }
        TextField{
           text: sut_pr_position2.X
           horizontalAlignment: TextInput.AlignHCenter
           validator: DoubleValidator{
               decimals: 6
               notation: DoubleValidator.StandardNotation
           }
           onEditingFinished: {
               sut_pr_position2.setX(text)
           }
        }
        Label{
           text:qsTr("Y")
        }
        TextField{
           text: sut_pr_position2.Y
           horizontalAlignment: TextInput.AlignHCenter
           validator: DoubleValidator{
               decimals: 6
               notation: DoubleValidator.StandardNotation
           }
           onEditingFinished: {
               sut_pr_position2.setY(text)
           }
        }

        Button{
           text:title_read_encoder
           onClicked: {
               var x = baseModuleManager.getMotorFeedbackPos(sensorPickArmParams.motorXName)
               var y = baseModuleManager.getMotorFeedbackPos(sensorPickArmParams.motorYName)
               sut_pr_position2.setX(x);
               sut_pr_position2.setY(y);
           }
        }
        Button{
           text:title_move_to
           onClicked: {
               logicManager.performHandling(sensorLoaderParameter.moduleName,SensorLoaderModule.SUT_POS2)
           }
        }
    }

    RowLayout {
        RoundButton{
            id: sut_1_downlook_location_lock
            transformOrigin: Item.Center
            display: Button.TextBesideIcon
            icon.width: 30
            icon.height: 30
            icon.source: "../../../icons/unlock.png"
            icon.color: "red"
            onClicked: {
                sut_1_downlook_location_slider.enabled = !sut_1_downlook_location_slider.enabled
                if (sut_1_downlook_location_slider.enabled) {
                    sut_1_downlook_location_lock.icon.color = "lightGreen"
                } else {
                    sut_1_downlook_location_lock.icon.color = "red"
                }
                sut_1_downlook_location_slider.value = tcp_vision_location_aa1_downlook.lightBrightness
            }
        }
        Slider {
            id: sut_1_downlook_location_slider
            enabled: false
            width: 25
            from: 0
            value: tcp_vision_location_aa1_downlook.lightBrightness
            to: 255
            stepSize: 1
            onValueChanged: {
                tcp_vision_location_aa1_downlook.setLightBrightness(value)
                lightingController.setDownlookLighting(value)
            }
        }
        Label {
            text: sut_1_downlook_location_slider.value
        }
        Button{
            text:qsTr("SUT 1 Camera PR Draw")
            onClicked: {
            }
        }
        TextField{
            color: "#57f529"
            text: tcp_vision_location_aa1_downlook.prFileName
            font.pixelSize: 14
            width: 600
            Layout.preferredWidth: 600
        }
    }
    RowLayout {
        Button{
            text:qsTr("执行Calibration")
            onClicked:{
                logicManager.performHandling("", LogicManager.HANDLING_CALIBRATION, "aa1_downLook_location")
            }
        }
        Button{
            text:qsTr("执行PR")
            onClicked:{
                logicManager.performHandling("", LogicManager.HANDLING_ORIGIN_LOCATION,"aa1_downLook_location")
            }
        }
        Button{
            text:qsTr("执行PR偏移")
            onClicked:{
                logicManager.performHandling("", LogicManager.HANDLING_OFFSET_LOCATION,"aa1_downLook_location")
            }
        }
    }


    RowLayout {
        RoundButton{
            id: sut_2_downlook_location_lock
            transformOrigin: Item.Center
            display: Button.TextBesideIcon
            icon.width: 30
            icon.height: 30
            icon.source: "../../../icons/unlock.png"
            icon.color: "red"
            onClicked: {
                sut_2_downlook_location_slider.enabled = !sut_2_downlook_location_slider.enabled
                if (sut_2_downlook_location_slider.enabled) {
                    sut_2_downlook_location_lock.icon.color = "lightGreen"
                } else {
                    sut_2_downlook_location_lock.icon.color = "red"
                }
            }
        }
        Slider {
            id: sut_2_downlook_location_slider
            enabled: false
            width: 25
            from: 0
            value: aa2_downLook_location.lightBrightness
            to: 255
            stepSize: 1
            onValueChanged: {
                aa2_downLook_location.setLightBrightness(value)
                lightingController.setAA2DownlookLighting(value)
            }
        }
        Label {
            text: sut_2_downlook_location_slider.value
        }
        Button{
            text:qsTr("SUT 2 Camera PR Draw")
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
        Button{
            text:qsTr("执行Calibration")
            onClicked:{
                logicManager.performHandling("", LogicManager.HANDLING_CALIBRATION, "aa2_downLook_location")
            }
        }
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
    }

    CameraPickFromSUTTestOperation {}
}
