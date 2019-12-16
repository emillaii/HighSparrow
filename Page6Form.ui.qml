import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Window 2.2
import QtQuick.Layouts 1.11
import "qml/AACore"
import "qml/PageMenuBar/"

Page {
    width: 1280
    height: 720

    header: ToolBar {
        id: toolBar
        x: 0
        y: 0
        width: 360
        height: 50
        font.weight: Font.Light
        font.capitalization: Font.Capitalize
        spacing: 14
        Rectangle {
            width: parent.width
            height: parent.height
            gradient: Gradient {
                GradientStop {
                    position: 0
                    color: "#9a4afe"
                }

                GradientStop {
                    position: 1
                    color: "#000000"
                }
            }
        }
        Page6MenuBar {
        }
    }
    ColumnLayout {
        anchors.fill: parent
        AACoreView {
            Layout.fillHeight: true
            Layout.fillWidth: true
        }
    }
}
