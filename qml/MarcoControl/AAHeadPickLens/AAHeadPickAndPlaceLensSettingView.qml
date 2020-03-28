import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11
import QtQuick.Dialogs 1.2
import LogicManagerLib 1.1
import SomeLib 1.1

ColumnLayout{
    RowLayout{ Label { text: qsTr("AA取放参数") } }

    AAPickParameter {}
    AAPlaceParameter {}
    AA1PickAndPlacePosition {}
    AA2PickAndPlacePosition {}

    RowLayout {
        Label { text: qsTr("AA Head Pick LUT Safety Position") }
    }
    RowLayout {
        Label { text: qsTr("LUT Safety Position") }
        Label {
            text: qsTr("X")
        }
        TextField {
            text: tcpLUTCarrierParams.SafetyX
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                tcpLUTCarrierParams.setSafetyX(text)
            }
        }
        Label {
            text: qsTr("Y")
        }
        TextField {
            text: tcpLUTCarrierParams.SafetyY
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                tcpLUTCarrierParams.setSafetyY(text)
            }
        }
        Label {
            text: qsTr("Z")
        }
        TextField {
            text: tcpLUTCarrierParams.SafetyZ
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                tcpLUTCarrierParams.setSafetyZ(text)
            }
        }
    }

    AA1TestOperation {}
    AA2TestOperation {}

    RowLayout {
        Button{
            text: qsTr("LUT Pocket 1 VAC")
            onClicked: {
                baseModuleManager.toogleIoState(tcpLUTParams.tcpLutVacuum1Name)
            }
        }
        RoundButton{
            background: Rectangle {
                radius: 6
                color: {
                    if (tcpLUTState.tcpVaccum1State) return "lightGreen"
                    else return "red"
                }
            }
        }
        Button{
            text: qsTr("LUT Pocket 2 VAC")
            onClicked: {
                 baseModuleManager.toogleIoState(tcpLUTParams.tcpLutVacuum2Name)
            }
        }
        RoundButton{
            background: Rectangle {
                radius: 6
                color: {
                    if (tcpLUTState.tcpVaccum2State) return "lightGreen"
                    else return "red"
                }
            }
        }
        Button{
            text: qsTr("AA1 Gripper Open/Closed")
            onClicked: {
                 baseModuleManager.toogleIoState(tcpAAHeadParams.gripperName)
            }
        }
        RoundButton{
            background: Rectangle {
                radius: 6
                color: {
                    if (tcpAACoreStates.tcpAAGripperState) return "lightGreen"
                    else return "red"
                }
            }
        }
        Button{
            text: qsTr("AA2 Gripper Open/Closed")
            onClicked: {
                 baseModuleManager.toogleIoState(aaHeadParams.gripperName)
            }
        }
        RoundButton{
            background: Rectangle {
                radius: 6
                color: {
                    if (aaCoreStates.aaGripperState) return "lightGreen"
                    else return "red"
                }
            }
        }
    }
}
