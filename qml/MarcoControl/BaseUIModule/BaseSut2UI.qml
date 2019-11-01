import QtQuick 2.0
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.3

RowLayout{
    Button{
        text: qsTr("SUT2 Clamp")
        onClicked: {
            baseModuleManager.toogleIoState(sutParams.tcpSUTPOGOPINName)
        }
    }
    RoundButton{
        background: Rectangle {
            radius: 6
            color: "red"
        }
    }

    Button{
        text: qsTr("SUT2 VAC")
        onClicked: {
            baseModuleManager.toogleIoState(sutParams.tcpSUTVaccumName)
        }
    }
    RoundButton{
        background: Rectangle {
            radius: 6
            color: "red"
        }
    }
}
