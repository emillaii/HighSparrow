import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11
import QtQuick.Dialogs 1.2
import LogicManagerLib 1.1
import SomeLib 1.1

Column {
   RowLayout {
       Label { text: qsTr("STL夹盘位置") }
       TextField {
           horizontalAlignment: TextInput.AlignHCenter
           validator: DoubleValidator {
               decimals: 6
               notation: DoubleValidator.StandardNotation
           }
           onEditingFinished: {
           }
       }
       Button{
           text:title_read_encoder
           onClicked: {
           }
       }
       Button{
           text:title_move_to
           onClicked: {
           }
       }
       Label { text: qsTr("STL夹盘结束位置") }
       TextField {
           horizontalAlignment: TextInput.AlignHCenter
           validator: DoubleValidator {
               decimals: 6
               notation: DoubleValidator.StandardNotation
           }
           onEditingFinished: {
           }
       }
       Button{
           text:title_read_encoder
           onClicked: {
           }
       }
       Button{
           text:title_move_to
           onClicked: {
           }
       }
   }
   RowLayout {
       Label { text: qsTr("STL Load 位置") }
       TextField {
           horizontalAlignment: TextInput.AlignHCenter
           validator: DoubleValidator {
               decimals: 6
               notation: DoubleValidator.StandardNotation
           }
           onEditingFinished: {
           }
       }
       Button{
           text:title_read_encoder
           onClicked: {
           }
       }
       Button{
           text:title_move_to
           onClicked: {
           }
       }
       Label { text: qsTr("STL 工作位置") }
       TextField {
           horizontalAlignment: TextInput.AlignHCenter
           validator: DoubleValidator {
               decimals: 6
               notation: DoubleValidator.StandardNotation
           }
           onEditingFinished: {
           }
       }
       Button{
           text:title_read_encoder
           onClicked: {
           }
       }
       Button{
           text:title_move_to
           onClicked: {
           }
       }
   }
   RowLayout {
       Label { text: qsTr("STL Unload 位置") }
       TextField {
           horizontalAlignment: TextInput.AlignHCenter
           validator: DoubleValidator {
               decimals: 6
               notation: DoubleValidator.StandardNotation
           }
           onEditingFinished: {
           }
       }
       Button{
           text:title_read_encoder
           onClicked: {
           }
       }
       Button{
           text:title_move_to
           onClicked: {
           }
       }
   }
}
