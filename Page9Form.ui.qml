import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Window 2.0
import "qml/MarcoControl"
import "qml/PageMenuBar"
Page {
    id: page9
    width: 1580
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
        Page9MenuBar{}
    }
    Frame {
        x: 0
        y: 0
        anchors.fill: parent
        visible: true
    }

    MainPage {
    }
}
