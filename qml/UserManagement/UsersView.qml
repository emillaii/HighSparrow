import QtQuick 2.11
import QtQuick.Layouts 1.11
import QtQuick.Controls 1.4

TableView{
    TableViewColumn{
        role: "name"
        title: qsTr("Name")
    }
    TableViewColumn{
        role: "authority"
        title: qsTr("Authority")
    }

    model: userModel

    horizontalScrollBarPolicy: Qt.ScrollBarAsNeeded
}
