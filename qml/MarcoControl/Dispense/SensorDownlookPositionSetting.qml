import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11

Column {
    RowLayout {
        Label { text: qsTr("Dispensing Glue PR Position") }
    }
    RowLayout {
        Label {
            text: qsTr("SUT 1 PR Position")
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

        Button{
           text:title_move_to
           onClicked: {
           }
        }
    }
    RowLayout {
        Label {
            text: qsTr("SUT 2 PR Position")
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

        Button{
           text:title_move_to
           onClicked: {
           }
        }
    }
}
