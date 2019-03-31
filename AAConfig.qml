import QtQuick 2.9
import QtQuick.Controls 2.2
Item {
    Rectangle {
        id: rectangle
        color: "#5d5d5d"
        anchors.fill: parent

        GroupBox {
            id: groupBox
            x: 18
            y: 19
            width: 578
            height: 108
            title: qsTr("Vision")
        }

        GroupBox {
            id: groupBox1
            x: 18
            y: 152
            width: 578
            height: 113
            title: qsTr("Calibration")
        }

        GroupBox {
            id: groupBox2
            x: 18
            y: 298
            width: 578
            height: 153
            title: qsTr("Motion")
        }
    }

}
