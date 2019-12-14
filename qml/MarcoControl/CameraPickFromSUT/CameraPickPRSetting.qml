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
            id: sut_empty_location_lock
            transformOrigin: Item.Center
            display: Button.TextBesideIcon
            icon.width: 30
            icon.height: 30
            icon.source: "../../../icons/unlock.png"
            icon.color: "red"
            onClicked: {
                sut_empty_location_lock_slider.enabled = !sut_empty_location_lock_slider.enabled
                if (sut_empty_location_lock_slider.enabled) {
                    sut_empty_location_lock.icon.color = "lightGreen"
                } else {
                    sut_empty_location_lock.icon.color = "red"
                }
                sut_empty_location_lock_slider.value = sut_location.lightBrightness
            }
        }
        Slider {
            id: sut_empty_location_lock_slider
            enabled: false
            width: 25
            from: 0
            value: sut_location.lightBrightness
            to: 255
            stepSize: 1
            onValueChanged: {
                sut_location.setLightBrightness(value)
                lightingController.setSensorPickarmLighting(value)
            }
        }
        Label {
            text: sut_empty_location_lock_slider.value
        }
        Button{
            text:qsTr("SUT Empty PR Draw")
            onClicked: {
                highSprrow.callAVSPRSettingProcess("GenericNCCavproj.avexe", "sut_location");
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
                sut_location_image.source = ""
                sut_location_image.source = "file:///" + dirPath + "//config//prConfig//sut_location_resultImage.jpg"
            }
        }
        TextField{
            color: "#57f529"
            text: sut_location.locationName
            font.pixelSize: 14
            width: 600
            Layout.preferredWidth: 600
        }
    }
    RowLayout {
        Button{
            text:qsTr("执行PR")
            onClicked:{
                logicManager.performHandling("", LogicManager.HANDLING_ORIGIN_LOCATION,"sut_location")
            }
        }
        Button{
            text:qsTr("执行PR偏移")
            onClicked:{
                logicManager.performHandling("", LogicManager.HANDLING_OFFSET_LOCATION,"sut_location")
            }
        }
        Image{
            id: sut_location_image
            cache: false
            sourceSize.width: 250
            sourceSize.height: 250
            fillMode: Image.PreserveAspectFit
            source: "file:///" + dirPath + "//config//prConfig//sut_location_resultImage.jpg"
        }
    }


    RowLayout {
        RoundButton{
            id: sut_camera_location_lock
            transformOrigin: Item.Center
            display: Button.TextBesideIcon
            icon.width: 30
            icon.height: 30
            icon.source: "../../../icons/unlock.png"
            icon.color: "red"
            onClicked: {
                sut_camera_location_slider.enabled = !sut_camera_location_slider.enabled
                if (sut_camera_location_slider.enabled) {
                    sut_camera_location_lock.icon.color = "lightGreen"
                } else {
                    sut_camera_location_lock.icon.color = "red"
                }
            }
        }
        Slider {
            id: sut_camera_location_slider
            enabled: false
            width: 25
            from: 0
            value: sut_camera_location.lightBrightness
            to: 255
            stepSize: 1
            onValueChanged: {
                sut_camera_location.setLightBrightness(value)
                lightingController.setSensorPickarmLighting(value)
            }
        }
        Label {
            text: sut_camera_location_slider.value
        }
        Button{
            text:qsTr("SUT Product PR Draw")
            onClicked: {
                highSprrow.callAVSPRSettingProcess("GenericNCCavproj.avexe", "sut_camera_location");
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
                sut_camera_location_image.source = ""
                sut_camera_location_image.source = "file:///" + dirPath + "//config//prConfig//sut_camera_location_resultImage.jpg"
            }
        }
        TextField{
            color: "#57f529"
            text: sut_camera_location.locationName
            font.pixelSize: 14
            width: 600
            Layout.preferredWidth: 600
        }
    }
    RowLayout {
        Button{
            text:qsTr("执行PR")
            onClicked:{
                logicManager.performHandling("", LogicManager.HANDLING_ORIGIN_LOCATION, "sut_camera_location")
            }
        }
        Button{
            text:qsTr("执行PR偏移")
            onClicked:{
                logicManager.performHandling("", LogicManager.HANDLING_OFFSET_LOCATION, "sut_camera_location")
            }
        }
        Image{
            id: sut_camera_location_image
            cache: false
            sourceSize.width: 250
            sourceSize.height: 250
            fillMode: Image.PreserveAspectFit
            source: "file:///" + dirPath + "//config//prConfig//sut_camera_location_resultImage.jpg"
        }
    }

    RowLayout {
        RoundButton{
            id: sut_ng_sensor_location_lock
            transformOrigin: Item.Center
            display: Button.TextBesideIcon
            icon.width: 30
            icon.height: 30
            icon.source: "../../../icons/unlock.png"
            icon.color: "red"
            onClicked: {
                sut_ng_sensor_location_slider.enabled = !sut_ng_sensor_location_slider.enabled
                if (sut_ng_sensor_location_slider.enabled) {
                    sut_ng_sensor_location_lock.icon.color = "lightGreen"
                } else {
                    sut_ng_sensor_location_lock.icon.color = "red"
                }
            }
        }
        Slider {
            id: sut_ng_sensor_location_slider
            enabled: false
            width: 25
            from: 0
            value: sut_ng_sensor_location.lightBrightness
            to: 255
            stepSize: 1
            onValueChanged: {
                sut_ng_sensor_location.setLightBrightness(value)
                lightingController.setSensorPickarmLighting(value)
            }
        }
        Label {
            text: sut_ng_sensor_location_slider.value
        }
        Button{
            text:qsTr("SUT Product PR Draw")
            onClicked: {
                highSprrow.callAVSPRSettingProcess("GenericNCCavproj.avexe", "sut_ng_sensor_location");
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
                sut_ng_sensor_location_image.source = ""
                sut_ng_sensor_location_image.source = "file:///" + dirPath + "//config//prConfig//sut_ng_sensor_location_resultImage.jpg"
            }
        }
        TextField{
            color: "#57f529"
            text: sut_ng_sensor_location.locationName
            font.pixelSize: 14
            width: 600
            Layout.preferredWidth: 600
        }
    }
    RowLayout {
        Button{
            text:qsTr("执行PR")
            onClicked:{
                logicManager.performHandling("", LogicManager.HANDLING_ORIGIN_LOCATION, "sut_ng_sensor_location")
            }
        }
        Button{
            text:qsTr("执行PR偏移")
            onClicked:{
                logicManager.performHandling("", LogicManager.HANDLING_OFFSET_LOCATION, "sut_ng_sensor_location")
            }
        }
        Image{
            id: sut_ng_sensor_location_image
            cache: false
            sourceSize.width: 250
            sourceSize.height: 250
            fillMode: Image.PreserveAspectFit
            source: "file:///" + dirPath + "//config//prConfig//sut_ng_sensor_location_resultImage.jpg"
        }
    }

    CameraPickFromSUTTestOperation {}
}
