import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11
import QtQuick.Dialogs 1.2
import LogicManagerLib 1.1
import SomeLib 1.1

ColumnLayout {
    RowLayout {
        Label { text: qsTr("取放参数") }
    }
    SensorPickAndPlaceParameter {}
    SPAPickAndPlacePosition {}
    SensorPickAndPlaceSUTTestOperation {}

}
