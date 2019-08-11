import QtQuick 2.5
import QtQuick.Controls 2.1

Popup
{
    property alias messageText: messageText
    id:myPopup
    width: 400
    height: 300
    modal: true
    focus: true
    closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutsideParent

    Rectangle
    {
        anchors.fill: parent
        color: "black"
        Text {
            id: messageText
            font.pixelSize: 24
            color: "white"
            text: qsTr("Popup 内容显示模块")
        }
    }
}
