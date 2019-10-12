import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11
import "../BaseUIModule"

ColumnLayout {
   RowLayout{ Label { text: qsTr("取放操作") } }
    RowLayout{
        Button{
            text: qsTr("Buffer盘位置")
        }
        RowColSelector{}
        Button{
            text: qsTr("Move")
        }
        Button{
            text: qsTr("Place")
        }
    }

    RowLayout{
        Button{
            text: qsTr("Reject盘位置")
        }
        RowColSelector{}
        Button{
            text: qsTr("Move")
        }
        Button{
            text: qsTr("Place")
        }
    }

    RowLayout{
        Button{
            text: qsTr("CPA Collet VAC")
        }
        RoundButton{
            background: Rectangle {
                radius: 6
                color: "red"
            }
        }
    }
}
