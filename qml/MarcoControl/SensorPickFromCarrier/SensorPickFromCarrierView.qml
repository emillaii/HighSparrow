import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11

ColumnLayout {
    width: 300
    Button {
        text: qsTr("Operation")
        Layout.minimumWidth: 400
        Layout.maximumWidth: 400
        width: 300
    }
    Button {
        text: qsTr("Input/Output Magazine And Carrier Position")
        Layout.minimumWidth: 400
        Layout.maximumWidth: 400
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
    Button {
        text: qsTr("Sensor Carrier Info& Position Setting")
        Layout.minimumWidth: 400
        Layout.maximumWidth: 400
        width: 300
        onClicked: { sensorCarrierInfoAndPositionSettingRow.visible = !sensorCarrierInfoAndPositionSettingRow.visible }
    }
    RowLayout{
        GroupBox {
            id: sensorCarrierInfoAndPositionSettingRow
            visible: false
            SensorCarrierInfoAndPositionSetting {}
        }
    }
}
