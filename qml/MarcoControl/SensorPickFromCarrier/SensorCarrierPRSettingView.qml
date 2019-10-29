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
                spa_sensor_location_dial.enabled = !spa_sensor_location_dial.enabled
                if (spa_sensor_location_dial.enabled) {
                    spa_sensor_location_lock.icon.color = "lightGreen"
                } else {
                    spa_sensor_location_lock.icon.color = "red"
                }
            }
        }

        Dial{
            id: spa_sensor_location_dial
            enabled: false
            width: 25
            from: 0
            value: spa_sensor_location.lightBrightness
            to: 255
            stepSize: 1
            Label {
                text: spa_sensor_location.lightBrightness
                color: "white"
                font.pixelSize: Qt.application.font.pixelSize * 3
                anchors.centerIn: parent
            }
            onValueChanged: {
                spa_sensor_location.setLightBrightness(value)
                baseModuleManager.setLightingBrightness(spa_sensor_location.locationName)
            }
        }
        Button{
            text:qsTr("SPA Sensor PR Draw")
            onClicked: {

            }
        }
        TextField{
            color: "#57f529"
            text: spa_sensor_location.prFileName
            font.pixelSize: 14
            width: 600
            Layout.preferredWidth: 600
        }
    }
    RowLayout {
        FileDialog{
            id:spa_sensor_location_file_dialog
            title:"选择加载PR文件"
            selectExisting: true
            selectFolder: false
            selectMultiple: false

            nameFilters: ["avdata文件 (*.avdata)"]
            onAccepted:{
                spa_sensor_location.setPrFileName(fileUrl)
            }
        }
        Button{
            text:qsTr("读取PR文件")
            onClicked: {
                spa_sensor_location_file_dialog.open()
            }
        }
        Button{
            text:qsTr("执行PR")
            onClicked:{
                logicManagerState.setCurrentLocationName(spa_sensor_location.locationName)
                logicManagerState.setUseOriginPr(true)
                logicManager.performHandling(LogicManager.PERFORM_LOCATION)
            }
        }
        Button{
            text:qsTr("执行PR偏移")
            onClicked:{
                logicManagerState.setCurrentLocationName(spa_sensor_location.locationName)
                logicManagerState.setUseOriginPr(false)
                logicManager.performHandling(LogicManager.PERFORM_LOCATION)
            }
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
                spa_vacancy_location_dial.enabled = !spa_vacancy_location_dial.enabled
                if (spa_vacancy_location_dial.enabled) {
                    spa_vacancy_location_lock.icon.color = "lightGreen"
                } else {
                    spa_vacancy_location_lock.icon.color = "red"
                }
            }
        }

        Dial{
            id: spa_vacancy_location_dial
            enabled: false
            width: 25
            from: 0
            value: spa_vacancy_location.lightBrightness
            to: 255
            stepSize: 1
            Label {
                text: spa_vacancy_location.lightBrightness
                color: "white"
                font.pixelSize: Qt.application.font.pixelSize * 3
                anchors.centerIn: parent
            }
            onValueChanged: {
                spa_vacancy_location.setLightBrightness(value)
                baseModuleManager.setLightingBrightness(spa_vacancy_location.locationName)
            }
        }
        Button{
            text:qsTr("SPA Sensor PR Draw")
            onClicked: {

            }
        }
        TextField{
            color: "#57f529"
            text: spa_vacancy_location.prFileName
            font.pixelSize: 14
            width: 600
            Layout.preferredWidth: 600
        }
    }
    RowLayout {
        FileDialog{
            id:spa_vacancy_location_file_dialog
            title:"选择加载PR文件"
            selectExisting: true
            selectFolder: false
            selectMultiple: false

            nameFilters: ["avdata文件 (*.avdata)"]
            onAccepted:{
                spa_vacancy_location.setPrFileName(fileUrl)
            }
        }
        Button{
            text:qsTr("读取PR文件")
            onClicked: {
                spa_vacancy_location_file_dialog.open()
            }
        }
        Button{
            text:qsTr("执行PR")
            onClicked:{
                logicManagerState.setCurrentLocationName(spa_vacancy_location.locationName)
                logicManagerState.setUseOriginPr(true)
                logicManager.performHandling(LogicManager.PERFORM_LOCATION)
            }
        }
        Button{
            text:qsTr("执行PR偏移")
            onClicked:{
                logicManagerState.setCurrentLocationName(spa_vacancy_location.locationName)
                logicManagerState.setUseOriginPr(false)
                logicManager.performHandling(LogicManager.PERFORM_LOCATION)
            }
        }
    }

   SensorCarrierInfoAndPositionTestOperation {}
}
