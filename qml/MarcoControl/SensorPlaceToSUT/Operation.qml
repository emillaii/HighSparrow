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
            enabled: false
        }
        Button{
            text: qsTr("SUT1 Pick sensor")
            enabled: false
        }
        BaseSut1UI{}
    }
    RowLayout{
        Button{
            text: qsTr("SUT2 Place sensor")
            enabled: false
        }
        Button{
            text: qsTr("SUT2 Pick sensor")
            enabled: false
        }
        BaseSut2UI{}
    }
}
