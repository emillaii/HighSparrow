import QtQuick 2.0
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.3

RowLayout{
    Button{
        text: qsTr("SUT2 Clamp")
        onClicked: {
            baseModuleManager.toogleIoState(sutParams.sutPOGOPINName)
        }
    }
    RoundButton{
        background: Rectangle {
            radius: 6
            color: {
                if (sutState.pogopinState) return "lightGreen"
                else return "red"
            }
        }
    }

    Button{
        text: qsTr("SUT2 VAC")
        onClicked: {
            baseModuleManager.toogleIoState(sutParams.sutVaccumName)
        }
    }
    RoundButton{
        background: Rectangle {
            radius: 6
            color: {
                if (sutState.vaccumState) return "lightGreen"
                else return "red"
            }
        }
    }
}
