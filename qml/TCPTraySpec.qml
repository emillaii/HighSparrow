import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11
ColumnLayout {
    RowLayout{
        Label{
            text:qsTr("列数")
        }
        TextField{
            text:tcp_tray_standard_parameter.columnCount
            horizontalAlignment: TextInput.AlignHCenter
            validator: IntValidator{
                bottom:0
            }

            onEditingFinished: {
                tcp_tray_standard_parameter.setColumnCount(text)
            }
        }
        Label{
            text:qsTr("行数")
        }
        TextField{
            text:tcp_tray_standard_parameter.rowCount
            horizontalAlignment: TextInput.AlignHCenter
            validator: IntValidator{
                bottom: 0
            }

            onEditingFinished: {
                tcp_tray_standard_parameter.setRowCount(text)
            }
        }
    }
    RowLayout{
        Label{
            text:qsTr("列距")
        }
        TextField{
            text:tcp_tray_standard_parameter.columnDeta
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                tcp_tray_standard_parameter.setColumnDeta(text)
            }
        }
        Label{
            text:qsTr("行距")
        }
        TextField{
            text:tcp_tray_standard_parameter.rowDelta
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                tcp_tray_standard_parameter.setRowDelta(text)
            }
        }
        Button{
            width:40
            height: 40
            text:qsTr("计算")
            onClicked: {
                tcp_material_tray.calculateDelta()
            }
        }
    }
}
