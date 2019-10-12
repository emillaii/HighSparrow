import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11
import TrayLoaderModuleLib 1.1
ColumnLayout{
    RowLayout{ Label { text: qsTr("AA1 Head操作") } }
    RowLayout{
        Button{
            text: qsTr("AA1 HEAD Pick Pocket1 POS")
        }
        Button{
            text: qsTr("AA1 HEAD Pick Pocket1")
        }
        Button{
            text: qsTr("AA1 HEAD Place Pocket1")
        }
    }
    RowLayout{
        Button{
            text: qsTr("AA1 HEAD Pick Pocket2 POS")
        }
        Button{
            text: qsTr("AA1 HEAD Pick Pocket2")
        }
        Button{
            text: qsTr("AA1 HEAD Place Pocket2")
        }
    }
    RowLayout{
        Button{
            text: qsTr("AA1 HEAD Up Look POS")
        }
        Button{
            text: qsTr("AA1 Gripper Open")
        }
        Button{
            text: qsTr("AA1 Gripper Close")
        }
    }

    RowLayout{ Label { text: qsTr("AA2 Head操作") } }
    RowLayout{
        Button{
            text: qsTr("AA2 HEAD Pick Pocket1 POS")
        }
        Button{
            text: qsTr("AA2 HEAD Pick Pocket1")
        }
        Button{
            text: qsTr("AA2 HEAD Place Pocket1")
        }
    }
    RowLayout{
        Button{
            text: qsTr("AA2 HEAD Pick Pocket2 POS")
        }
        Button{
            text: qsTr("AA2 HEAD Pick Pocket2")
        }
        Button{
            text: qsTr("AA2 HEAD Place Pocket2")
        }
    }
    RowLayout{
        Button{
            text: qsTr("AA2 HEAD Up Look POS")
        }
        Button{
            text: qsTr("AA2 Gripper Open")
        }
        Button{
            text: qsTr("AA2 Gripper Close")
        }
    }
}
