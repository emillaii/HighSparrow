import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11
import QtQuick.Dialogs 1.2
import LogicManagerLib 1.1
import SomeLib 1.1

ColumnLayout{
    RowLayout {
        Label { text: qsTr("SPA Standby Position") }
        Label {
            text: qsTr("X")
        }
        TextField {
            text: spa_standby_position.X
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                spa_standby_position.setX(text)
            }
        }
        Label {
            text: qsTr("Y")
        }
        TextField {
            text: spa_standby_position.Y
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                spa_standby_position.setY(text)
            }
        }
        Button {
            text: title_read_encoder
            onClicked: {
                var x = baseModuleManager.getMotorFeedbackPos(sensorPickArmParams.motorXName)
                var y = baseModuleManager.getMotorFeedbackPos(sensorPickArmParams.motorYName)
                spa_standby_position.setX(x);
                spa_standby_position.setY(y);
            }
        }
        Button {
            text: title_move_to
            onClicked: {
                logicManager.performHandling(sensorLoaderParameter.moduleName,SensorLoaderModule.SPA_STANDBY_POS)
            }
        }
    }
    RowLayout {
        RoundButton{
            id: spa_sensor_location_lock
            transformOrigin: Item.Center
            display: Button.TextBesideIcon
            icon.width: 30
            icon.height: 30
            icon.source: "../../../icons/unlock.png"
            icon.color: "red"
            onClicked: {
                spa_sensor_location_slider.enabled = !spa_sensor_location_slider.enabled
                if (spa_sensor_location_slider.enabled) {
                    spa_sensor_location_lock.icon.color = "lightGreen"
                } else {
                    spa_sensor_location_lock.icon.color = "red"
                }
            }
        }

        Slider {
            id: spa_sensor_location_slider
            enabled: false
            width: 25
            from: 0
            value: spa_sensor_location.lightBrightness
            to: 255
            stepSize: 1
            onValueChanged: {
                spa_sensor_location.setLightBrightness(value)
                lightingController.setSensorPickarmLighting(value)
            }
        }
        Label {
            text: spa_sensor_location_slider.value
        }
        Button{
            text:qsTr("SPA Sensor PR Draw")
            onClicked: {
                 highSprrow.callAVSPRSettingProcess("GenericNCCavproj.avexe", "spa_sensor_location");
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
                spa_sensor_location_image.source = ""
                spa_sensor_location_image.source = "file:///" + dirPath + "//config//prConfig//spa_sensor_location_resultImage.jpg"
            }
        }
        TextField{
            color: "#57f529"
            text: spa_sensor_location.locationName
            font.pixelSize: 14
            width: 200
            Layout.preferredWidth: 200
        }
    }
    RowLayout {
        Button{
            text:qsTr("执行PR")
            onClicked:{
                logicManager.performHandling("", LogicManager.HANDLING_ORIGIN_LOCATION, "spa_sensor_location")
            }
        }
        Button{
            text:qsTr("执行PR偏移")
            onClicked:{
                logicManager.performHandling("", LogicManager.HANDLING_OFFSET_LOCATION, "spa_sensor_location")
            }
        }
        Image{
            id: spa_sensor_location_image
            cache: false
            sourceSize.width: 250
            sourceSize.height: 250
            fillMode: Image.PreserveAspectFit
            source: "file:///" + dirPath + "//config//prConfig//spa_sensor_location_resultImage.jpg"
        }
    }

    RowLayout {
        RoundButton{
            id: spa_vacancy_location_lock
            transformOrigin: Item.Center
            display: Button.TextBesideIcon
            icon.width: 30
            icon.height: 30
            icon.source: "../../../icons/unlock.png"
            icon.color: "red"
            onClicked: {
                spa_vacancy_location_image.source = ""
                spa_vacancy_location_image.source = "file:///" + dirPath + "//config//prConfig//spa_vacancy_location_resultImage.jpg"
                spa_vacancy_location_slider.enabled = !spa_vacancy_location_slider.enabled
                if (spa_vacancy_location_slider.enabled) {
                    spa_vacancy_location_lock.icon.color = "lightGreen"
                } else {
                    spa_vacancy_location_lock.icon.color = "red"
                }
            }
        }

        Slider {
            id: spa_vacancy_location_slider
            enabled: false
            width: 25
            from: 0
            value: spa_vacancy_location.lightBrightness
            to: 255
            stepSize: 1
            onValueChanged: {
                spa_vacancy_location.setLightBrightness(value)
                lightingController.setSensorPickarmLighting(value)
            }
        }
        Label {
            text: spa_vacancy_location_slider.value
        }
        Button{
            text:qsTr("SPA Vacancy PR Draw")
            onClicked: {
                 highSprrow.callAVSPRSettingProcess("GenericNCCavproj.avexe", "spa_vacancy_location");
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
                spa_vacancy_location_image.source = ""
                spa_vacancy_location_image.source = "file:///" + dirPath + "//config//prConfig//spa_vacancy_location_resultImage.jpg"
            }
        }

        TextField{
            color: "#57f529"
            text: spa_vacancy_location.locationName
            font.pixelSize: 14
            width: 200
            Layout.preferredWidth: 200
        }
    }
    RowLayout {
        Button{
            text:qsTr("执行PR")
            onClicked:{
                logicManager.performHandling("", LogicManager.HANDLING_ORIGIN_LOCATION, "spa_vacancy_location")
            }
        }
        Button{
            text:qsTr("执行PR偏移")
            onClicked:{
                logicManager.performHandling("", LogicManager.HANDLING_OFFSET_LOCATION, "spa_vacancy_location")
            }
        }
        Image{
            id: spa_vacancy_location_image
            cache: false
            sourceSize.width: 250
            sourceSize.height: 250
            fillMode: Image.PreserveAspectFit
            source: "file:///" + dirPath + "//config//prConfig//spa_vacancy_location_resultImage.jpg"
        }
    }

   SensorCarrierInfoAndPositionTestOperation {}
}
