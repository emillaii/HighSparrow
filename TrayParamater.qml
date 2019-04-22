import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11

ColumnLayout{
    RowLayout{
        Label{
            text: qsTr("tray_start_X1")
        }
        TextField{
            text: tray_start_x1
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 3
                notation: DoubleValidator.StandardNotation
            }
        }
        Label{
            text: qsTr("tray_start_Y1")
        }
        TextField{
            text: tray_start_y1
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 3
                notation: DoubleValidator.StandardNotation
            }
        }
        Button{
            text:title_move_to
            width: 40
            height: 40
        }
        Button{
            text:title_read_encoder
            width: 40
            height: 40
        }
    }

    RowLayout{
        Label{
            text: qsTr("tray_start_X2")
        }
        TextField{
            text: tray_start_x2
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 3
                notation: DoubleValidator.StandardNotation
            }
        }
        Label{
            text: qsTr("tray_start_Y2")
        }
        TextField{
            text: tray_start_y2
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 3
                notation: DoubleValidator.StandardNotation
            }
        }
        Button{
            text:title_move_to
            width: 40
            height: 40
        }
        Button{
            text:title_read_encoder
            width: 40
            height: 40
        }
    }

    RowLayout{
        Label{
            text: qsTr("tray_end_X")
        }
        TextField{
            text: tray_end_x
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 3
                notation: DoubleValidator.StandardNotation
            }
        }
        Label{
            text: qsTr("tray_start_y1")
        }
        TextField{
            text: tray_end_y
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 3
                notation: DoubleValidator.StandardNotation
            }
        }
        Button{
            text:title_move_to
            width: 40
            height: 40
        }
        Button{
            text:title_read_encoder
            width: 40
            height: 40
        }
    }
}

