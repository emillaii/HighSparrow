import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11
import QtQuick.Dialogs 1.2
import LogicManagerLib 1.1
import SomeLib 1.1

Column {
    RowLayout { Label { text: qsTr("Bond Position") } }
    RowLayout {
        Label {
            text: qsTr("SUT 1 Bond Position")
        }
        Label{
           text:qsTr("X")
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
           text:qsTr("Y")
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
           text:qsTr("Z")
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
        Button{
           text:title_read_encoder
           onClicked: {
           }
        }
    }
    RowLayout {
        Label {
            text: qsTr("SUT 2 Bond Position")
        }
        Label{
           text:qsTr("X")
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
           text:qsTr("Y")
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
           text:qsTr("Z")
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
        Button{
           text:title_read_encoder
           onClicked: {
           }
        }
    }
    RowLayout {
        Label { text: qsTr("AA 1 Head Position") }
        Label{
           text:qsTr("A")
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
           text:qsTr("B")
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
           text:qsTr("C")
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
        Button{
           text: qsTr("Home Tilt")
           onClicked: {
           }
        }
    }
    RowLayout {
        Label { text: qsTr("AA 2 Head Position") }
        Label{
           text:qsTr("A")
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
           text:qsTr("B")
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
           text:qsTr("C")
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
        Button{
           text: qsTr("Home Tilt")
           onClicked: {
           }
        }
    }

    AA1OperationTest {}
    AA2OperationTest {}
}
