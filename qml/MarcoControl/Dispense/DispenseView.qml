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
        onClicked: {
            operation.visible = !operation.visible
        }
    }
    RowLayout{
        GroupBox {
            id: operation
            visible: false
            Operation {}
        }
    }

    Button {
        text: qsTr("Sensor Down Look Position Setting")
        Layout.minimumWidth: 400
        Layout.maximumWidth: 400
        width: 400
        onClicked: { sensorDownlookPositionSettingRow.visible = !sensorDownlookPositionSettingRow.visible }
    }

    RowLayout{
        GroupBox {
            id: sensorDownlookPositionSettingRow
            visible: false
            SensorDownlookPositionSetting {}
        }
    }

    Button {
        text: qsTr("Sensor Disepnsing Glue PR Setting")
        Layout.minimumWidth: 400
        Layout.maximumWidth: 400
        width: 400
        onClicked: { sensorDispensingGluePRSettingRow.visible = !sensorDispensingGluePRSettingRow.visible }
    }

    RowLayout{
        GroupBox {
            id: sensorDispensingGluePRSettingRow
            visible: false
            SensorDispensingGluePRSetting {}
        }
    }

    Button {
        text: qsTr("Sensor Disepnsing Glue Setting")
        Layout.minimumWidth: 400
        Layout.maximumWidth: 400
        width: 400
        onClicked: { sensorDispensingGlueSettingRow.visible = !sensorDispensingGlueSettingRow.visible }
    }

    RowLayout{
        GroupBox {
            id: sensorDispensingGlueSettingRow
            visible: false
            SensorDispensingGlueSetting {}
        }
    }
}
