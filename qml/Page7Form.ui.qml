import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Window 2.2
import QtQuick.Layouts 1.11
import "HWAA"

Page {
    width: 1280
    height: 720

    ColumnLayout {
        anchors.fill: parent
        HWAACoreView {
            font.family: "Times New Roman"
            spacing: 19
            Layout.fillHeight: true
            Layout.fillWidth: true
        }
    }
}
