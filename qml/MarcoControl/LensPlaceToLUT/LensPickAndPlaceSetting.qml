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
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator{
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
            }
        }
        Label{
            text:qsTr("速度")
        }
        TextField{
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator{
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
            }
        }
        Label{
            text:qsTr("限力区间")
        }
        TextField{
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator{
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
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
            text: "0"
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator{
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
            }
        }
        Label{
            text:qsTr("Y")
        }
        TextField{
            text: "0"
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator{
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
            }
        }
        Label{
            text:qsTr("Z")
        }
        TextField{
            text: "0"
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator{
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
            }
        }
        Label{
            text:qsTr("Theta")
        }
        TextField{
            text: "0"
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator{
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
            }
        }
        Button {
            text: title_read_encoder
            onClicked: {
            }
        }
        Button {
            text: qsTr("测高")
            onClicked: {
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
            text: "0"
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator{
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
            }
        }
        Label{
            text:qsTr("Y")
        }
        TextField{
            text: "0"
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator{
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
            }
        }
        Label{
            text:qsTr("Z")
        }
        TextField{
            text: "0"
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator{
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
            }
        }
        Label{
            text:qsTr("Theta")
        }
        TextField{
            text: "0"
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator{
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
            }
        }
        Button {
            text: title_read_encoder
            onClicked: {
            }
        }
        Button {
            text: qsTr("测高")
            onClicked: {
            }
        }
    }
    RowLayout {
        Label { text: qsTr("测试") }
    }
    RowLayout {
        Button { text: qsTr("移动到LUT1 Pocket视觉位置") }
        Button { text: qsTr("执行视觉") }
        Button { text: qsTr("LUT1 Pocket Place") }
        Button { text: qsTr("LUT1 Pocket Pick") }
    }
    RowLayout {
        Button { text: qsTr("移动到LUT2 Pocket视觉位置") }
        Button { text: qsTr("执行视觉") }
        Button { text: qsTr("LUT2 Pocket Place") }
        Button { text: qsTr("LUT2 Pocket Pick") }
    }
    RowLayout {
        Button{ text: qsTr("LUT Pocket1 VAC") }
        RoundButton{
            background: Rectangle {
                radius: 6
                color: {
                    if (tcpLUTState.tcpVaccum1State) return "lightGreen"
                    else return "red"
                }
            }
        }
        Button{ text: qsTr("LUT Pocket2 VAC") }
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
