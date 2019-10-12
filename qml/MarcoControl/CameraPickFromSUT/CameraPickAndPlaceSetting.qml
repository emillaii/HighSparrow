import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11
Column {
    RowLayout { Label { text: qsTr("取放参数") } }
    RowLayout {
        Label { text: qsTr("取料压力") }
        TextField{
           horizontalAlignment: TextInput.AlignHCenter
           validator: DoubleValidator{
               decimals: 6
               notation: DoubleValidator.StandardNotation
           }
           onEditingFinished: {
           }
        }
        Label { text: qsTr("速度") }
        TextField{
           horizontalAlignment: TextInput.AlignHCenter
           validator: DoubleValidator{
               decimals: 6
               notation: DoubleValidator.StandardNotation
           }
           onEditingFinished: {
           }
        }
        Label { text: qsTr("限力区间") }
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
        Label { text: qsTr("SUT2 放料区间") }
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
        Label { text: qsTr("SUT2 取料区间") }
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
        Label { text: qsTr("CPA Pick And Place SUT1 Position") }
        Label { text: qsTr("X") }
        TextField{
           horizontalAlignment: TextInput.AlignHCenter
           validator: DoubleValidator{
               decimals: 6
               notation: DoubleValidator.StandardNotation
           }
           onEditingFinished: {
           }
        }
        Label { text: qsTr("Y") }
        TextField{
           horizontalAlignment: TextInput.AlignHCenter
           validator: DoubleValidator{
               decimals: 6
               notation: DoubleValidator.StandardNotation
           }
           onEditingFinished: {
           }
        }
        Label { text: qsTr("Z") }
        TextField{
           horizontalAlignment: TextInput.AlignHCenter
           validator: DoubleValidator{
               decimals: 6
               notation: DoubleValidator.StandardNotation
           }
           onEditingFinished: {
           }
        }
        Label { text: qsTr("Theta") }
        TextField{
           horizontalAlignment: TextInput.AlignHCenter
           validator: DoubleValidator{
               decimals: 6
               notation: DoubleValidator.StandardNotation
           }
           onEditingFinished: {
           }
        }
        Button{
           text: title_read_encoder
           onClicked: {
           }
        }
        Button{
           text: title_move_to
           onClicked: {
           }
        }
        Button{
           text: qsTr("Camera视觉")
           onClicked: {
           }
        }
        Button{
           text: qsTr("移动到取放位置")
           onClicked: {
           }
        }
        Button{
           text: qsTr("测高")
           onClicked: {
           }
        }
    }

    RowLayout {
        Label { text: qsTr("CPA Pick And Place SUT2 Position") }
        Label { text: qsTr("X") }
        TextField{
           horizontalAlignment: TextInput.AlignHCenter
           validator: DoubleValidator{
               decimals: 6
               notation: DoubleValidator.StandardNotation
           }
           onEditingFinished: {
           }
        }
        Label { text: qsTr("Y") }
        TextField{
           horizontalAlignment: TextInput.AlignHCenter
           validator: DoubleValidator{
               decimals: 6
               notation: DoubleValidator.StandardNotation
           }
           onEditingFinished: {
           }
        }
        Label { text: qsTr("Z") }
        TextField{
           horizontalAlignment: TextInput.AlignHCenter
           validator: DoubleValidator{
               decimals: 6
               notation: DoubleValidator.StandardNotation
           }
           onEditingFinished: {
           }
        }
        Label { text: qsTr("Theta") }
        TextField{
           horizontalAlignment: TextInput.AlignHCenter
           validator: DoubleValidator{
               decimals: 6
               notation: DoubleValidator.StandardNotation
           }
           onEditingFinished: {
           }
        }
        Button{
           text: title_read_encoder
           onClicked: {
           }
        }
        Button{
           text: title_move_to
           onClicked: {
           }
        }
        Button{
           text: qsTr("Camera视觉")
           onClicked: {
           }
        }
        Button{
           text: qsTr("移动到取放位置")
           onClicked: {
           }
        }
        Button{
           text: qsTr("测高")
           onClicked: {
           }
        }
    }

    RowLayout {
        Label { text: qsTr("SUT1 Escape") }
        Label { text: qsTr("X") }
        TextField{
           horizontalAlignment: TextInput.AlignHCenter
           validator: DoubleValidator{
               decimals: 6
               notation: DoubleValidator.StandardNotation
           }
           onEditingFinished: {
           }
        }
        Label { text: qsTr("Y") }
        TextField{
           horizontalAlignment: TextInput.AlignHCenter
           validator: DoubleValidator{
               decimals: 6
               notation: DoubleValidator.StandardNotation
           }
           onEditingFinished: {
           }
        }
        Label { text: qsTr("Z") }
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
        Label { text: qsTr("SUT2 Escape") }
        Label { text: qsTr("X") }
        TextField{
           horizontalAlignment: TextInput.AlignHCenter
           validator: DoubleValidator{
               decimals: 6
               notation: DoubleValidator.StandardNotation
           }
           onEditingFinished: {
           }
        }
        Label { text: qsTr("Y") }
        TextField{
           horizontalAlignment: TextInput.AlignHCenter
           validator: DoubleValidator{
               decimals: 6
               notation: DoubleValidator.StandardNotation
           }
           onEditingFinished: {
           }
        }
        Label { text: qsTr("Z") }
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

    CameraPickAndPlaceSettingTestOperation {}
}
