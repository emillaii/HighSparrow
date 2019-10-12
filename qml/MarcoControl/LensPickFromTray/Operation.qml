import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11
import TrayLoaderModuleLib 1.1
ColumnLayout{
    RowLayout{ Label { text: qsTr("弹夹操作") } }
    RowLayout{
        Button{
            text: qsTr("Load Lens Tray")
        }
        TextField{
            text:"1"
            horizontalAlignment:TextInput.AlignHCenter
            validator: IntValidator{

            }
        }
    }
    RowLayout{
        Button{
            text: qsTr("Unload Lens Tray")
        }
        TextField{
            text:"1"
            horizontalAlignment:TextInput.AlignHCenter
            validator: IntValidator{

            }
        }
    }
    RowLayout{ Label { text: qsTr("料盘信息") } }
    RowLayout{
        Button{
            text: qsTr("Tray1目标位置")
        }
        Label{
            text: qsTr("Row")
        }
        TextField{
            text: "1"
            horizontalAlignment: TextInput.AlignHCenter
            validator: IntValidator{

            }
        }
        Label{
            text: qsTr("COL")
        }
        TextField{
            text: "1"
            horizontalAlignment: TextInput.AlignHCenter
            validator: IntValidator{

            }
        }
        Button{
            text: qsTr("Move")
        }
        Button{
            text: qsTr("Pick")
        }
        Button{
            text: qsTr("Place")
        }
    }
    RowLayout{
        Button{
            text: qsTr("Tray2目标位置")
        }
        Label{
            text: qsTr("Row")
        }
        TextField{
            text: "1"
            horizontalAlignment: TextInput.AlignHCenter
            validator: IntValidator{

            }
        }
        Label{
            text: qsTr("COL")
        }
        TextField{
            text: "1"
            horizontalAlignment: TextInput.AlignHCenter
            validator: IntValidator{

            }
        }
        Button{
            text: qsTr("Move")
        }
        Button{
            text: qsTr("Pick")
        }
        Button{
            text: qsTr("Place")
        }
    }
}
