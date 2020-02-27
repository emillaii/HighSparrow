import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11
import FileContentItem 1.0
import QtQuick.Dialogs 1.2

ColumnLayout {
    width: 300
    GroupBox{
        RowLayout {
            Label {
                text: qsTr("Lot Number: ")
            }
            TextField {
                text: aaCoreParams.lotNumber
                onEditingFinished: {
                    aaCoreParams.setLotNumber(text)
                }
            }
        }
    }
}
