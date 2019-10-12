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
        text: qsTr("Camera Pick And Place PR Setting")
        Layout.minimumWidth: 400
        Layout.maximumWidth: 400
        width: 300
        onClicked: { cameraPickAndPlacePRSetting.visible = !cameraPickAndPlacePRSetting.visible }
    }
    RowLayout{
        GroupBox {
            id: cameraPickAndPlacePRSetting
            visible: false
            CameraPickAndPlacePRSetting {}
        }
    }

    Button {
        text: qsTr("Camera Place Setting")
        Layout.minimumWidth: 400
        Layout.maximumWidth: 400
        width: 300
        onClicked: { cameraPlaceSetting.visible = !cameraPlaceSetting.visible }
    }
    RowLayout{
        GroupBox {
            id: cameraPlaceSetting
            visible: false
            CameraPlaceSetting {}
        }
    }
}
