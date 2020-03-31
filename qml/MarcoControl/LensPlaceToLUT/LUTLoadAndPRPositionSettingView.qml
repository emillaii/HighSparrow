import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11
import QtQuick.Dialogs 1.2
import LogicManagerLib 1.1
import SomeLib 1.1
import LutModuleLib 1.1
ColumnLayout{
    RowLayout { Label { text: qsTr("LUT Load Position") }}
    RowLayout {
        Label { text: qsTr("LUT Load Position") }
        Label {
            text: qsTr("X")
        }
        TextField {
            text: tcpLUTLoadPosition.X
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                tcpLUTLoadPosition.setX(text)
            }
        }
        Label {
            text: qsTr("Y")
        }
        TextField {
            text: tcpLUTLoadPosition.Y
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                tcpLUTLoadPosition.setY(text)
            }
        }
        Label {
            text: qsTr("Z")
        }
        TextField {
            text: tcpLUTLoadPosition.Z
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                tcpLUTLoadPosition.setZ(text)
            }
        }
        Button {
            text: title_read_encoder
            onClicked: {
            }
        }
    }
    RowLayout { Label { text: qsTr("PR Position") }}
    RowLayout {
        Label {
            text: { text: qsTr("LUT1 Pocket PR Position") }
        }
        Label {
            text: qsTr("X")
        }
        TextField {
            text: tcp_lut_pr_position1.X
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                tcp_lut_pr_position1.setX(text)
            }
        }
        Label {
            text: qsTr("Y")
        }
        TextField {
            text: tcp_lut_pr_position1.Y
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                tcp_lut_pr_position1.setY(text)
            }
        }
        Button {
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
        Label {
            text: { text: qsTr("LUT2 Pocket PR Position") }
        }
        Label {
            text: qsTr("X")
        }
        TextField {
            text: tcp_lut_pr_position2.X
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                tcp_lut_pr_position2.setX(text)
            }
        }
        Label {
            text: qsTr("Y")
        }
        TextField {
            text: tcp_lut_pr_position2.Y
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
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
        Label {
            text: { text: qsTr("LUT Up Look PR Position") }
        }
        Label {
            text: qsTr("X")
        }
        TextField {
            text: tcp_lut_picker_position.X
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                tcp_lut_picker_position.setX(text)
            }
        }
        Label {
            text: qsTr("Y")
        }
        TextField {
            text: tcp_lut_picker_position.Y
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                tcp_lut_picker_position.setY(text)
            }
        }
        Button {
            text: title_read_encoder
            onClicked: {
                var x = baseModuleManager.getMotorFeedbackPos(tcp_lens_loader_parameter.motorXName)
                var y = baseModuleManager.getMotorFeedbackPos(tcp_lens_loader_parameter.motorYName)
                tcp_lut_picker_position.setX(x)
                tcp_lut_picker_position.setY(y)
            }
        }
    }
    RowLayout {
        Label { text: qsTr("测试")}
    }
    Button{
        text: qsTr("LUT 移动到 Load 位置")
        onClicked: {
            logicManager.performHandling(tcpLUTParams.moduleName,LutModule.LOAD_POS)
        }
    }
    Button{
        text: qsTr("LPA 移动到LUT1 Pocket PR Position")
        onClicked: {
            logicManager.performHandling(tcp_lens_loader_parameter.moduleName,LensLoaderModule.LUT_POS1)
        }
    }
    Button{
        text: qsTr("LPA 移动到 LUT2 Pocket PR Position")
        onClicked: {
            logicManager.performHandling(tcp_lens_loader_parameter.moduleName,LensLoaderModule.LUT_POS2)
        }
    }
    Button{
        text: qsTr("LPA 移动到 Up Look PR Position")
        onClicked: {
             logicManager.performHandling(tcp_lens_loader_parameter.moduleName,LensLoaderModule.UPDOWNLOOK_DOWN_POS)
        }
    }
}
