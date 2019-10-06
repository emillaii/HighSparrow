import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11

ColumnLayout {
    width: 300
    Button {
        text: qsTr("Operation")
        Layout.minimumWidth: 300
        Layout.maximumWidth: 300
        width: 300
    }
    Button {
        text: qsTr("Input/Output Magazine And Carrier Position")
        Layout.minimumWidth: 300
        Layout.maximumWidth: 300
        width: 300
        onClicked: { inputAndOutputMagazineAndCarrierPositionRow.visible = !inputAndOutputMagazineAndCarrierPositionRow.visible }
    }
    RowLayout{
        GroupBox {
            id: inputAndOutputMagazineAndCarrierPositionRow
            visible: false
            InputAndOutputMagazineAndCarrierPosition {}
        }
    }
}
