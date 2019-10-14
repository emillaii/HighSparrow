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
        text: qsTr("Buffer Carrier Setting")
        Layout.minimumWidth: 400
        Layout.maximumWidth: 400
        width: 300
        onClicked: { bufferCarrierSetting.visible = !bufferCarrierSetting.visible }
    }
    RowLayout{
        GroupBox {
            id: bufferCarrierSetting
            visible: false
            BufferCarrierSetting {}
        }
    }
    Button {
        text: qsTr("Reject Carrier Setting")
        Layout.minimumWidth: 400
        Layout.maximumWidth: 400
        width: 300
        onClicked: { rejectCarrierSetting.visible = !rejectCarrierSetting.visible }
    }
    RowLayout{
        GroupBox {
            id: rejectCarrierSetting
            visible: false
            RejectCarrierSetting {}
        }
    }
}

