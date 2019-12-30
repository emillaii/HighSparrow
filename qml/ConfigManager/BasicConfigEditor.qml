import QtQuick 2.0
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3

ColumnLayout{
    property var configModel: null
    property string sectionName: "name"
    property int listMaxWidth: 1000
    property int listMaxHeight: 500
    property bool isExpand: false

    RowLayout{
        Label{
            text: sectionName
        }
        RoundButton{
            radius: 8
            icon.source: isExpand ? "/icons/collapse.png" : "/icons/expand.png"
            icon.color: "transparent"
            onClicked: {
                isExpand = !isExpand
            }
        }
    }
}
