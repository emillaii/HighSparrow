import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Window 2.0

Page {
    GroupBox{
        title: "MsgBoxTest"
        RowLayout{
            Button{
                text: "ShowMsg"
                onClicked: {
                    msgBoxTester.randomShowMsg()
                }
            }
            Button{
                text: "GetUIResponse"
                onClicked: {
                    msgBoxTester.randomGetMsgRsp()
                }
            }
        }
    }
}
