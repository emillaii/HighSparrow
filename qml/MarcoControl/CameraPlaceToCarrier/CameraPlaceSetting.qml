import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11

Column {
    RowLayout { Label { text: qsTr("取放参数") } }
    RowLayout {
        Label { text: qsTr("SUT1 放料区间") }
        TextField{
           horizontalAlignment: TextInput.AlignHCenter
           validator: DoubleValidator{
               decimals: 6
               notation: DoubleValidator.StandardNotation
           }
           onEditingFinished: {
           }
        }
        CheckBox{
            text: qsTr("不限力")
            onCheckedChanged: {
            }
        }
        Label { text: qsTr("SUT1 取料区间") }
        TextField{
           horizontalAlignment: TextInput.AlignHCenter
           validator: DoubleValidator{
               decimals: 6
               notation: DoubleValidator.StandardNotation
           }
           onEditingFinished: {
           }
        }
        CheckBox{
            text: qsTr("不限力")
            onCheckedChanged: {
            }
        }
    }

    RowLayout {
        Label { text: qsTr("Carrier1 放料区间") }
        TextField{
           horizontalAlignment: TextInput.AlignHCenter
           validator: DoubleValidator{
               decimals: 6
               notation: DoubleValidator.StandardNotation
           }
           onEditingFinished: {
           }
        }
        CheckBox{
            text: qsTr("不限力")
            onCheckedChanged: {
            }
        }
        Label { text: qsTr("Carrier1 取料区间") }
        TextField{
           horizontalAlignment: TextInput.AlignHCenter
           validator: DoubleValidator{
               decimals: 6
               notation: DoubleValidator.StandardNotation
           }
           onEditingFinished: {
           }
        }
        CheckBox{
            text: qsTr("不限力")
            onCheckedChanged: {
            }
        }
    }

    RowLayout {
        Label { text: qsTr("Carrier2 放料区间") }
        TextField{
           horizontalAlignment: TextInput.AlignHCenter
           validator: DoubleValidator{
               decimals: 6
               notation: DoubleValidator.StandardNotation
           }
           onEditingFinished: {
           }
        }
        CheckBox{
            text: qsTr("不限力")
            onCheckedChanged: {
            }
        }
        Label { text: qsTr("Carrier1 取料区间") }
        TextField{
           horizontalAlignment: TextInput.AlignHCenter
           validator: DoubleValidator{
               decimals: 6
               notation: DoubleValidator.StandardNotation
           }
           onEditingFinished: {
           }
        }
        CheckBox{
            text: qsTr("不限力")
            onCheckedChanged: {
            }
        }
    }

    RowLayout {
        Label { text: qsTr("Carrier1 Pick And Place 高度") }
        TextField{
           horizontalAlignment: TextInput.AlignHCenter
           validator: DoubleValidator{
               decimals: 6
               notation: DoubleValidator.StandardNotation
           }
           onEditingFinished: {
           }
        }
        Button { text: qsTr("Camera视觉") }
        Button { text: qsTr("移动到取放位置") }
        Button { text: qsTr("测高") }
    }

    RowLayout {
        Label { text: qsTr("Carrier2 Pick And Place 高度") }
        TextField{
           horizontalAlignment: TextInput.AlignHCenter
           validator: DoubleValidator{
               decimals: 6
               notation: DoubleValidator.StandardNotation
           }
           onEditingFinished: {
           }
        }
        Button { text: qsTr("Camera视觉") }
        Button { text: qsTr("移动到取放位置") }
        Button { text: qsTr("测高") }
    }
    CameraPlaceSettingTestOperation {}

}
