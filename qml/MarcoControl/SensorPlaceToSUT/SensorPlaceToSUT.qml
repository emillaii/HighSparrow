import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11

ColumnLayout {
    width: 300
    Button {
        text: qsTr("Operation")
        Layout.minimumWidth: 400
        Layout.maximumWidth: 400
        width: 400
        onClicked: { operation.visible = !operation.visible }
    }
    RowLayout{
        GroupBox {
            id: operation
            visible: false
            Operation {}
        }
    }

    Button {
        text: qsTr("SUT Load Position Setting")
        Layout.minimumWidth: 400
        Layout.maximumWidth: 400
        width: 400
        onClicked: { sutLoadPositionRow.visible = !sutLoadPositionRow.visible }
    }

    RowLayout{
        GroupBox {
            id: sutLoadPositionRow
            visible: false
            SUTLoadPosition {}
        }
    }

    Button {
        text: qsTr("Sensor Pick And Place To SUT Setting")
        Layout.minimumWidth: 400
        Layout.maximumWidth: 400
        width: 400
        onClicked: { sensorPickAndPlaceToSUTSettingRow.visible = !sensorPickAndPlaceToSUTSettingRow.visible }
    }

    RowLayout{
        GroupBox {
            id: sensorPickAndPlaceToSUTSettingRow
            visible: false
            SensorPickAndPlaceToSUTSetting {}
        }
    }

}
