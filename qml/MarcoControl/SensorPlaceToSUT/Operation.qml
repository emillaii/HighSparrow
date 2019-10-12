import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11
import TrayLoaderModuleLib 1.1
import "../BaseUIModule"

ColumnLayout{
    RowLayout{ Label { text: qsTr("取放操作") } }
    RowLayout{
        Button{
            text: qsTr("SUT1 Place sensor")
        }
        Button{
            text: qsTr("SUT1 Pick sensor")
        }
        BaseSut1UI{}
    }
    RowLayout{
        Button{
            text: qsTr("SUT2 Place sensor")
        }
        Button{
            text: qsTr("SUT2 Pick sensor")
        }
        BaseSut2UI{}
    }
}
