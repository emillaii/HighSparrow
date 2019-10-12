import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11
import TrayLoaderModuleLib 1.1
ColumnLayout{
    RowLayout{ Label { text: qsTr("LUT操作") } }
    RowLayout{
        Button{
            text: qsTr("LUT Pocket1 POS")
        }
        Button{
            text: qsTr("Place to Pocket1")
        }
        Button{
            text: qsTr("Pick From Pocket1")
        }
    }
    RowLayout{
        Button{
            text: qsTr("LUT Pocket2 POS")
        }
        Button{
            text: qsTr("Place to Pocket2")
        }
        Button{
            text: qsTr("Pick From Pocket2")
        }
    }
}
