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
        text: qsTr("LUT Load And PR Position Setting")
        Layout.minimumWidth: 300
        Layout.maximumWidth: 300
        width: 300
        onClicked: { lutLoadAndPRPositionSettingRow.visible = !lutLoadAndPRPositionSettingRow.visible }
    }

    RowLayout{
        GroupBox {
            id: lutLoadAndPRPositionSettingRow
            visible: false
            LUTLoadAndPRPositionSettingView {}
        }
    }

    Button {
        text: qsTr("LUT Pocket And Up Look PR Setting")
        Layout.minimumWidth: 300
        Layout.maximumWidth: 300
        width: 300
        onClicked: { lutPocketAndUplookPRSettingRow.visible = !lutPocketAndUplookPRSettingRow.visible }
    }

    RowLayout{
        GroupBox {
            id: lutPocketAndUplookPRSettingRow
            visible: false
            LUTPocketAndUplookPRSettingView {}
        }
    }

    Button {
        text: qsTr("Lens Pick And Place Setting")
        Layout.minimumWidth: 300
        Layout.maximumWidth: 300
        width: 300
        onClicked: { lensPickAndPlaceSettingRow.visible = !lensPickAndPlaceSettingRow.visible }
    }

    RowLayout{
        GroupBox {
            id: lensPickAndPlaceSettingRow
            visible: false
            LensPickAndPlaceSetting {}
        }
    }
}
