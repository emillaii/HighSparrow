import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11
import QtQuick.Dialogs 1.2
import LogicManagerLib 1.1
import SomeLib 1.1

ColumnLayout {
    RowLayout {
        Label { text: qsTr("取放参数") }
    }
    RowLayout {
        Label { text: qsTr("取料力度") }
        TextField{
            text: tcp_lens_loader_parameter.vcmWorkForce
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator{
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                tcp_lens_loader_parameter.setVcmWorkForce(text)
            }
        }
        Label{
            text:qsTr("速度")
        }
        TextField{
            text: tcp_lens_loader_parameter.vcmWorkSpeed
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator{
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                tcp_lens_loader_parameter.setVcmWorkSpeed(text)
            }
        }
        Label{
            text:qsTr("限力区间")
        }
        TextField{
            text: tcp_lens_loader_parameter.vcmMargin
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator{
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                tcp_lens_loader_parameter.setVcmMargin(text)
            }
        }
    }
    RowLayout {
        Label { text: qsTr("取放LUT1 Pocket Lens 位置") }
    }
    RowLayout {
        Label { text: qsTr("LPA Position") }
        Label{
            text:qsTr("X")
        }
        TextField{
            text: tcp_lut_pr_position1.X
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator{
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                tcp_lut_pr_position1.setX(text)
            }
        }
        Label{
            text:qsTr("Y")
        }
        TextField{
            text: tcp_lut_pr_position1.Y
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator{
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                tcp_lut_pr_position1.setY(text)
            }
        }
        Button{
            text: title_read_encoder
            onClicked: {
                var x = baseModuleManager.getMotorFeedbackPos(tcp_lens_loader_parameter.motorXName)
                var y = baseModuleManager.getMotorFeedbackPos(tcp_lens_loader_parameter.motorYName)
                tcp_lut_pr_position1.setX(x)
                tcp_lut_pr_position1.setY(y)
            }
        }
    }
    RowLayout {
        Label{
            text:qsTr("物料高度")
        }
        TextField{
            text:tcp_lens_loader_parameter.placeLensZ
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator{
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                tcp_lens_loader_parameter.setPlaceLensZ(text)
            }
        }
        Label{
            text:qsTr("Theta")
        }
        TextField{
            text:tcp_lens_loader_parameter.placeTheta
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator{
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                tcp_lens_loader_parameter.setPlaceTheta(text)
            }
        }
        Button{
            text:qsTr("测高")
            width: 40
            height: 40
            onClicked: {
                logicManager.performHandling(tcp_lens_loader_parameter.moduleName,LensLoaderModule.MeasureLensInLUT)
            }
        }
    }
    RowLayout {
        Label { text: qsTr("取放LUT2 Pocket Lens 位置") }
    }
    RowLayout {
        Label { text: qsTr("LPA Position") }
        Label{
            text:qsTr("X")
        }
        TextField{
            text: tcp_lut_pr_position2.X
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator{
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                tcp_lut_pr_position2.setX(text)
            }
        }
        Label{
            text:qsTr("Y")
        }
        TextField{
            text: tcp_lut_pr_position2.Y
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator{
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                tcp_lut_pr_position2.setY(text)
            }
        }
        Button {
            text: title_read_encoder
            onClicked: {
                var x = baseModuleManager.getMotorFeedbackPos(tcp_lens_loader_parameter.motorXName)
                var y = baseModuleManager.getMotorFeedbackPos(tcp_lens_loader_parameter.motorYName)
                tcp_lut_pr_position2.setX(x)
                tcp_lut_pr_position2.setY(y)
            }           
        }
    }
    RowLayout {
        Label { text: qsTr("测试") }
    }
    RowLayout {
        Button {
            text: qsTr("移动到LUT1 Pocket视觉位置")
            onClicked: {
                logicManager.performHandling(tcp_lens_loader_parameter.moduleName,LensLoaderModule.LUT_POS1)
            }
        }
        Button {
            text: qsTr("执行视觉")
            onClicked: {
                logicManager.performHandling(tcp_lens_loader_parameter.moduleName,LensLoaderModule.LUT_POS1+
                                             lensLoaderModule.LENS_PR)
            }
        }
        Button {
            text: qsTr("LUT1 Pocket Place")
            onClicked: {
                var param = {}
                param["skip_dialog"] = true
                logicManager.performHandling(tcp_lens_loader_parameter.moduleName,LensLoaderModule.LUT_POS1+
                                             LensLoaderModule.LUT_PR+
                                             LensLoaderModule.ToWork+
                                             LensLoaderModule.PLACE_LENS_TO_LUT, JSON.stringify(param))
            }
        }
        Button {
            text: qsTr("LUT1 Pocket Pick")
            onClicked: {
                var param = {}
                param["skip_dialog"] = true
                logicManager.performHandling(tcp_lens_loader_parameter.moduleName,LensLoaderModule.LUT_POS1+
                                             LensLoaderModule.LUT_LENS_PR+
                                             LensLoaderModule.ToWork+
                                             LensLoaderModule.PICK_NG_LENS_FROM_LUT1, JSON.stringify(param))
            }
        }
    }
    RowLayout {
        Button {
            text: qsTr("移动到LUT2 Pocket视觉位置")
            onClicked: {
                logicManager.performHandling(tcp_lens_loader_parameter.moduleName,LensLoaderModule.LUT_POS2)
            }
        }
        Button {
            text: qsTr("执行视觉")
            onClicked: {
                logicManager.performHandling(tcp_lens_loader_parameter.moduleName,LensLoaderModule.LUT_POS2
                                             +lensLoaderModule.LUT_PR)
            }
        }
        Button {
            text: qsTr("LUT2 Pocket Place")
            onClicked: {
                var param = {}
                param["skip_dialog"] = true
                logicManager.performHandling(tcp_lens_loader_parameter.moduleName,LensLoaderModule.LUT_POS2+
                                             LensLoaderModule.LUT_NG_SLOT_PR+
                                             LensLoaderModule.ToWork+
                                             LensLoaderModule.PLACE_LENS_TO_LUT2, JSON.stringify(param))
            }
        }
        Button {
            text: qsTr("LUT2 Pocket Pick")
            onClicked: {
                var param = {}
                param["skip_dialog"] = true
                logicManager.performHandling(tcp_lens_loader_parameter.moduleName,LensLoaderModule.LUT_POS2+
                                             LensLoaderModule.LUT_PR+
                                             LensLoaderModule.ToWork+
                                             LensLoaderModule.PICK_NG_LENS_FROM_LUT2, JSON.stringify(param))
            }
        }
    }
    RowLayout {
        Button{
            text: qsTr("LUT Pocket 1 VAC")
            onClicked: {
                baseModuleManager.toogleIoState(tcpLUTParams.tcpLutVacuum1Name)
            }
        }
        RoundButton{
            background: Rectangle {
                radius: 6
                color: {
                    if (tcpLUTState.tcpVaccum1State) return "lightGreen"
                    else return "red"
                }
            }
        }
        Button{
            text: qsTr("LUT Pocket2 VAC")
            onClicked: {
                baseModuleManager.toogleIoState(tcpLUTParams.tcpLutVacuum2Name)
            }
        }
        RoundButton{
            background: Rectangle {
                radius: 6
                color: {
                    if (tcpLUTState.tcpVaccum2State) return "lightGreen"
                    else return "red"
                }
            }
        }
    }
}
