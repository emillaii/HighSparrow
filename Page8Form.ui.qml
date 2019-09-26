import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Window 2.0
import "qml/Report"
import Qt.labs.folderlistmodel 2.12

Page {
    id: page8
    width: 1580
    height: 720

    header: RowLayout {
        Button {
            text: "Load file"
        }
    }

    Frame {
        x: 0
        y: 0
        anchors.fill: parent
        visible: true

        ListView {
            width: 200
            height: 400

            FolderListModel {
                id: folderModel
                nameFilters: ["*.qml"]
            }

            model: folderModel
            delegate: fileDelegate
        }

        ReportView {
        }
    }
}
