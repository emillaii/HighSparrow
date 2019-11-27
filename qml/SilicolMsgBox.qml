import QtQuick 2.0
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0
import MsgBoxModel 1.0

Popup {
    property int msgBoxWidth: 500
    property int msgBoxHeight: 200
    property int maxMsgBoxesToShow: 3
    property int msgBoxesMargin: 10
    property int msgBoxCount: 0

    anchors.centerIn: parent
    closePolicy: Popup.NoAutoClose

    width: msgBoxWidth
    height: {
        if(msgBoxCount > maxMsgBoxesToShow)
        {
            return msgBoxHeight * maxMsgBoxesToShow + msgBoxesMargin * (maxMsgBoxesToShow - 1)
        }
        else{
            return msgBoxHeight * msgBoxCount + msgBoxesMargin * (msgBoxCount - 1)
        }
    }

    contentItem: ListView{
        anchors.fill: parent
        spacing: msgBoxesMargin
        model: msgBoxModel
        delegate: Rectangle{
            radius: 20
            width: msgBoxWidth
            height: msgBoxHeight
            border.width: 0

            Text {
                id: txtTitle
                anchors.left: parent.left
                anchors.leftMargin: 50
                text: title
                horizontalAlignment: Text.AlignHCenter
            }
            Rectangle{
                id: rectLine
                anchors.top: txtTitle.bottom
                width: msgBoxWidth
                height: 2
                border.color: "black"
                border.width: 1
            }
            Image {
                id: imgIcon
                anchors.top: rectLine.bottom
                anchors.topMargin: 20
                anchors.leftMargin: 40
                width: 80
                height: 80
                source: {
                    switch(icon)
                    {
                    case MsgBoxModel.Information:
                        return "/icons/information.png";
                    case MsgBoxModel.Question:
                        return "/icons/question.png";
                    case MsgBoxModel.Warning:
                        return "/icons/warning.png";
                    case MsgBoxModel.Error:
                        return "/icons/error.png";
                    default:
                        return "";
                    }
                }
            }
            TextArea{
                anchors.top: rectLine.bottom
                anchors.left: imgIcon.right
                anchors.bottom: rowButtons.top
                anchors.right: parent.right
                readOnly: true
                text: content
                color: "black"
            }
            RowLayout{
                id: rowButtons
                anchors.bottom: parent.bottom
                height: 50
                width: msgBoxWidth
                Repeater{
                    model: buttons
                    delegate: Button{
                        text: modelData
                        onClicked: {
                            uiOperation.onUIResponse(uuid, text)
                        }
                    }
                }
            }
        }

        ScrollBar.vertical: ScrollBar{}
    }
}
