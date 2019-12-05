import QtQuick 2.9
import QtQuick.Controls 2.2
import VisionModuleLib 1.1

Item {
    CameraView1 {
        visible: {
            if (baseModuleManager.getServerMode() === 0) return true
            else return false
        }
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.topMargin: 10
        anchors.bottomMargin: 10
        anchors.leftMargin: 10
        anchors.rightMargin: 10
    }

    CameraView2 {
        visible: {
            if (baseModuleManager.getServerMode() === 0) return false
            else return true
        }
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.topMargin: 10
        anchors.bottomMargin: 10
        anchors.leftMargin: 10
        anchors.rightMargin: 10
    }
}
