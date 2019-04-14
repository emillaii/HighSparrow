import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11
import QtQuick.Dialogs 1.2

ColumnLayout {
    RowLayout {
        Button {
            text: qsTr("Start Chart Cali")
            onClicked: {
                logicManager.performChartCalibration()
            }
        }
        Button {
            text: qsTr("Perform Chart Alignment")
            onClicked: {
                logicManager.performOC()
            }
        }
    }
}
