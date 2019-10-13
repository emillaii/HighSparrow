import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11

Column {
    RowLayout {
        Label { text: qsTr("料盘信息") }
    }
    RowLayout {
        Label { text: qsTr("料盘1起点") }
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
        Button {
            text: title_read_encoder
        }
        Label { text: qsTr("料盘1终点") }
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
        Button {
            text: title_read_encoder
        }
    }
    RowLayout {
        Label { text: qsTr("参数设置") }
    }
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
        Label { text: qsTr("Buffer 放料区间") }
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
        Label { text: qsTr("Buffer Place Offset") }
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
        Button {
            text: title_read_encoder
        }
        Button {
            text: title_move_to
        }
    }

    RowLayout {
        Label { text: qsTr("Buffer Place高度") }
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
            text: qsTr("移动到取放位置")
        }
        Button {
            text: qsTr("测高")
        }
    }

    RowLayout {
        BufferCarrierTestOperation {}
    }
}
