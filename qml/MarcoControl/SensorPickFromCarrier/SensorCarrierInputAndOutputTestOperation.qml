import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11
import FileContentItem 1.0
import QtQuick.Dialogs 1.2

Column{
    RowLayout {
        Label {
            text: qsTr("进盘弹夹层数")
        }
        TextField {
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
            }
        }
        Button {
            text: title_move_to
            width: 40
            height: 40
            onClicked: {
            }
        }
        Button {
            text: qsTr("Boat1 取盘")
        }
        Button {
            text: qsTr("Boat2 取盘")
        }
    }
    RowLayout {
        Label {
            text: qsTr("出盘弹夹层数")
        }
        TextField {
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
            }
        }
        Button {
            text: title_move_to
            width: 40
            height: 40
            onClicked: {
            }
        }
        Button {
            text: qsTr("推盘")
        }
    }

}
