import QtQuick 2.0
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.3

RowLayout{
    Button{
        text: qsTr("SUT1 Clamp")
        onClicked: {
            baseModuleManager.toogleIoState(tcpSUTParams.tcpSUTPOGOPINName)
        }
    }
    RoundButton{
        background: Rectangle {
            radius: 6
            color: {
                if (tcpSUTState.tcpPogopinState) return "lightGreen"
                else return "red"
            }
        }
    }

    Button{
        text: qsTr("SUT1 VAC")
        onClicked: {
            baseModuleManager.toogleIoState(tcpSUTParams.tcpSUTVaccumName)
        }
    }
    RoundButton{
        background: Rectangle {
            radius: 6
            color: {
                if (tcpSUTState.tcpVaccumState) return "lightGreen"
                else return "red"
            }
        }
    }
}
