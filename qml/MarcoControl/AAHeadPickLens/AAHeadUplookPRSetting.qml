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
            id: aa1_uplook_location_lock
            transformOrigin: Item.Center
            display: Button.TextBesideIcon
            icon.width: 30
            icon.height: 30
            icon.source: "../../../icons/unlock.png"
            icon.color: "red"
            onClicked: {
                aa1_uplook_location_slider.enabled = !aa1_uplook_location_slider.enabled
                if (aa1_uplook_location_slider.enabled) {
                    aa1_uplook_location_lock.icon.color = "lightGreen"
                } else {
                    aa1_uplook_location_lock.icon.color = "red"
                }
                aa1_uplook_location_slider.value = tcp_vision_location_aa1_uplook.lightBrightness
            }
        }
        Slider {
            id: aa1_uplook_location_slider
            enabled: false
            width: 25
            from: 0
            value: tcp_vision_location_aa1_uplook.lightBrightness
            to: 255
            stepSize: 1
            onValueChanged: {
                tcp_vision_location_aa1_uplook.setLightBrightness(value)
                lightingController.setUplookLighting(value)
            }
        }
        Label {
            text: aa1_uplook_location_slider.value
        }
        Button{
            text:qsTr("AA1 Uplook PR Draw")
            onClicked: {
                 highSprrow.callAVSPRSettingProcess("GenericNCCavproj.avexe", "aa1_upLook_location");
            }
        }
        TextField{
            color: "#57f529"
            text: tcp_vision_location_aa1_uplook.locationName
            font.pixelSize: 14
            width: 200
            Layout.preferredWidth: 200
        }
        RoundButton{
            transformOrigin: Item.Center
            display: Button.TextBesideIcon
            icon.width: 30
            icon.height: 30
            icon.source: "../../../icons/refresh.png"
            icon.color: "cyan"
            onClicked: {
                aa1_upLook_location_image.source = ""
                aa1_upLook_location_image.source = "file:///" + dirPath + "//config//prConfig//aa1_upLook_location_resultImage.jpg"
            }
        }
    }
    RowLayout {
        Button{
            text:qsTr("执行PR")
            onClicked:{
                logicManagerState.setCurrentLocationName(tcp_vision_location_aa1_uplook.locationName)
                logicManagerState.setUseOriginPr(true)
                logicManager.performHandling("", LogicManager.HANDLING_ORIGIN_LOCATION, "aa1_upLook_location")
            }
        }
        Button{
            text:qsTr("执行PR偏移")
            onClicked:{
                logicManager.performHandling("", LogicManager.HANDLING_OFFSET_LOCATION, "aa1_upLook_location")
            }
        }
        Image{
            id: aa1_upLook_location_image
            cache: false
            sourceSize.width: 250
            sourceSize.height: 250
            fillMode: Image.PreserveAspectFit
            source: "file:///" + dirPath + "//config//prConfig//aa1_upLook_location_resultImage.jpg"
        }
    }
}
