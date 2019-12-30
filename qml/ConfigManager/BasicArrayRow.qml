import QtQuick 2.0
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3
import ConfigElementType 1.0

RowLayout{
    property var configModel: null
    property int index: -1
    property var rowModel: null

    property alias connRowIndexChanged: connRowIndexChanged

    Connections{
        id: connRowIndexChanged
        target: null
        onRowIndexChanged: {
            index = target.rowIndex
        }
    }

    Item {
        width: 35
        height: 20
        Label{
            anchors.fill: parent
            text: index + ":"
            horizontalAlignment: Qt.AlignRight
            verticalAlignment: Qt.AlignVCenter
        }
    }
}
