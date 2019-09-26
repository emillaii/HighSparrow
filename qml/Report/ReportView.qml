import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11
import FileContentItem 1.0
import QtQuick.Dialogs 1.2
import AACoreNew 1.1
import QtWebEngine 1.4

RowLayout {

    anchors.centerIn: parent
    width: parent.width
    height: parent.height

    FileDialog {
        id: loadLogDialog
        title: qsTr("选择加载文件")
        selectExisting: true
        selectFolder: false

        nameFilters: ["json文件 (*.json)"]
        onAccepted: {
            file.setSource(loadLogDialog.fileUrl)
            var result = file.read()
            var command = "displayData('" + result + "')";
            reportWebview.runJavaScript(command, function(result) {

            })
        }
    }


    ColumnLayout {
        width: 200
        Layout.alignment: Qt.AlignLeft | Qt.AlignTop
        RoundButton {
             text: qsTr("加載數據")
             onClicked: {
                 loadLogDialog.open()
             }
        }
    }

    WebEngineView {
        id: reportWebview
        Layout.fillHeight: true
        Layout.fillWidth: true
        url: {
            var url = moduleManagerParam.flowChartURL
            url = url.replace('flowchart', 'unitReport')
            return url
        }
    }
}


