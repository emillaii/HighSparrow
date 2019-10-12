import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11
import TrayLoaderModuleLib 1.1
import "../BaseUIModule"

ColumnLayout{
    RowLayout{ Label { text: qsTr("取放操作") } }
    RowLayout{
        Button{
            text: qsTr("Sensor料盘1位置")
        }
       RowColSelector{}
        Button{
            text: qsTr("Pick")
        }
        Button{
            text: qsTr("Place")
        }

        Button{
            text: qsTr("Sensor料盘2位置")
        }
       RowColSelector{}
        Button{
            text: qsTr("Pick")
        }
        Button{
            text: qsTr("Place")
        }
    }


    RowLayout{
        Button{
            text: qsTr("Lens料盘1位置")
        }
        RowColSelector{}
        Button{
            text: qsTr("Pick")
        }
        Button{
            text: qsTr("Place")
        }

        Button{
            text: qsTr("Lens料盘2位置")
        }
       RowColSelector{}
        Button{
            text: qsTr("Pick")
        }
        Button{
            text: qsTr("Place")
        }
    }

    RowLayout{
        Button{
            text: qsTr("SUT1 Load POS")
        }
        Button{
            text: qsTr("Place Sensor to SUT1")
        }
        Button{
            text: qsTr("Pick Sensor to SUT1")
        }
    }

    RowLayout{
        Button{
            text: qsTr("SUT2 Load POS")
        }
        Button{
            text: qsTr("Place Sensor to SUT2")
        }
        Button{
            text: qsTr("Pick Sensor to SUT2")
        }
    }

    RowLayout{
        Button{
            text: qsTr("LUT Load POS")
        }
        Button{
            text: qsTr("Place Lens to LUT")
        }
        Button{
            text: qsTr("Pick from LUT")
        }
    }

    RowLayout{
        Button{
            text: qsTr("AA1 Place Lens Pocket1")
        }
        Button{
            text: qsTr("AA1 Pick Lens Pocket1")
        }
    }

    RowLayout{
        Button{
            text: qsTr("AA2 Place Lens Pocket1")
        }
        Button{
            text: qsTr("AA2 Pick Lens Pocket1")
        }
    }

    RowLayout{ Label {text: qsTr("AA操作") } }

    RowLayout{
        Button{
            text: qsTr("AA1 Dispense PR")
        }
        Button{
            text: qsTr("AA1 Lens Up Look PR")
        }
        Button{
            text: qsTr("PR To Bond")
        }
        Button{
            text: qsTr("AA Batch Run")
        }
    }

    RowLayout{
        Button{
            text: qsTr("AA1 Gripper")
        }
        RoundButton{
            background: Rectangle {
                radius: 6
                color: "red"
            }
        }

        BaseSut1UI{
        }

        Button{
            text: qsTr("LUT Pocket1 VAC")
        }
        RoundButton{
            background: Rectangle {
                radius: 6
                color: "red"
            }
        }
    }

    RowLayout{
        Button{
            text: qsTr("AA2 Dispense PR")
        }
        Button{
            text: qsTr("AA2 Lens Up Look PR")
        }
        Button{
            text: qsTr("PR To Bond")
        }
        Button{
            text: qsTr("AA Batch Run")
        }
    }

    RowLayout{
        Button{
            text: qsTr("AA2 Gripper")
        }
        RoundButton{
            background: Rectangle {
                radius: 6
                color: "red"
            }
        }

        BaseSut2UI{

        }

        Button{
            text: qsTr("LUT Pocket1 VAC")
        }
        RoundButton{
            background: Rectangle {
                radius: 6
                color: "red"
            }
        }
    }
}
