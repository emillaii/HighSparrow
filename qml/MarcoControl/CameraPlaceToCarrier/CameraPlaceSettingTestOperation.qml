import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11

Column {
    RowLayout {
        Label { text: qsTr("料盘1位置") }
        Label { text: qsTr("Row") }
        TextField{
           horizontalAlignment: TextInput.AlignHCenter
           validator: DoubleValidator{
               decimals: 6
               notation: DoubleValidator.StandardNotation
           }
           onEditingFinished: {
           }
        }
        Label { text: qsTr("Col") }
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
            text: title_move_to
        }
    }
    RowLayout {
        Button { text: qsTr("Pick Camera") }
        Button { text: qsTr("Place Camera") }
    }
    RowLayout {
        Button{
            text: qsTr("CPA Collet VAC")
        }
        RoundButton{
            background: Rectangle {
                radius: 6
                color: "red"
            }
        }
    }
}



