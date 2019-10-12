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
        onClicked: {
            operation.visible = !operation.visible
        }
    }
    RowLayout{
        GroupBox{
            id: operation
            visible: false
            Operation{}
        }
    }

    Button {
        text: qsTr("Lens Tray Input And Output")
        Layout.minimumWidth: 300
        Layout.maximumWidth: 300
        width: 300
        onClicked: { lensTrayInputAndOutputRow.visible = !lensTrayInputAndOutputRow.visible }
    }

    RowLayout{
        GroupBox {
            id: lensTrayInputAndOutputRow
            visible: false
            LensTrayInputAndOutputRowView {}
        }
    }

    Button {
        text: qsTr("Lens Tray Info Setting")
        Layout.minimumWidth: 300
        Layout.maximumWidth: 300
        width: 300
        onClicked: { lensTrayInfoSettingRow.visible = !lensTrayInfoSettingRow.visible }
    }
    RowLayout{
        GroupBox{
            id: lensTrayInfoSettingRow
            visible: false
            LensTrayInfoSettingRowView {}
        }
    }

    Button {
        text: qsTr("Lens Pick And Place PR Setting")
        Layout.minimumWidth: 300
        Layout.maximumWidth: 300
        width: 300
        onClicked: {
            lensPickAndPlacePRSettingRow.visible = !lensPickAndPlacePRSettingRow.visible
        }
    }
    RowLayout{
        id: lensPickAndPlacePRSettingRow
        visible: false
        LenPickAndPlacePRSetting {}
    }

    Button {
        text: qsTr("Lens Pick And Place Setting")
        Layout.minimumWidth: 300
        Layout.maximumWidth: 300
        width: 300
        onClicked: { lensPickAndPlaceSettingRow.visible = !lensPickAndPlaceSettingRow.visible }
    }
    RowLayout{
        id: lensPickAndPlaceSettingRow
        visible: false
        GroupBox {
            LensPickAndPlaceSetting {}
        }
    }
}
