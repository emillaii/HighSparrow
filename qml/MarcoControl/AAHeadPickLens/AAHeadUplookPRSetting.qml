import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11
import QtQuick.Dialogs 1.2
import LogicManagerLib 1.1
import SomeLib 1.1
import LutModuleLib 1.1
ColumnLayout{
    RowLayout {
        Label { text: qsTr("AA Up Look PR Position") }
    }
    RowLayout {
        Label { text: qsTr("AA 1 PR Position") }
        Label {
            text: qsTr("X")
        }
        TextField {
            text: tcpLUTUplookPositionAA1.X
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                tcpLUTUplookPositionAA1.setX(text)
            }
        }
        Label {
            text: qsTr("Y")
        }
        TextField {
            text: tcpLUTUplookPositionAA1.Y
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                tcpLUTUplookPositionAA1.setY(text)
            }
        }
        Label {
            text: qsTr("Z")
        }
        TextField {
            text: tcpLUTUplookPositionAA1.Z
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                tcpLUTUplookPositionAA1.setZ(text)
            }
        }
        Button {
            text: title_read_encoder
            onClicked: {
                var x = baseModuleManager.getMotorFeedbackPos(tcpLUTParams.motorXName)
                var y = baseModuleManager.getMotorFeedbackPos(tcpLUTParams.motorYName)
                var z = baseModuleManager.getMotorFeedbackPos(tcpLUTParams.motorZName)
                tcpLUTPickLensPositionAA1.setX(x)
                tcpLUTPickLensPositionAA1.setY(y)
                tcpLUTPickLensPositionAA1.setZ(z)
            }
        }
        Button {
            text: title_move_to
            onClicked: {
                logicManager.performHandling(tcpLUTParams.moduleName,LutModule.AA1_UPLOOK_POS)
            }
        }
    }
    RowLayout {
        Label { text: qsTr("AA 2 PR Position") }
        Label {
            text: qsTr("X")
        }
        TextField {
            text: tcpLUTUplookPositionAA2.X
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                tcpLUTUplookPositionAA2.setX(text)
            }
        }
        Label {
            text: qsTr("Y")
        }
        TextField {
            text: tcpLUTUplookPositionAA2.Y
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                tcpLUTUplookPositionAA2.setY(text)
            }
        }
        Label {
            text: qsTr("Z")
        }
        TextField {
            text: tcpLUTUplookPositionAA2.Z
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                tcpLUTUplookPositionAA2.setZ(text)
            }
        }
        Button {
            text: title_read_encoder
            onClicked: {
                var x = baseModuleManager.getMotorFeedbackPos(tcpLUTParams.motorXName)
                var y = baseModuleManager.getMotorFeedbackPos(tcpLUTParams.motorYName)
                var z = baseModuleManager.getMotorFeedbackPos(tcpLUTParams.motorZName)
                tcpLUTPickLensPositionAA2.setX(x)
                tcpLUTPickLensPositionAA2.setY(y)
                tcpLUTPickLensPositionAA2.setZ(z)
            }
        }
        Button {
            text: title_move_to
            onClicked: {
                logicManager.performHandling(tcpLUTParams.moduleName,LutModule.AA2_UPLOOK_POS)
            }
        }
    }
    RowLayout {
        RoundButton{
            id: aa_uplook_location_lock
            transformOrigin: Item.Center
            display: Button.TextBesideIcon
            icon.width: 30
            icon.height: 30
            icon.source: "../../../icons/unlock.png"
            icon.color: "red"
            onClicked: {
                aa_uplook_location_dial.enabled = !aa_uplook_location_dial.enabled
                if (aa_uplook_location_dial.enabled) {
                    aa_uplook_location_lock.icon.color = "lightGreen"
                } else {
                    aa_uplook_location_lock.icon.color = "red"
                }
            }
        }
        Dial{
            id: aa_uplook_location_dial
            enabled: false
            width: 25
            from: 0
            value: aa_uplook_location.lightBrightness
            to: 255
            stepSize: 1
            Label {
                text: aa_uplook_location.lightBrightness
                color: "white"
                font.pixelSize: Qt.application.font.pixelSize * 3
                anchors.centerIn: parent
            }
            onValueChanged: {
            }
        }
        Button{
            text:qsTr("AA Uplook PR Draw")
            onClicked: {
            }
        }
        TextField{
            color: "#57f529"
            text: aa_uplook_location.prFileName
            font.pixelSize: 14
            width: 600
            Layout.preferredWidth: 600
        }
    }
    RowLayout {
        FileDialog{
            id:aa_uplook_location_file_dialog
            title:"选择加载PR文件"
            selectExisting: true
            selectFolder: false
            selectMultiple: false

            nameFilters: ["avdata文件 (*.avdata)"]
            onAccepted:{
                aa_uplook_location.setPrFileName(fileUrl)
            }
        }
        Button{
            text:qsTr("读取PR文件")
            onClicked: {
                aa_uplook_location_file_dialog.open()
            }
        }
        Button{
            text:qsTr("执行PR")
            onClicked:{
                logicManagerState.setCurrentLocationName(aa_uplook_location.locationName)
                logicManagerState.setUseOriginPr(true)
                logicManager.performHandling(LogicManager.PERFORM_LOCATION)
            }
        }
        Button{
            text:qsTr("执行PR偏移")
            onClicked:{
                logicManagerState.setCurrentLocationName(aa_uplook_location.locationName)
                logicManagerState.setUseOriginPr(false)
                logicManager.performHandling(LogicManager.PERFORM_LOCATION)
            }
        }
    }
}
