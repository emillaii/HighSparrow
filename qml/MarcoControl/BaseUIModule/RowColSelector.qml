import QtQuick 2.0
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.3

RowLayout{
    property alias txtRow: txtRow
    property alias txtCol: txtCol

    Label{
        text: qsTr("Row")
    }
    TextField{
        id: txtRow
        text: "1"
        horizontalAlignment: TextInput.AlignHCenter
        validator: IntValidator{

        }
    }
    Label{
        text: qsTr("COL")
    }
    TextField{
        id: txtCol
        text: "1"
        horizontalAlignment: TextInput.AlignHCenter
        validator: IntValidator{

        }
    }
}
