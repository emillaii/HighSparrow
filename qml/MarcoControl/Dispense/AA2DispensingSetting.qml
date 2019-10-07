import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11
import QtQuick.Dialogs 1.2
import LogicManagerLib 1.1
import SomeLib 1.1

Column {
    RowLayout {
        Label { text : qsTr("AA2 点胶设置") }
    }
    RowLayout {
        Label { text : qsTr("测高力度") }
        TextField{
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator{
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
            }
        }
        Label { text : qsTr("针头抬高") }
        TextField{
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator{
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
            }
        }
        Label { text : qsTr("模板角度") }
        TextField{
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator{
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
            }
        }
        Label { text : qsTr("UV Time（ms）") }
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
        Label { text : qsTr("出胶延时") }
        TextField{
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator{
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
            }
        }
        Label { text : qsTr("收胶延时") }
        TextField{
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator{
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
            }
        }
        Button { text : qsTr("Dispensing Path Edit") }
        Button { text : qsTr("Dispensing Line Speed") }
    }

    RowLayout {
        Label { text : qsTr("Dispensing Offset") }
        Label { text : qsTr("SUT X") }
        TextField{
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator{
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
            }
        }
        Label { text : qsTr("Y") }
        TextField{
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator{
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
            }
        }
        Button {
            text: qsTr("视觉")
        }
        Button {
            text: qsTr("喷胶")
        }
        Button {
            text: qsTr("读取")
        }
    }

    RowLayout {
        Label { text : qsTr("点胶高度 SUT Z") }
        TextField{
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator{
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
            }
        }
        Button {
            text: qsTr("测高")
        }
    }
}
