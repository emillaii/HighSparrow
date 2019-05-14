import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11

ColumnLayout{
    RowLayout{
        Label{
            text: qsTr("tray_start_X1")
        }
        TextField{
            text: tray_start_point1.X
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                tray_start_point1.setX(text)
            }
        }
        Label{
            text: qsTr("tray_start_Y1")
        }
        TextField{
            text: tray_start_point1.Y
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                tray_start_point1.setY(text)
            }
        }
        Button{
            text:title_move_to
            width: 40
            height: 40
            onClicked: {
                logicManager.lensPickArmMoveToTray1StartPos()
            }
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
            text: tray_start_point2.X
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                tray_start_point2.setX(text)
            }
        }
        Label{
            text: qsTr("tray_start_Y2")
        }
        TextField{
            text: tray_start_point2.Y
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                tray_start_point2.setY(text)
            }
        }
        Button{
            text:title_move_to
            width: 40
            height: 40
            onClicked: {
                logicManager.lensPickArmMoveToTray2StartPos()
            }
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
            text: first_tray_end_position.X
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                first_tray_end_position.setX(text)
            }
        }
        Label{
            text: qsTr("tray_end_Y")
        }
        TextField{
            text: first_tray_end_position.Y
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                first_tray_end_position.setY(text)
            }
        }
        Button{
            text:title_move_to
            width: 40
            height: 40
            onClicked: {

                logicManager.lensPickArmMoveToTray1EndPos()
            }
        }
        Button{
            text:title_read_encoder
            width: 40
            height: 40
        }
    }

    RowLayout{
        Label{
            text:qsTr("column count")
        }
        TextField{
            text:tray_standard_parameter.columnCount
            horizontalAlignment: TextInput.AlignHCenter
            validator: IntValidator
            onEditingFinished: {
                tray_standard_parameter.setColumnCount(text)
            }
        }
        Label{
            text:qsTr("row count")
        }
        TextField{
            text:tray_standard_parameter.rowCount
            horizontalAlignment: TextInput.AlignHCenter
            validator: IntValidator
            onEditingFinished: {
                tray_standard_parameter.setRowCount(text)
            }
        }
    }
    RowLayout{
        Label{
            text:qsTr("columnDeta")
        }
        TextField{
            text:tray_standard_parameter.columnDeta
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                tray_standard_parameter.setColumnDeta(text)
            }
        }
        Label{
            text:qsTr("rowDeta")
        }
        TextField{
            text:tray_standard_parameter.rowDelta
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                tray_standard_parameter.setRowDelta(text)
            }
        }
    }

    RowLayout{
        Label{
            text: qsTr("tray1_nColumn")
        }
        TextField{
            text: "0"
            horizontalAlignment: TextInput.AlignHCenter
            validator: IntValidator
        }
        Label{
            text: qsTr("tray1_nRow")
        }
        TextField{
            text: "0"
            horizontalAlignment: TextInput.AlignHCenter
            validator: IntValidator
            onEditingFinished: {

            }
        }
        Button{
            text:title_move_to
            width: 40
            height: 40
            onClicked: {
                //logicManager.lensPickArmMoveToTray1Pos();
            }
        }
    }
}

