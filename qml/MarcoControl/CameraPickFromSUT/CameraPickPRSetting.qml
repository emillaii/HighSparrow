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
                sut_1_downlook_location_dial.enabled = !sut_1_downlook_location_dial.enabled
                if (sut_1_downlook_location_dial.enabled) {
                    sut_1_downlook_location_lock.icon.color = "lightGreen"
                } else {
                    sut_1_downlook_location_lock.icon.color = "red"
                }
            }
        }
        Dial{
            id: sut_1_downlook_location_dial
            enabled: false
            width: 25
            from: 0
            value: sut_1_downlook_location.lightBrightness
            to: 255
            stepSize: 1
            Label {
                text: sut_1_downlook_location.lightBrightness
                color: "white"
                font.pixelSize: Qt.application.font.pixelSize * 3
                anchors.centerIn: parent
            }
            onValueChanged: {
            }
        }
        Button{
            text:qsTr("SUT 1 Camera PR Draw")
            onClicked: {
            }
        }
        TextField{
            color: "#57f529"
            text: sut_1_downlook_location.prFileName
            font.pixelSize: 14
            width: 600
            Layout.preferredWidth: 600
        }
    }
    RowLayout {
        FileDialog{
            id:sut_1_downlook_location_file_dialog
            title:"选择加载PR文件"
            selectExisting: true
            selectFolder: false
            selectMultiple: false

            nameFilters: ["avdata文件 (*.avdata)"]
            onAccepted:{
                sut_1_downlook_location.setPrFileName(fileUrl)
            }
        }
        Button{
            text:qsTr("读取PR文件")
            onClicked: {
                sut_1_downlook_location_file_dialog.open()
            }
        }
        Button{
            text:qsTr("执行PR")
            onClicked:{
                logicManagerState.setCurrentLocationName(sut_1_downlook_location.locationName)
                logicManagerState.setUseOriginPr(true)
                logicManager.performHandling(LogicManager.PERFORM_LOCATION)
            }
        }
        Button{
            text:qsTr("执行PR偏移")
            onClicked:{
                logicManagerState.setCurrentLocationName(sut_1_downlook_location.locationName)
                logicManagerState.setUseOriginPr(false)
                logicManager.performHandling(LogicManager.PERFORM_LOCATION)
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
                sut_2_downlook_location_dial.enabled = !sut_2_downlook_location_dial.enabled
                if (sut_2_downlook_location_dial.enabled) {
                    sut_2_downlook_location_lock.icon.color = "lightGreen"
                } else {
                    sut_2_downlook_location_lock.icon.color = "red"
                }
            }
        }
        Dial{
            id: sut_2_downlook_location_dial
            enabled: false
            width: 25
            from: 0
            value: sut_2_downlook_location.lightBrightness
            to: 255
            stepSize: 1
            Label {
                text: sut_2_downlook_location.lightBrightness
                color: "white"
                font.pixelSize: Qt.application.font.pixelSize * 3
                anchors.centerIn: parent
            }
            onValueChanged: {
            }
        }
        Button{
            text:qsTr("SUT 2 Camera PR Draw")
            onClicked: {
            }
        }
        TextField{
            color: "#57f529"
            text: sut_2_downlook_location.prFileName
            font.pixelSize: 14
            width: 600
            Layout.preferredWidth: 600
        }
    }
    RowLayout {
        FileDialog{
            id:sut_2_downlook_location_file_dialog
            title:"选择加载PR文件"
            selectExisting: true
            selectFolder: false
            selectMultiple: false

            nameFilters: ["avdata文件 (*.avdata)"]
            onAccepted:{
                sut_2_downlook_location.setPrFileName(fileUrl)
            }
        }
        Button{
            text:qsTr("读取PR文件")
            onClicked: {
                sut_2_downlook_location_file_dialog.open()
            }
        }
        Button{
            text:qsTr("执行PR")
            onClicked:{
                logicManagerState.setCurrentLocationName(sut_2_downlook_location.locationName)
                logicManagerState.setUseOriginPr(true)
                logicManager.performHandling(LogicManager.PERFORM_LOCATION)
            }
        }
        Button{
            text:qsTr("执行PR偏移")
            onClicked:{
                logicManagerState.setCurrentLocationName(sut_2_downlook_location.locationName)
                logicManagerState.setUseOriginPr(false)
                logicManager.performHandling(LogicManager.PERFORM_LOCATION)
            }
        }
    }

    CameraPickFromSUTTestOperation {}
}
