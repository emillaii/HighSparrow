import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11
ColumnLayout {
    RowLayout{
        Label{
            text:qsTr("常用操作1")
        }
        Button{
            text:qsTr("测试1")
        }
        Button{
            text:qsTr("测试2")
        }
    }
    RowLayout{
        Label{
            text:qsTr("常用操作2")
        }
        Button{
            text:qsTr("测试1")
        }
        Button{
            text:qsTr("测试2")
        }
    }
}
