import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11
import TrayLoaderModuleLib 1.1
ColumnLayout{
    RowLayout{ Label { text: qsTr("Input & Output 操作") } }
    RowLayout{
        Button{
            text: qsTr("Input Magazine1 POS")
        }
        Button{
            text: qsTr("Input Magazine2 POS")
        }
        Button{
            text: qsTr("Current Input Slot")
        }
        TextField{
            text:"9"
            horizontalAlignment:TextInput.AlignHCenter
            validator: IntValidator{

            }
        }
    }

    RowLayout{
        Button{
            text: qsTr("Output Magazine1 POS")
        }
        Button{
            text: qsTr("Output Magazine2 POS")
        }
        Button{
            text: qsTr("Current Output Slot")
        }
        TextField{
            text:"9"
            horizontalAlignment:TextInput.AlignHCenter
            validator: IntValidator{

            }
        }
    }

    RowLayout{
        Button{
            text: qsTr("Boat1 Load Carrier")
        }
        Button{
            text: qsTr("Boat2 Load Carrier")
        }
        Button{
            text: qsTr("Output Carrier")
        }
    }

    RowLayout{ Label { text: qsTr("取放操作") } }
    RowLayout{
        Button{
            text: qsTr("料盘1位置")
        }
        Label{
            text: qsTr("Row")
        }
        TextField{
            text: "1"
            horizontalAlignment: TextInput.AlignHCenter
            validator: IntValidator{
            }
        }
        Label{
            text: qsTr("COL")
        }
        TextField{
            text: "1"
            horizontalAlignment: TextInput.AlignHCenter
            validator: IntValidator{

            }
        }
        Button{
            text: qsTr("Move")
        }
        Button{
            text: qsTr("Pick")
        }
        Button{
            text: qsTr("Place")
        }
    }

    RowLayout{
        Button{
            text: qsTr("料盘2位置")
        }
        Label{
            text: qsTr("Row")
        }
        TextField{
            text: "1"
            horizontalAlignment: TextInput.AlignHCenter
            validator: IntValidator{

            }
        }
        Label{
            text: qsTr("COL")
        }
        TextField{
            text: "1"
            horizontalAlignment: TextInput.AlignHCenter
            validator: IntValidator{

            }
        }
        Button{
            text: qsTr("Move")
        }
        Button{
            text: qsTr("Pick")
        }
        Button{
            text: qsTr("Place")
        }
    }

    RowLayout{
        Button{
            text: qsTr("SPA Collet VAC")
            onClicked: {
                baseModuleManager.toogleIoState(sensorPickArmParams.spaVaccum1OutIoName)
            }
        }
        RoundButton{
            background: Rectangle {
                radius: 6
                color: {
                    if (sensorLoaderModuleState.spaVaccum1State) return "lightGreen"
                    else return "red"
                }
            }
        }
    }
}
