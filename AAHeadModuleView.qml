import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11
import FileContentItem 1.0
import QtQuick.Dialogs 1.2

ColumnLayout {
    RowLayout {
        Label {
            text: qsTr("")
        }
    }
    GroupBox{
        title: qsTr("OC Position")
        OCPositionView {}
    }
    GroupBox{
        title: qsTr("Pick Lens Position")
        PickLensPositionView {}
    }
}
