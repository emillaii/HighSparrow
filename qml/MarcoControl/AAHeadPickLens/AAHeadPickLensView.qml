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
        onClicked: { operation.visible = !operation.visible }
    }
    RowLayout{
        GroupBox{
            id: operation
            visible: false
            Operation{}
        }
    }

    Button {
        text: qsTr("AA Head Pick And Place Lens Setting")
        Layout.minimumWidth: 300
        Layout.maximumWidth: 300
        width: 300
        onClicked: { aaHeadPickAndPlaceLensSettingRow.visible = !aaHeadPickAndPlaceLensSettingRow.visible }
    }

    RowLayout{
        GroupBox {
            id: aaHeadPickAndPlaceLensSettingRow
            visible: false
            AAHeadPickAndPlaceLensSettingView {}
        }
    }

    Button {
        text: qsTr("AA Head Up Look PR Setting")
        Layout.minimumWidth: 300
        Layout.maximumWidth: 300
        width: 300
        onClicked: { aaHeadUplookPRSettingRow.visible = !aaHeadUplookPRSettingRow.visible }
    }

    RowLayout{
        GroupBox {
            id: aaHeadUplookPRSettingRow
            visible: false
            AAHeadUplookPRSetting {}
        }
    }

}
